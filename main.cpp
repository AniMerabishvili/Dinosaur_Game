#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

class Dinosaur {
private:
    int posX;
    int posY;
    int jumpHeight;

public:
    Dinosaur(int posX, int posY, int jumpHeight)
        : posX(posX), posY(posY), jumpHeight(jumpHeight) {}

    void draw(WINDOW* win) {
        mvwprintw(win, posY - jumpHeight - 3, posX, "            ___");
        mvwprintw(win, posY - jumpHeight - 2, posX, "           / ._)");
        mvwprintw(win, posY - jumpHeight - 1, posX, "    .-^^^-/ /");
        mvwprintw(win, posY - jumpHeight, posX, " __/       /");
        mvwprintw(win, posY - jumpHeight + 1, posX, "<__.|_|-|_|");
    }

    void jump() {
        if (jumpHeight == 0) {
            jumpHeight = 12;
        }
    }

    void update() {
        if (jumpHeight > 0) {
            jumpHeight--;
        }
    }

    bool isOnGround() const {
        return jumpHeight == 0;
    }

    int getX() const {
        return posX;
    }

    int getY() const {
        return posY - jumpHeight;
    }

    int getJumpHeight() const {
        return jumpHeight;
    }

    // Hitbox for collision (approximate body center)
    int hitLeft() const { return posX + 2; }
    int hitRight() const { return posX + 12; }
    int hitBottom() const { return posY - jumpHeight + 1; }
};

class Cactus {
private:
    int posX;
    int posY;

public:
    Cactus(int x, int y) : posX(x), posY(y) {}

    void draw(WINDOW* win) {
        mvwprintw(win, posY - 2, posX, " _  ");
        mvwprintw(win, posY - 1, posX, "|_| ");
        mvwprintw(win, posY, posX, "|| |");
        mvwprintw(win, posY + 1, posX, "||_|");
    }

    void move(int amount = 1) {
        posX -= amount;
    }

    void reset(int x, int y) {
        posX = x;
        posY = y;
    }

    int getX() const { return posX; }
    int getY() const { return posY; }
    int hitLeft() const { return posX; }
    int hitRight() const { return posX + 2; }
    int hitTop() const { return posY - 2; }
};

class Game {
private:
    WINDOW* win;
    int score;
    int highScore;
    int speed;
    bool gameOver;
    bool running;
    Dinosaur dinosaur;
    std::vector<Cactus> cacti;
    int frame;
    int groundY;

    bool collides(const Cactus& c) const {
        // Only collide when dinosaur is low enough to hit cactus
        if (dinosaur.getJumpHeight() > 6) {
            return false;
        }
        bool overlapX = dinosaur.hitRight() >= c.hitLeft() &&
                        dinosaur.hitLeft() <= c.hitRight();
        return overlapX;
    }

    void spawnCactus() {
        int lastX = cacti.empty() ? COLS : cacti.back().getX();
        int gap = 35 + (rand() % 40);
        cacti.push_back(Cactus(std::max(lastX + gap, COLS + 5), groundY));
    }

    void resetGame() {
        score = 0;
        speed = 45;
        gameOver = false;
        frame = 0;
        dinosaur = Dinosaur(8, groundY - 1, 0);
        cacti.clear();
        cacti.push_back(Cactus(COLS - 10, groundY));
        cacti.push_back(Cactus(COLS + 40, groundY));
    }

public:
    Game(WINDOW* window)
        : win(window),
          score(0),
          highScore(0),
          speed(45),
          gameOver(false),
          running(true),
          dinosaur(8, LINES - 4, 0),
          frame(0),
          groundY(LINES - 3) {
        resetGame();
    }

    void run() {
        nodelay(win, TRUE);
        keypad(win, TRUE);

        while (running) {
            if (gameOver) {
                showGameOver();
                continue;
            }

            werase(win);

            // Ground
            for (int x = 0; x < COLS; x++) {
                mvwaddch(win, groundY + 1, x, '_');
            }

            // HUD
            mvwprintw(win, 1, 2, "DINOSAUR GAME");
            mvwprintw(win, 1, COLS - 22, "Score: %04d", score);
            mvwprintw(win, 2, COLS - 22, "Best:  %04d", highScore);
            mvwprintw(win, 2, 2, "SPACE / UP = jump   Q = quit");

            // Input
            int c = wgetch(win);
            if (c == ' ' || c == KEY_UP) {
                dinosaur.jump();
            }
            if (c == 'q' || c == 'Q') {
                running = false;
                break;
            }

            dinosaur.update();
            dinosaur.draw(win);

            // Move obstacles; speed up slowly with score
            int moveAmount = 1 + (score / 15);
            for (auto& cactus : cacti) {
                cactus.move(moveAmount);
                cactus.draw(win);

                if (collides(cactus)) {
                    gameOver = true;
                    if (score > highScore) {
                        highScore = score;
                    }
                }
            }

            // Score when cactus passes dinosaur
            for (auto& cactus : cacti) {
                if (cactus.getX() + 2 == dinosaur.getX()) {
                    score++;
                }
            }

            // Recycle off-screen cacti
            cacti.erase(
                std::remove_if(cacti.begin(), cacti.end(),
                    [](const Cactus& c) { return c.getX() < -5; }),
                cacti.end());

            while (cacti.size() < 2) {
                spawnCactus();
            }

            // Keep cacti spaced
            if (!cacti.empty() && cacti.back().getX() < COLS - 20) {
                spawnCactus();
            }

            wrefresh(win);

            int delay = std::max(18, speed - (score / 2));
            usleep(delay * 1000);
            frame++;
        }

        endwin();
    }

    void showGameOver() {
        werase(win);
        mvwprintw(win, LINES / 2 - 2, COLS / 2 - 8, "GAME OVER!");
        mvwprintw(win, LINES / 2, COLS / 2 - 12, "Final Score: %d", score);
        mvwprintw(win, LINES / 2 + 1, COLS / 2 - 12, "Best Score:  %d", highScore);
        mvwprintw(win, LINES / 2 + 3, COLS / 2 - 16, "Press SPACE to play again");
        mvwprintw(win, LINES / 2 + 4, COLS / 2 - 10, "Press Q to quit");
        wrefresh(win);

        int c = wgetch(win);
        if (c == ' ' || c == KEY_UP) {
            resetGame();
        } else if (c == 'q' || c == 'Q') {
            running = false;
            gameOver = false;
        }
        usleep(50000);
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    if (LINES < 20 || COLS < 60) {
        endwin();
        printf("Terminal too small. Need at least 60x20.\n");
        return 1;
    }

    WINDOW* win = stdscr;
    Game game(win);
    game.run();

    return 0;
}
