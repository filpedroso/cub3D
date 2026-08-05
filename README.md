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
| `M`                     | Toggle minimap display                |
| `R`                     | Toggle ray display (debug)            |
| `T`                     | Toggle between textures and debug view|
| `ESC` / close window    | Quit the game                         |

### `.cub` file format

```
NO ./textures/north.png
SO ./textures/south.png
WE ./textures/west.png
EA ./textures/east.png

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

* Four CC0 wall textures (stone, brick, timber) from Kenney.nl's *Retro Textures Fantasy* pack, cropped to 64x64 — see [Resources](#resources) below
* A visual/thematic pass on the textures is planned as a follow-up, once the README and core features are settled

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
  cropped to 64x64 (see `assets/textures/CREDITS.txt`).

The AI did not write the final ray-casting logic or the projection math
— those parts were implemented and debugged by hand by the pair, with
the AI acting as a review aid, debugging support, and study resource.
