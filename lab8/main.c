#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define INPUT_IMAGE_NAME "images/roseGarden.png"
#define OUTPUT_IMAGE_NAME "output/roseGarden.png"

static int32_t output[LAYERS][HEIGHT][WIDTH];
static int32_t expected_output[LAYERS][HEIGHT][WIDTH];
static uint8_t image[LAYERS][HEIGHT][WIDTH];

int main(int argc, const char **argv) {
    // load png from disk
    test_read_png(INPUT_IMAGE_NAME, image);

    // prepare inputs

    // // blur
    // uint8_t normalize_filter = 16;
    // int8_t filter[3][3] = {
    //     {1, 2, 1},
    //     {2, 4, 2},
    //     {1, 2, 1},
    // };

    // sharpen
    uint8_t normalize_filter = 1;
    int8_t filter[3][3] = {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0},
    };

    // generate expected output to test correctness
    test_base_convolve(image, filter, expected_output);
    int is_correct;

    // time the base and optimized implementation
    uint64_t base_time =
        test_invoke_convolve("base", test_base_convolve, image, filter, output,
                             expected_output, &is_correct);

    uint64_t optimized_time =
        test_invoke_convolve("optimized", convolve, image, filter, output,
                             expected_output, &is_correct);

    // report results
    if (!is_correct) {
        fprintf(stderr, "FAILED\n");
        exit(1);
    }
    printf(
        "The optimized version takes %.2f percent of the base version "
        "(comparing best times)\n",
        (double)optimized_time / base_time * 100);

    // replace the image with the normalized result
    for (int i = 0; i < LAYERS; i++)
        for (int j = 0; j < HEIGHT; j++)
            for (int k = 0; k < WIDTH; k++) {
                int32_t result = output[i][j][k] / normalize_filter;
                // pixel values are always in range [0, 256)
                if (result < 0) {
                    image[i][j][k] = 0;
                } else if (result > UINT8_MAX) {
                    image[i][j][k] = UINT8_MAX;
                } else {
                    image[i][j][k] = result;
                }
            }

    // write png to disk
    test_write_png(OUTPUT_IMAGE_NAME, image);

    // cleanup resources
    return 0;
}
