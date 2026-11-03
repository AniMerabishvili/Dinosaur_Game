# Dinosaur Game

A Chrome-style dinosaur runner with ASCII art, written in C++17 for the terminal (ncurses), with a JavaScript version that runs in the browser.

## Terminal version (C++17 / ncurses)

Requires a C++ compiler and ncurses (Linux/macOS, or WSL/MSYS2 on Windows).

```bash
make
./dinosaur
```

Or build directly:

```bash
g++ -std=c++17 main.cpp -o dinosaur -lncurses
./dinosaur
```

Controls: **Space** / **↑** jump, **Q** quit.

## Browser version (JavaScript)

Play online: https://animerabishvili.github.io/Dinosaur_Game/

Controls: **Space** / **↑** / tap to jump.

To run it locally, open `index.html` in a browser, or serve the folder:

```bash
python -m http.server 8000
# then visit http://localhost:8000
```

## Project files

| File | Purpose |
|------|---------|
| `main.cpp` | Terminal game (C++17, ncurses) |
| `Makefile` | Builds the C++ game |
| `index.html` / `style.css` / `game.js` | Browser version (GitHub Pages) |

## Author

Ani Merabishvili, 2026
