# Dinosaur Game

Chrome-style dinosaur runner with your ASCII dino art — playable in the browser, plus a terminal (ncurses) C++ version.

## Play online (GitHub Pages)

**Demo URL (after Pages is enabled):**  
https://animerabishvili.github.io/Dinosaur_Game/

### Enable GitHub Pages

1. Open the repo: https://github.com/AniMerabishvili/Dinosaur_Game  
2. Go to **Settings → Pages**  
3. Under **Source**, choose **Deploy from a branch**  
4. Branch: **main**, folder: **/ (root)** → **Save**  
5. Wait ~1 minute, then open the demo URL above

Controls: **Space** / **↑** / tap to jump.

## Play locally (web)

Open `index.html` in a browser, or serve the folder:

```bash
# Python
python -m http.server 8000
# then visit http://localhost:8000
```

## Terminal version (C++ / ncurses)

Requires a C++ compiler and ncurses (Linux/macOS, or WSL/MSYS2 on Windows).

```bash
make
./dinosaur
```

Or:

```bash
g++ -std=c++17 main.cpp -o dinosaur -lncurses
./dinosaur
```

Controls: **Space** / **↑** jump, **Q** quit.

## Project files

| File | Purpose |
|------|---------|
| `index.html` / `style.css` / `game.js` | Browser demo (GitHub Pages) |
| `main.cpp` | Terminal ncurses game |
| `Makefile` | Build the C++ game |
