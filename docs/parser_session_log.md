# 📋 cub3D — Log de Implementação: Parser
> Sessão: 07/06/2026 | Branch: `feat/parser-map`

---

## 🏗️ Decisões de arquitetura

### Bibliotecas
- **MLX42** em vez de minilibX — suporte nativo a `.png`, melhor suporte no macOS, sem gambiarras com `.xpm`
- **`get_next_line`** adicionada na libft do Fil junto com `ft_memdel` e `ft_append_line`

### Structs definidas
```c
t_config   // paths das 4 texturas + cores floor/ceiling RGB
t_map      // char** grid + rows + cols
t_player   // double x, y + char dir ('N','S','E','W')
t_game     // mlx + image + config + map + player
```

### Separação de responsabilidades
```
parse_cub         ← entry point, orquestra tudo (parser.c)
  ├── parse_meta        ← lê NO/SO/WE/EA/F/C (parser_meta.c)
  └── parse_map_grid    ← lê o grid (parser_map.c)
```

### Decisão sobre first_map_line (Opção A)
`get_next_line` é destrutivo — quando `parse_meta` detecta o início
do mapa, já consumiu a linha. Solução: passar `char **first_map_line`
como parâmetro pra `parse_meta`, que preenche antes de retornar.
`parse_map_grid` recebe essa linha já pronta.

---

## 📁 Arquivos criados/modificados

### Novos
| Arquivo | O que tem |
|---|---|
| `src/parser/parser.c` | `parse_cub` — entry point |
| `src/parser/parser_meta.c` | `is_map_line`, `parse_texture`, `parse_color`(placeholder), `dispatch_meta_line`, `parse_meta` |
| `src/parser/parser_map.c` | `has_only_valid_chars`, `find_player`, `parse_map_grid` |
| `src/parser/file_utils.c` | `has_cub_extension`, `has_png_extension`, `trim_newline` |
| `src/parser/free_utils.c` | `free_map`, `free_visited`, `free_visited_partial` |
| `maps/valid/map01.cub` | Mapa válido 21x13 com corredores e sala central |
| `maps/invalid/invalid01.cub` | Mapa com caractere `X` inválido no grid |
| `assets/textures/` | Pasta criada, aguarda PNGs |

### Modificados
| Arquivo | O que mudou |
|---|---|
| `include/cub3d.h` | Structs, enum `t_error`, protótipos, includes |
| `src/utils/utils.c` | `handle_error` com tabela de mensagens, `printf` em vez de `ft_printf` |
| `libft/libft.h` | Protótipos de `get_next_line`, `ft_memdel`, `ft_append_line`, `BUFFER_SIZE` |
| `libft/Makefile` | Novos arquivos adicionados em `MANDATORY` |
| `Makefile` | Novos arquivos do parser em `FILES`, cores, banner |

---

## ✅ Funções implementadas

### `src/parser/file_utils.c`
- **`has_cub_extension`** — valida extensão `.cub`
- **`has_png_extension`** — valida extensão `.png`
- **`trim_newline`** — remove `\n` do final de strings lidas pelo gnl

### `src/parser/parser_meta.c`
- **`is_map_line`** — detecta se uma linha é início do grid (primeiro char é `0`,`1`,`N`,`S`,`E`,`W`,` `)
- **`parse_texture`** — extrai e valida path de textura, faz `ft_strdup`
- **`parse_color`** — placeholder, retorna `ERR_NONE` sem validar ⚠️
- **`dispatch_meta_line`** — identifica identificador e chama `parse_texture` ou `parse_color`
- **`parse_meta`** — loop de leitura dos metadados, para quando detecta `is_map_line`

### `src/parser/parser_map.c`
- **`has_only_valid_chars`** — valida que grid tem só `0`,`1`,`N`,`S`,`E`,`W`,`D`,` `
- **`find_player`** — encontra spawn, popula `t_player`, substitui por `'0'`
- **`parse_map_grid`** — monta o `char**` com `ft_append_line`, valida chars e player

### `src/parser/free_utils.c`
- **`free_map`** — libera `char**`
- **`free_visited`** — libera mapa de visitados do flood fill
- **`free_visited_partial`** — libera parcialmente em caso de erro de malloc

### `src/utils/utils.c`
- **`handle_error`** — tabela estática de mensagens, retorna `ERROR` (-1)

---

## ⚠️ Placeholders — TODO

### `parse_color` — `src/parser/parser_meta.c`
Retorna `ERR_NONE` sem validar. Implementar quando tiver texturas pra testar.
- Usar `ft_split(line, ',')`
- Validar 3 partes, cada uma dígito, range `[0, 255]`
- Guardar em `dest[0]`, `dest[1]`, `dest[2]`
- Após implementar: adicionar sentinela `floor[0] = -1` antes do parse e checar no final do `parse_meta`

### `has_closed_walls` — `src/parser/parser_map.c`
Não implementada — mapa aberto não gera erro ainda.
- Abordagem: verificar vizinhos de cada `0`/`N`/`S`/`E`/`W`/`D`
- Não pode usar a do so_long (mapa não é retangular)
- Chamar ANTES de `find_player` no `parse_map_grid`
- Cuidado com `j+1 < ft_strlen(map[i])` em linhas curtas

### `free_game` — `src/utils/free_utils.c`
Não implementada — necessária para não vazar memória no exit.
- Liberar os 4 paths de textura em `config`
- Liberar `map.grid` com `free_map`

---

## 🗺️ Mapas de teste

### `maps/valid/map01.cub` — 21x13
```
111111111111111111111
100000000000000000001
100000000000000000001
100001111011110000001
100001000010000000001
100001000010000000001
1000010000100001N0001   ← player spawn
100001000010000000001
100001000010000000001
100001111011110000001
100000000000000000001
100000000000000000001
111111111111111111111
```
Dois corredores internos formando cruz, espaço amplo à direita para testar movimento.

### `maps/invalid/invalid01.cub` — ERR_MAP_CHARS
Igual ao válido mas com `X` na linha 6 — deve retornar `ERR_MAP_CHARS`.

### Mapas inválidos ainda por criar
| Arquivo | Erro |
|---|---|
| `invalid02.cub` | `ERR_MAP_PLAYER` — sem spawn |
| `invalid03.cub` | `ERR_MISSING_TEX` — faltando `WE` |
| `invalid04.cub` | `ERR_INVALID_ID` — identificador `XX` |
| `invalid05.cub` | `ERR_MAP_OPEN` — mapa aberto ← após `has_closed_walls` |

---

## 🔌 Como o Fil integra o parser

```c
// main.c — substitui o gridmap hardcoded por:
if (parse_cub(argv[1], &game) != ERR_NONE)
    return (ERROR);

// campos disponíveis:
game.map.grid          // char** — grid do mapa
game.map.rows          // int    — número de linhas
game.player.x          // double — posição x
game.player.y          // double — posição y
game.player.dir        // char   — 'N', 'S', 'E' ou 'W'
game.config.tex_north  // char*  — path textura norte
game.config.tex_south  // char*  — path textura sul
game.config.tex_west   // char*  — path textura oeste
game.config.tex_east   // char*  — path textura leste
```

---

## 💬 Pontos em aberto pra discutir com o Fil

- Build da MLX42 automatizado no Makefile com cmake
- Remover `mlx_ref.c` e defines hardcoded (`MAP_W`, `MAP_H`, `CIRCLE_R` etc)
- MLX42 como submódulo git em vez de pasta commitada
- Padronizar mensagens de commit com conventional commits + gitmoji

---

## 🐛 Bugs encontrados nos testes — `main_test.c`

### Bug 1 — `is_map_line` confundia `N` de spawn com `N` de `NO`

**Sintoma:** `ERR_MISSING_TEX` ao rodar com mapa válido.

**Causa:** a linha `NO ./assets/textures/north.png` começa com `N` —
`is_map_line` retornava `TRUE` imediatamente e o `parse_meta` parava
antes de ler qualquer metadado.

**Correção:** checar o segundo caractere para diferenciar spawn de identificador:
```c
if (line[0] == 'N' || line[0] == 'S'
    || line[0] == 'E' || line[0] == 'W')
    return (line[1] == ' ' || line[1] == '\0' || line[1] == '\n');
```

### Bug 2 — `trim_newline` era chamado depois de `is_map_line`

**Sintoma:** `ERR_INVALID_ID` ao rodar com mapa válido.

**Causa:** `get_next_line` retorna linhas com `\n`. Linhas vazias chegavam
como `"\n"` — não eram detectadas como mapa nem como vazio, e o
`dispatch_meta_line` retornava `ERR_INVALID_ID`.

**Correção:** chamar `trim_newline` antes de qualquer verificação no loop,
e tratar linha vazia explicitamente com `continue`:
```c
while (line)
{
    trim_newline(line);
    if (is_map_line(line))
        break ;
    if (line[0] != '\0')
    {
        err = dispatch_meta_line(line, config);
        ...
    }
    free(line);
    line = get_next_line(fd);
}
```

### Resultado final dos testes
```
./cub3D maps/valid/map01.cub         → Parse bem-sucedido ✅
./cub3D maps/invalid/invalid01.cub   → ERR_MAP_CHARS ✅
./cub3D maps/valid/map01.cub.ber     → ERR_NO_CUB ✅
./cub3D                              → ERR_ARGS ✅
```


---

# 📋 cub3D — Log de Implementação: Parser (continuação)
> Sessão: 11/07/2026 | Branch: `feat/parser-todo`

## 🎯 Objetivo da sessão

Completar os 4 itens pendentes do `parser_todo.md` (parse_color,
has_closed_walls, free_game, mapas inválidos), garantir norminette
limpa, e começar a investigar a integração com o raycasting do Fil.

---

## ✅ Funções implementadas

### `parse_color` — `src/parser/parser_color.c`
- `count_parts`, `check_color_part`, `parse_color`
- Validação: `ft_split` por vírgula, exatamente 3 partes, cada uma
  só dígitos, range `[0, 255]`
- `color_error` (libera `parts` + chama `handle_error`) ficou em
  `free_utils.c`, por analogia com `free_map`/`free_visited`

### `has_closed_walls` — `src/parser/parser_walls.c`
- `is_valid_neighbor`, `check_cell_neighbors`, `has_closed_walls`
- Abordagem: checagem de vizinhos por célula, **não** flood fill
  clássico a partir do player
- Motivo: detecta vazamentos em qualquer célula caminhável do grid,
  mesmo em áreas desconectadas do player — flood fill clássico só
  valida o que é alcançável. Também evita recursão (o flood fill do
  so_long da Mona tinha dado problema de profundidade de pilha antes)
- Cuidado com linhas de tamanhos diferentes: usa `ft_strlen(map[row])`
  por linha, não um valor fixo de colunas

### `free_game` — `src/parser/free_utils.c`
- Libera as 4 texturas de `config` + `map.grid`
- Integrado no `main_test.c` em ambos os caminhos (sucesso e erro
  de parse) para não vazar memória em nenhum dos dois

### `map->cols` — `src/parser/parser_map.c`
- `update_max_cols`, chamada dentro de `build_map_grid`
- Rastreia o comprimento da maior linha (o mapa não é retangular)
- Fechava o último `TODO` pendente do parser

---

## 🏗️ Reorganização por norminette (`TOO_MANY_FUNCS`)

Ao implementar tudo acima, `parser_meta.c` e `parser_map.c`
passaram do limite de 5 funções por arquivo. Reorganização por
responsabilidade:

| Antes | Depois |
|---|---|
| `parser_meta.c` (9 funções) | `parser_meta.c` (5): `is_map_line`, `dispatch_meta_line`, `validate_config`, `read_meta_lines`, `parse_meta` |
| — | `parser_color.c` (novo, 3): `count_parts`, `check_color_part`, `parse_color` |
| `parser_map.c` (7 funções) | `parser_map.c` (5): `has_only_valid_chars`, `find_player`, `update_max_cols`, `build_map_grid`, `parse_map_grid` |
| — | `parser_walls.c` (novo, 3): `is_valid_neighbor`, `check_cell_neighbors`, `has_closed_walls` |
| `file_utils.c` (3) | `file_utils.c` (4): + `parse_texture` (movida de `parser_meta.c`) |
| `free_utils.c` (3) | `free_utils.c` (5): + `color_error`, `free_game` |

`parser_color.c` e `parser_walls.c` exigiram tirar `parse_color` e
`has_closed_walls` de `static`, com prototypes novas em `cub3d.h`.

**Resultado:** norminette 100% limpa em todos os arquivos do parser
(zero `TOO_MANY_FUNCS`, zero `TOO_MANY_LINES`). Os únicos erros
restantes (`PREPROC_CONSTANT`, `LINE_TOO_LONG`, `CONSECUTIVE_NEWLINES`
em `cub3d.h`) são pré-existentes, de antes desta sessão.

---

## 🐛 Bug encontrado e corrigido: mensagem de erro duplicada

**Sintoma:** rodando `invalid02.cub` (mapa sem player), a mensagem
`"Error / invalid number of player"` aparecia **duas vezes**.

**Causa:** `find_player` chamava `handle_error(ERR_MAP_PLAYER)`
internamente, **e** `parse_map_grid` chamava `handle_error` de novo
em cima do retorno:
```c
// find_player, ao não achar spawn:
return (handle_error(ERR_MAP_PLAYER));  // já imprime aqui

// parse_map_grid:
if (find_player(map->grid, player) != ERR_NONE)
    return (handle_error(ERR_MAP_PLAYER));  // imprime de novo!
```
Inconsistente com `has_only_valid_chars` e `has_closed_walls`, que
só retornam booleano e deixam o `handle_error` único pro
`parse_map_grid` chamar.

**Correção:** `find_player` passou a retornar `ERR_MAP_PLAYER` puro,
sem chamar `handle_error` — mesma responsabilidade das funções irmãs:
```c
return (ERR_MAP_PLAYER);
```

---

## 🗺️ Mapas de teste criados

| Arquivo | Testa |
|---|---|
| `invalid02.cub` | `ERR_MAP_PLAYER` — sem spawn no grid |
| `invalid03.cub` | `ERR_MISSING_TEX` — faltando linha `WE` |
| `invalid04.cub` | `ERR_INVALID_ID` — identificador `XX` desconhecido |
| `invalid05.cub` | `ERR_MAP_OPEN` — buraco na borda externa |
| `invalid06_space.cub` | `ERR_MAP_OPEN` — espaço interno (não é borda) |
| `subject_map.cub` | Mapa oficial do capítulo V do subject — linhas com indentação irregular e espaços internos no meio de linhas curtas; fixture de validação mais forte que temos |

Todos os 4 tipos de erro do `parser_todo.md` original agora têm
cobertura de teste. `subject_map.cub` foi feito pela própria Mona,
copiando o exemplo do PDF do subject.

---

## 🔧 Nova ferramenta: `make test`

O `main.c` atual é o POC do minimap do Fil (não chama `parse_cub`),
então não dá pra testar o parser via `./cub3D`. Adicionado target
no `Makefile`:
```makefile
test: $(TEST_OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(TEST_OBJS) $(LIBFT) $(MLX) $(GLFW) \
		-framework Cocoa -framework OpenGL -framework IOKit -o $(TEST_NAME)
```
Compila `main_test.c` + objetos do parser/utils num binário
separado (`test_parser`), sem tocar em `main.c`/`render.c`. `fclean`
também limpa esse binário. `test_parser` **não é commitado** (não
está no `.gitignore` ainda, mas é apagado manualmente após cada uso).

---

## 🎨 Tentativas com texturas (não finalizado)

1. **`bricks.db32.png`** — imagem baixada de um site de assets pela
   Mona. Grid de 4 colunas × 8 linhas (confirmado por crop +
   inspeção visual), tiles de 44×42px, 32 cores da paleta
   DawnBringer. Apagada ao final da sessão — decisão de textura
   final adiada.
2. **Assets do so_long**
   (`/Users/mona/Documents/42-2025/so_long/assets/tiles/tile-wall-square.png`)
   — textura de tijolo pronta do projeto anterior, mas só 1
   variante (não 4 direções diferentes). Considerada, não usada.
3. **Placeholders temporários** — 4 PNGs RGBA sólidos (cores
   distintas), gerados via `magick`, em `assets/textures/`. Servem
   só pra estabelecer a estrutura de diretórios que os `.cub`
   referenciam. **Mantidos no repo** — decisão explícita da Mona:
   estrutura correta importa mesmo com conteúdo placeholder.
4. **Limitação técnica encontrada:** a renderização de imagem (via
   `desktop-commander:read_file`) parou de funcionar no meio da
   sessão — funcionou pra `bricks.db32.png` no início, depois parou
   de funcionar até pra essa mesma imagem, sem causa identificada.
   Pendente revisitar na próxima sessão.

---

## 🔌 Investigação de integração com `feat/render`

Verificado via `git merge-base main feat/parser-todo` e
`git merge-base main origin/feat/render`: as duas branches
compartilham o **mesmo ancestral comum** (`2bd255b`) — ou seja, o
Fil nunca criou a branch de integração a partir da `feat/parser-todo`
como combinado inicialmente; ele seguiu direto na `feat/render`,
criada a partir da `main` desde o início.

Branch do Fil já tem bastante coisa: raycasting funcionando,
minimap, colisão, pan/walk (isso já conta como bonus). Diff vs
`main`: ~666 linhas adicionadas, incluindo mudanças grandes em
`cub3d.h` (117 linhas) e reescrita de `main.c`.

**Plano de integração revisado** (documentado com detalhes em
`docs/next_session_todo.md`):
1. Mona faz reconhecimento numa scratch branch a partir da
   `feat/parser-todo` (não da `main`, pra não mexer numa branch
   compartilhada sem alinhar com o Fil primeiro)
2. Corrige o que precisar direto na `feat/parser-todo`
3. Só depois de uma call com o Fil, decide o timing do merge pra
   `main` e como ele reconcilia a `feat/render` dele

---

## 📌 Estado final da sessão

Commits com push feito (branch `feat/parser-todo`):
- `0b63cf2` — parse_color inicial
- `9b815f0` — has_closed_walls + free_game
- `712a076` — reorganização por norminette + bugfix double-print
- `bf9f983` — subject_map.cub + placeholders de textura
- `0fd040d` — map->cols

`docs/next_session_todo.md` criado e commitado — próxima sessão foca
em integração com o Fil e texturas reais.

**Os 4 itens do `parser_todo.md` original estão completos:**
`parse_color` ✅ · `has_closed_walls` ✅ · `free_game` ✅ ·
mapas inválidos ✅ (6 cenários cobertos, incluindo o exemplo oficial
do subject).
