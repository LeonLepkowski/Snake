#include <ncurses.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>

#define HIGHT 25
#define WIDTH 80

int movement(int* snake_body, int moveX, int moveY, int speed, int lenght, WINDOW* win)
{
    int oldX, oldY;
    int curX = *snake_body;
    int curY = *(snake_body + 1);
    int newX = curX + moveX;
    int newY = curY + moveY;

    *(snake_body) = newX;
    *(snake_body + 1) = newY;
    mvwprintw(win, newY, newX, "0");

    for (int i = 2; i < lenght * 2; i += 2) {
        oldX = curX;
        oldY = curY;
        curX = *(snake_body + i);
        curY = *(snake_body + i + 1);
        newX = oldX;
        newY = oldY;
        *(snake_body + i) = newX;
        *(snake_body + i + 1) = newY;

        mvwprintw(win, newY, newX, "0");
    }

    if (moveY != 0) {
        usleep(500000 / speed);
    }
    usleep(1000000 / speed);
}

void apple_gen(int* appleX, int* appleY, int* apple)
{
    int w = WIDTH - 1;
    int h = HIGHT - 2;
    if (*apple) {
        srand(time(0));
        *appleX = (rand() % w) + 1;
        *appleY = (rand() % h) + 2;
        *apple = 0;
    }
    mvprintw(*appleY, *appleX, "@");
}
void eating_apple(int* snake_body, int appleX, int appleY, int* apple, int* length)
{
    int x = *snake_body;
    int y = *(snake_body + 1);
    if (x == appleX-1 && y == appleY-1) {
        *apple = 1;
        *length += 1;
        int snakeLen = *length;
        int lastX = *(snake_body + snakeLen * 2 - 2);
        int lastY = *(snake_body + snakeLen * 2 - 2 + 1);
        *(snake_body + snakeLen * 2) = lastX;
        *(snake_body + snakeLen * 2 + 1) = lastY;
    }
    mvprintw(0, 0, "Snake length: %d", *length);
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

int game_border(int* snake_body)
{
    int x = *snake_body;
    int y = *(snake_body + 1);

    if (x < 1 || x > WIDTH - 2) {
        return 1;
    } else if (y < 1 || y > HIGHT - 2) {
        return 1;
    }
    return 0;
}

int main()
{
    int snake_body[100][2];
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 2; j++) {
            snake_body[i][j] = 0;
        }
    }
    snake_body[0][0] = 3;
    snake_body[0][1] = 3;

    int action;
    int moveX = 1;
    int moveY = 0;
    int speed = 10;
    int appleX = 0;
    int appleY = 0;
    int apple = 1;
    int length = 5;

    initscr();
    curs_set(false);
    noecho();

    WINDOW* win = newwin(HIGHT, WIDTH, 1, 1);
    keypad(win, TRUE);
    box(win, 0, 0);
    wrefresh(win);

    while (!game_border(&snake_body[0][0])) {
        werase(win);
        box(win, 0, 0);
        refresh();
        apple_gen(&appleX, &appleY, &apple);

        movement(&snake_body[0][0], moveX, moveY, speed, length, win);
        eating_apple(&snake_body[0][0], appleX, appleY, &apple, &length);

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

    mvprintw(12, 35, "GAME OVER!");
    getch();
    endwin();
}