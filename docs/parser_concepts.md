# 🧠 Conceitos de Fundo — Parser & Integração

Este guia explica os conceitos gerais (não específicos do cub3D) que
sustentam o parser e os achados da sessão de integração. Pra ver como
eles aparecem no código real, ver `docs/parser_explanation.md`.

---

## 📚 Índice

1. [Arrays 2D dinâmicos (`char **`)](#1-arrays-2d-dinâmicos-char-)
2. [Ownership de memória](#2-ownership-de-memória)
3. [Valores sentinela](#3-valores-sentinela)
4. [`get_next_line` é destrutiva](#4-get_next_line-é-destrutiva)
5. [Vizinhos vs. flood fill](#5-vizinhos-vs-flood-fill)
6. [Merge: fast-forward vs. 3-way vs. conflito semântico](#6-merge-fast-forward-vs-3-way-vs-conflito-semântico)
7. [Concatenação de string literals em C](#7-concatenação-de-string-literals-em-c)
8. [Decode vs. contexto gráfico (MLX42)](#8-decode-vs-contexto-gráfico-mlx42)
9. [Coordenada contínua sobre grid discreto](#9-coordenada-contínua-sobre-grid-discreto)
10. [Categorias de leak do valgrind](#10-categorias-de-leak-do-valgrind)
11. [Validar antes, normalizar depois](#11-validar-antes-normalizar-depois)

---

## 1. Arrays 2D dinâmicos (`char **`)

Um mapa `.cub` não tem tamanho fixo, então não dá pra usar
`char grid[N][M]`. A solução é um array de ponteiros — cada ponteiro
aponta pra uma string (uma linha), e o array de ponteiros termina em
`NULL`:

```
grid ─┬─→ "111111111111111111111\0"
      ├─→ "100000000000000000001\0"
      ├─→ "100001111011110000001\0"
      ...
      └─→ NULL   ← marca o fim, em vez de guardar um "rows" separado aqui
```

Cada linha pode ter um tamanho **diferente** (mapa não-retangular) —
por isso `map->cols` é calculado como o máximo, não assumido fixo.
Percorrer esse array sempre segue o padrão:
```c
i = 0;
while (map[i])       // até achar NULL
{
    j = 0;
    while (map[i][j]) // até achar '\0' da linha
        j++;
    i++;
}
```

---

## 2. Ownership de memória

Regra geral em C: **quem aloca decide quem libera** — normalmente
quem alocou, ou uma função que recebeu o ponteiro *e sabe* que é
dona dele. Passar um ponteiro pra uma função não transfere posse
automaticamente.

No parser, essa regra apareceu em dois lugares:
- `check_color_part` recebe uma *parte* do array `parts` (alocado
  por `ft_split` dentro de `parse_color`) — ela não é dona, então só
  retorna um código de erro cru, nunca libera nem imprime.
- `find_player` **não** deveria chamar `handle_error` porque quem
  decide o resultado final da validação do mapa é `parse_map_grid`
  — quando as duas chamavam, a mensagem saía duplicada.

**Heurística prática:** se uma função só *lê* um pedaço de uma
estrutura que veio de fora, ela não deveria alocar, liberar, nem
imprimir erro sobre essa estrutura — só reportar pra cima.

### O corolário: quem é dono também é quem reporta

Posse e responsabilidade de reportar andam juntas. Se duas funções da
mesma cadeia acham que ambas devem reportar o erro, o usuário vê a
mensagem duas vezes — e o bug é chato de achar, porque cada função
isolada parece certa.

A forma de manter isso consistente é escolher **uma altura** na cadeia
onde o erro vira mensagem, e fazer todo mundo abaixo dela retornar
código cru.

### Sentinela zerada: `memset` como pré-condição de limpeza

Um truque que faz os dois lados da regra baterem: se a struct inteira
começa zerada, então **todo ponteiro não usado é `NULL`**, e como
`free(NULL)` é no-op, a função de limpeza pode ser chamada em qualquer
ponto de falha sem saber até onde a inicialização chegou.

Sem isso, cada caminho de erro precisaria liberar exatamente o que ele
alocou — n caminhos, n funções de limpeza diferentes, e um deles vai
estar errado.

---

## 3. Valores sentinela

Como saber se `config->floor` foi realmente preenchido pelo parser,
ou se ainda tem lixo de memória não inicializada? Convenção: setar
um valor **impossível** antes de tentar preencher, e checar esse
valor no final.

```c
config->floor[0] = -1;   // -1 nunca é um componente RGB válido (range 0-255)
// ... parse roda, ou não roda (linha F ausente) ...
if (config->floor[0] == -1)
    return (handle_error(ERR_INVALID_COLOR)); // nunca foi setado de verdade
```

Isso é o que permite a garantia: "se `parse_meta` retornar sucesso,
`config.floor` tem valores reais" — sem sentinela, sucesso não
provaria nada sobre esse campo específico.

### Uma sentinela, duas perguntas

O mesmo valor impossível responde duas perguntas opostas, dependendo de
**quando** você olha:

| pergunta | quando | teste |
|---|---|---|
| "esse campo chegou a ser preenchido?" | no fim, na validação | `campo == sentinela` |
| "esse campo já foi preenchido antes?" | durante, antes de escrever | `campo != sentinela` |

A segunda é o que detecta **entrada duplicada** num formato onde as
chaves podem vir em qualquer ordem. Sem ela, a segunda ocorrência
sobrescreve a primeira em silêncio — e se o campo for um ponteiro
alocado, sobrescrever é vazar.

Repara que o tipo do campo já pode dar a sentinela de graça: pra
ponteiro, `NULL` vindo de um `memset` faz esse papel sem precisar de
valor inventado.

---

## 4. `get_next_line` é destrutiva

Ler uma linha com `get_next_line(fd)` **consome** essa linha do
arquivo — não tem como "devolver" ou reler sem dar seek. Isso é um
problema quando uma função de leitura genérica (`read_meta_lines`)
precisa parar de ler assim que detecta que a próxima linha já
pertence a outra seção (o mapa) — mas nesse ponto, ela já leu e tem
essa linha em mãos.

**Padrão usado:** passar um `char **first_map_line` de saída. A
função que detectou o fim da sua seção guarda a linha já lida ali,
em vez de tentar "devolvê-la" pro fd. Quem chama recebe essa linha
pronta como parâmetro de entrada, sem precisar ler de novo.

**Pegadinha real que isso causou (achada com `leaks`, 08/08):** guardar
a linha num out-param só resolve "não perder o dado" — não resolve
"quem é dono dela agora". `read_meta_lines` pode ter sucesso (achou a
linha do mapa, preencheu `*first_map_line`) e o `parse_meta` mesmo
assim falhar depois, no `validate_config` (ex.: faltou uma textura).
Nesse caso a linha já tinha sido alocada, ninguém nunca ficava dono
dela de verdade, e o `parse_cub` fechava o fd no caminho de erro sem
dar `free` nela — vazamento de 32 bytes, só nesse caminho específico
(`maps/invalid/invalid03_missing_tex.cub` é o único mapa de teste que passa
exatamente por ali). Corrigido inicializando `first_line = NULL` no
`parse_cub` e dando `free(first_line)` antes de fechar o fd nesse
ramo — `free(NULL)` não faz nada nos outros casos, então é seguro
chamar sempre.

**Segunda pegadinha, mesma raiz:** `get_next_line` guarda um buffer
estático por fd, que só é liberado sozinho quando o `read()` interno
bate em EOF de verdade. Se o parser para de ler antes disso (erro, ou
simplesmente não precisar do resto do arquivo) e só dá `close(fd)`,
esse buffer interno fica preso pra sempre — `close()` não sabe nada
sobre o heap do `get_next_line`. Solução, funciona com qualquer
implementação de GNL: antes de fechar, ler até `NULL` de propósito
(`close_and_drain`, em `parser.c`).

---

## 5. Vizinhos vs. flood fill

Duas formas de checar se um mapa está "fechado":

```
PERÍMETRO (so_long, mapa retangular)      VIZINHOS (cub3D, mapa irregular)
┌───────────────┐                         percorre TODOS os '0'
│ só checa a     │                         e pra cada um, checa se
│ borda externa  │                         os 4 vizinhos são válidos
└───────────────┘                         (não fora do grid, não ' ')
```

Flood fill clássico (a partir do player, marcando visitados) só
valida o que é **alcançável** a partir de onde o player está — uma
área desconectada com uma abertura não seria detectada. Checar
vizinho-por-vizinho em todo o grid pega qualquer vazamento, mesmo
em bolsões isolados, e evita o custo/risco de recursão profunda
(relevante — a versão flood fill do so_long já tinha dado problema
de stack overflow antes).

**A pegadinha da linha irregular:** checar vizinho num grid ragged não
pode usar a largura *máxima* do grid como limite — tem que usar o
comprimento **daquela linha específica**. Com a largura máxima, uma
linha curta pareceria ter células válidas onde não existe nada
alocado, e o vazamento passaria batido.

**Limitação da abordagem:** checar só as 4 cardeais deixa passar
aberturas *diagonais* — duas paredes que se tocam apenas na quina.
Pra um jogo de grid comum isso não importa (ninguém anda na diagonal
exata), mas num raycaster DDA um raio pode atravessar exatamente esse
vértice.

---

## 6. Merge: fast-forward vs. 3-way vs. conflito semântico

```
FAST-FORWARD                    3-WAY (merge normal)
main ──●                        main ──●───────●  (só andou,
        \                              \         \  branch não
         ●──●──● feat/x                 ●──●──●   diverge)
(branch só andou;               (as duas mudaram;
git só move o ponteiro,         git cria um commit
sem novo commit)                de merge combinando)
```

Um conflito de **texto** acontece quando as duas branches editaram
a *mesma linha* — o git literalmente não sabe qual manter e pede
pra decidir. Mas um conflito **semântico** (ou "silencioso") é
diferente: as duas branches editam partes diferentes do arquivo,
então o git consegue combinar automaticamente — só que o resultado
não compila ou não funciona, porque uma branch dependia de algo que
a outra mudou (ex.: renomeou um campo de struct que a outra branch
usava, em outro arquivo, sem overlap de texto nenhum).

**Consequência prática:** merge sem conflito reportado pelo git ≠
merge seguro. Sempre compilar (e idealmente testar) depois.

---

## 7. Concatenação de string literals em C

Regra pouco conhecida do C: strings literais **adjacentes** (só
espaço/quebra de linha entre elas, sem operador) são concatenadas
automaticamente pelo compilador em uma única string:

```c
char *s = "abc" "def";   // equivale a "abcdef" — 1 string, não 2
```

Isso vira bug quando alguém esquece uma vírgula num array de
strings:
```c
static const char *messages[] = {
    "erro um\n",
    "erro dois\n"
    "erro tres\n",   // ↑ faltou vírgula na linha de cima!
};
// resultado: só 2 entries no array, não 3 — "erro dois" e "erro tres"
// viraram uma string só, e todo índice depois desliza
```
Sem warning do compilador na maioria dos casos — é sintaxe válida,
só não é o que a pessoa queria dizer.

---

## 8. Decode vs. contexto gráfico (MLX42)

Nem toda função de "carregar imagem" precisa de uma janela gráfica
já aberta. A MLX42 separa isso em duas etapas:

```c
// decodifica o PNG puro (bytes → largura/altura/pixels em memória)
// NÃO precisa de mlx_t* — pode rodar antes de qualquer mlx_init
mlx_texture_t* mlx_load_png(const char* path);

// converte pra algo desenhável na tela
// PRECISA de mlx_t* já inicializado (contexto gráfico/GPU)
mlx_image_t* mlx_texture_to_image(mlx_t* mlx, mlx_texture_t* texture);
```

Essa distinção (decode de arquivo vs. recurso gráfico) é comum em
APIs de imagem/gráficos em geral, não só na MLX42 — vale reconhecer
o padrão: "isso precisa de um contexto ativo, ou só de bytes em
disco?" muda onde na ordem de inicialização do programa aquele
código pode rodar.

---

## 9. Coordenada contínua sobre grid discreto

Quando um programa mistura **posição contínua** (`double`) com **grid
discreto** (índices `int`), tem uma pergunta que precisa de resposta
explícita: a coordenada inteira `5` é a *quina* da célula 5, ou o
*centro* dela?

A conversão `(int)pos` responde por você: ela mapeia todo o intervalo
`[5.0, 6.0)` pra célula 5. Ou seja, **a coordenada inteira é a quina** —
a borda esquerda/superior da célula.

```
   5.0        6.0
    |----------|
    |          |     (int)5.0 = 5   ← borda
    |   5.5    |     (int)5.5 = 5   ← centro
    |    •     |     (int)5.9 = 5
    |          |
    |----------|
```

Colocar uma entidade numa coordenada inteira significa colocá-la
**exatamente em cima da divisa** entre células — o que costuma quebrar
duas coisas:

**1. Qualquer caixa de colisão vira ambígua.** Uma caixa centrada na
quina com meia-largura `r` se espalha por 4 células ao mesmo tempo. Se
uma delas for sólida, a entidade fica presa. Colocando no centro, com
`r < 0.5`, a caixa cabe inteira numa célula só — a célula onde você
sabe que ela pode estar.

**2. Algoritmos incrementais começam com distância zero.** DDA,
Bresenham e parentes calculam "quanto falta até a próxima linha de
grade". Estando em cima da linha, essa distância é `0`, o primeiro
passo não percorre nada, e a distância resultante vira `0` — que
geralmente é divisor de alguma coisa.

**A regra prática:** posição de entidade num grid é sempre
`índice + 0.5`, salvo motivo explícito pro contrário.

---

## 10. Categorias de leak do valgrind

O `LEAK SUMMARY` tem quatro linhas, e elas **não** são igualmente
graves:

| categoria | significa | é problema? |
|---|---|---|
| `definitely lost` | nenhum ponteiro aponta mais pro bloco | **Sim.** É leak. |
| `indirectly lost` | só era alcançável através de um bloco `definitely lost` | **Sim.** Some junto quando você corrige o pai. |
| `possibly lost` | só há ponteiro pro *meio* do bloco, não pro início | Talvez — comum com struct interna |
| `still reachable` | **ainda havia ponteiro válido no fim do programa** | Normalmente não |

`still reachable` é memória que o programa simplesmente não liberou
antes de terminar — mas ninguém perdeu o endereço dela. Bibliotecas
grandes (drivers gráficos, X11, GLFW, runtimes) alocam tabelas internas
de propósito e deixam o SO recolher no `exit`. Isso é escolha de design
delas, não bug seu.

**Como testar seu próprio código sem o ruído da biblioteca:** rode um
caminho de execução que **não chega a inicializar** a biblioteca. Num
programa gráfico, isso normalmente é um caminho de erro de validação —
ele sai antes de abrir janela. Aí o total volta a ser só o seu, e dá pra
exigir zero absoluto.

---

## 11. Validar antes, normalizar depois

Muito parser tem duas etapas que é tentador juntar: **validar** (a
entrada é aceitável?) e **normalizar** (deixar num formato uniforme pro
resto do programa consumir).

Juntar dá errado de um jeito específico: você acaba **validando dados
que o seu próprio código inventou**. Se a normalização preenche buracos
com um valor de preenchimento, e a validação roda depois, ela vai
aprovar ou reprovar com base nesse preenchimento — não no que o usuário
escreveu. Mensagens de erro passam a apontar pra posições que não
existem no arquivo original.

```
ERRADO:  ler → normalizar → validar    (valida o que você inventou)
CERTO:   ler → validar → normalizar    (valida o que veio; normaliza o aprovado)
```

O ganho de normalizar por último é virar uma **pós-condição**: depois
dessa etapa, todo consumidor lá na frente pode assumir o formato
uniforme sem checar. É o que transforma "o grid pode ser irregular" num
problema que existe em uma função só, em vez de em todas as que indexam
o grid.
