# 🗺️ Parser do cub3D - Guia Completo

## 📚 O Que é o Parser do cub3D?

Um arquivo `.cub` descreve uma cena: 4 texturas de parede (uma por
direção), a cor do chão e do teto, e o mapa em si (um grid de texto).
O parser é a camada que lê esse arquivo e transforma texto solto em
dados confiáveis — se `parse_cub` retorna `ERR_NONE`, o resto do
programa (render, raycasting) pode confiar 100% no conteúdo de
`t_game` sem revalidar nada.

Isso envolve resolver 3 problemas clássicos de parsing em C:
- **Ler linha por linha** de um arquivo sem tamanho fixo (`get_next_line`)
- **Validar sem confiar** no formato — qualquer linha pode estar malformada
- **Não vazar memória** mesmo nos caminhos de erro (early return no meio de uma leitura)

---

## 🎯 Estrutura do Projeto

### Arquivos do parser (`src/parser/`)

| Arquivo | O que tem |
|---|---|
| `parser.c` | `parse_cub` — entry point, orquestra tudo; `close_and_drain` — esgota o fd antes de fechar (evita leak do buffer estático do GNL) |
| `parser_meta.c` | Lê a seção de metadados (`NO/SO/WE/EA/F/C`) até achar o início do mapa |
| `parser_color.c` | Valida e converte as linhas `F`/`C` em RGB |
| `parser_map.c` | Monta o grid do mapa e acha o spawn do player |
| `parser_grid.c` | `pad_grid` — enche as linhas mais curtas com espaço até virar retangular |
| `parser_walls.c` | Valida que o mapa está fechado |
| `file_utils.c` | Checagens de extensão + `trim_newline` |
| `free_utils.c` | Toda a limpeza de memória, incluindo `free_game` |

### Utilitário (`src/utils/`)

| Arquivo | O que tem |
|---|---|
| `utils.c` | `handle_error` — tabela central de mensagens de erro |

---

## 🏗️ Estruturas de Dados

### `t_config` (as 4 texturas + cores)
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

### `t_map` (o grid)
```c
typedef struct s_map
{
	char	**grid;   // NULL-terminated, uma string por linha
	int		rows;
	int		cols;     // linha mais longa (mapa não é retangular)
}	t_map;
```

### `t_player` (spawn)
```c
typedef struct s_player
{
	double	x;
	double	y;
	char	dir;      // 'N', 'S', 'E' ou 'W'
	float	dir_ang;  // ângulo da câmera em graus, derivado de dir (ver main.c)
}	t_player;
```

### `t_game` (junta tudo)

O parser só popula `map`, `config` e `player`. O resto do struct é
território do render (imagens, texturas carregadas, minimapa, o fan de
raios pré-computado, `horizon`/`proj_plane`) — listado aqui só pra
situar onde o resultado do parser se encaixa no resto do programa:
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

## 🔄 Fluxo de Execução

```
parse_cub(path, game)
  ├── has_cub_extension(path)          → ERR_NO_CUB se falhar
  ├── open(path)                       → ERR_NO_FILE se falhar
  ├── parse_meta(fd, &game->config, &first_line)
  │     ├── seta sentinelas: floor[0] = -1, ceil[0] = -1
  │     ├── read_meta_lines            → loop de get_next_line
  │     │     ├── trim_newline em cada linha
  │     │     ├── is_map_line?         → break, guarda em first_map_line
  │     │     └── dispatch_meta_line   → parse_texture ou parse_color
  │     └── validate_config            → confere as 4 texturas + sentinelas
  └── parse_map_grid(fd, &game->map, first_line, &game->player)
        ├── build_map_grid             → monta o char**, rastreia cols
        ├── has_only_valid_chars
        ├── has_closed_walls           ← ANTES de find_player
        ├── find_player                → popula player, spawn vira '0'
        └── pad_grid                   → enche linhas curtas com ' ' até `cols`
```

### Por que `first_map_line` existe

`get_next_line` é destrutiva — uma vez lida, a linha não volta pro
fd. Quando `read_meta_lines` detecta que a linha atual já é o início
do mapa (via `is_map_line`), ela já consumiu essa linha do fd. A
solução: `parse_meta` recebe `char **first_map_line` e preenche esse
ponteiro antes de retornar, e `parse_map_grid` recebe essa linha
já pronta como primeiro parâmetro de dados — sem precisar reler ou
dar seek no fd.

**Achado com `leaks` (08/08):** guardar a linha no out-param resolve
não perder o dado, mas não decide quem é dono dela. `read_meta_lines`
pode preencher `*first_map_line` com sucesso e o `parse_meta` mesmo
assim falhar logo depois, no `validate_config` — nesse caminho a
linha nunca era liberada. `parse_cub` agora inicializa
`first_line = NULL` e dá `free(first_line)` no ramo de erro do
`parse_meta` antes de fechar o fd (`free(NULL)` é no-op nos outros
casos, então é seguro incondicionalmente). De quebra, os três
`close(fd)` do `parse_cub` viraram `close_and_drain(fd)`: uma função
nova que lê até `NULL` antes de fechar, porque o `get_next_line`
também guarda um buffer estático por fd que só se libera sozinho ao
bater EOF — parar de ler mais cedo (erro, ou não precisar do resto do
arquivo) e só fechar o fd deixa esse buffer preso pra sempre. Ver
`docs/parser_concepts.md`, seção 4, pra mais detalhe.

### Por que `is_map_line` olha o segundo caractere pra N/S/E/W

Uma linha de metadado (`"NO ./assets/textures/north.png"`) e uma
linha de mapa que começa com o spawn (`"N0001..."`) começam com a
mesma letra. `is_map_line` só confirma que é uma linha de mapa se,
depois da letra de direção, vier espaço, fim de string ou `\n` —
senão é claramente um identificador de textura.

---

## 🎨 `parse_color` — de perto

Entrada: `"F 220,100,0"` → saída: `dest[3] = {220, 100, 0}`

```
count_parts(parts)         conta quantos elementos o ft_split devolveu
check_color_part(part)     valida 1 parte: só dígitos, range [0,255]
                            → retorna o código de erro cru, não libera nada
parse_color(line, dest)    avança o ponteiro além do identificador,
                            ft_split(",")  , confere count == 3,
                            valida as 3 partes, preenche dest, libera parts
color_error(parts)         helper de erro: libera parts + chama handle_error
                            (só quem tem o parts alocado decide liberar)
```

**Regra de ownership:** `check_color_part` não é dona do array
`parts` (só recebeu uma parte dele pra validar), então não libera
nada e não imprime erro — só devolve o código. Quem aloca (`parse_color`,
via `ft_split`) é quem libera e quem decide chamar `handle_error`.

---

## 🧱 `has_closed_walls` — de perto

Não dá pra usar a abordagem do so_long (checar só o perímetro) porque
as linhas do mapa podem ter tamanhos diferentes — não é uma matriz
retangular. A solução é checar os **vizinhos de cada célula
caminhável**:

```
is_valid_neighbor(map, rows, row, col)
    FALSE se row/col fora dos limites do grid
    FALSE se map[row][col] == ' '
    TRUE caso contrário

check_cell_neighbors(map, rows, i, j)
    checa os 4 vizinhos (cima/baixo/esquerda/direita) de map[i][j]

has_closed_walls(map, rows)
    para cada '0'/'N'/'S'/'E'/'W'/'D', chama check_cell_neighbors
    se qualquer um falhar → mapa aberto
```

Roda **antes** de `find_player`, porque o player também é uma célula
caminhável que precisa ter vizinhos válidos — se rodasse depois, o
caractere de spawn já teria virado `'0'` (tanto faz pro resultado,
mas a ordem importa por outros motivos: ver `parser_todo.md`).

---

## ⚠️ Tratamento de Erros

### `t_error` (enum)
```c
ERR_NONE = 0, ERR_ARGS, ERR_NO_FILE, ERR_NO_CUB, ERR_MALLOC,
ERR_MAP_CHARS, ERR_MAP_OPEN, ERR_MAP_PLAYER, ERR_MISSING_TEX,
ERR_INVALID_COLOR, ERR_INVALID_ID, ERR_MLX
```
`ERR_MLX` foi adicionado depois (integração com o render, ver
`docs/next_session_todo.md`) — o parser nunca o retorna, é usado
quando `config_mlx`/`init_render` falham.

### Regra única do projeto: quem decide, chama `handle_error` uma vez

`handle_error` centraliza a tabela de mensagens e sempre retorna
`ERROR` (-1). A convenção do parser inteiro é: **funções auxiliares
retornam o código de erro cru; só a função que toma a decisão final
chama `handle_error`.** Isso evitou um bug real (ver
`docs/parser_session_log.md`, sessão de 11/07): `find_player` chamava
`handle_error` internamente **e** `parse_map_grid` chamava de novo
em cima do retorno — mensagem de erro duplicada. Corrigido fazendo
`find_player` devolver o código puro.

---

## 🧹 `free_game`

```c
free(game->config.tex_north);
free(game->config.tex_south);
free(game->config.tex_west);
free(game->config.tex_east);
free_map(game->map.grid);
```
Chamado nos dois caminhos possíveis depois de um parse bem-sucedido:
saída normal do programa, e qualquer erro que aconteça depois do
`parse_cub` retornar sucesso (ex.: falha de MLX). Se o `parse_cub`
falhou, `free_game` ainda é seguro de chamar — os campos não
alocados ficam `NULL`/0 (por isso `main_test.c` usa
`memset(&game, 0, sizeof(game))` antes de tudo).

---

## 🔌 Onde o parser termina

O parser **não abre PNGs, não inicializa MLX, não desenha nada**.
Ele só:
1. Valida e guarda os *paths* das texturas (string, extensão `.png`)
2. Valida e guarda as cores (RGB, `[0,255]`)
3. Monta o grid e acha o player

Carregar o conteúdo do PNG e desenhar (raycasting, texture mapping) é
responsabilidade de outra camada, hoje implementada em
`src/render/texture_load.c`:

- **Loading**: `load_textures(t_game *game)` chama `mlx_load_png` pras
  4 texturas e guarda o resultado em `game->tex[4]` (`mlx_texture_t*`,
  indexado pelo enum `t_face`) — não em campos novos de `t_config`,
  que era o plano original em `docs/next_session_todo.md`. Falha
  libera o que já carregou (`free_textures`) antes de reportar
  `ERR_MISSING_TEX`.
- **Rendering**: `render/draw_tex_view.c` e cia. já existem e usam
  esse `tex[4]` pra desenhar as paredes.

`docs/next_session_todo.md` continua útil como registro do
raciocínio da época, mas descreve um **plano**; pra ver o que
realmente foi implementado, ler o código em `src/render/` direto.
