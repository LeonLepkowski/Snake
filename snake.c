#include <ncurses.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>

#define HEIGHT 23
#define WIDTH 79

int movement(int *snakeX, int *snakeY, int moveX, int moveY, int speed, WINDOW* win){
    *snakeX = *snakeX + moveX;
    *snakeY = *snakeY + moveY;
    mvwprintw(win, *snakeY, *snakeX, "0");
    usleep(100000 * speed);
}   



int main(){
    int snake_body[100][2];
    int score = 0;
    int snakeX = 79;
    int snakeY = 0;
    int action;
    int moveX = -1;
    int moveY = 0;
    int speed = 1;


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

    WINDOW* win = newwin(HEIGHT, WIDTH, 1, 1);
    keypad(win, TRUE);
    mvprintw(0, 1, "Score: %i", score);
    box(win, 0, 0);
    wrefresh(win);

    while(1){
        werase(win);
        box(win, 0, 0);
        movement(&snakeX, &snakeY, moveX, moveY, speed, win);
        wrefresh(win);

        int action = wgetch(win);

        if (action == KEY_LEFT) {
            moveX = -1;
            moveY = 0;
        } else if (action == KEY_RIGHT) {
            moveX = 1;
            moveY = 0;
        } else if (action == KEY_UP) {
            moveX = 0;
            moveY = -1;
        } else if (action == KEY_DOWN) {
            moveX = 0;
            moveY = 1;
        }
        
    }

    getch();
    endwin();
}