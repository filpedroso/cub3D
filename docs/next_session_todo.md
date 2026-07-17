# 🔌 Integração & Texturas — Pauta pra Call com o Fil

> Sessão de 11/07 concluiu o reconhecimento de integração (merge de
> teste com `feat/render`) e trocou os placeholders por texturas
> reais. Ambos os itens abaixo eram TODO no início do dia — o que
> falta agora é **decisão em conjunto com o Fil**, não mais
> exploração solo.

---

## 1. Reconhecimento da branch do Fil — ✅ feito, resultado abaixo

Testado via `scratch/integration` (merge `--no-commit --no-ff` de
`origin/feat/render` em cima de `feat/parser-todo`, sem tocar na
`main`). Branch ficou pushed como referência, mas é descartável —
avaliar com o Fil se apaga depois da call.

### O que achamos
- **Makefile (`FILES`):** conflito de texto trivial, aditivo — só
  concatenar as duas listas.
- **`cub3d.h` — `t_player.dir` → `dir_c`:** o Fil renomeou o campo
  (e adicionou `dir_ang`, float, pro ângulo da câmera). Como o lado
  da Mona não tocava nessa struct, o git faz merge automático **sem
  marcar conflito** — mas quebra o build (`parser_map.c`,
  `main_test.c` ainda escreviam em `player->dir`). Um exemplo real
  de conflito semântico que o git não vê.
- **`utils.c` — bug só na branch dele:** falta vírgula entre duas
  strings adjacentes no array `messages[]` (entre `ERR_INVALID_ID` e
  `ERR_MLX`). C concatena as duas em uma entry só — mensagem de
  `ERR_INVALID_ID` sai errada e `ERR_MLX` fica sem mensagem. Ele
  ainda não sabe disso, vale mostrar na call (fix de 1 linha).
- **Validação visual:** feita com um `main.c` temporário (só na
  branch scratch) chamando `parse_cub` de verdade e alimentando o
  minimap/player/rays dele com o grid parseado. Renderizou certo —
  prova que o parser integra com o loop de render, ainda que em 2D
  (que é tudo que existe hoje).

### O que ainda precisa de decisão conjunta (pauta da call)
- [ ] **`MAP_W`/`MAP_H` fixos vs `t_map.rows`/`cols` dinâmico** — o
      Fil confirmou que o array fixo era só scaffold de teste dele,
      não a versão final. `draw_minimap.c`, `update_pl_pos.c` e
      `raycasting.c` leem todos do `gridmap[10][10]` global hoje.
      Trocar pra dinâmico é o item estrutural real, não é conflito
      de merge — é feature a implementar.
- [ ] `main.c`: quem chama `parse_cub`, quem monta a janela MLX, em
      que ordem.
- [ ] Timing do merge `feat/parser-todo` → `main`, e como o Fil
      reconcilia `feat/render` depois.

---

## 2. Texturas reais — ✅ feito

4 texturas CC0 do pack "Retro Textures Fantasy" (kenney.nl), 64×64,
uma por direção. Em `assets/textures/`, licença em
`assets/textures/CREDITS.txt`. Commitado direto na `feat/parser-todo`
(não ficou preso na branch scratch).

**Nota:** hoje não existe nenhum código de texture mapping no
`feat/render` (só linhas 2D de debug) — então essas texturas ainda
não aparecem visualmente em lugar nenhum. Isso é esperado, não é bug.

---

## 3. Carregamento de textura (`mlx_load_png`) — decidir na call antes de implementar

Descoberta de hoje: a API da MLX42 separa carregar do arquivo de
converter pra imagem desenhável:

```c
// não precisa de mlx_t* — decodifica o PNG puro em memória
mlx_texture_t* mlx_load_png(const char* path);

// essa sim precisa do contexto MLX já inicializado
mlx_image_t* mlx_texture_to_image(mlx_t* mlx, mlx_texture_t* texture);
```

`mlx_load_png` não depende de MLX estar inicializada — dá pra rodar
logo depois do `parse_cub`, é natural a Mona ficar com essa parte
(extensão do que `parse_texture` já faz, só que carregando o
conteúdo em vez de só validar o path). `mlx_texture_to_image` já
depende da ordem de init que tá em aberto no item 1.

- [ ] Decidir com o Fil: Mona implementa `mlx_load_png` pras 4
      texturas (novos campos `mlx_texture_t*` em `t_config`, erro se
      PNG inválido/corrompido)?
- [ ] **Cuidado:** mexe em `cub3d.h` de novo — mesmo arquivo do
      conflito silencioso do item 1. Avisar o Fil antes de mudar a
      struct, não fazer em paralelo sem sincronizar.

### Mapeamento das funções que faltam

Ordem de dependência, 4 peças:

**1. `load_one_texture(const char *path)` → `mlx_texture_t*`**
Wrapper fino em volta de `mlx_load_png`. Só chama e checa `NULL`
(falha de decode/arquivo não existe/PNG corrompido).

**2. `load_textures(t_config *config)` → `int` (ERR_NONE ou erro)**
Orquestrador: chama `load_one_texture` pras 4
(`tex_north/south/west/east`), guarda o resultado nos campos novos
de `t_config` (item 3). Se qualquer uma falhar, libera as que já
carregaram (evita leak) e retorna erro.

**3. Campos novos em `t_config`** (mexe em `cub3d.h`):
```c
mlx_texture_t *img_north;
mlx_texture_t *img_south;
mlx_texture_t *img_west;
mlx_texture_t *img_east;
```

**4. Extensão do `free_game`** (ou um `free_textures` novo em
`free_utils.c`): hoje só libera as *strings* dos paths. Precisa
chamar `mlx_delete_texture` nos 4 `mlx_texture_t*` também, senão
vaza memória.

**Onde chamar:** não dentro do `parse_cub` — mistura parsing com
loading. Fica como passo separado logo depois, em `main.c`:
```c
if (parse_cub(argv[1], &game) != ERR_NONE)
    return (ERROR);
if (load_textures(&game.config) != ERR_NONE)
    return (ERROR);
```

**Em aberto:** qual código de erro usar — reaproveitar
`ERR_MISSING_TEX` (hoje só valida o path) ou criar `ERR_TEX_LOAD`
novo pra diferenciar "extensão errada" de "PNG existe mas não
carregou".

---

## 4. Pendências menores (não travam nada, só não esquecer)
- [ ] `valgrind` na máquina Linux do trabalho — leak check definitivo
      (o Mac não suporta `detect_leaks` do ASan nem tem valgrind nativo)
- [ ] Marcar `parser_todo.md` como oficialmente concluído
