#include "kernel.h"
#include "vga.h"
#include "idt.h"
#include "keyboard_map.h"
#include "utils.h"
#include "font.h"

#define TILE_SIZE   20
#define GRID_WIDTH  (SCREEN_WIDTH  / TILE_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / TILE_SIZE)
#define GRID_SIZE   (SCREEN_SIZE   / TILE_SIZE)

#define START_MENU_WIDTH  125
#define START_MENU_HEIGHT 66
#define G_O_MENU_WIDTH    160
#define G_O_MENU_HEIGHT   76
#define START_MENU_COLOR  0x1
#define G_O_MENU_COLOR    0x1

#define SNAKE_COLOR 0x2
#define BG_COLOR    0x0
#define FOOD_COLOR  0x4

#define FPS 10

char running = 0;
char start   = 1;

int score;

char x[GRID_SIZE] = { };
char y[GRID_SIZE] = { };
int lenght;
char vx, vy;

char fx, fy;

void random_food() {
    if (lenght >= GRID_SIZE) { running = 0; return; }

    fx = rand_range(0, GRID_WIDTH - 1);
    fy = rand_range(0, GRID_HEIGHT - 1);

    for (int i = lenght; i > 0; i--) {
        if (x[i] == fx && y[i] == fy) {
            random_food();
        }
    }
}

void restart(int new_food) {
    score = 0;

    lenght = 3;
    vx = 1;
    vy = 0;
    memset(x, 0, GRID_SIZE);
    memset(y, 0, GRID_SIZE);

    x[0] = 3;
    x[1] = 2;
    x[2] = 1;
    y[0] = y[1] = y[2] = 1;

    if (new_food) { random_food(); }

    running = 1;
}

char next_keycode = -1;
void key_press(char keycode) {
    if (running) {
        next_keycode = keycode;
    } else {
        if (keycode == 57) { // space
            if (start) {
                start = 0;
                restart(0);
            } else {
                restart(1);
            }
        }
    }
}

void draw_tile(int x, int y, unsigned short color) {
    rect(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
}

char redraw = 1;
void draw() {
    if (running) { redraw = 1; }
    if (!redraw) { return; }

    clear_screen(BG_COLOR);

    draw_tile(fx, fy, FOOD_COLOR);

    for (int i = 0; i < lenght; i++) {
        draw_tile(x[i], y[i], SNAKE_COLOR);
    }

    if (!running) {
        if (start) {
            rect(
                (SCREEN_WIDTH - START_MENU_WIDTH) * 0.5, (SCREEN_HEIGHT - START_MENU_HEIGHT) * 0.5,
                START_MENU_WIDTH, START_MENU_HEIGHT, START_MENU_COLOR
            );
            font_str_big("SNAKE", (SCREEN_WIDTH - 2 * font_width("SNAKE")) * 0.5, ((SCREEN_HEIGHT - START_MENU_HEIGHT) * 0.5) + 10, 0xF);
            font_str("Press SPACE", (SCREEN_WIDTH - font_width("Press SPACE")) * 0.5, ((SCREEN_HEIGHT - START_MENU_HEIGHT) * 0.5) + 36, 0xF);
            font_str("to start", (SCREEN_WIDTH - font_width("to start")) * 0.5, ((SCREEN_HEIGHT - START_MENU_HEIGHT) * 0.5) + 46, 0xF);
        } else {
            char buf_score[64];
            itoa(score, buf_score, 64);

            char score_msg[72] = "Score: ";
            strcat(score_msg, buf_score);

            rect(
                (SCREEN_WIDTH - G_O_MENU_WIDTH) * 0.5, (SCREEN_HEIGHT - G_O_MENU_HEIGHT) * 0.5,
                G_O_MENU_WIDTH, G_O_MENU_HEIGHT, G_O_MENU_COLOR
            );
            font_str_big("GAME OVER", (SCREEN_WIDTH - 2 * font_width("GAME OVER")) * 0.5, ((SCREEN_HEIGHT - G_O_MENU_HEIGHT) * 0.5) + 10, 0xF);
            font_str(score_msg, (SCREEN_WIDTH - font_width(score_msg)) * 0.5, ((SCREEN_HEIGHT - G_O_MENU_HEIGHT) * 0.5) + 36, 0xF);
            font_str("Press SPACE", (SCREEN_WIDTH - font_width("Press SPACE")) * 0.5, ((SCREEN_HEIGHT - G_O_MENU_HEIGHT) * 0.5) + 46, 0xF);
            font_str("to play again", (SCREEN_WIDTH - font_width("to play again")) * 0.5, ((SCREEN_HEIGHT - G_O_MENU_HEIGHT) * 0.5) + 56, 0xF);
        }

        redraw = 0;
    }
}

void update_vel() {
    if (next_keycode == -1) { return; }

    char key = keyboard_map[next_keycode];
    if (key == 'w' && vy != 1) {
        vx = 0;
        vy = -1;
    } else if (key == 'a' && vx != 1) {
        vx = -1;
        vy = 0;
    } else if (key == 's' && vy != -1) {
        vx = 0;
        vy = 1;
    } else if (key == 'd' && vx != -1) {
        vx = 1;
        vy = 0;
    }

    next_keycode = -1;
}

void update() {
    if (!running) { return; }

    update_vel();

    for (int i = lenght; i > 0; i--) {
        x[i] = x[i - 1];
        y[i] = y[i - 1];
    }

    x[0] += vx;
    y[0] += vy;

    if (x[0] < 0) { x[0] = GRID_WIDTH - 1; }
    else if (x[0] >= GRID_WIDTH) { x[0] = 0; }
    if (y[0] < 0) { y[0] = GRID_HEIGHT - 1; }
    else if (y[0] >= GRID_HEIGHT) { y[0] = 0; }

    if (x[0] == fx && y[0] == fy) {
        lenght++;
        score++;
        random_food();
    }

    for (int i = lenght; i > 0; i--) {
        if (x[i] == x[0] && y[i] == y[0]) {
            running = 0;
        }
    }
}

void kernel_main() {
    init_idt();
    init_kb();
    init_pit();
    enable_ints();

    restart(1);
    running = 0;

    unsigned int last = 0;
    while (1) {
        unsigned int now = (unsigned int) get_ticks();
        if ((now - last) > (TIMER_TPS / FPS)) {
            last = now;

            draw();
            update();
        }
    };
}