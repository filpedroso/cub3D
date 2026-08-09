*This project was created as part of the curriculum at 42 by fpedroso, maria-ol.*

# cub3D

## 📖 Description

`cub3D` is a first-person 3D engine written in C, inspired by Wolfenstein
3D — one of the founding games of the FPS genre. The project implements
**ray-casting** from scratch: starting from a 2D map described in a text
file (`.cub`), the program computes in real time the projection of walls,
textures, floor and ceiling, creating the illusion of depth and letting
the player move freely through a maze.

The pedagogical goal is to understand, hands-on, the fundamentals of 3D
rendering (trigonometry, DDA for ray/wall intersection, texture mapping)
without relying on any game engine or high-level graphics library — only
[MLX42](https://github.com/codam-coding-college/MLX42) to open the window
and draw pixels to the screen.

The `.cub` file parser fully validates the map (closed walls, a single
player position, floor/ceiling colors, per-orientation textures) before
any rendering happens, following 42's project standards (Norminette, no
leaks, no crashes).

## ⚙️ Instructions

### Dependencies

- `gcc`/`clang`, `make`, `cmake`
- `glfw` (used by MLX42; if not found on the system, it is fetched
  automatically via CMake during the build)
- macOS: `Cocoa`, `OpenGL` and `IOKit` frameworks (native to the system)
- Linux: `libdl`, `pthread`, `libm` and a working X11/OpenGL setup

### Build

```bash
git clone git@github.com:filpedroso/cub3D.git
cd cub3D
make
```

The first `make` builds `libft`, builds MLX42 via CMake, and produces
the `cub3D` binary at the project root.

Other useful targets:

```bash
make clean   # remove intermediate object files
make fclean  # remove object files + binaries
make re      # fclean + all
make val     # build + run under valgrind (full leak-check)
make test    # build only the parser as a standalone binary (test_parser)
```

### Running

```bash
./cub3D maps/valid/map01.cub
```

The program expects exactly one argument: the path to a valid `.cub`
map. Invalid maps (open walls, malformed colors, missing textures,
etc.) are rejected with a specific error message — see examples in
`maps/invalid/`.

### 🎮 Controls

| Key                     | Action                                |
|-------------------------|---------------------------------------|
| `↑` / `↓`               | Move forward / backward               |
| `←` / `→`               | Turn the camera left / right          |
| `W` / `S`               | Move forward / backward (mirrors `↑`/`↓`) |
| `A` / `D`               | Strafe left / right, without turning  |
| `M`                     | Toggle minimap display                |
| `R`                     | Toggle ray display (debug)            |
| `T`                     | Toggle between textures and debug view|
| `ESC` / close window    | Quit the game                         |

### `.cub` file format

```
NO ./assets/textures/kenney_retro/north.png
SO ./assets/textures/kenney_retro/south.png
WE ./assets/textures/kenney_retro/west.png
EA ./assets/textures/kenney_retro/east.png

F 220,100,0
C 225,30,0

111111
100001
1000N1
111111
```

- `NO`/`SO`/`WE`/`EA`: texture path for each wall orientation (North,
  South, West, East).
- `F`/`C`: floor and ceiling color, as RGB (`R,G,B`).
- Map grid: `1` wall, `0` walkable floor, `N`/`S`/`E`/`W` player's
  starting position and facing direction, `D` door (bonus), space/empty
  outside the map bounds. The map must be fully enclosed by walls.

## 📁 Project Structure

Eight parser files, fourteen render files, one header holding every
struct and prototype.

<details>
<summary><b>▸ Full directory tree</b></summary>

<br>

```
cub3D/
├── Makefile                    all · clean · fclean · re · val · test
├── include/
│   └── cub3d.h                 every struct, enum, macro and prototype
│
├── src/
│   ├── main.c                  entry point: parse → render → free
│   ├── main_test.c             entry point of the parser-only binary
│   ├── main_test_with_render.c ─┐ not in the Makefile: scratch files
│   ├── mlx_ref.c               ─┘ kept for reference, never compiled
│   │
│   ├── parser/                 .cub file → validated t_game
│   │   ├── parser.c            parse_cub — orchestrates; drains the fd
│   │   ├── parser_meta.c       NO/SO/WE/EA/F/C section, until the map starts
│   │   ├── file_utils.c        extension checks, trim, texture paths
│   │   ├── parser_color.c      "F 220,100,0" → int[3]
│   │   ├── parser_map.c        builds the grid, finds the player
│   │   ├── parser_grid.c       row padding, spawn collection
│   │   ├── parser_walls.c      map-is-closed validation
│   │   └── free_utils.c        all parser-side cleanup
│   │
│   ├── render/                 t_game → pixels, 60x per second
│   │   ├── render.c            render — init, MLX loop, teardown
│   │   ├── render_init.c       one-time derived state: FOV LUT, colors
│   │   ├── mlx_config.c        window + the two images
│   │   ├── texture_load.c      mlx_load_png → game->tex[4]
│   │   ├── compute.c           per-frame input and state update
│   │   ├── update_pl_pos.c     movement + collision box
│   │   ├── raycasting.c        DDA: ray → wall hit + distance
│   │   ├── draw_frame.c        per-frame draw orchestration
│   │   ├── draw_tex_view.c     textured walls (T on)
│   │   ├── draw_dbg_view.c     flat debug walls (T off)
│   │   ├── draw_floor.c        floor and ceiling fill
│   │   ├── draw_ray.c          ray overlay (R)
│   │   ├── draw_minimap.c      minimap overlay (M)
│   │   └── minimap_bake.c      minimap background, baked once
│   │
│   └── utils/
│       └── utils.c             handle_error, is_solid, is_spawn
│
├── maps/
│   ├── valid/                  11 maps — subject_map.cub is the official one
│   └── invalid/                7 maps, one per rejection rule
│
├── assets/textures/            four packs, each with its own CREDITS.txt
│   ├── kenney_retro/           CC0 walls used by subject_map.cub
│   ├── greybox/                flat high-contrast debugging walls
│   ├── lodev_classic/          the classic Lodev tutorial set
│   └── caquinho/               terrazzo original + 6 hue-rotated recolors
│
├── libft/                      47 sources, including get_next_line
│                               and ft_append_line (grows the map grid)
├── MLX42/                      graphics library (submodule, built via CMake)
│
└── docs/
    ├── parser_explanation.md   the parser, function by function
    ├── parser_concepts.md      the general concepts behind it
    ├── parser_study_plan.md    revision roadmap pointing at both
    ├── parser_session_log.md   dated work log
    ├── parser_todo.md          the original plan (historical)
    ├── next_session_todo.md    render integration plan (historical)
    └── defense_prep_log.md     pre-defense QA notes
```

</details>

## 🔄 Execution Flow

Everything before `mlx_loop()` runs **once**; everything inside
`on_update` runs **every frame**. Anything computable from data fixed
for the whole run — the FOV table, the packed floor/ceiling colors, the
minimap background — is deliberately hoisted above the loop.

<details>
<summary><b>▸ Full flow, from <code>argv[1]</code> to exit</b></summary>

<br>

```
main()                                                    src/main.c
 │
 ├─ ft_memset(&game, 0, sizeof(game))
 │      Zeroes every field, which is what makes free_game safe to call
 │      at any later point of failure: unset pointers are NULL.
 │
 ├─ parse_cub(argv[1], &game) ─────────────── error ──► free_game, exit 1
 │      .cub file → validated t_config + t_map + t_player.
 │      Nothing downstream revalidates. See docs/parser_explanation.md
 │
 ├─ dir_to_angle(player.dir)
 │      'N'/'S'/'E'/'W' → the starting camera angle in degrees.
 │
 └─ render(&game) ─────────────────────────── error ──► free_game, exit 1
     │
     ├─ init_minimap_geometry()
     │      Pure arithmetic on the parsed map — sizes the minimap to the
     │      map's aspect ratio. Runs before MLX exists because it needs
     │      no graphics context.
     │
     ├─ init_or_fail()
     │   ├─ load_textures()      mlx_load_png × 4 → game->tex[4]
     │   │                       Decode only: still no mlx_t* needed.
     │   ├─ config_mlx()         mlx_init + the two images:
     │   │                       main_img (3D view) then map_img (minimap),
     │   │                       in that order so the minimap sits on top.
     │   └─ init_render()        FOV lookup table, horizon, projection
     │                           plane, F/C packed to RGBA, minimap
     │                           background baked once.
     │      Each stage unwinds exactly what it had allocated on failure.
     │
     ├─ mlx_loop_hook(on_update) ┐
     ├─ mlx_cursor_hook(mouse)   │ registered, not yet running
     │                           │
     ├─ mlx_loop() ──────────────┘  ← blocks here until the window closes
     │   │
     │   └─ on_update(), once per frame:
     │       │
     │       ├─ compute()
     │       │   ├─ ESC pressed? → mlx_close_window
     │       │   ├─ view_toggles()      M / R / T, on key-down edge
     │       │   ├─ update_pl_position() move + collision box test
     │       │   └─ cast_rays()         DDA per column → rays[i].perp_dist
     │       │
     │       └─ draw_frame()
     │           ├─ draw_minimap()   if show_minimap
     │           └─ draw_3d()
     │               ├─ draw_view_tex()  if show_tex  → samples tex[face]
     │               └─ draw_view_dbg()  otherwise    → flat per-face colors
     │
     ├─ mlx_terminate()      window and images
     ├─ free_textures()      the four mlx_texture_t*
     └─ free(map_pixels_buf)

 └─ free_game(&game)         the four texture paths + the map grid → exit 0
```

</details>

## 🔨 Technical Overview

### Raycasting Engine

The 3D view is built one vertical screen column at a time:

* One ray per column — `RAY_COUNT` equals the window width (900 px), across a 60° FOV
* Wall intersections are found with **DDA** (Digital Differential Analysis) stepping through the grid
* Distance is corrected with `cos(ray angle − view angle)` to remove the fisheye bulge before projecting wall height
* The hit side (N/S vs. E/W) and ray direction select which of the four `NO`/`SO`/`WE`/`EA` textures gets sampled

### Map System

* The `.cub` file is parsed into a **dynamic** grid (`rows`/`cols` tracked per map, not a fixed-size array), together with the floor/ceiling colors and the four wall textures
* Every walkable cell (`0`, `N`, `S`, `E`, `W`, `D`) must have all four cardinal neighbors inside the grid and non-space, otherwise the map is rejected as open — checked cell-by-cell rather than via flood fill. A flood fill only walks the region reachable from the player's starting position, so a leak in a pocket the player can't reach would slip through; scanning every cell catches that case too, at the cost of not needing a visited-set or recursion
* `D` is recognized by the parser as a walkable tile, but we did **not** implement doors as an interactive bonus (no open/close state, no animation) — out of scope for this pass

### Minimap & Debug View

* Toggleable minimap (`M`) — wall layout is baked once into an offscreen buffer, then the player marker (and, with `R`, the ray fan) is redrawn on top of it each frame
* Toggleable debug view (`T`) swaps the textured walls for flat per-face colors with a floor ruled along world-tile boundaries — bent or uneven lines make perspective bugs (wrong horizon, bad fisheye correction, wrong distance scale) visible immediately, which textures alone would hide

### Textures

`assets/textures/` is organized by pack, one folder per source, each
with its own `CREDITS.txt`:

* `kenney_retro/` — the four CC0 walls (stone, brick, timber) from
  Kenney.nl's *Retro Textures Fantasy* pack, used by `subject_map.cub`
* `greybox/` — flat, high-contrast placeholder walls (11 colors ×
  grid/solid variants), useful for spotting perspective or texture-
  mapping bugs without a "real" texture's detail getting in the way
* `lodev_classic/` — the recognizable stone/eagle set from the
  classic Lodev raycasting tutorial (see [Resources](#resources))
* `caquinho/` — a *piso de caquinho* (broken-tile terrazzo, a staple
  of Brazilian architecture) turned into a wall texture: one original
  plus 6 recolors (blue, green, pink, lilac, gray, salmon), all
  generated from the same source image via a hue-rotation script —
  same shapes and grout lines, different color story per variant

Several `maps/valid/*.cub` files exist mainly to show these packs off
in combination (`caquinho_pilares.cub`, `greybox_l.cub`,
`lodev_map01.cub`, etc.) rather than to test any specific parser edge
case.

## 📚 Resources

### References and documentation

- [Ray Casting Tutorial](https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/) — F. Permadi's classic step-by-step walkthrough of the raycasting algorithm
- [Raycasting engine explanation (video)](https://www.youtube.com/watch?v=gYRrGTC7GtA)
- [Raycasting engine explanation (video)](https://www.youtube.com/watch?v=PC1RaETIx3Y)
- [MLX42](https://github.com/codam-coding-college/MLX42) — the graphics library used to open the window and draw pixels


### AI usage

The AI assistant Claude (Anthropic), via Claude Code, was used as a
pair-programming aid throughout the project, mainly for:

- **Reviewing and explaining parser code** — discussing design decisions
  (for example, why `check_color_part` should not call `handle_error`
  directly, since it doesn't own the `parts` array it allocates) and
  generating study plans to consolidate understanding of the parser
  (`parse_color`, `has_closed_walls`, validating maps with openings via
  interior spaces, not just at the border).
- **Norminette-driven refactoring** — splitting long functions into
  smaller, single-responsibility units (`validate_config`,
  `read_meta_lines`, `build_map_grid`), respecting the line-count and
  variable-count limits per function.
- **Merge/integration recon between branches** — before merging the
  parser (`feat/parser-todo`) with the rendering branch (`feat/render`),
  the AI helped map out real vs. trivial conflicts, and identified a
  *silent* conflict that git didn't flag (renaming `player->dir` to
  `dir_c`, which broke the build without producing a merge conflict) and
  a pre-existing bug in `utils.c` (an implicit concatenation of two
  adjacent string literals in the error-message array, shifting
  `ERR_MLX` out of its expected index).
- **Asset curation** — researching and selecting CC0 (public domain)
  wall textures from Kenney.nl's *Retro Textures Fantasy* pack
  ([kenney.nl/assets/retro-textures-fantasy](https://kenney.nl/assets/retro-textures-fantasy)),
  cropped to 64x64, plus organizing `assets/textures/` by pack once
  more sources (Lodev's classic set, a custom terrazzo texture) were
  added. The `caquinho/` palette variants were generated with a small
  HSV hue-rotation script, not hand-edited pixel by pixel.
- **Pre-defense QA pass** — working through the mandatory-part
  checklist end to end and finding real bugs: `W`/`A`/`S`/`D` movement
  was entirely missing (only arrow keys existed); a repeated
  identifier in a `.cub` file (e.g. two `NO` lines) was silently
  accepted instead of erroring; a failed texture load printed the
  right error message but still exited `0` instead of a failure code,
  because `render()` was `void` and nothing checked whether it had
  succeeded. Also caught a genuine memory leak with macOS's `leaks`
  tool (not guessed) — a map line allocated by `get_next_line` wasn't
  freed on one specific error path — and fixed it, verifying 0 leaks
  across every valid and invalid test map afterward.

The AI did not write the final ray-casting logic or the projection math
— those parts were implemented and debugged by hand by the pair, with
the AI acting as a review aid, debugging support, and study resource.
