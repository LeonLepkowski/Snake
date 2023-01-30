#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>

#define HIGHT 25
#define WIDTH 80

int movement(int* snake_body, int moveX, int moveY, int speed, int lenght, WINDOW* win);
void apple_gen(int* appleX, int* appleY, int* apple);
void eating_apple(int* snake_body, int appleX, int appleY, int* apple, int* length);
int kbhit(void);
int game_border(int* snake_body, int length);
void options(int* speed, int* difficulty);
int highscore(int length);
void title_name(int xMAX);