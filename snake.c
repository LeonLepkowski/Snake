#include <ncurses.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

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
    wattron(win, COLOR_PAIR(1));

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

    wattroff(win, COLOR_PAIR(1));

    if (moveY != 0) {
        usleep(500000 / speed);
    }
    usleep(1000000 / speed);
}

void apple_gen(int* appleX, int* appleY, int* apple)
{
    int w = WIDTH - 2;
    int h = HIGHT - 2;
    if (*apple) {
        srand(time(0));
        *appleX = (rand() % w) + 2;
        *appleY = (rand() % h) + 2;
        *apple = 0;
    }
    attron(COLOR_PAIR(2));
    mvprintw(*appleY, *appleX, "@");
    attroff(COLOR_PAIR(2));
}
void eating_apple(int* snake_body, int appleX, int appleY, int* apple, int* length)
{
    int x = *snake_body;
    int y = *(snake_body + 1);
    if (x == appleX - 1 && y == appleY - 1) {
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

int game_border(int* snake_body, int length)
{
    int x = *snake_body;
    int y = *(snake_body + 1);

    if (x < 1 || x > WIDTH - 2) {
        return 1;
    } else if (y < 1 || y > HIGHT - 2) {
        return 1;
    }

    for (size_t i = 2; i < length * 2 + 2; i += 2) {
        if (x == *(snake_body + i) && y == *(snake_body + i + 1)) {
            return 1;
        }
    }

    return 0;
}




void options(int* speed, int* difficulty)
{
    int yMAX, xMAX;
    getmaxyx(stdscr, yMAX, xMAX);
    WINDOW* optionwin = newwin(6, 14, yMAX / 2 - 5, xMAX / 2 - 5);
    box(optionwin, 0, 0);
    refresh();
    wrefresh(optionwin);

    keypad(optionwin, true);
    char* choices[] = { "Beginner", "Intermediate", "Expert", "Return" };
    int choice;
    int highlight = 0;
    while (1) {
        while (1) {
            box(optionwin, 0, 0);
            for (int i = 0; i < 4; i++) {
                if(*difficulty == i) {
                    attron(COLOR_PAIR(2));
                    mvprintw(yMAX/2 - 4 + i, xMAX/2 - 6, ">");
                    attroff(COLOR_PAIR(2));
                    refresh();
                }
                if (i == highlight)
                    wattron(optionwin, A_REVERSE);
                mvwprintw(optionwin, i + 1, 1, choices[i]);
                wattroff(optionwin, A_REVERSE);
            }
            choice = wgetch(optionwin);
            switch (choice) {
            case KEY_UP:
                highlight--;
                if (highlight == -1)
                    highlight = 0;
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight == 4)
                    highlight = 3;
                break;
            default:
                break;
            }
            if (choice == 10)
                break;
        }
        if (strcmp(choices[highlight], "Beginner") == 0) {
            *difficulty = 0;
            *speed = 10;
        }

        if (strcmp(choices[highlight], "Intermediate") == 0) {
            *difficulty = 1;
            *speed = 20;
        }

        if (strcmp(choices[highlight], "Expert") == 0) {
            *difficulty = 2;
            *speed = 30;
        }
        if (strcmp(choices[highlight], "Return") == 0) {
            clear();
            return;
        }
        erase();
    }
}







int highscore(int length)
{
    FILE* file;
    file = fopen("highscore.txt", "r");

    char text[100];
    fscanf(file, "%s", text);
    int number = atoi(text);
    fclose(file);

    file = fopen("highscore.txt", "w");
    if (number < length) {
        fprintf(file, "%i", length);
    } else {
        fprintf(file, "%i", number);
    }
    fclose(file);

    return (length >= number) ? length : number;
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
    int speed = 20;
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
    mvprintw(0, 30, "Highscore: %i", hscore);



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

while(1) {
    while (1) {
            while (1) {
                box(menuwin, 0, 0);
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
            printw("Your choice was: %s\n", choices[highlight]);

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
                break;
            }
        }
        erase();



    

    while (!game_border(&snake_body[0][0], length)) {
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

    if (length >= hscore) {
        mvprintw(13, 35, "New highscore: %i", length);
    } else {
        mvprintw(13, 35, "Your score: %i", length);
    }
    highscore(length);
    int u = ' ';
    while (u != '\n') {
        u = getch();
    }
    endwin();
}
}
