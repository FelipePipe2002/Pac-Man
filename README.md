# Pac-Man (Console Edition)

A Pac-Man clone written in modern C++ (C++20) that runs entirely inside the Windows terminal. Everything is drawn with colored text and emoji. Maps are loaded from JSON files, and each ghost has its own pathfinding AI.

```
          ███████╗  █████╗  ██████╗     ███╗   ███╗ █████╗ ███╗   ██╗
          ██╔═══██╗██╔══██╗██╔════╝     ████╗ ████║██╔══██╗████╗  ██║
          ███████╔╝███████║██║          ██╔████╔██║███████║██╔██╗ ██║
          ██╔════╝ ██╔══██║██║          ██║╚██╔╝██║██╔══██║██║╚██╗██║
          ██║      ██║  ██║╚██████╗     ██║ ╚═╝ ██║██║  ██║██║ ╚████║
          ╚═╝      ╚═╝  ╚═╝ ╚═════╝     ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝
```

---

## Table of contents

- [Features](#features)
- [Requirements](#requirements)
- [Building](#building)
- [Running](#running)
- [How to play](#how-to-play)
- [Game rules and scoring](#game-rules-and-scoring)
- [Ghost AI](#ghost-ai)
- [Creating your own maps](#creating-your-own-maps)
- [Project structure](#project-structure)
- [Architecture overview](#architecture-overview)
- [Credits](#credits)

---

## Features

- 🎮 **Runs in the terminal.** The game is rendered with ANSI colors and emoji, with no graphics window.
- 🗺️ **Map selector.** Any `.json` file in `maps/` shows up on the animated start screen.
- 👻 **Ghost AI.** Ghosts use BFS pathfinding (portals included) and switch between Scatter, Chase, Frightened and Eaten states.
- ⚡ **Power pellets.** Eating one turns the ghosts blue so you can eat them, and the points double for each ghost you catch.
- 🍒 **Bonus fruit.** A cherry appears twice per level.
- 🌀 **Portals.** Tunnels can link any two tiles, not just the left and right edges.
- 🎨 **Custom map colors.** Each map sets its own wall color, and each ghost sets its own color.
- 🔁 **Play again screen** with an animated Pac-Man.

---

## Requirements

| Requirement | Notes |
|---|---|
| **Windows 10 / 11** | Input and console handling use the Win32 API (`GetAsyncKeyState`, `SetConsoleMode`, …). |
| **CMake ≥ 3.18** | Used to generate the build. |
| **A C++20 compiler** | MSVC (Visual Studio 2019/2022) is recommended. ClangCL also works. |
| **Git** | CMake uses `FetchContent` to download dependencies (jngl, SDL2). |
| **An emoji-capable terminal** | [Windows Terminal](https://aka.ms/terminal) is strongly recommended. The legacy `conhost` window may not draw emoji correctly. |

---

## Building

The repository includes a helper script, `generate_sln.bat`, that wraps the CMake commands.

### Option A: Visual Studio solution (recommended)

```bat
generate_sln.bat open
```

This generates `build/pacman.sln` and opens it in Visual Studio. Set **`pacman`** as the startup project and press **F5**.

### Option B: Command line

```bat
:: Configure
cmake project -Bbuild -DCMAKE_POLICY_VERSION_MINIMUM=3.5

:: Build
cmake --build build --config Release
```

### Option C: ClangCL toolset

```bat
generate_sln.bat clang open
```

> **Note:** The first configure takes a while, because CMake downloads and builds the dependencies.

---

## Running

The game loads maps from `../maps/`, **relative to the current working directory**. Run it from inside the `build` folder:

```bat
cd build
Release\pacman.exe
```

When you launch from Visual Studio, the default working directory is already `build/`, so it works without changes.

For the best experience, use **Windows Terminal** with a font that supports emoji, and make the window large enough to fit the whole map.

---

## How to play

### Controls

| Key | Action |
|---|---|
| `W` / `↑` | Move up |
| `A` / `←` | Move left |
| `S` / `↓` | Move down |
| `D` / `→` | Move right |
| `Enter` | Start a game / play again |
| `Esc` | Quit the current game / exit |

On the **start screen**, use `↑` / `↓` (or `W` / `S`) to pick a map and press **Enter** to start.

Movement is **buffered**. If you press a direction before reaching a corner, Pac-Man turns as soon as that path opens up, just like in the arcade game.

### Symbols

| Symbol | Meaning |
|---|---|
| 🌜 / 🌛 | Pac-Man (facing right / left) |
| 👻 | Ghost (in its own color, or dark blue when frightened) |
| 👀 | Eaten ghost returning home |
| `.` | Dot |
| ⚈ | Power pellet |
| 🍒 | Bonus fruit |
| `██` | Wall |

---

## Game rules and scoring

| Item | Points |
|---|---|
| Dot | **10** |
| Power pellet | **50** |
| Bonus fruit 🍒 | **100** |
| 1st ghost eaten (per pellet) | **200** |
| 2nd ghost | **400** |
| 3rd ghost | **800** |
| 4th ghost | **1600** |

- **Win:** eat every dot on the map.
- **Lose:** touch a ghost that is not frightened or eaten.
- **Power pellets** frighten all ghosts for **8 seconds**. Frightened ghosts move randomly and a bit slower. The ghost-point multiplier resets when the effect ends.
- **Bonus fruit** spawns at Pac-Man's starting tile after **65%** and again after **85%** of the dots are eaten. Each fruit disappears after **7 seconds**.
- **Ghost cycle:** ghosts alternate between **Scatter (7 s)** and **Chase (20 s)**.
- **Eaten ghosts** run back to the ghost home (`H`) as 👀, then rejoin the game.

The game runs at a fixed 25 ms tick. Pac-Man and normal ghosts move every 150 ms, frightened ghosts every 175 ms, and eaten ghosts every 125 ms.

---

## Ghost AI

Every ghost uses a **BFS pathfinder** (`MovementStrategy::findWay`) that knows about portals and never reverses direction. What changes between ghosts is the **target tile** they chase in **Chase** mode. Each map's JSON sets a ghost's behavior with its `mode` letter:

| Mode | Name | Behavior in Chase mode |
|---|---|---|
| `B` | **Blinky** | Goes straight for Pac-Man's current tile. |
| `P` | **Pinky** | Aims 2 tiles ahead of where Pac-Man is facing, trying to cut him off. If that tile is a wall, chases Pac-Man directly. |
| `C` | **Clyde** | Chases Pac-Man while more than 8 tiles away. Once closer, retreats to his scatter corner. |
| `O` | **Portal** | Original ghost: when Pac-Man is near a portal, it heads to the *other side* of that portal to ambush him. Otherwise it patrols its scatter point. |
| `I` | **Inky** | *Reserved / not implemented yet.* |

The other states behave the same for every ghost:

| State | Behavior |
|---|---|
| **Scatter** | Heads to its assigned scatter point. |
| **Frightened** | Turns dark blue and wanders randomly. |
| **Eaten** | Turns into 👀 and pathfinds back to the ghost home. |

---

## Creating your own maps

Drop a new `.json` file into the `maps/` folder. It appears on the start screen automatically, with underscores in the file name shown as spaces (for example, `My_Cool_Map.json` → "My Cool Map"). A commented template is in [`maps/MAP_TEMPLATE.txt`](maps/MAP_TEMPLATE.txt).

### Example

```json
{
  "name": "Classic Map",
  "width": 28,
  "height": 22,
  "mapcolor": "NB",
  "layout": [
    "############################",
    "#............##............#",
    "...",
    "      .   #  H   #   .      ",
    "     #.##    P     ##.#     ",
    "..."
  ],
  "scatterPoints": {
    "1": [1, 2],
    "2": [26, 2]
  },
  "portals": {
    "1": [[0, 10], [27, 10]]
  },
  "ghosts": [
    { "id": 1, "color": "NR", "pos": [13, 8],  "scatter": "1", "mode": "B" },
    { "id": 2, "color": "NM", "pos": [12, 10], "scatter": "2", "mode": "P" }
  ]
}
```

### Fields

| Field | Description |
|---|---|
| `name` | Name of the map. |
| `width`, `height` | Size of the grid. **Every** `layout` row must be exactly `width` characters long, and there must be exactly `height` rows. |
| `mapcolor` | Wall color (see color codes below). |
| `layout` | The map, one string per row (see tile characters below). |
| `scatterPoints` | `"id": [x, y]`. Corners the ghosts go to in Scatter mode. **Each scatter point also becomes a power pellet.** They must be on walkable tiles. |
| `portals` | `"id": [[x1, y1], [x2, y2]]`. Two linked tiles. Stepping on one teleports you to the other. |
| `ghosts` | List of ghosts: `id`, `color`, starting `pos` `[x, y]`, the `scatter` point id, and the chase `mode` (`B`, `P`, `C`, `O`). |

Coordinates are `[x, y]`, with `x` the column and `y` the row, both **starting at 0** from the top-left corner.

### Tile characters

| Char | Meaning |
|---|---|
| `#` | Wall |
| `.` | Walkable tile with a dot |
| ` ` (space) | Empty walkable tile |
| `P` | Pac-Man's starting position (also where fruit spawns) |
| `H` | Ghost home (where eaten ghosts respawn) |

### Color codes

A color is two letters: a **brightness** followed by a **base color**.

| Brightness | | Base color | |
|---|---|---|---|
| `N` | Normal | `R` | Red |
| `D` | Dark | `G` | Green |
| `L` | Light | `Y` | Yellow |
| | | `B` | Blue |
| | | `M` | Magenta |
| | | `C` | Cyan |
| | | `W` | White |

For example, `NB` is normal blue, `LR` is light red and `DG` is dark green.

---

## Project structure

```
Pac-Man/
├── include/pacman/          # Headers
│   ├── Board.h              # Grid, tiles, portals, scatter points
│   ├── GameLogic.h          # GameData + all game rules
│   ├── Rendered.h           # Terminal renderer / screens
│   ├── entities/
│   │   ├── Entity.h         # Base class (position + last position)
│   │   ├── Player.h         # Pac-Man
│   │   ├── Ghost.h          # Ghost + GhostState
│   │   └── MovementStrategy.h  # Ghost AI strategies (Strategy pattern)
│   ├── json/
│   │   ├── json.hpp         # nlohmann/json (single header)
│   │   └── MapJson.h        # Map file schema / (de)serialization
│   └── utils/               # Color, Console, Coordinate, Direction,
│                            # InputManager, StrongType
├── src/
│   ├── main.cpp             # Entry point and main game loop
│   └── pacman/              # Implementations of the headers above
├── maps/                    # Playable maps (*.json) + MAP_TEMPLATE.txt
├── data/                    # Assets copied next to the build
├── project/                 # CMake project + reusable cmake utilities
│   ├── CMakeLists.txt
│   ├── project_config.cmake # Project name, C++ standard, dependencies
│   └── cmake_utils/
├── generate_sln.bat         # Helper to configure / build / open the solution
├── .clang-format            # Code style
└── .clang-tidy              # Static analysis rules
```

---

## Architecture overview

The game loop in `src/main.cpp` runs once every 25 ms and does the following each frame:

1. **Input.** `InputManager` reads the keyboard and stores the last direction pressed.
2. **Turn.** If the buffered direction is walkable, Pac-Man turns.
3. **Update.** `gameLogic` advances the frame counters, then moves Pac-Man and the ghosts (each at its own speed). It also handles dot, pellet, ghost and fruit pickups, and the timers for Scatter, Chase and Frightened.
4. **Render.** `Renderer::drawGrid` builds the whole frame into one string and writes it in a single call, with the cursor moved back home first, to avoid flicker.
5. **Win / lose check.** When the game ends, the loop moves on to the play-again screen.

Main design choices:

- **Strategy pattern for ghost AI.** Each `Ghost` owns separate `MovementStrategy` objects for Chase, Scatter, Frightened and Eaten. Adding a new ghost personality only takes a new strategy class and a letter in `strategyFromString`.
- **Data-driven maps.** All level data (layout, colors, ghosts, portals) lives in JSON, parsed with [nlohmann/json](https://github.com/nlohmann/json).
- **Strong types.** `StrongType<int, Tag>` keeps values like `Width` and `Height` from being mixed up.

---

## Credits

- Made by **FelipePipe2002**.
- Thanks to **Franco** for the emoji idea and for the idea behind the Portal ghost.
- JSON parsing with [nlohmann/json](https://github.com/nlohmann/json).
- CMake project template uses [jngl](https://github.com/jhasse/jngl) and [SDL2](https://github.com/libsdl-org/SDL).
- Inspired by the original *Pac-Man* © Bandai Namco. This is a fan-made, non-commercial project.
