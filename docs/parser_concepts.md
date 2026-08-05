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
