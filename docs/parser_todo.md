# 🗺️ Parser — Tarefas Restantes

> Guia de implementação das funções que faltam no parser do cub3D.
> Ordem sugerida: parse_color → has_closed_walls (flood fill) → free_game → mapas inválidos

---

## 1. `parse_color` — `src/parser/parser_meta.c`

**O que faz:** lê e valida os valores RGB das linhas `F` e `C` do `.cub`.

**Entrada:** `"F 220,100,0"` ou `"C 225,30,0"`

**Passos:**
1. Avançar além do identificador (`"F "` ou `"C "`) e pular espaços extras
2. Usar `ft_split(line, ',')` para separar os três valores
3. Verificar que o split retornou exatamente 3 partes — senão `ERR_INVALID_COLOR`
4. Para cada parte:
   - Verificar que todos os chars são dígitos (`ft_isdigit`)
   - Converter com `ft_atoi`
   - Verificar que está no range `[0, 255]`
   - Se qualquer check falhar → liberar o split e retornar `ERR_INVALID_COLOR`
5. Guardar em `dest[0]`, `dest[1]`, `dest[2]`
6. Liberar o resultado do `ft_split`
7. Retornar `ERR_NONE`

**Dica — contar partes do split:**
```c
int i = 0;
while (parts[i])
    i++;
if (i != 3)
    // erro
```

**Depois de implementar:** atualizar `parse_meta` para validar que `floor` e `ceil`
foram preenchidos. Sugestão: inicializar `config->floor[0] = -1` antes do parse
e checar `if (config->floor[0] == -1)` no final.

---

## 2. `has_closed_walls` — `src/parser/parser_map.c`

**O que faz:** valida que o mapa está completamente fechado por paredes —
nenhum `0` pode estar adjacente a um espaço `' '` ou à borda do grid.

**Por que não usar a do so_long:**
No so_long o mapa era retangular e bastava checar o perímetro.
No cub3D as linhas podem ter tamanhos diferentes, então a abordagem
correta é verificar os **vizinhos** de cada célula caminhável.

### Abordagem — flood fill a partir de cada `0`

A ideia: para cada `0` no grid, verificar se algum dos 4 vizinhos
(cima, baixo, esquerda, direita) é `' '` ou está fora dos limites do grid.
Se qualquer `0` tiver um vizinho inválido, o mapa está aberto.

**Funções auxiliares sugeridas:**

```
is_valid_neighbor(map, rows, row, col)
    - retorna FALSE se row/col está fora dos limites
    - retorna FALSE se map[row][col] == ' '
    - retorna TRUE caso contrário

check_cell_neighbors(map, rows, i, j)
    - chama is_valid_neighbor para os 4 vizinhos de map[i][j]
    - retorna FALSE se qualquer vizinho for inválido

has_closed_walls(map, rows)
    - percorre todo o grid
    - para cada '0', 'N', 'S', 'E', 'W' ou 'D':
        chama check_cell_neighbors
        se retornar FALSE → retorna FALSE (mapa aberto)
    - retorna TRUE se todos passaram
```

**Por que checar `N/S/E/W/D` também:**
O player e as portas são células caminháveis — também precisam
de vizinhos válidos. (O `find_player` já substituiu o spawn por `'0'`,
mas `has_closed_walls` é chamada ANTES de `find_player` — ver nota abaixo.)

> ⚠️ **Ordem de chamada no `parse_map_grid`:**
> 1. `has_only_valid_chars` 
> 2. `has_closed_walls`  ← antes do find_player
> 3. `find_player`

**Cuidado com linhas de tamanho diferente:**
`map[i][j+1]` pode acessar além do fim de uma linha curta.
Antes de acessar o vizinho da direita, verificar `j+1 < ft_strlen(map[i])`.

---

## 3. `free_game` — `src/utils/free_utils.c`

**O que faz:** libera toda a memória alocada em `t_game` no exit.

**Passos:**
```
free(game->config.tex_north)
free(game->config.tex_south)
free(game->config.tex_west)
free(game->config.tex_east)
free_map(game->map.grid)
```

**Onde chamar:** no `main`, após o loop do jogo terminar, e em qualquer
caminho de erro após o parse ter sido bem-sucedido.

---

## 4. Mapas inválidos — `maps/invalid/`

Criar um arquivo para cada tipo de erro implementado:

| Arquivo | Erro testado |
|---|---|
| `invalid01.cub` | `ERR_MAP_CHARS` — caractere `X` no grid ✅ já existe |
| `invalid02.cub` | `ERR_MAP_PLAYER` — nenhum spawn no grid |
| `invalid03.cub` | `ERR_MISSING_TEX` — faltando linha `WE` |
| `invalid04.cub` | `ERR_INVALID_ID` — identificador desconhecido `XX` |
| `invalid05.cub` | `ERR_MAP_OPEN` — mapa não fechado ← após `has_closed_walls` |

---

## 5. Resumo de prioridade

```
[ ] parse_color          ← desbloqueia leitura de cores
[ ] has_closed_walls     ← fecha a validação do parser
[ ] free_game            ← necessário para não vazar memória
[ ] mapas inválidos      ← testes, pode ir fazendo junto com cada função
```
