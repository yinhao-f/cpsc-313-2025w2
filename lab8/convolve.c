#include <stdint.h>

#include "utils.h"

void convolve(const uint8_t input[LAYERS][HEIGHT][WIDTH],
              const int8_t filter[3][3],
              int32_t output[LAYERS][HEIGHT][WIDTH]) {
    // TO BE IMPROVED BY STUDENTS

    int8_t topLeftFilter = filter[0][0];
    int8_t topMidFilter = filter[0][1];
    int8_t topRightFilter = filter[0][2];
    int8_t midLeftFilter = filter[1][0];
    int8_t centerFilter = filter[1][1];
    int8_t midRightFilter = filter[1][2];
    int8_t botLeftFilter = filter[2][0];
    int8_t botMidFilter = filter[2][1];
    int8_t botRightFilter = filter[2][2];

    // clear output
    for (unsigned c = 0; c < LAYERS; c++) {
        for (unsigned y = 0; y < HEIGHT; y++) {
            for (unsigned x = 0; x < WIDTH; x++) {
                output[c][y][x] = 0;
            }
        }
    }

    // for pixels that don't have to check bounds
    for (unsigned c = 0; c < LAYERS; c++) {
        for (unsigned y = 1; y < HEIGHT - 1; y++) {
            for (unsigned x = 1; x < WIDTH - 1; x++) {
                output[c][y][x] += ((int32_t)input[c][y - 1][x - 1]) * topLeftFilter;
                output[c][y][x] += ((int32_t)input[c][y - 1][x]) * topMidFilter;
                output[c][y][x] += ((int32_t)input[c][y - 1][x + 1]) * topRightFilter;
                output[c][y][x] += ((int32_t)input[c][y][x - 1]) * midLeftFilter;
                output[c][y][x] += ((int32_t)input[c][y][x]) * centerFilter;
                output[c][y][x] += ((int32_t)input[c][y][x + 1]) * midRightFilter;
                output[c][y][x] += ((int32_t)input[c][y + 1][x - 1]) * botLeftFilter;
                output[c][y][x] += ((int32_t)input[c][y + 1][x]) * botMidFilter;
                output[c][y][x] += ((int32_t)input[c][y + 1][x + 1]) * botRightFilter;
            }
        }
    }

    // top row
    for (unsigned c = 0; c < LAYERS; c++) {
        unsigned y = 0;
        for (unsigned x = 1; x < WIDTH - 1; x++) {
            output[c][y][x] += ((int32_t)input[c][y][x - 1]) * midLeftFilter;
            output[c][y][x] += ((int32_t)input[c][y][x]) * centerFilter;
            output[c][y][x] += ((int32_t)input[c][y][x + 1]) * midRightFilter;
            output[c][y][x] += ((int32_t)input[c][y + 1][x - 1]) * botLeftFilter;
            output[c][y][x] += ((int32_t)input[c][y + 1][x]) * botMidFilter;
            output[c][y][x] += ((int32_t)input[c][y + 1][x + 1]) * botRightFilter;
        }
    }

    // bottom row
    for (unsigned c = 0; c < LAYERS; c++) {
        unsigned y = HEIGHT - 1;
        for (unsigned x = 1; x < WIDTH - 1; x++) {
            output[c][y][x] += ((int32_t)input[c][y - 1][x - 1]) * topLeftFilter;
            output[c][y][x] += ((int32_t)input[c][y - 1][x]) * topMidFilter;
            output[c][y][x] += ((int32_t)input[c][y - 1][x + 1]) * topRightFilter;
            output[c][y][x] += ((int32_t)input[c][y][x - 1]) * midLeftFilter;
            output[c][y][x] += ((int32_t)input[c][y][x]) * centerFilter;
            output[c][y][x] += ((int32_t)input[c][y][x + 1]) * midRightFilter;
        }
    }

    // left side
    for (unsigned c = 0; c < LAYERS; c++) {
        unsigned x = 0;
        for (unsigned y = 1; y < HEIGHT - 1; y++) {
            output[c][y][x] += ((int32_t)input[c][y - 1][x]) * topMidFilter;
            output[c][y][x] += ((int32_t)input[c][y - 1][x + 1]) * topRightFilter;
            output[c][y][x] += ((int32_t)input[c][y][x]) * centerFilter;
            output[c][y][x] += ((int32_t)input[c][y][x + 1]) * midRightFilter;
            output[c][y][x] += ((int32_t)input[c][y + 1][x]) * botMidFilter;
            output[c][y][x] += ((int32_t)input[c][y + 1][x + 1]) * botRightFilter;
        }
    }
            

    // right side
    for (unsigned c = 0; c < LAYERS; c++) {
        unsigned x = WIDTH - 1;
        for (unsigned y = 1; y < HEIGHT - 1; y++) {
            output[c][y][x] += ((int32_t)input[c][y - 1][x - 1]) * topLeftFilter;
            output[c][y][x] += ((int32_t)input[c][y - 1][x]) * topMidFilter;
            output[c][y][x] += ((int32_t)input[c][y][x - 1]) * midLeftFilter;
            output[c][y][x] += ((int32_t)input[c][y][x]) * centerFilter;
            output[c][y][x] += ((int32_t)input[c][y + 1][x - 1]) * botLeftFilter;
            output[c][y][x] += ((int32_t)input[c][y + 1][x]) * botMidFilter;
        }
    }


    // four corners
    for (unsigned c = 0; c < LAYERS; c++) {
        // top left corner
        output[c][0][0] += ((int32_t)input[c][0][0]) * centerFilter;
        output[c][0][0] += ((int32_t)input[c][0][1]) * midRightFilter;
        output[c][0][0] += ((int32_t)input[c][1][0]) * botMidFilter;
        output[c][0][0] += ((int32_t)input[c][1][1]) * botRightFilter;

        // top right corner
        output[c][0][WIDTH - 1] += ((int32_t)input[c][0][WIDTH - 2]) * midLeftFilter;
        output[c][0][WIDTH - 1] += ((int32_t)input[c][0][WIDTH - 1]) * centerFilter;
        output[c][0][WIDTH - 1] += ((int32_t)input[c][1][WIDTH - 2]) * botLeftFilter;
        output[c][0][WIDTH - 1] += ((int32_t)input[c][1][WIDTH - 1]) * botMidFilter;

        // bottom left corner
        output[c][HEIGHT - 1][0] += ((int32_t)input[c][HEIGHT - 2][0]) * topMidFilter;
        output[c][HEIGHT - 1][0] += ((int32_t)input[c][HEIGHT - 2][1]) * topRightFilter;
        output[c][HEIGHT - 1][0] += ((int32_t)input[c][HEIGHT - 1][0]) * centerFilter;
        output[c][HEIGHT - 1][0] += ((int32_t)input[c][HEIGHT - 1][1]) * midRightFilter;

        // bottom right corner
        output[c][HEIGHT - 1][WIDTH - 1] += ((int32_t)input[c][HEIGHT - 2][WIDTH - 2]) * topLeftFilter;
        output[c][HEIGHT - 1][WIDTH - 1] += ((int32_t)input[c][HEIGHT - 2][WIDTH - 1]) * topMidFilter;
        output[c][HEIGHT - 1][WIDTH - 1] += ((int32_t)input[c][HEIGHT - 1][WIDTH - 2]) * midLeftFilter;
        output[c][HEIGHT - 1][WIDTH - 1] += ((int32_t)input[c][HEIGHT - 1][WIDTH - 1]) * centerFilter;

    }
}
