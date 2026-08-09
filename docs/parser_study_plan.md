# 📖 Plano de Estudo — Parser do cub3D

Roteiro de revisão pra defesa. **Este arquivo não explica nada** — ele
diz o que ler, em que ordem, e o que praticar depois de cada leitura.

O conteúdo mora em dois lugares:

| documento | o que tem |
|---|---|
| [`../README.md`](../README.md#-execution-flow) | O fluxo do **projeto inteiro** — `main` → parser → render → saída — e a árvore de arquivos |
| [`parser_explanation.md`](parser_explanation.md) | O parser do cub3D: cada arquivo, função, validação e regra de memória |
| [`parser_concepts.md`](parser_concepts.md) | Os conceitos gerais por trás (não específicos do cub3D) |

**Como usar:** cada sessão aponta a seção exata dos dois. Leia o
conceito **antes** do código — o conceito explica *por quê*, o
explanation mostra *como ficou aqui*.

---

## 🎯 Método: Pomodoro Modificado

```
15-20 min → ESTUDO ATIVO (ler + anotar)
5-10 min  → PRÁTICA (rodar / explicar em voz alta)
5 min     → PAUSA (levantar, água, alongar)
```

⏰ Timer visível · ✅ marca cada sessão · 📱 celular longe

**Regra que vale mais que o resto:** se você não consegue explicar em
voz alta sem olhar, você não revisou — você leu. A prática de cada
sessão é a parte que conta.

---

## 🗓️ Trilha — 7 sessões

### ✅ Sessão 0: Onde o parser se encaixa (15 min)

**OBJETIVO:** situar o parser no projeto inteiro, antes de entrar nele.

Esta é a única sessão que **não** é sobre o parser. Ela existe porque na
defesa a primeira pergunta costuma ser sobre o programa todo, não sobre
uma função — e porque é muito mais fácil estudar uma peça sabendo onde
ela encaixa.

**📖 Ler:**
- README, [**Execution Flow**](../README.md#-execution-flow) — o fluxo
  de `argv[1]` até a saída
- README, [**Project Structure**](../README.md#-project-structure) — só
  passar o olho no `src/`

**✋ Praticar:**
1. **Fechar o README.** Desenhar as três fases numa folha: o que roda
   **antes** do `mlx_loop`, o que roda **dentro** dele, e o que roda
   **depois** que ele retorna
2. Marcar no desenho onde o parser começa e onde ele termina — e
   perceber que ele é uma caixa só, no topo

**✍️ Fixar:**
- [ ] Três fases: parse → init → loop → teardown
- [ ] Tudo antes do `mlx_loop` roda **uma vez**; o `on_update` roda **por frame**
- [ ] `load_textures` vem antes do `config_mlx` — `mlx_load_png` só
      decodifica bytes, não precisa de `mlx_t*`
- [ ] `init_minimap_geometry` roda antes da MLX existir: é aritmética
      pura sobre o mapa já parseado
- [ ] Ordem do teardown: `mlx_terminate` → `free_textures` →
      `map_pixels_buf` → `free_game`
- [ ] O parser é uma caixa só do fluxo, e nada depois dele revalida nada

**🎮 Desafio:** em 60 segundos, sem olhar — o que acontece entre digitar
`./cub3D maps/valid/map01.cub` e a janela aparecer?

---

### ✅ Sessão 1: O pipeline do parser (25 min)

**OBJETIVO:** desenhar o fluxo do parser de ponta a ponta sem consultar.

**📖 Ler:**
- `explanation` §1 (o que o parser entrega) e §4 (pipeline completo)

**✋ Praticar:**
1. **Fechar o doc.** Desenhar num papel: `main` → `parse_cub` →
   `parse_meta` → `parse_map_grid`, com os erros saindo de cada caixa
2. Conferir com o diagrama e circular o que faltou

**✍️ Fixar:**
- [ ] As 4 validações do mapa **em ordem**, e por que essa ordem
- [ ] `pad_grid` é o último de propósito
- [ ] `close_and_drain` roda em todos os caminhos
- [ ] As 7 pós-condições — o que o render pode assumir sem checar

**🎮 Desafio:** explique em voz alta por que `has_closed_walls` vem
antes de `find_player`.

---

### ✅ Sessão 2: Metadados (25 min)

**OBJETIVO:** explicar ordem livre, duplicata e as duas sentinelas.

**📖 Ler:**
- `concepts` §3 (valores sentinela) — **antes** do código
- `explanation` §6 inteira

**✋ Praticar:**
1. Abrir `parser_meta.c` e acompanhar uma linha `NO ./x.png` do
   `read_meta_lines` até o `ft_strdup`
2. Responder sem olhar: por que `is_map_line` checa `line[1]` quando o
   primeiro char é cardeal?

**✍️ Fixar:**
- [ ] `NO`/`SO`/`WE`/`EA` começam com cardeal — daí o `line[1]`
- [ ] Ordem das 6 chaves é livre; repetir é `ERR_DUPLICATE_ID`
- [ ] Uma sentinela responde "veio?" **e** "veio duas vezes?"
- [ ] Parsing valida o *path*; quem abre o PNG é o `load_textures`

**🎮 Desafio:** o que aconteceria se `NO` aparecesse duas vezes e o
`dispatch_tex` não existisse? (dica: não é só "sobrescreve")

---

### ⏸️ PAUSA LONGA: 15-30 min

---

### ✅ Sessão 3: Geometria do mapa (25 min)

**OBJETIVO:** explicar por que vizinho ≠ perímetro.

**📖 Ler:**
- `concepts` §5 (vizinhos vs flood fill) — **antes** do código
- `explanation` §7, até `has_closed_walls`

**✋ Praticar:**
1. Abrir `maps/valid/subject_map.cub`, escolher um `0` numa linha curta
   e achar os 4 vizinhos **na mão**
2. Rodar `invalid05_open_border` e `invalid06_open_space`, e apontar
   **qual das três condições** do `is_valid_neighbor` pegou cada um

**✍️ Fixar:**
- [ ] so_long retangular = perímetro; cub3D ragged = vizinho por célula
- [ ] `ft_strlen(map[row])` e não `map->cols` — e por quê
- [ ] Linha de tamanho diferente **não** é erro
- [ ] Só cardeais, sem diagonal — limitação assumida

**🎮 Desafio:** desenhe um mapa de 3 linhas que passe no
`has_only_valid_chars` e reprove no `has_closed_walls`.

---

### ✅ Sessão 4: Spawn e coordenadas (25 min)

**OBJETIVO:** explicar o "exatamente um" e o `+ 0.5`.

**📖 Ler:**
- `concepts` §9 (coordenada contínua sobre grid discreto)
- `explanation` §7, de `find_player` até `pad_grid`

**✋ Praticar:**
1. Rodar `invalid07_two+_spawns.cub`
2. Desenhar um tile no papel com as coordenadas `5.0` e `5.5`, e
   calcular **à mão** a caixa de colisão nos dois casos com
   `PL_RADIUS = 0.25`

**✍️ Fixar:**
- [ ] Contar tudo rejeita "zero" e "dois" com o mesmo teste
- [ ] Antes: spawn duplicado virava parede fantasma (`is_solid` = `c != '0'`)
- [ ] Coordenada inteira = quina do tile, não centro
- [ ] `+0.5` → caixa inteira dentro da célula do spawn
- [ ] Sem `+0.5` → `side_dist = 0` → `hit_dist = 0` → divisão por zero

**🎮 Desafio:** explique o `+ 0.5` pro Fil usando **só** o desenho.

---

### ⏸️ PAUSA LONGA: 15-30 min

---

### ✅ Sessão 5: Memória (25 min)

**OBJETIVO:** defender a gestão de memória com número na mão.

**📖 Ler:**
- `concepts` §2 (ownership) e §10 (categorias de leak)
- `explanation` §9 inteira

**✋ Praticar:**
1. Rodar valgrind nos 7 mapas inválidos e confirmar
   `0 bytes in 0 blocks` em todos (comandos no `explanation` §11)
2. Rodar num mapa **válido**, ver os ~313 KB `still reachable`, e
   treinar a frase de explicação

**✍️ Fixar:**
- [ ] `still reachable` ≠ leak; vem do MLX42/GLFW/X11
- [ ] O critério real é `definitely` / `indirectly lost`
- [ ] `ft_memset` no `main` é o que torna `free_game` seguro em qualquer ponto
- [ ] `close_and_drain` existe pelo buffer estático do GNL
- [ ] Demonstrar memória **sempre** com mapa inválido

**🎮 Desafio:** a frase de 15 segundos pro avaliador —
*"still reachable é o MLX42; o nosso está em zero. Posso mostrar num
mapa inválido, que é o único jeito de rodar sem inicializar a MLX."*

---

### ✅ Sessão 6: Simular a defesa (25 min)

**OBJETIVO:** responder em voz alta, cronometrado, sem consultar.

**✋ Praticar:** passar pelo banco de perguntas abaixo. Marque as que
travaram e releia **só** a seção correspondente.

**📖 Antes de encerrar:** ler `explanation` §12 (pontos fracos
conhecidos) — é melhor você citar antes de perguntarem.

---

## 🎤 Banco de perguntas

Resposta curta aqui; o detalhe está na seção indicada.

| pergunta | resposta em 1 linha | detalhe |
|---|---|---|
| Como garante que o mapa está fechado? | Vizinho de cada célula andável, não perímetro — porque as linhas têm tamanhos diferentes | `expl` §7 |
| E se as linhas tiverem tamanhos diferentes? | É válido; o subject não exige retangular. O `pad_grid` iguala **depois** de validar | `expl` §7 |
| Por que padding depois da validação? | Pra não validar espaços que o meu próprio código inventou | `conc` §11 |
| E se tiver dois players? | Recuso. Conto todos e exijo exatamente 1 — mesmo teste rejeita zero e dois | `expl` §7 |
| Por que `+ 0.5` na posição? | Coordenada inteira é a quina do tile. Sem isso, colisão espalha por 4 células e o DDA começa com distância zero | `conc` §9 |
| Textura repetida duas vezes? | `ERR_DUPLICATE_ID` — sem a guarda, o segundo `strdup` sobrescreve e vaza o primeiro | `expl` §6 |
| As chaves precisam estar em ordem? | Não. Qualquer ordem, qualquer número de linhas vazias. Só o mapa tem que ser o último bloco | `expl` §6 |
| Onde validam se a textura existe no disco? | Não no parser. O parser valida o *path*; quem abre é o `load_textures` | `expl` §10 |
| Tem leak? | Não. `definitely lost: 0`. O `still reachable` de mapa válido é MLX42/GLFW/X11 | `conc` §10 |
| Por que `close_and_drain` e não `close`? | O GNL tem buffer estático por fd que só é liberado ao ler EOF | `conc` §4 |
| Por que `take_row_spawns` não fica junto do `find_player`? | Norminette: `parser_map.c` já estava em 5/5 funções | `expl` §3 |
| O que é esse `'D'` no `has_closed_walls`? | Código morto — o `has_only_valid_chars` não aceita `'D'` | `expl` §12 |
| E se o `malloc` falhar? | `ERR_MALLOC` e o `main` chama `free_game`. Tem um buraco conhecido no `ft_append_line`, só em OOM real | `expl` §12 |
| Por que a mensagem de erro não sai duplicada? | Só a função que decide chama `handle_error`; as auxiliares retornam código cru | `expl` §8 |

---

## 📊 Registro de Progresso

```
Sessão 0 (fluxo do projeto):  [ ] Foco: ☆☆☆☆☆
Sessão 1 (pipeline parser):   [ ] Foco: ☆☆☆☆☆
Sessão 2 (metadados):         [ ] Foco: ☆☆☆☆☆
Sessão 3 (geometria):         [ ] Foco: ☆☆☆☆☆
Sessão 4 (spawn / +0.5):      [ ] Foco: ☆☆☆☆☆
Sessão 5 (memória):           [ ] Foco: ☆☆☆☆☆
Sessão 6 (simular defesa):    [ ] Foco: ☆☆☆☆☆

TOTAL: ___ / 7 sessões
```

**Recompensas:** 4 sessões = lanche favorito · 7 sessões = PRONTA 🎊

---

## ✅ Checklist de Domínio

- [ ] Conto o fluxo do projeto inteiro em 60s, do `argv[1]` até a janela
- [ ] Sei o que roda uma vez e o que roda por frame
- [ ] Desenho o pipeline do parser de cabeça
- [ ] Explico as 4 validações do mapa **e a ordem delas**
- [ ] Explico vizinho vs perímetro, e `ft_strlen` vs `cols`
- [ ] Explico as duas sentinelas e as duas perguntas de cada uma
- [ ] Explico por que `is_map_line` olha o `line[1]`
- [ ] Explico "exatamente um spawn" e o que quebrava antes
- [ ] Explico o `+ 0.5` com colisão **e** com DDA
- [ ] Explico por que `pad_grid` é o último
- [ ] Explico a regra de ownership (quem aloca libera, quem é dono reporta)
- [ ] Explico `still reachable` sem gaguejar
- [ ] Sei rodar os 7 inválidos, os 11 válidos e o valgrind
- [ ] Sei citar os 3 pontos fracos conhecidos antes de perguntarem

---

## 💡 Dicas

**✅ FAÇA:** sessões curtas, mude de ambiente, explique em voz alta,
gamifique (1 sessão = 1 ponto), timer visível

**❌ EVITE:** sessão sem pausa +30min, "preciso terminar tudo hoje",
ler sem anotar

**🆘 Perdeu o foco?** Pausa 5 min → volta com sessão mais curta (10 min)
→ troca leitura por desenhar/explicar em voz alta

---

**LEMBRA:** isso é revisão de trabalho que já está pronto. O objetivo é
conseguir *explicar*, não aprender do zero. 💪
