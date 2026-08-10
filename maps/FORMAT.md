# `.cub` scene file format

A scene file is metadata first, map last. Elements may appear in any order and
be separated by blank lines; the map is always the final block and is parsed
exactly as it looks in the file.

## Elements

| Identifier | Meaning | Example |
| --- | --- | --- |
| `NO` | North wall texture | `NO ./assets/textures/kenney_retro/north.png` |
| `SO` | South wall texture | `SO ./assets/textures/kenney_retro/south.png` |
| `WE` | West wall texture | `WE ./assets/textures/kenney_retro/west.png` |
| `EA` | East wall texture | `EA ./assets/textures/kenney_retro/east.png` |
| `F` | Floor colour, `R,G,B` in `[0,255]` | `F 220,100,0` |
| `C` | Ceiling colour, `R,G,B` in `[0,255]` | `C 225,30,0` |

All four textures and both colours are required, exactly once each. Texture
paths are resolved relative to the working directory, so run the binary from
the repository root. Only `.png` is accepted — MLX42 decodes nothing else.

## Map characters

| Char | Meaning |
| --- | --- |
| `0` | Walkable floor |
| `1` | Wall |
| `N` | Player start, facing North |
| `S` | Player start, facing South |
| `E` | Player start, facing East |
| `W` | Player start, facing West |
| ` ` | Void — valid inside the map, must never touch a walkable cell |

Exactly one spawn marker. The walkable area must be fully enclosed by walls:
every `0`/`N`/`S`/`E`/`W` cell needs its four neighbours inside the grid and
non-void. Rows may be ragged; they are padded with spaces after validation.

Doors (`D`) are **not** implemented — the parser rejects them as an invalid
map character.

## Minimal example

```
NO ./assets/textures/kenney_retro/north.png
SO ./assets/textures/kenney_retro/south.png
WE ./assets/textures/kenney_retro/west.png
EA ./assets/textures/kenney_retro/east.png

F 220,100,0
C 225,30,0

111111
100101
101001
1100N1
111111
```

## Test scenes

- `maps/valid/` — scenes that must load. `subject_map.cub` is the one from the
  subject: ragged rows plus interior voids, so it is the one that exercises
  `pad_grid` and the wall-closure check hardest.
- `maps/invalid/` — one file per rejection path. Every one must print
  `Error` plus a message on **stderr** and exit `1`, without crashing.
