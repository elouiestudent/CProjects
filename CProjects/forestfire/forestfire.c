#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int FOREST_SIZE_X = 400;
const int FOREST_SIZE_Y = 400;
const float TRIALS = 100;
char forest[FOREST_SIZE_X][FOREST_SIZE_Y];
double prob;

void count_percent() {
    double sum = 0;
    for(int i = 0; i < FOREST_SIZE_X; i++) {
        for(int j = 0; j < FOREST_SIZE_Y; j++) {
            if(forest[i][j] == 'T') {
                sum++;
            }
        }
    }
    printf("%f\n", sum/(FOREST_SIZE_X * FOREST_SIZE_Y));
}

void print_forest() {
    int lever = 0;
    if(lever) {
        for(int i = 0; i < FOREST_SIZE_X; i++) {
            for(int j = 0; j < FOREST_SIZE_Y; j++) {
                printf("%c ", forest[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void double_logger(double var) {
    int lever = 1;
    if(lever) {
        printf("%f\n", var);
    }
}

void str_logger(char* var) {
    int lever = 0;
    if(lever) {
        printf("%s\n", var);
    }
}

void make_trees() {
    for(int i = 0; i < FOREST_SIZE_X; i++) {
        for(int j = 0; j < FOREST_SIZE_Y; j++) {
            float r = (float)rand()/RAND_MAX;
            if(r < prob) {
                forest[i][j] = 'T';
            }
            else {
                forest[i][j] = ' ';
            }
        }
    }
}

void start_fire() {
    for(int i = 0; i < FOREST_SIZE_X; i++) {
        if(forest[i][0] == 'T') {
            forest[i][0] = 'F';
        }
    }
}

int spread_fire() {
    char temp[FOREST_SIZE_X][FOREST_SIZE_Y];
    memcpy(temp, forest, sizeof (char) * FOREST_SIZE_X * FOREST_SIZE_Y);
    int fire = 0;
    for(int i = 0; i < FOREST_SIZE_X; i++) {
        for(int j = 0; j < FOREST_SIZE_Y; j++) {
            if(temp[i][j] == 'F') {
                if(i + 1 < FOREST_SIZE_X) {
                    if(forest[i + 1][j] == 'T') {
                        forest[i + 1][j] = 'F';
                    }
                }
                if(i - 1 >= 0) {
                    if(forest[i - 1][j] == 'T') {
                        forest[i - 1][j] = 'F';
                    }
                }
                if(j + 1 < FOREST_SIZE_Y) {
                    if(forest[i][j + 1] == 'T') {
                        forest[i][j + 1] = 'F';
                    }
                }
                if(j - 1 >= 0) {
                    if(forest[i][j - 1] == 'T') {
                        forest[i][j - 1] = 'F';
                    }
                }
                forest[i][j] = ' ';
                fire++;
            }
        }
    }
    return fire == 0;
}

int fire_iterations() {
    int time = 0;
    make_trees();
    start_fire();
    while(1) {
        print_forest();
        if(spread_fire()) {
            return time;
        }
        time++;
    }
}

int main() {
    srand(7777777);
    FILE* fout = fopen("forestfire.csv", "w");
    for(double j = 0.01; j < 1.01; j += 0.01) {
        prob = j;
        double_logger(prob);
        float sum = 0;
        for(int i = 0; i < TRIALS; i++) {
            str_logger("NEW TRIAL");
            sum += fire_iterations();
            // printf("%d\n", fire_iterations());
        }
        float average = (sum/TRIALS)/FOREST_SIZE_X;
        double_logger(average);
        fprintf(fout, "%f,%f\n", prob, average);
    }
    fclose(fout);
    return 0;
}
