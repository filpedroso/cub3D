# 🛡️ cub3D — Log de Preparação pra Defesa

Baseado em uma escala hipotética para servir de guia de avaliação. Este arquivo registra o que já foi testado, o que quebrava e foi corrigido, e o que ainda falta checar antes da defesa de verdade.

**Como usar:** cada seção é um bloco. Os comandos listados são pra reproduzir os testes vocês mesmas, sem precisar decorar nada.

---

## ✅ Executable name

- `make fclean && make` do zero compila sem erro e sem re-link, gera
  exatamente `cub3D`.

---

## ✅ Configuration file

Todos os 6 elementos (`NO/SO/WE/EA/F/C`) + mapa são configuráveis e
testados nos mapas em `maps/valid/`.

### 🐛 Achado e corrigido: chave duplicada não dava erro

Um `.cub` com `NO` (ou qualquer outro identificador) repetido duas
vezes era aceito silenciosamente — a segunda ocorrência só
sobrescrevia a primeira (e vazava a alocação anterior). A régua pede
isso explicitamente: *"an unknown key, **double keys**, an invalid
path..."*.

**Fix:** novo erro `ERR_DUPLICATE_ID`. `dispatch_tex`
(`file_utils.c`) e `dispatch_color` (`parser_color.c`) checam se o
campo já tinha sido preenchido antes de chamar `parse_texture`/
`parse_color`. Vale pros 6 identificadores.

### 🐛 Achado e corrigido: exit code errado quando a textura falha ao carregar

`render()` era `void` — quando `load_textures`/`config_mlx`/
`init_render` falhavam, a mensagem de erro certa era impressa, mas o
`main()` nunca sabia disso e sempre retornava `SUCCESS` (`exit 0`)
mesmo depois de imprimir `Error\n...`.

**Fix:** `render()` agora retorna `bool`, `main()` checa e retorna
`ERROR` quando falha. Extraído `init_or_fail` (fase de setup) pra
`render()` não estourar o limite de 25 linhas da Norminette.

### Testes rápidos (`./cub3D <arquivo>`, todos devem sair com `exit 255`)

```bash
# identificador desconhecido
# chave duplicada (NO duas vezes, ou F duas vezes, etc.)
# path de textura inexistente
# extensão de textura != .png
# arquivo sem extensão .cub
```

---

## ✅ Walls

1. **Textura varia por direção + perspectiva correta** — confirmado
   visualmente em vários mapas (`caquinho_pilares.cub`,
   `greybox_l.cub`, etc.), cada face mostrando a textura certa,
   convergindo certinho nas quinas.
2. **Trocar o path de uma textura muda o render**: testado ao vivo —
   trocado `SO` de `south.png` pra `purplestone.png`, re-executado,
   parede mudou, resto ficou igual.
3. **Path inexistente dá erro**: coberto pelo mesmo teste de
   "Configuration file" acima.
4. **Cor de chão/teto muda no render**: testado ao vivo — trocado
   `F`/`C` pra valores bem diferentes, re-executado, bateu certinho.

---

## ✅ Error management

### Argumentos numerosos/aleatórios
Testado com: string vazia, string de 5000 caracteres, flags tipo
`--help -h`, path com espaços, bytes de controle, argumentos
repetidos. Todos deram erro limpo, `exit 255`, sem crash.

### 🐛 Achado e corrigido: leak real no parser

`leaks -atExit -- ./test_parser maps/invalid/invalid03.cub` apontou
**1 leak, 32 bytes**, em `ft_substr` (dentro de `get_next_line`).

**Causa:** `read_meta_lines` lê e guarda com sucesso a primeira linha
do mapa em `*first_map_line`; o `parse_meta` só falha *depois*, no
`validate_config` (esse mapa não tem `WE`). Nesse caminho, o
`parse_cub` nunca dava `free` nessa linha antes de fechar o fd.

**Fix** (`parser.c`):
- `first_line` inicializado como `NULL` em `parse_cub`.
- `free(first_line)` no ramo de erro do `parse_meta`, antes de fechar
  o fd (`free(NULL)` é no-op nos outros casos — seguro chamar sempre).
- Bônus: novo `close_and_drain(fd)` — esgota o fd (lê até `NULL`)
  antes de fechar, porque o `get_next_line` guarda um buffer estático
  por fd que só se libera sozinho ao bater EOF de verdade. Parar de
  ler mais cedo (erro, ou não precisar do resto do arquivo) e só dar
  `close()` deixava esse buffer preso. Substituiu os 3 `close(fd)` do
  `parse_cub`.

**Resultado:** rodado `leaks -atExit` nos 19 cenários (11 mapas
válidos + 6 inválidos + 2 de chave duplicada) — **0 leaks em todos**
depois do fix.

**⚠️ Nota sobre `leaks` no `cub3D` rodando de verdade:** aparecem ~288
"leaks" / 14.4KB, mas são todos `NSXPCConnection`/`AppIntents`/
`LNDaemonApplicationInterface` — mecanismo interno do macOS que
qualquer app de janela dispara sozinho (confirmado vasculhando o
relatório completo: nenhum stack trace aponta pro nosso binário). Não
é nosso malloc. Se um avaliador rodar `leaks` na defesa e aparecer
número != 0, vale saber explicar essa diferença.

### Teclado aleatório
Por leitura de código: só teclas específicas são checadas
(`mlx_is_key_down` pra `UP/DOWN/LEFT/RIGHT/W/A/S/D/M/R/T/ESC`),
qualquer outra é ignorada — não tem `default`/`else` que reaja a tecla
desconhecida. Não testado apertando de verdade (sem automação de
teclado disponível no ambiente onde isso foi checado).

### Modificar o mapa
Coberto pelos 19 cenários do teste de leak acima, mais os mapas de
`maps/valid/` e `maps/invalid/`.

---

## ✅ Technical elements of display

Tudo certo (janela abre, imagem do labirinto aparece, conteúdo se mantém consistente ao esconder/minimizar/maximizar).

---

## ✅ Movements

### Orientação de spawn por direção cardinal — testado rigorosamente

Montada uma sala simétrica pequena com 4 texturas bem diferentes e reconhecíveis (uma por identificador), player bem no centro, e testado spawn nas 4 direções (`N`/`S`/`E`/`W`), conferindo qual textura aparece de frente em cada uma:

| Spawn | Textura esperada de frente | Resultado |
|---|---|---|
| `N` | `SO` | ✅ |
| `S` | `NO` | ✅ |
| `E` | `WE` | ✅ |
| `W` | `EA` | ✅ |

(A correspondência spawn→identificador parece invertida à primeira vista — `N` mostra a textura `SO`, não `NO` — mas é a mesma lógica de "a face é nomeada pelo lado que ela encara, não por quem chegou nela" já documentada em `docs/parser_explanation.md`/conversas anteriores.
Bateu certinho nos 4 casos.)

### Girar, andar, strafe — confirmado por código + teste manual da Maria

- **Girar `←`/`→`**: `pan_side` diminui `dir_ang` pra esquerda, aumenta
  pra direita — consistente com o sistema de ângulo do motor
  (`E=0°→S=90°→W=180°→N=270°`, sentido horário conforme o ângulo
  cresce), então diminuir é sentido anti-horário = girar pra esquerda.
- **`W`/`S` frente-trás**: `walk()` usa `dir_ang` direto, `'B'` só
  inverte o sinal do vetor. Sem ambiguidade.
- **`A`/`D` strafe**: `strafe()` usa `dir_ang + 90°`; testado
  apertando de verdade pela Maria, confirmado funcionando (ver seção
  de Controles acima).
- **Fluidez**: testada pela Maria, considerada ok. Não é algo
  mensurável objetivamente por aqui.

---

## 🔧 Controles (achado nessa prep, não era item isolado da régua)

### 🐛 Achado e corrigido: WASD/strafe não existiam

O subject (pág. 11) e a régua exigem `W/A/S/D` movendo o jogador,
separado das setas (que só giram a câmera). O código só tinha setas
(`↑/↓` = anda, `←/→` = gira) — não tinha `WASD` nem strafe (`A`/`D`
andando de lado sem girar) em lugar nenhum.

**Fix** (`update_pl_pos.c`): `W`/`S` espelham `↑`/`↓`. `A`/`D` chamam
uma função `strafe` nova — mesmo formato do `walk`, mas com o ângulo
deslocado 90° a mais, sem tocar em `dir_ang`. Testado apertando de
verdade pela Maria — confirmado funcionando.

Fechar janela (clique no X e `ESC`) — confirmado funcionando pela
Maria antes mesmo dessa prep.

---

## 🎁 Bonus — 2 de 5 implementados

Regra do subject: **bônus só é avaliado se a parte obrigatória tirar
TODOS os pontos**, inclusive robustez contra uso incorreto/crasso —
se a obrigatória não for perfeita, o bônus é **totalmente ignorado**.
Reforça a prioridade dos bugs corrigidos hoje.

Checado no código (`grep` por `mouse`/`cursor`, `sprite`, estado de
porta):

| Bônus | Status |
|---|---|
| Colisões nas paredes | ✅ `will_collide` em `update_pl_pos.c`, usado por `walk` e `strafe` |
| Minimapa | ✅ `draw_minimap.c`, toggle `M` |
| Portas que abrem/fecham | ❌ `D` só é tratado como andável, igual `0` — nenhum estado/animação |
| Sprites animadas | ❌ não existe em lugar nenhum do código |
| Rotacionar com o mouse | ❌ só teclado — nenhum hook de mouse |

---

## 📋 Referência rápida de comandos

```bash
# build limpo
make fclean && make

# bateria de parser (válidos + inválidos)
make test
for m in maps/valid/*.cub maps/invalid/*.cub; do ./test_parser "$m"; done

# leak check por mapa (parser)
leaks -atExit -- ./test_parser maps/valid/map01.cub

# leak check no jogo rodando de verdade (abre janela)
./cub3D maps/valid/map01.cub &
leaks $!   # rodar de novo depois de mexer, comparar
kill %1
```
