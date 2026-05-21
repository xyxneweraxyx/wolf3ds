# wolf3d — 3D Raycasting Game

A Wolfenstein-style 3D raycasting engine built in C with CSFML. Features textured walls, collision detection, player movement, and jumping, built on top of a custom OOP-style framework.

## Build

```sh
make
./wolf3d
```

Requires `libcsfml-graphics`, `libcsfml-window`, `libcsfml-system`, `libcsfml-audio`, and `libm`.

## Controls

| Key | Action |
|-----|--------|
| `Z` | Move forward |
| `S` | Move backward |
| `Q` | Rotate left |
| `D` | Rotate right |
| `Space` | Jump |

## Architecture

The main struct `wolf_t` acts as a god object and holds every subsystem.

```
source/
  src/
    wolf3d.c             — Entry point: allocates wolf_t, wires everything, starts the loop
    callbacks/
      raycast.c          — Connects the raycaster to LOOP_DRAW; applies texture + distance shading
      keyboard.c         — Connects player movement and jump to LOOP_DATA; handles wall collisions
    class_creator/       — Initializes classhandler classes and default entities (player, items)
    csfml/init_csfml.c   — Initializes setfml + buttonfml, loads wall texture
    destroy.c            — Cleanup helpers (destroy all subsystems, free map)

  classes/
    player/              — player_t: position, rotation, speed, gravity, jump; GET/SET/ADD/gameplay methods
    item/                — item_t: world positions, detection radius/angle, spawn delay; closest/pickup logic
    enemy/               — Stub (planned)
    inventory/           — Stub (planned)

  libs/
    raycaster/           — DDA raycasting engine; produces one column per screen pixel, with fish-eye correction and a user-supplied modification callback for shading/texturing
    setfml/              — CSFML wrapper: event-driven callback system, sprite/texture management, configurable loop rates (event, data, render, draw)
    buttonfml/           — Button abstraction on top of setfml: idle/hover/click textures, per-button callbacks
    classhandler/        — Minimal OOP system: classes (typed by byte_size) + named entities with fetch-by-name/comp/score API
    c_alloc/             — malloc wrapper with retry logic, pointer tracking, and realloc
    linkedlist/          — Doubly linked list
    str/                 — String utility functions

bonus/
  tests/                 — Manual/disabled test files kept outside the main build tree
```

### Raycaster

`raycast_raycast` casts one ray per screen column using the DDA algorithm. Each ray steps through the map grid until it hits a wall character, computes perpendicular distance (correcting for fish-eye), and draws a scaled vertical rectangle. A `modification` function pointer on `raycast_t` lets the caller customize each column's color and texture — currently used for distance-based shading and texture mapping with horizontal face offset (`face_x`).

### setfml event loop

`setfml` maintains 27 linked lists of callbacks (one per `sfEventType` + `LOOP_DATA`, `LOOP_RENDER`, `LOOP_DRAW`). Each iteration checks nanosecond-precision timers to decide which loops to fire, then dispatches callbacks in order. `buttonfml` hooks into this system by registering its own mouse move/click/release handlers automatically on init.

### classhandler

A lightweight OOP layer: `classhandler_classcreate` registers a type by `sizeof(struct)`, and `classhandler_entitycreate` allocates a named instance of that type. Entities are retrieved by name, by a comparison function, or by a score function (highest score wins). All memory goes through the shared `c_alloc_t`.

## What's next (from `todo`)

Near future: ceiling/floor rendering, world items (first real sprite test), basic UI. Further out: inventory, enemies with decision trees, multi-floor ladders.

## Author

EPITECH PROJECT, 2025 — wolf3d
