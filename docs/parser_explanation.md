# 🗺️ Parser do cub3D — Guia Completo

Referência do parser **como ele é hoje**: cada arquivo, cada função,
cada validação, cada regra de memória.

Os conceitos gerais que sustentam essas decisões (ownership, sentinela,
`get_next_line` destrutiva, vizinhos vs flood fill, coordenada contínua
sobre grid discreto, categorias de leak) estão em
[`parser_concepts.md`](parser_concepts.md) — este documento aponta pra lá
sempre que um deles aparece.

Pra estudar isso numa ordem, ver [`parser_study_plan.md`](parser_study_plan.md).

---

## 📚 Índice

1. [O que o parser entrega](#1-o-que-o-parser-entrega)
2. [Estruturas de dados](#2-estruturas-de-dados)
3. [Arquivos e funções](#3-arquivos-e-funções)
4. [O pipeline completo](#4-o-pipeline-completo)
5. [Camada de entrada — `parse_cub`](#5-camada-de-entrada--parse_cub)
6. [Camada de metadados — `parse_meta`](#6-camada-de-metadados--parse_meta)
7. [Camada de mapa — `parse_map_grid`](#7-camada-de-mapa--parse_map_grid)
8. [Tratamento de erros](#8-tratamento-de-erros)
9. [Memória](#9-memória)
10. [Onde o parser termina](#10-onde-o-parser-termina)
11. [Mapas de teste](#11-mapas-de-teste)
12. [Pontos fracos conhecidos](#12-pontos-fracos-conhecidos)

---

## 1. O que o parser entrega

Um arquivo `.cub` descreve uma cena: 4 texturas de parede (uma por
direção), a cor do chão e do teto, e o mapa (um grid de texto). O parser
lê esse arquivo e transforma texto solto em dados confiáveis.

O contrato é: **se `parse_cub` retorna `ERR_NONE`, o resto do programa
pode confiar em tudo.** Nenhuma função de render revalida nada.

**Pós-condições garantidas:**

| garantia | quem garante |
|---|---|
| As 4 texturas são não-NULL e terminam em `.png` | `validate_config` + `parse_texture` |
| `floor` e `ceil` têm 3 valores em `[0,255]` | `validate_config` + `check_color_part` |
| `grid` só contém `0`, `1` e espaço | `has_only_valid_chars` + `find_player` |
| Nenhuma célula andável toca espaço ou borda | `has_closed_walls` |
| Existe **exatamente um** spawn, já convertido em `'0'` | `find_player` |
| `player.x/y` está no centro de um tile de chão | `take_row_spawns` |
| **Toda linha tem exatamente `cols` chars** (grid retangular) | `pad_grid` |

A última é a mais fácil de esquecer e a mais usada: o raycaster indexa
`grid[y][x]` com um bounds check de `rows`/`cols` e nada mais. Sem o
`pad_grid`, linha curta = leitura fora do buffer.

Isso envolve três problemas clássicos de parsing em C:

- **Ler linha por linha** de um arquivo sem tamanho fixo (`get_next_line`)
- **Validar sem confiar** — qualquer linha pode estar malformada
- **Não vazar memória** nos caminhos de erro, com early return no meio
  de uma leitura

---

## 2. Estruturas de dados

### `t_config` — texturas + cores

```c
typedef struct s_config
{
	char	*tex_north;
	char	*tex_south;
	char	*tex_west;
	char	*tex_east;
	int		floor[3];   // RGB
	int		ceil[3];    // RGB
}	t_config;
```

### `t_map` — o grid

```c
typedef struct s_map
{
	char	**grid;   // NULL-terminated, uma string por linha
	int		rows;
	int		cols;     // linha mais longa (mapa não é retangular)
}	t_map;
```

> Ver [`parser_concepts.md` §1](parser_concepts.md#1-arrays-2d-dinâmicos-char-)
> sobre o `char **` dinâmico.

### `t_player` — spawn

```c
typedef struct s_player
{
	double	x;        // world space, em tiles — centro do tile, não a quina
	double	y;
	char	dir;      // 'N', 'S', 'E' ou 'W'
	float	dir_ang;  // ângulo em graus, derivado de dir (ver main.c)
}	t_player;
```

### `t_game` — junta tudo

O parser popula **só** `map`, `config` e `player`. O resto é território
do render, listado aqui só pra situar:

```c
typedef struct s_game
{
	mlx_t			*mlx;
	mlx_image_t		*map_img;
	mlx_image_t		*main_img;
	uint8_t			*map_pixels_buf;
	mlx_texture_t	*tex[4];
	uint32_t		ceil_rgba;
	uint32_t		floor_rgba;
	int32_t			horizon;
	double			proj_plane;
	t_ray			rays[RAY_COUNT];
	bool			show_minimap;
	bool			show_rays;
	bool			show_tex;
	t_map			map;      // ← parser popula
	t_minimap		minimap;
	t_config		config;   // ← parser popula
	t_player		player;   // ← parser popula
}	t_game;
```

---

## 3. Arquivos e funções

O parser são **8 arquivos, 32 funções**. A norminette limita 5 funções
por arquivo — **três estão no teto**, o que restringe onde dá pra
adicionar coisa nova.

| arquivo | funções | ocupação |
|---|---|---|
| [`parser.c`](../src/parser/parser.c) | `close_and_drain`, `parse_cub` | 2/5 |
| [`parser_meta.c`](../src/parser/parser_meta.c) | `is_map_line`, `dispatch_meta_line`, `validate_config`, `read_meta_lines`, `parse_meta` | **5/5** |
| [`file_utils.c`](../src/parser/file_utils.c) | `has_cub_extension`, `has_png_extension`, `trim_newline`, `parse_texture`, `dispatch_tex` | **5/5** |
| [`parser_color.c`](../src/parser/parser_color.c) | `count_parts`, `check_color_part`, `parse_color`, `dispatch_color` | 4/5 |
| [`parser_map.c`](../src/parser/parser_map.c) | `has_only_valid_chars`, `find_player`, `update_max_cols`, `build_map_grid`, `parse_map_grid` | **5/5** |
| [`parser_grid.c`](../src/parser/parser_grid.c) | `grow_row`, `pad_grid`, `take_row_spawns` | 3/5 |
| [`parser_walls.c`](../src/parser/parser_walls.c) | `is_valid_neighbor`, `check_cell_neighbors`, `has_closed_walls` | 3/5 |
| [`free_utils.c`](../src/parser/free_utils.c) | `free_map`, `free_visited_partial`, `free_visited`, `color_error`, `free_game` | **5/5** |

Fora do parser, mas usado por ele: [`utils.c`](../src/utils/utils.c) —
`handle_error`, `is_solid`, `is_spawn`.

> 💡 É por isso que `take_row_spawns` mora no `parser_grid.c` e não ao
> lado do `find_player`, que é quem o usa: o `parser_map.c` já estava
> em 5/5.

---

## 4. O pipeline completo

> Este diagrama cobre **só a parte do parser**, de `main` até o
> `parse_cub` retornar. Pro fluxo de ponta a ponta — incluindo
> `render`, o `mlx_loop` e o teardown — ver
> [**Execution Flow** no README](../README.md#-execution-flow).

```
main.c
 │  argc != 2 ────────────────────────────────► ERR_ARGS
 ├─ ft_memset(&game, 0, sizeof(game))    ← zera tudo: sentinela de free
 └─ parse_cub(argv[1], &game)
     │
     ├─ has_cub_extension() ─────────────────► ERR_NO_CUB
     ├─ open(path, O_RDONLY) ────────────────► ERR_NO_FILE
     │
     ├─ parse_meta(fd, &game->config, &first_line)
     │   ├─ floor[0] = ceil[0] = -1          ← sentinela de "não lido"
     │   ├─ read_meta_lines()
     │   │   └─ loop get_next_line:
     │   │       ├─ trim_newline()
     │   │       ├─ is_map_line()? ──► break, guarda em first_line
     │   │       ├─ linha vazia? ────► pula
     │   │       └─ dispatch_meta_line()
     │   │            ├─ "NO "/"SO "/"WE "/"EA " → dispatch_tex()
     │   │            │                            ├─ já setado? ► ERR_DUPLICATE_ID
     │   │            │                            └─ parse_texture()
     │   │            │                                ├─ path vazio ► ERR_MISSING_TEX
     │   │            │                                ├─ não é .png ► ERR_MISSING_TEX
     │   │            │                                └─ ft_strdup()
     │   │            ├─ "F "/"C " ──────────────► dispatch_color()
     │   │            │                            ├─ != -1? ► ERR_DUPLICATE_ID
     │   │            │                            └─ parse_color()
     │   │            │                                ├─ ft_split(',')
     │   │            │                                ├─ != 3 partes ► ERR_INVALID_COLOR
     │   │            │                                └─ check_color_part() x3
     │   │            │                                    ├─ não-dígito ► ERR_INVALID_COLOR
     │   │            │                                    └─ fora 0-255 ► ERR_INVALID_COLOR
     │   │            └─ nada casou ─────────────► ERR_INVALID_ID
     │   └─ validate_config()
     │       ├─ alguma textura NULL ─────────► ERR_MISSING_TEX
     │       └─ floor[0] ou ceil[0] == -1 ───► ERR_INVALID_COLOR
     │
     ├─ parse_map_grid(fd, &game->map, first_line, &game->player)
     │   ├─ build_map_grid()
     │   │   └─ loop até EOF: trim → update_max_cols → ft_append_line
     │   ├─ has_only_valid_chars() ──────────► ERR_MAP_CHARS
     │   ├─ has_closed_walls() ──────────────► ERR_MAP_OPEN
     │   ├─ find_player() ───────────────────► ERR_MAP_PLAYER
     │   └─ pad_grid() ──────────────────────► ERR_MALLOC
     │
     └─ close_and_drain(fd)   ← em TODOS os caminhos
```

### A ordem das validações do mapa não é arbitrária

1. **`has_only_valid_chars` primeiro** — não faz sentido analisar
   geometria de um grid com lixo dentro.
2. **`has_closed_walls` antes de `find_player`** — ele ainda precisa
   enxergar `N`/`S`/`E`/`W` como célula andável. Depois do `find_player`
   o spawn já virou `'0'`, o que daria no mesmo resultado, mas a ordem
   atual mantém a lista de caracteres andáveis explícita e legível.
3. **`pad_grid` por último** — assim as validações veem o arquivo
   exatamente como foi escrito, sem espaços que o próprio programa
   inventou. Está documentado no código, em
   [parser_grid.c:61-62](../src/parser/parser_grid.c#L61-L62).

---

## 5. Camada de entrada — `parse_cub`

### `has_cub_extension` / `has_png_extension`

Comparam os **4 últimos caracteres**. Rejeitam NULL e nomes com menos
de 4 chars.

> ⚠️ Consequência: um arquivo chamado exatamente `.cub` (4 chars, sem
> nome) passa.

### `close_and_drain` — o leak que não é óbvio

```c
line = get_next_line(fd);
while (line) { free(line); line = get_next_line(fd); }
close(fd);
```

O `get_next_line` guarda um **buffer estático de leftover por fd**, e
só libera esse buffer quando lê EOF de verdade. Qualquer caminho que
pare no meio do arquivo — um erro de parsing — e simplesmente chame
`close(fd)` deixa esse buffer órfão pra sempre: o `close()` não sabe
nada do heap interno do GNL.

Drenar até EOF antes de fechar força o caminho de free-no-EOF do
próprio GNL a rodar, **seja qual for a implementação** linkada.

> Ver [`parser_concepts.md` §4](parser_concepts.md#4-get_next_line-é-destrutiva).

### `first_map_line` — por que existe, e o leak que ele causou

O `get_next_line` é destrutivo: uma vez lida, a linha não volta pro fd.
Quando o `read_meta_lines` detecta que a linha atual já é o início do
mapa, ela já foi consumida. A solução é um out-param: `parse_meta`
recebe `char **first_map_line`, preenche antes de retornar, e o
`parse_map_grid` recebe essa linha pronta.

**A pegadinha:** guardar a linha num out-param resolve "não perder o
dado", mas não decide **quem é dono dela**. O `read_meta_lines` pode
ter sucesso (achou a linha do mapa, preencheu o ponteiro) e o
`parse_meta` mesmo assim falhar logo depois, no `validate_config` —
por exemplo, faltou a textura `WE`. Nesse caminho a linha já estava
alocada e ninguém nunca era dono.

Corrigido inicializando `first_line = NULL` no topo do `parse_cub`
([parser.c:75](../src/parser/parser.c#L75)) e dando `free(first_line)`
no ramo de erro. Como `free(NULL)` é no-op, é seguro chamar sempre.

O `invalid03_missing_tex.cub` é o mapa de teste que passa exatamente
por esse caminho.

---

## 6. Camada de metadados — `parse_meta`

### `is_map_line` — a sutileza dos identificadores

```c
if (line[0] == '1' || line[0] == '0' || line[0] == ' ')
    return (TRUE);
if (line[0] == 'N' || line[0] == 'S' || line[0] == 'E' || line[0] == 'W')
    return (line[1] == ' ' || line[1] == '\0' || line[1] == '\n');
return (FALSE);
```

**Por que os cardeais precisam checar `line[1]`?** Porque os quatro
identificadores de textura começam com um cardeal:

```
NO → 'N'      SO → 'S'      WE → 'W'      EA → 'E'
```

Sem o segundo teste, a linha `NO ./assets/north.png` seria lida como
"começou o mapa" e todo o resto do arquivo iria pro grid.

> 🔍 **Nuance:** uma linha de mapa que comece com spawn seguido de
> dígito (`N0001...`) tem `line[1] == '0'` e **não** é reconhecida como
> mapa — cai em `dispatch_meta_line` e vira `ERR_INVALID_ID`. Na
> prática isso nunca acontece num mapa válido: a primeira linha do mapa
> tem que ser toda parede, senão o `has_closed_walls` reprova por
> célula andável na borda. O arquivo é recusado nos dois casos, só muda
> a mensagem.

### Ordem livre, duplicata proibida

O `dispatch_meta_line` é uma cadeia de `ft_strncmp`. As 6 chaves podem
vir em **qualquer ordem**, separadas por quantas linhas vazias quiser
(`if (line[0] != '\0')` pula as vazias) — exatamente o que o subject
permite.

O que **não** pode é repetir:

```c
int dispatch_tex(const char *line, char **dest)     // *dest != NULL → já veio
int dispatch_color(const char *line, int dest[3])   // dest[0] != -1 → já veio
```

Sem essas duas guardas, a segunda linha `NO` sobrescreveria o ponteiro
da primeira e **vazaria** a string já alocada. É um leak que o valgrind
pega, e a guarda é mais barata que o free.

### As duas sentinelas

`parse_meta` começa com `config->floor[0] = -1` e `ceil[0] = -1`. Como
`check_color_part` só aceita `[0, 255]`, o `-1` é um valor que nenhum
parse bem-sucedido produz. Ele responde **duas** perguntas com o mesmo
teste:

| pergunta | quem faz | teste |
|---|---|---|
| "essa cor chegou a vir?" | `validate_config` | `floor[0] == -1` |
| "essa cor veio duas vezes?" | `dispatch_color` | `dest[0] != -1` |

As texturas usam `NULL` como a mesma sentinela — só que essa vem de
graça do `ft_memset(&game, 0, sizeof(game))` no `main`.

> Ver [`parser_concepts.md` §3](parser_concepts.md#3-valores-sentinela).

### `parse_texture` — o que ele **não** faz

Pula os 2 chars do id, pula espaços, exige path não-vazio e terminando
em `.png`, e faz `ft_strdup`.

**Ele não abre o arquivo.** Textura que não existe no disco só falha
depois, no `load_textures` — ver [§10](#10-onde-o-parser-termina).

### `parse_color` e a regra de ownership

```c
parts = ft_split(line, ',');            // parse_color vira DONO de parts
if (count_parts(parts) != 3)
    return (color_error(parts));        // libera + reporta
...
    if (check_color_part(parts, i, dest) != ERR_NONE)
        return (color_error(parts));    // libera + reporta
free_map(parts);
```

`check_color_part` retorna `ERR_INVALID_COLOR` **cru**, sem chamar
`handle_error`. Ele não é dono do `parts` e não decide nada — só
informa. Quem libera e quem reporta é o dono, via `color_error`
([free_utils.c:90](../src/parser/free_utils.c#L90)), que faz as duas
coisas numa chamada só pra não repetir o par em quatro lugares.

Cada componente passa por: pular espaços à esquerda → contar os dígitos
→ pular espaços à direita → exigir que a string tenha **acabado**. Só
então o `ft_atoi` roda e o valor é testado contra `[0,255]`.

Tolerar espaço em volta é o que faz `C 255,214, 178` parsear igual a
`C 255,214,178`. O subject diz que as informações de um elemento podem
ser separadas por um ou mais espaços, sem definir se R, G e B contam
como informações separadas — aceitar não custa nada, já que espaço
dentro de uma cor não tem outro significado possível.

O `*s != '\0'` no fim é o que impede a tolerância de ir longe demais:
`2 20` continua sendo recusado, porque o espaço no meio deixa
caracteres por consumir. Espaço em volta, sim; espaço no meio do
número, não.

> Ver [`parser_concepts.md` §2](parser_concepts.md#2-ownership-de-memória).

---

## 7. Camada de mapa — `parse_map_grid`

### `build_map_grid` + `ft_append_line`

Lê do `first_map_line` até EOF, e pra cada linha: `trim_newline` →
`update_max_cols` → `ft_append_line`.

O [`ft_append_line`](../libft/ft_append_line.c) realoca o array inteiro
a cada linha (`ft_calloc(count + 2, ...)`), copia os ponteiros antigos,
dá `ft_strdup` na linha nova e libera **só o array antigo** — os strings
antigos são reaproveitados (shallow copy). Por isso o `build_map_grid`
dá `free(line)` logo depois: a cópia já foi feita.

É O(n²) no número de linhas. Pra mapa de defesa, irrelevante.

### `update_max_cols` — por que `cols` existe

Mapa de cub3D **não é retangular**. O `subject_map.cub` tem linhas de
33, 31 e 29 chars. O `cols` guarda a mais longa, e é o alvo do
`pad_grid`.

### `has_only_valid_chars`

Aceita `0`, `1`, `N`, `S`, `E`, `W`, `' '`. Qualquer outra coisa →
`ERR_MAP_CHARS`.

É também o que rejeita **metadado depois do mapa**: uma linha
`NO ./x.png` no fim do arquivo entra no grid e morre aqui, porque tem
`.`, `/` e minúsculas. Recusa correta, mensagem um pouco enviesada.

### `has_closed_walls` — vizinho, não perímetro

A diferença mais importante em relação ao so_long:

```
so_long: mapa retangular  → basta checar as 4 bordas
cub3D:   linhas ragged    → checa VIZINHO DE CADA CÉLULA ANDÁVEL
```

Pra toda célula andável (`0`, `N`, `S`, `E`, `W`), os **4 vizinhos
cardeais** precisam ser válidos. `is_valid_neighbor` retorna `FALSE` em
três casos:

```c
if (row < 0 || row >= rows)                        return (FALSE);  // fora do grid
if (col < 0 || (size_t)col >= ft_strlen(map[row])) return (FALSE);  // fora DAQUELA linha
if (map[row][col] == ' ')                          return (FALSE);  // buraco
```

O do meio é o que faz funcionar com linhas de tamanhos diferentes: usa
`ft_strlen(map[row])`, o comprimento **real daquela linha**, e não
`map->cols`. Se usasse `cols`, uma linha curta pareceria ter células
válidas onde não há nada.

Isso pega **os dois** tipos de abertura com o mesmo código: buraco na
borda (`invalid05_open_border.cub`) e espaço no meio do grid
(`invalid06_open_space.cub`).

> **Linhas de comprimento diferente NÃO são erro.** O subject só exige
> "closed/surrounded by walls". Um mapa ragged onde nenhum `0` encosta
> em espaço ou borda é válido, e passar é o comportamento correto.

> Ver [`parser_concepts.md` §5](parser_concepts.md#5-vizinhos-vs-flood-fill)
> pro porquê de não ser flood fill.

### `find_player` + `take_row_spawns` — exatamente um

```c
count = 0;
while (map[i]) { count += take_row_spawns(map[i], player, i); i++; }
if (count != 1) return (ERR_MAP_PLAYER);
```

O [`take_row_spawns`](../src/parser/parser_grid.c#L110) varre uma linha
e pra cada spawn: grava `x`, `y`, `dir` e **reescreve a célula pra
`'0'`**. Ele não para no primeiro — se parasse, uma linha com dois
spawns contaria 1 e passaria.

**Por que contar tudo?** O subject fala em *"the player's start
position"*, singular, e não define comportamento pra um segundo spawn.
Contar e exigir exatamente 1 rejeita o mapa sem spawn e o mapa com dois
**pelo mesmo teste**, sem branch separado.

**O que acontecia antes (corrigido em 09/08):** o `find_player`
retornava no primeiro spawn encontrado. O segundo ficava no grid como
`'S'`, e como `is_solid` é `return (c != '0')`, ele virava **parede
fantasma no meio da sala** — texturizada normalmente pelo lado do raio,
sólida no minimapa, invisível como erro. O mapa abria numa boa.

### O `+ 0.5` — centro do tile

`player.x/y` são world space medido em tiles. A célula `(j, i)` ocupa o
quadrado de `j` a `j+1`. **Coordenada inteira é a quina, não o meio.**

Sem o `+0.5`, duas coisas quebram:

**Colisão.** `will_collide` testa uma caixa centrada em `(x,y)` com
`PL_RADIUS = 0.25`:

| posição | `box.left` | `box.right` | resultado |
|---|---|---|---|
| `5.5` | `floor(5.25)` = 5 | `floor(5.75)` = 5 | caixa inteira dentro da célula do spawn — que é chão por definição |
| `5.0` | `floor(4.75)` = 4 | `floor(5.25)` = 5 | espalha por 4 células; se qualquer uma for parede, o jogador nasce **travado pra sempre** |

**Raycasting.** `calc_step_and_side` calcula
`side_dist.x = (pl->x - map_pos.x) * delta_dist.x`. Com `x = 5.0` isso
dá **zero**: o DDA dá o primeiro passo sem percorrer distância nenhuma,
e `hit_dist = side_dist.x - delta_dist.x` sai `0`. Divisão por zero na
altura da parede.

> Ver [`parser_concepts.md` §9](parser_concepts.md#9-coordenada-contínua-sobre-grid-discreto).

### `pad_grid` + `grow_row`

Cresce toda linha curta até `cols`, preenchendo com **espaço** — que o
`is_solid` já trata como bloqueante, então não abre buraco nenhum.

Torna "o grid é retangular" uma **pós-condição do parser**. Todo
consumidor lá na frente indexa `grid[y][x]` com bounds check de
`rows`/`cols` e nada mais.

---

## 8. Tratamento de erros

`handle_error` ([utils.c:45](../src/utils/utils.c#L45)) é uma lookup
table indexada pelo enum — O(1), sem cadeia de `if`. Todas as mensagens
seguem `"Error\n<descrição>\n"`, como o subject exige.

| enum | mensagem | disparado por |
|---|---|---|
| `ERR_ARGS` | invalid arguments | `argc != 2` |
| `ERR_NO_FILE` | invalid file | `open()` falhou |
| `ERR_NO_CUB` | missing .cub extension | `has_cub_extension` |
| `ERR_MALLOC` | allocation with malloc failed | `ft_strdup`/`ft_split`/`grow_row` |
| `ERR_MAP_CHARS` | map has invalid characters | `has_only_valid_chars` |
| `ERR_MAP_OPEN` | map is not closed by walls | `has_closed_walls` |
| `ERR_MAP_PLAYER` | map must have exactly one player spawn (N/S/E/W) | `find_player` |
| `ERR_MISSING_TEX` | failed to load/missing texture: (NO/SO/WE/EA) | `parse_texture`, `validate_config` |
| `ERR_INVALID_COLOR` | invalid RGB | `parse_color`, `validate_config` |
| `ERR_INVALID_ID` | invalid identifier in .cub | `dispatch_meta_line` |
| `ERR_MLX` | MLX42 initialization failed | `config_mlx` (não é do parser) |
| `ERR_DUPLICATE_ID` | duplicate identifier in .cub | `dispatch_tex`, `dispatch_color` |

### Cada erro imprime uma vez só

Já houve um bug de mensagem duplicada aqui. A disciplina atual:

- **Camada de metadados** — cada função chama `handle_error` ela mesma
  e o código sobe intacto até o `main`.
- **Camada de mapa** — os validadores retornam bool ou código cru, e
  **só** o `parse_map_grid` chama `handle_error`.

É por isso que o `find_player` retorna `ERR_MAP_PLAYER` puro: o
`parse_map_grid` já reporta na
[linha 182](../src/parser/parser_map.c#L182). Se ele também reportasse,
sairiam duas mensagens.

> ✅ Verificável: `./cub3D maps/invalid/<qualquer>.cub | wc -l` dá
> sempre **2** — a linha `Error` mais a descrição.

> 🔍 `handle_error` retorna `ERROR` (`-1`), não o código do enum. Os
> `@retval ERR_MISSING_TEX` dos docstrings descrevem a *causa*, não o
> valor literal — o que sobe é sempre `-1`. Funciona porque todo mundo
> só testa `!= ERR_NONE`.

---

## 9. Memória

### Quem libera o quê

| alocação | dono | liberado em |
|---|---|---|
| `line` do `get_next_line` | quem chamou | `free(line)` no próprio loop |
| leftover estático do GNL | o GNL | `close_and_drain` (drenar até EOF) |
| `first_map_line` | `parse_cub` | `free(first_line)` no erro; consumido pelo `build_map_grid` no sucesso |
| `parts` do `ft_split` | `parse_color` | `color_error` ou `free_map` |
| 4 paths de textura | `t_config` | `free_game` |
| `map.grid` | `t_map` | `free_game` → `free_map` |
| `mlx_texture_t*` | `t_game.tex[4]` | `free_textures` (camada de render) |

### `free_game` cobre os três caminhos do `main`

```c
if (parse_cub(...) != ERR_NONE) { free_game(&game); return (ERROR); }  // erro de parse
...
if (!render(&game))             { free_game(&game); return (ERROR); }  // erro de render
free_game(&game);                                                      // sucesso
```

O caminho de erro do parser **não tinha** o `free_game` até 09/08 — os
paths de textura já guardados no `config` vazavam em todo mapa
inválido, entre 40 e 240 bytes conforme o mapa.

O `ft_memset(&game, 0, sizeof(game))` na entrada do `main` é o que
torna isso seguro: como todo campo começa zerado e `free(NULL)` é
no-op, dá pra chamar `free_game` em **qualquer** ponto de falha do
parser sem saber até onde ele chegou.

### Demonstrar memória: use um mapa inválido

Rodando um mapa **válido**, o valgrind reporta ~313 KB em ~3.000 blocos
`still reachable`. Isso é o MLX42 → GLFW → X11/Mesa, não é leak.

Rodando um mapa **inválido**, a MLX nem inicializa:

```
$ valgrind --leak-check=full ./cub3D maps/invalid/invalid02_no_spawn.cub
==...== in use at exit: 0 bytes in 0 blocks
```

É o único jeito de mostrar zero absoluto.

> Ver [`parser_concepts.md` §10](parser_concepts.md#10-categorias-de-leak-do-valgrind).

---

## 10. Onde o parser termina

O parser **não abre PNGs, não inicializa MLX, não desenha nada**. Ele:

1. Valida e guarda os *paths* das texturas (não-vazio, extensão `.png`)
2. Valida e guarda as cores (RGB em `[0,255]`)
3. Monta o grid e acha o player

| camada | onde | o que faz |
|---|---|---|
| **Parsing** | `src/parser/` | valida o *path* — sintaxe e extensão |
| **Loading** | [`src/render/texture_load.c`](../src/render/texture_load.c) | `mlx_load_png` → `game->tex[4]` |
| **Rendering** | [`src/render/draw_tex_view.c`](../src/render/draw_tex_view.c) | `tex[col->face]`, sampling por coluna |

O `load_textures` roda **antes** do `config_mlx` — `mlx_load_png` só
decodifica bytes, não precisa de `mlx_t*`. Falha libera o que já
carregou (`free_textures`) antes de reportar `ERR_MISSING_TEX`.

O `tex[4]` é indexado pelo enum `t_face` (`F_NORTH`, `F_SOUTH`,
`F_WEST`, `F_EAST`), e não por 4 campos nomeados — assim o render
escolhe a textura com `tex[col->face]` em vez de um branch de 4 vias
por coluna.

> Ver [`parser_concepts.md` §8](parser_concepts.md#8-decode-vs-contexto-gráfico-mlx42).

---

## 11. Mapas de teste

```
maps/invalid/
  invalid01_chars.cub        ERR_MAP_CHARS      caractere 'X' no grid
  invalid02_no_spawn.cub     ERR_MAP_PLAYER     nenhum spawn
  invalid03_missing_tex.cub  ERR_MISSING_TEX    falta a linha WE
  invalid04_invalid_id.cub   ERR_INVALID_ID     identificador 'XX'
  invalid05_open_border.cub  ERR_MAP_OPEN       buraco na borda de baixo
  invalid06_open_space.cub   ERR_MAP_OPEN       espaço interno colado no andável
  invalid07_two+_spawns.cub  ERR_MAP_PLAYER     três spawns (E, N, S)
```

Um mapa por regra, e cada um **válido em todo o resto** — senão ele
morre numa validação anterior e não testa o que deveria. O
`invalid07_two+_spawns.cub` é o exemplo: precisou ser um mapa
perfeitamente fechado, com todas as texturas e cores, só com spawn
sobrando.

### Casos de borda verificados

| caso | resultado |
|---|---|
| Metadado **depois** do mapa | `ERR_MAP_CHARS` — recusa |
| Linha em branco **no meio** do mapa | `ERR_MAP_OPEN` — recusa (a linha vazia vira row de tamanho 0, e os vizinhos ficam inválidos) |
| Linha em branco **no fim** do arquivo | **Aceito** — vira row vazia que o `pad_grid` preenche de espaço; espaço é sólido, então é inofensivo |
| Linhas de comprimentos diferentes | **Aceito** — é o caso normal |
| `F 220, 100, 0` (espaço após a vírgula) | **Aceito** — espaço em volta do número é tolerado |
| `F 2 20,100,0` (espaço no meio do número) | `ERR_INVALID_COLOR` — recusa |

### Comandos

```bash
# todos os inválidos de uma vez
for m in maps/invalid/*.cub; do
    printf "%-30s -> " "$(basename $m)"; ./cub3D "$m" 2>&1 | tail -1
done

# todos os válidos abrem? (124 = timeout matou a janela = abriu)
for m in maps/valid/*.cub; do
    timeout 2 ./cub3D "$m" >/dev/null 2>&1
    echo "$(basename $m): $?"
done

# memória — sempre num mapa inválido
valgrind --leak-check=full ./cub3D maps/invalid/invalid07_two+_spawns.cub

# binário só-parser, sem abrir janela
make test && ./test_parser maps/valid/subject_map.cub
```

---

## 12. Pontos fracos conhecidos

Duas coisas que a gente sabe e assume. Melhor citar antes de perguntarem.

**1. `has_closed_walls` só olha as 4 cardeais, sem diagonais.**
Num DDA, um raio exatamente diagonal pode atravessar o vértice entre
duas paredes que se tocam só na quina. É caso de canto; alguns
avaliadores testam.

