#include <ncurses.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>

#define HEIGHT 23
#define WIDTH 79

int movement(int* snakeX, int* snakeY, int moveX, int moveY, int speed, WINDOW* win)
{
    *snakeX = *snakeX + moveX;
    *snakeY = *snakeY + moveY;
    mvwprintw(win, *snakeY, *snakeX, "0");
    usleep(100000 * speed);
}

int kbhit(void)
{
    struct timeval tv;
    fd_set read_fd;

    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&read_fd);
    FD_SET(0, &read_fd);

    if (select(1, &read_fd, NULL, NULL, &tv) == -1)
        return 0;

    if (FD_ISSET(0, &read_fd))
        return 1;

    return 0;
}

int game_border(int snakeX, int snakeY)
{
    if (snakeX < 1 || snakeX > WIDTH-2) {
        return 1;
    } else if (snakeY < 1 || snakeY > HEIGHT-2) {
        return 1;
    }
    return 0;
}

int main()
{
    int snake_body[100][2];
    int score = 0;
    int snakeX = 1;
    int snakeY = 1;
    int action;
    int moveX = 1;
    int moveY = 0;
    int speed = 1;

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 2; j++) {
            snake_body[i][j] = 0;
        }
    }
    snake_body[0][0] = 3;
    snake_body[0][1] = 3;

    initscr();
    curs_set(false);
    noecho();

    WINDOW* win = newwin(HEIGHT, WIDTH, 1, 1);
    keypad(win, TRUE);
    box(win, 0, 0);
    wrefresh(win);

    while (!game_border(snakeX, snakeY)) {
        werase(win);
        box(win, 0, 0);
        mvprintw(0, 0, "Score: %i", score);
        refresh();
        movement(&snakeX, &snakeY, moveX, moveY, speed, win);
        wrefresh(win);

        if (kbhit()) {
            int action = wgetch(win);

            if (action == KEY_LEFT && moveX != 1) {
                moveX = -1;
                moveY = 0;
            } else if (action == KEY_RIGHT && moveX != -1) {
                moveX = 1;
                moveY = 0;
            } else if (action == KEY_UP && moveY != 1) {
                moveX = 0;
                moveY = -1;
            } else if (action == KEY_DOWN && moveY != -1) {
                moveX = 0;
                moveY = 1;
            }
        }
    }

    mvprintw(12, 30, "game over!");
    getch();
    endwin();
}