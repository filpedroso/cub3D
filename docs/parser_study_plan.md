# 📖 Plano de Estudo — Parser do cub3D (feat/parser-todo)

**Adaptado pro mesmo método do STUDY_PLAN.md do philosophers** (sessões
curtas, gamificação, pausas programadas — bom pra manter o foco).

Cobre **tudo** que foi feito na `feat/parser-todo`: as 4 funções do
`parser_todo.md` original, as refatorações de norminette, e a sessão
de integração/texturas de hoje. Trilha de **2 dias, 4 sessões cada**.

> **Atualizado em 04/08:** escrito em 11/07, quando `feat/parser-todo`
> ainda não tinha sido mergeada. Desde então o merge com `feat/render`
> aconteceu de verdade (não só no teste em `scratch/integration`), e
> as duas maiores pendências do Dia 2 (mapa dinâmico e loading de
> textura) já foram resolvidas — as Sessões 6 e 7 foram ajustadas pra
> refletir isso; use-as pra revisar/explicar o que já está pronto, não
> como decisão em aberto.

---

## 🎯 Método: Pomodoro Modificado

```
15-20 min → ESTUDO ATIVO (ler + anotar)
5-10 min  → PRÁTICA (testar código / explicar em voz alta)
5 min     → PAUSA (levantar, água, alongar)
```

⏰ Timer visível · ✅ marca cada sessão · 📱 celular longe

---

## 🗓️ DIA 1: O Parser em Si

### ✅ Sessão 1: `parse_color` (25 min)
**OBJETIVO:** explicar como `"F 220,100,0"` vira `dest[3] = {220,100,0}`

**📖 Fazer:**
1. Abrir `src/parser/parser_color.c` (2 min)
2. Traçar na cabeça: a string avança (`line += 2`), `ft_split(line, ',')`
   — o que retorna pra `"220,100,0"`? E pra `"220,,0"`? E pra `""`? (10 min)
3. Achar `check_color_part` e `color_error` — quem chama `handle_error`
   e quem não chama (8 min)
4. Explicar em voz alta a resposta (5 min)

**✍️ Anotar:**
- [ ] `count_parts`, `check_color_part`, `parse_color` — 3 funções, 1 arquivo
- [ ] `check_color_part` retorna o código de erro **cru** — não é dono do `parts`
- [ ] `parse_color`/`color_error` são donos do `parts` alocado → só eles liberam e chamam `handle_error`
- [ ] Regra geral: **quem aloca, libera** (ownership)

**🎮 Mini-Desafio:** sem olhar o código, lista as 3 validações que cada parte da cor passa (dígito → range 0-255 → 3 partes exatas)

---

### ✅ Sessão 2: `has_closed_walls` (25 min)
**OBJETIVO:** explicar por que checar vizinhos ≠ checar perímetro

**📖 Fazer:**
1. Abrir `maps/valid/map01.cub`, escolher um `0` no meio do mapa,
   achar os 4 vizinhos na mão (5 min)
2. Escolher um `0` numa borda de linha mais curta — o que
   `is_valid_neighbor` faz quando `col >= ft_strlen(map[row])`? (10 min)
3. Ler `src/parser/parser_walls.c` — `is_valid_neighbor` →
   `check_cell_neighbors` → `has_closed_walls` (10 min)

**✍️ Anotar:**
- [ ] so_long: mapa retangular, basta checar o perímetro
- [ ] cub3D: linhas de tamanho diferente, então checa **vizinho por célula**
- [ ] `has_closed_walls` roda **antes** de `find_player` (player ainda não virou `'0'`)
- [ ] Detecta aberturas na borda **e** por espaço no meio do grid

**🎮 Mini-Desafio:** por que usar `ft_strlen(map[row])` em vez de `map->cols`? (dica: sessão 3)

---

### ⏸️ PAUSA LONGA: 15-30 min

---

### ✅ Sessão 3: `map->cols` + refatoração por norminette (25 min)
**OBJETIVO:** entender o que mudou depois do plano original

**📖 Fazer:**
1. Abrir `src/parser/parser_map.c` — achar `update_max_cols` e
   `build_map_grid` (8 min)
2. Responder o mini-desafio da sessão 2: `map->cols` não existia
   quando `has_closed_walls` foi escrita — por isso o TODO ficou
   pendente e depois foi fechado (5 min)
3. Ler `docs/parser_session_log.md`, seção "Reorganização por
   norminette" — a tabela antes/depois dos arquivos (7 min)
4. Explicar em voz alta pro Fil: "por que isso está em duas funções
   e não uma?" sobre `validate_config`/`read_meta_lines`/`build_map_grid` (5 min)

**✍️ Anotar:**
- [ ] `update_max_cols`: rastreia a linha mais longa (mapa não é retangular)
- [ ] Motivo da refatoração: limite de 5 funções/arquivo (`TOO_MANY_FUNCS`)
- [ ] `parser_color.c` e `parser_walls.c` são arquivos **novos**, extraídos de `parser_meta.c`/`parser_map.c`
- [ ] Resultado: norminette 100% limpa no parser

**🎮 Mini-Desafio:** explica as duas funções sem olhar o código — o critério é responsabilidade única, não tamanho

---

### ✅ Sessão 4: `free_game` + mapas de teste (25 min)
**OBJETIVO:** saber os 6 cenários de erro cobertos e o bug do double-print

**📖 Fazer:**
1. Ler `src/utils/free_utils.c` — `free_game` (5 min)
2. Rodar `make test` e testar os 6 mapas inválidos (10 min):
   ```
   ./test_parser maps/invalid/invalid02.cub  # ERR_MAP_PLAYER
   ./test_parser maps/invalid/invalid03.cub  # ERR_MISSING_TEX
   ./test_parser maps/invalid/invalid04.cub  # ERR_INVALID_ID
   ./test_parser maps/invalid/invalid05.cub  # ERR_MAP_OPEN (borda)
   ./test_parser maps/invalid/invalid06_space.cub  # ERR_MAP_OPEN (espaço interno)
   ./test_parser maps/valid/subject_map.cub  # sucesso — exemplo oficial
   ```
3. Ler `docs/parser_session_log.md`, seção "Bug encontrado e
   corrigido: mensagem de erro duplicada" (10 min)

**✍️ Anotar:**
- [ ] `free_game` libera as 4 texturas + `map.grid`, chamado nos 2 caminhos (sucesso e erro)
- [ ] Bug: `find_player` chamava `handle_error` **e** `parse_map_grid` chamava de novo em cima — mensagem duplicada
- [ ] Fix: `find_player` retorna o código puro, só `parse_map_grid` chama `handle_error` (mesma regra da sessão 1: quem decide, decide uma vez só)

> **Atualização 08/08:** rodando `leaks -atExit -- ./test_parser` em
> cada um desses 6 mapas (prep pra defesa), achamos que
> `invalid03.cub` especificamente vazava 32 bytes — não tem nada a ver
> com `free_game`, é o `first_map_line` nunca sendo liberado quando
> `parse_meta` falha depois de `read_meta_lines` já ter lido a linha
> do mapa. Corrigido em `parser.c`. Detalhe completo em
> `docs/parser_concepts.md` (seção 4) e `docs/parser_explanation.md`
> ("Por que `first_map_line` existe"). Vale rodar `leaks` nos 6 de
> novo como parte dessa sessão a partir de agora — é rápido e teria
> pego isso na hora.

**🎮 Mini-Desafio:** prepara as 2 frases pro Fil (já estavam no plano de hoje cedo):
- "O parser agora recusa mapas abertos, incluindo aberturas por espaço no meio do grid, não só na borda."
- "As cores de floor/ceiling são validadas com sentinela -1 — se parse_meta retornar sucesso, você pode confiar que config.floor e config.ceil têm valores reais."

---

## 🗓️ DIA 2: Integração & Próximos Passos

### ✅ Sessão 5: Merge e conflito silencioso (25 min)
**OBJETIVO:** explicar a diferença entre conflito de texto e conflito semântico

**📖 Fazer:**
1. Ler `docs/next_session_todo.md`, seção 1 "O que achamos" (10 min)
2. Achar no histórico o commit `b24418f` (`scratch/integration`) e
   ler a mensagem completa: `git show b24418f --stat` (5 min)
3. Explicar em voz alta: por que o `git merge` não marcou conflito
   no rename `dir` → `dir_c`, mas o build quebrou mesmo assim? (10 min)

**✍️ Anotar:**
- [ ] Conflito de texto (Makefile `FILES`): as duas branches mudaram **a mesma linha** → git avisa
- [ ] Conflito silencioso (`dir`→`dir_c`): o Fil renomeou um campo que o lado da Mona nunca tocava → git faz merge automático, mas quem *usa* o campo antigo quebra na compilação
- [ ] Lição: `git merge` sem conflito ≠ merge seguro. Sempre builda depois.
- [ ] Bônus: bug da vírgula faltando em `utils.c` (concatenação de string literals em C) — só na branch do Fil, não é da Mona

**🎮 Mini-Desafio:** explica pra alguém (ou em voz alta) por que "compilou sem erro" não é suficiente prova de que um merge deu certo

---

### ✅ Sessão 6: `MAP_W`/`MAP_H` fixo vs `t_map` dinâmico — RESOLVIDO (25 min)
**OBJETIVO:** entender a decisão que foi tomada e onde ela aparece hoje no código

> Quando essa sessão foi escrita isso ainda era pauta de call. Já foi
> decidido e implementado: venceu o `t_map` dinâmico, o `gridmap`
> fixo não existe mais em lugar nenhum do código.

**📖 Fazer:**
1. Ler `docs/next_session_todo.md`, item "O que ainda precisa de
   decisão conjunta" — só pra ver o estado *antes* da decisão (5 min)
2. Conferir que não sobrou nenhum `gridmap`/`MAP_W`/`MAP_H`:
   `grep -rn "gridmap\|MAP_W\|MAP_H" src/` (deve voltar vazio) (5 min)
3. Ler `draw_minimap.c`, `update_pl_pos.c`, `raycasting.c` e achar
   onde cada um lê `game->map.grid`/`game->map.rows`/`game->map.cols` (10 min)
4. Explicar em voz alta: como esses três arquivos ficaram sabendo o
   tamanho real do mapa sem nenhum `MAP_W`/`MAP_H`? (5 min)

**✍️ Anotar:**
- [ ] `game.map` = struct dinâmica (`char **grid; int rows; int cols;`), populada pelo parser
- [ ] Não sobrou nenhum array fixo — todo acesso ao grid passa por `game->map`
- [ ] A decisão não foi um meio-termo: o scaffold do Fil foi substituído, não mantido em paralelo

**🎮 Mini-Desafio:** explica pro Fil (ou em voz alta) como um mapa de 21x13 (`map01.cub`) e um mapa de outro tamanho qualquer rodam com o mesmo binário, sem recompilar nada

---

### ⏸️ PAUSA LONGA: 15-30 min

---

### ✅ Sessão 7: Texturas — parsing vs loading vs rendering — IMPLEMENTADO (25 min)
**OBJETIVO:** saber explicar a fronteira entre as 3 camadas, como ela ficou de verdade

> Quando essa sessão foi escrita, loading e rendering eram só um
> plano (`docs/next_session_todo.md`). As duas camadas existem agora
> em `src/render/texture_load.c` e `src/render/draw_tex_view.c`, e o
> design final é mais simples que o planejado: `t_game.tex[4]`
> indexado pelo enum `t_face`, em vez de 4 campos `img_*` separados
> em `t_config`.

**📖 Fazer:**
1. Ler `assets/textures/CREDITS.txt` e olhar as 4 texturas (5 min)
2. Ler `src/render/texture_load.c` inteiro — `load_textures`,
   `load_one`, `free_textures` (10 min)
3. Explicar em voz alta a diferença entre as 3 camadas, agora
   apontando pro código real de cada uma (10 min)

**✍️ Anotar:**
- [ ] **Parsing** (`src/parser/`): valida o *path* da textura, extensão `.png` — não abre o arquivo
- [ ] **Loading** (`src/render/texture_load.c`): `load_textures(t_game *game)` chama `mlx_load_png` pras 4 e guarda em `game->tex[4]` (`mlx_texture_t*`) — roda antes de `config_mlx`, não precisa de `mlx_t*` ainda
- [ ] **Rendering** (`src/render/draw_tex_view.c`): `tex[col->face]` escolhe a textura certa por coluna, e faz o sampling pixel a pixel
- [ ] `free_textures` é idempotente (chamado tanto no caminho de erro de `load_one` quanto no shutdown normal em `render.c`) — evita duplicar a lógica de free em dois lugares

**🎮 Mini-Desafio:** explica pro Fil por que `t_game.tex[4]` indexado por `t_face` (`F_NORTH`, `F_SOUTH`, `F_WEST`, `F_EAST`) evita o branch de 4 vias que 4 campos nomeados (`img_north`/`img_south`/...) exigiriam no render

---

### ✅ Sessão 8: Simular a call com o Fil (25 min)
**OBJETIVO:** chegar na call com as explicações prontas, não improvisadas

**📖 Fazer:**
1. Reler o resumo do PR (descrição que foi colada no GitHub) (5 min)
2. Timer de 4 min: explicar o que o parser faz, do zero, como se
   o Fil não soubesse nada
3. Timer de 4 min: explicar o achado do `dir_c` e por que git não
   avisou
4. Timer de 4 min: explicar `MAP_W`/`MAP_H` vs dinâmico e por que
   isso é decisão dele também
5. Timer de 4 min: propor o plano de `mlx_load_png` (quem faz o quê)
6. Timer de 4 min: perguntas em aberto pra fazer pra ele (main.c real, timing do merge)

**✍️ Gravar-se explicando (celular):** depois ouve e vê o que ficou confuso

---

## 🎯 Checklist de Domínio

### Parser (Dia 1)
- [ ] Explico `parse_color` e a regra de ownership do `parts`
- [ ] Explico por que `has_closed_walls` checa vizinho, não perímetro
- [ ] Sei por que `map->cols` não existia antes e o que ele resolve
- [ ] Explico o bug do double-print e a correção
- [ ] Sei rodar `make test` e interpretar os 6 cenários de erro

### Integração (Dia 2)
- [ ] Explico a diferença entre conflito de texto e conflito silencioso
- [ ] Sei apontar, no código atual, as 3 leituras de `game->map` que antes eram `gridmap` fixo
- [ ] Explico a fronteira parsing → loading → rendering pra texturas, com o design real (`tex[4]`)
- [ ] Tenho as perguntas em aberto prontas pra call

---

## 💡 Dicas (as mesmas que funcionaram no philosophers)

**✅ FAÇA:** sessões curtas (15-25 min), mude de ambiente, explique em
voz alta, gamifique (1 sessão = 1 ponto, meta 8 pontos), timer visível

**❌ EVITE:** sessão sem pausa +30min, "preciso terminar tudo hoje",
ler sem anotar

**🆘 Perdeu o foco?** Pausa 5 min → volta com sessão mais curta (10 min)
→ troca leitura por desenhar/explicar em voz alta

---

## 📊 Registro de Progresso

```
DIA 1:
Sessão 1 (parse_color):        [ ] Foco: ☆☆☆☆☆
Sessão 2 (has_closed_walls):   [ ] Foco: ☆☆☆☆☆
Sessão 3 (map->cols/norm):     [ ] Foco: ☆☆☆☆☆
Sessão 4 (free_game/testes):   [ ] Foco: ☆☆☆☆☆

DIA 2:
Sessão 5 (conflito silencioso):[ ] Foco: ☆☆☆☆☆
Sessão 6 (MAP_W/MAP_H):        [ ] Foco: ☆☆☆☆☆
Sessão 7 (parsing/loading):    [ ] Foco: ☆☆☆☆☆
Sessão 8 (simular call):       [ ] Foco: ☆☆☆☆☆

TOTAL: ___ / 8 sessões
```

**Recompensas:** 4 sessões = lanche favorito · 8 sessões = PRONTA PRA CALL 🎊

---

**LEMBRA:** isso é revisão de trabalho que já tá pronto e commitado —
o objetivo é conseguir *explicar*, não aprender do zero. Vai no teu
ritmo. 💪
