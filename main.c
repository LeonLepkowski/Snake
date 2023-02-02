#include "snake.h"

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

    start_color();
    init_pair(1, COLOR_GREEN, A_NORMAL);
    init_pair(2, COLOR_RED, A_NORMAL);

    WINDOW* win = newwin(HIGHT, WIDTH, 1, 1);
    keypad(win, TRUE);
    box(win, 0, 0);
    wrefresh(win);

    int hscore = highscore(length);

    int yMAX, xMAX;
    getmaxyx(stdscr, yMAX, xMAX);

    WINDOW* menuwin = newwin(5, 12, yMAX / 2 - 5, xMAX / 2 - 5);
    box(menuwin, 0, 0);
    refresh();
    wrefresh(menuwin);

    keypad(menuwin, true);
    char* choices[] = { "New game", "Options", "Exit game" };
    int choice;
    int highlight = 0;
    int difficulty = 0;

    while (1) {
        while (1) {
            while (1) {
                box(menuwin, 0, 0);
                title_name(xMAX);
                for (int i = 0; i < 3; i++) {
                    erase();
                    // wrefresh(menuwin);
                    if (i == highlight)
                        wattron(menuwin, A_REVERSE);
                    mvwprintw(menuwin, i + 1, 1, choices[i]);
                    wattroff(menuwin, A_REVERSE);
                }
                choice = wgetch(menuwin);
                switch (choice) {
                case KEY_UP:
                    highlight--;
                    if (highlight == -1)
                        highlight = 0;
                    break;
                case KEY_DOWN:
                    highlight++;
                    if (highlight == 3)
                        highlight = 2;
                    break;
                default:
                    break;
                }
                if (choice == 10)
                    break;
            }
            if (strcmp(choices[highlight], "Options") == 0) {
                erase();
                options(&speed, &difficulty);
            }
            refresh();
            if (strcmp(choices[highlight], "Exit game") == 0) {
                endwin();
                return 0;
            }

            if (strcmp(choices[highlight], "New game") == 0) {
                for (int i = 0; i < 100; i++) {
                    for (int j = 0; j < 2; j++) {
                        snake_body[i][j] = 0;
                    }
                }
                snake_body[0][0] = 3;
                snake_body[0][1] = 3;

                action;
                moveX = 1;
                moveY = 0;
                appleX = 0;
                appleY = 0;
                apple = 1;
                length = 5;
                break;
            }
        }
        erase();

        while (!game_border(&snake_body[0][0], length)) {
            werase(win);
            mvprintw(0, 30, "Highscore: %i", hscore);
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

        if (length >= hscore) {
            mvprintw(13, 35, "New highscore: %i", length);
            hscore = highscore(length);
        } else {
            mvprintw(13, 35, "Your score: %i", length);
        }
        int u = ' ';
        while (u != '\n') {
            u = getch();
        }
        erase();
        clear();
        endwin();
    }
}
