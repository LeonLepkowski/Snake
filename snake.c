#include <ncurses.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>

#define HEIGHT 23
#define WIDTH 79

int main(){
    int snake_body[100][2];
    int score = 0;
    for(int i = 0; i < 100; i++){
        for(int j = 0; j < 2; j++){
            snake_body[i][j] = 0;
        }
    }
    snake_body[0][0] = 3;
    snake_body[0][1] = 3;

    initscr();
    curs_set(false);
    noecho();

    WINDOW * win = newwin(HEIGHT, WIDTH, 1, 1);
    mvprintw(0, 1, "Score: %i", score);
    refresh();
    box(win, 0, 0);
    wrefresh(win);
    getch();
    endwin();
}