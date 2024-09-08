#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25

void input_matrix(int arr[HEIGHT][WIDTH]);
void output_matrix(int arr[HEIGHT][WIDTH]);

int count_neighbours(int arr[HEIGHT][WIDTH], int i, int j);
int count_live_cell(int arr[HEIGHT][WIDTH]);

int speed(char select_speed, int game_speed, int *end_of_game);
void update_matrix(int arr_old[HEIGHT][WIDTH], int arr_new[HEIGHT][WIDTH]);
void copy(int arr_old[HEIGHT][WIDTH], int arr_new[HEIGHT][WIDTH]);
int check_update(int arr_old[HEIGHT][WIDTH], int arr_new[HEIGHT][WIDTH]);

int main() {
    int field_old[HEIGHT][WIDTH];

    int field_new[HEIGHT][WIDTH];

    int game_speed = 500000;
    int end_of_game = 1;

    input_matrix(field_old);
    copy(field_old, field_new);

    if (freopen("/dev/tty", "r", stdin)) initscr();

    noecho();

    nodelay(stdscr, true);

    while (end_of_game) {
        char select_speed = getch();

        game_speed = speed(select_speed, game_speed, &end_of_game);
        usleep(game_speed);
        update_matrix(field_old, field_new);

        clear();
        output_matrix(field_new);

        if (check_update(field_old, field_new) == 0 || count_live_cell(field_new) == 0) {
            end_of_game = 0;
        }

        copy(field_new, field_old);
    }

    endwin();
    return 0;
}

int speed(char select_speed, int game_speed, int *end_of_game) {
    if (select_speed == '1')
        game_speed = 100000;
    else if (select_speed == '2')
        game_speed = 50000;
    else if (select_speed == '3')
        game_speed = 20000;
    else if (select_speed == '4')
        game_speed = 7000;
    else if (select_speed == 'q')
        *end_of_game = 0;

    return game_speed;
}

int check_update(int arr_old[HEIGHT][WIDTH], int arr_new[HEIGHT][WIDTH]) {
    int updates = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (arr_old[i][j] != arr_new[i][j]) updates = 1;
        }
    }
    return updates;
}

void update_matrix(int arr_old[HEIGHT][WIDTH], int arr_new[HEIGHT][WIDTH]) {
    int count_is_live;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            count_is_live = count_neighbours(arr_old, i, j);
            if (count_is_live < 2 || count_is_live > 3) {
                arr_new[i][j] = 0;
            }

            if (count_is_live == 3 && arr_old[i][j] == 0) arr_new[i][j] = 1;

            if ((count_is_live == 2 || count_is_live == 3) && (arr_old[i][j] == 1)) arr_new[i][j] = 1;
        }
    }
}

int count_neighbours(int arr[HEIGHT][WIDTH], int x, int y) {
    int count = 0;
    int xPrev = x - 1, xNext = x + 1, yPrev = y - 1, yNext = y + 1;

    if (xPrev < 0) xPrev = (xPrev + HEIGHT) % HEIGHT;

    if (yPrev < 0) yPrev = (yPrev + WIDTH) % WIDTH;

    if (xNext > HEIGHT - 1) xNext = (xNext + HEIGHT) % HEIGHT;
    if (yNext > WIDTH - 1) yNext = (yNext + WIDTH) % WIDTH;

    count += arr[xPrev][yPrev];
    count += arr[xPrev][y];
    count += arr[xPrev][yNext];

    count += arr[x][yPrev];
    count += arr[x][yNext];

    count += arr[xNext][yPrev];
    count += arr[xNext][y];
    count += arr[xNext][yNext];

    return count;
}

void copy(int arr_old[HEIGHT][WIDTH], int arr_new[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            arr_new[i][j] = arr_old[i][j];
        }
    }
}

void output_matrix(int arr[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (arr[i][j] == 0)
                printw("-");
            else if (arr[i][j] == 1)
                printw("*");
        }
        printw("\n");
    }
}

void input_matrix(int arr[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            scanf("%d", &arr[i][j]);
        }
    }
}

int count_live_cell(int arr[HEIGHT][WIDTH]) {
    int count = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (arr[i][j] == 1) count++;
        }
    }
    return count;
}
