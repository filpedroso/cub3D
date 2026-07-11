# 🔌 Integração & Texturas — Próxima Sessão

> Parser está completo (parse_color, has_closed_walls, free_game,
> map->cols, mapas inválidos 01-06, subject_map.cub) — tudo commitado
> e com push na `feat/parser-todo`. Foco agora: integrar com o Fil
> e ter texturas de verdade para testar visualmente.

---

## 1. Reconhecimento da branch do Fil (`feat/render`)

Ele já tem: raycasting funcionando, minimap, colisão, pan/walk (isso
já é bonus). A branch dele foi criada a partir de `main`, **não** da
`feat/parser-todo` — ou seja, divergiu antes de toda a reorganização
de hoje. Risco real de conflito, principalmente em `cub3d.h` e `main.c`.

### Testar sem risco (scratch branch, descartável)

**Passo 0 — pré-requisito**: mergear o PR da `feat/parser-todo` na
`main` primeiro (fast-forward puro, sem conflito — confirmado dia
11/07). Depois disso:

```bash
git checkout main
git pull origin main
git checkout -b scratch/test-integration main
git merge origin/feat/render --no-commit --no-ff
```
- `--no-commit` deixa ver os conflitos sem finalizar nada
- Se der ruim: `git merge --abort` desfaz tudo, `main` fica intocada
- Essa branch `scratch/*` não precisa nem de push, é só pra você
  entender antes da call com o Fil: (1) se o parser dela funciona
  de verdade integrado ao raycasting dele, (2) onde estão os
  atritos reais, pra explicar melhor pra ele e não perder tempo
  na call resolvendo isso ao vivo

### O que checar especificamente
- [ ] `cub3d.h`: `MAP_W`/`MAP_H` (defines fixos do Fil, usados no
      minimap POC) vs `t_map.rows`/`t_map.cols` (dinâmico, calculado
      hoje) — precisa reconciliar, o minimap real tem que usar o
      grid parseado, não um mapa hardcoded
- [ ] `main.c`: hoje ainda é o POC do minimap dele (não chama
      `parse_cub`) — decidir juntos como fica o `main` real
      (quem monta a janela MLX, quem chama o parser, em que ordem)
- [ ] Includes duplicados/conflitantes entre parser e render no
      `cub3d.h` (ele adicionou bastante coisa lá — 117 linhas)

### Plano de integração atualizado (11/07)
O plano original era "Fil cria branch de integração a partir da
`feat/parser-todo`" — mas na prática ele continuou na `feat/render`
dele, criada direto da `main` desde o início (confirmado via
`git merge-base`: as duas branches têm o mesmo ancestral comum,
`2bd255b`). Novo plano, mais simples:
1. Mona mergeia `feat/parser-todo` → `main` (fast-forward, sem
   conflito, já que ninguém mais tocou a `main` nesse meio tempo)
2. Fil reconcilia a `feat/render` dele contra a `main` atualizada
   (merge ou rebase — ele decide, é o código dele)
3. Reconhecimento na scratch branch (acima) é feito por Mona ANTES
   da call com o Fil, só pra entender os atritos e explicar melhor
   pra ele, sem perder tempo resolvendo isso ao vivo

---

## 2. Texturas reais para testar

Hoje: 4 PNGs placeholder sólidos (cor lisa, RGBA) em
`assets/textures/`. Servem para o parser (só valida extensão `.png`,
não conteúdo), mas não dão pra avaliar visualmente o raycasting.

### Opções levantadas nesta sessão
- Fontes externas pra buscar: **Kenney.nl** (CC0, zero risco de
  direitos autorais), **OpenGameArt.org** (filtrar por CC0),
  **itch.io/game-assets/free**. Procurar "wall texture" ou "dungeon
  tileset", formato `.png` quadrado (64×64 ou 128×128), idealmente
  tileable (repete sem costura visível).

### Passos sugeridos
- [ ] Decidir: 4 texturas distintas por direção, ou 1 textura
      repetida nas 4? (subject aceita as duas, "the choice is yours")
- [ ] Se buscar arte nova: baixar, conferir licença, colocar em
      `assets/textures/`, atualizar os 4 arquivos
      (north/south/west/east.png)
- [ ] Nota técnica: a renderização de imagem quebrou no meio desta
      sessão do Claude e não voltou a funcionar — se persistir na
      próxima, pedir pra Mona abrir/descrever a imagem ela mesma

---

## 3. Pendências menores (não travam nada, só não esquecer)
- [ ] `valgrind` na máquina Linux do trabalho — leak check definitivo
      (o Mac não suporta `detect_leaks` do ASan nem tem valgrind nativo)
- [ ] Marcar `parser_todo.md` como oficialmente concluído
