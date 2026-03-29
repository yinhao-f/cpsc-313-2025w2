#include "utils.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lodepng/lodepng.h"

void test_base_convolve(const uint8_t input[LAYERS][HEIGHT][WIDTH],
                        const int8_t filter[3][3],
                        int32_t output[LAYERS][HEIGHT][WIDTH]) {
    // clear output
    for (unsigned x = 0; x < WIDTH; x++) {
        for (unsigned y = 0; y < HEIGHT; y++) {
            for (unsigned c = 0; c < LAYERS; c++) {
                output[c][y][x] = 0;
            }
        }
    }

    // top left pixel
    for (unsigned x = 0; x < WIDTH; x++) {
        for (unsigned y = 0; y < HEIGHT; y++) {
            for (unsigned c = 0; c < LAYERS; c++) {
                if (x != 0 && y != 0)
                    output[c][y][x] += ((int32_t)input[c][y - 1][x - 1]) * filter[0][0];
            }
        }
    }

    // top pixel
    for (unsigned x = 0; x < WIDTH; x++) {
        for (unsigned y = 0; y < HEIGHT; y++) {
            for (unsigned c = 0; c < LAYERS; c++) {
                if (y != 0)
                    output[c][y][x] += ((int32_t)input[c][y - 1][x]) * filter[0][1];
            }
        }
    }

    // top right pixel
    for (unsigned x = 0; x < WIDTH; x++) {
        for (unsigned y = 0; y < HEIGHT; y++) {
            for (unsigned c = 0; c < LAYERS; c++) {
                if (x != WIDTH - 1 && y != 0)
                    output[c][y][x] += ((int32_t)input[c][y - 1][x + 1]) * filter[0][2];
            }
        }
    }

    // left pixel
    for (unsigned x = 0; x < WIDTH; x++) {
        for (unsigned y = 0; y < HEIGHT; y++) {
            for (unsigned c = 0; c < LAYERS; c++) {
                if (x != 0)
                    output[c][y][x] += ((int32_t)input[c][y][x - 1]) * filter[1][0];
            }
        }
    }

    // center pixel
    for (unsigned x = 0; x < WIDTH; x++) {
        for (unsigned y = 0; y < HEIGHT; y++) {
            for (unsigned c = 0; c < LAYERS; c++) {
                output[c][y][x] += ((int32_t)input[c][y][x]) * filter[1][1];
            }
        }
    }

    // right pixel
    for (unsigned x = 0; x < WIDTH; x++) {
        for (unsigned y = 0; y < HEIGHT; y++) {
            for (unsigned c = 0; c < LAYERS; c++) {
                if (x != WIDTH - 1)
                    output[c][y][x] += ((int32_t)input[c][y][x + 1]) * filter[1][2];
            }
        }
    }

    // bottom left pixel
    for (unsigned x = 0; x < WIDTH; x++) {
        for (unsigned y = 0; y < HEIGHT; y++) {
            for (unsigned c = 0; c < LAYERS; c++) {
                if (x != 0 && y != HEIGHT - 1)
                    output[c][y][x] += ((int32_t)input[c][y + 1][x - 1]) * filter[2][0];
            }
        }
    }

    // bottom pixel
    for (unsigned x = 0; x < WIDTH; x++) {
        for (unsigned y = 0; y < HEIGHT; y++) {
            for (unsigned c = 0; c < LAYERS; c++) {
                if (y != HEIGHT - 1)
                    output[c][y][x] += ((int32_t)input[c][y + 1][x]) * filter[2][1];
            }
        }
    }

    // bottom right pixel
    for (unsigned x = 0; x < WIDTH; x++) {
        for (unsigned y = 0; y < HEIGHT; y++) {
            for (unsigned c = 0; c < LAYERS; c++) {
                if (x != WIDTH - 1 && y != HEIGHT - 1)
                    output[c][y][x] += ((int32_t)input[c][y + 1][x + 1]) * filter[2][2];
            }
        }
    }
}

typedef struct timespec timespec_t;
static timespec_t timeminus(timespec_t a, timespec_t b) {
    timespec_t res;
    res.tv_sec = a.tv_sec - b.tv_sec;
    if (a.tv_nsec > b.tv_nsec) {
        res.tv_nsec = a.tv_nsec - b.tv_nsec;
    } else {
        res.tv_nsec = 1000000000 + a.tv_nsec - b.tv_nsec;
        res.tv_sec--;
    }
    return res;
}

uint64_t test_invoke_convolve(const char *func_name,
                              convolve_func_t convolve_func,
                              const uint8_t input[LAYERS][HEIGHT][WIDTH],
                              const int8_t filter[3][3],
                              int32_t output[LAYERS][HEIGHT][WIDTH],
                              int32_t expected_output[LAYERS][HEIGHT][WIDTH],
                              int *is_correct) {
    // test config
    const int n_trials = 10;
    uint64_t best_time = UINT64_MAX;
    uint64_t times[n_trials];
    size_t output_size = sizeof(uint32_t) * LAYERS * WIDTH * HEIGHT;

    timespec_t start, end;

    // run base_convolve 10 times
    for (int i = 0; i < n_trials; i++) {
        // reset output
        memset(output, 0, output_size);

        // start the timer and run code
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        convolve_func(input, filter, output);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
        timespec_t diff_ts = timeminus(end, start);
        uint64_t time = diff_ts.tv_nsec / 1000 + 1000000 * diff_ts.tv_sec;

        // record time
        times[i] = time;
        if (time < best_time) {
            best_time = time;
        }

        // check correctness
        int error = memcmp(output, expected_output, output_size);

        // print results
        if (error) {
            printf("Output of implementation %s run %d is incorrect!\n",
                   func_name, i);
            *is_correct = 0;
            return best_time;
        }
        printf("Implementation %s run %d done.\n", func_name, i);
    }

    // print time
    printf("The best time for implementation %s took: %" PRIu64 " us\n",
           func_name, best_time);
    for (int i = 0; i < n_trials; i++) {
        printf("Implementation %s run[%d] = %" PRIu64 "\n", func_name, i,
               times[i]);
    }

    *is_correct = 1;
    return best_time;
}

unsigned int test_read_png(const char *filename,
                           uint8_t image[LAYERS][HEIGHT][WIDTH]) {
    uint8_t *original_image = NULL;
    unsigned w = 0;
    unsigned h = 0;

    // read from disk
    unsigned error = lodepng_decode24_file(&original_image, &w, &h, filename);
    if (error) {
        fprintf(stderr, "error %u (%s): %s\n", error, filename,
                lodepng_error_text(error));
        exit(1);
    }

    if (w != WIDTH || h != HEIGHT) {
        fprintf(stderr, "Image must be %d by %d\n", HEIGHT, WIDTH);
        exit(1);
    }

    // swap the axes from (y, x, c) to (c, y, x)
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            for (int c = 0; c < 3; c++) {
                image[c][j][i] = original_image[j * w * 3 + i * 3 + c];
            }
        }
    }
    free(original_image);
    return 0;
}

unsigned int test_write_png(const char *filename,
                            const uint8_t image[LAYERS][HEIGHT][WIDTH]) {
    // swap axes from (c, y, x) back to the original (y, x, c)
    int l = LAYERS;
    int h = HEIGHT;
    int w = WIDTH;
    uint8_t *original_image = malloc(sizeof(uint8_t) * l * w * h);

    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            for (int c = 0; c < 3; c++) {
                original_image[j * w * 3 + i * 3 + c] = image[c][j][i];
            }
        }
    }

    // write the png to disk
    unsigned error =
        lodepng_encode24_file(filename, original_image, WIDTH, HEIGHT);
    if (error) {
        fprintf(stderr, "error %u (%s): %s\n", error, filename,
                lodepng_error_text(error));
        exit(1);
    }

    // cleanup resources
    free(original_image);
    return 0;
}
