#ifndef CPSC313_PNG_H
#define CPSC313_PNG_H

#include <limits.h>
#include <stdint.h>

#define LAYERS 3
#define WIDTH 1920
#define HEIGHT 1440

typedef void (*convolve_func_t)(const uint8_t input[LAYERS][HEIGHT][WIDTH],
                                const int8_t filter[3][3],
                                int32_t output[LAYERS][HEIGHT][WIDTH]);

void convolve(const uint8_t input[LAYERS][HEIGHT][WIDTH],
              const int8_t filter[3][3],
              int32_t output[LAYERS][HEIGHT][WIDTH]);

void test_base_convolve(const uint8_t input[LAYERS][HEIGHT][WIDTH],
                        const int8_t filter[3][3],
                        int32_t output[LAYERS][HEIGHT][WIDTH]);

uint64_t test_invoke_convolve(const char *func_name,
                              convolve_func_t convolve_func,
                              const uint8_t input[LAYERS][HEIGHT][WIDTH],
                              const int8_t filter[3][3],
                              int32_t output[LAYERS][HEIGHT][WIDTH],
                              int32_t expected_output[LAYERS][HEIGHT][WIDTH],
                              int *is_correct);

unsigned int test_read_png(const char *filename,
                           uint8_t image[LAYERS][HEIGHT][WIDTH]);

unsigned int test_write_png(const char *filename,
                            const uint8_t image[LAYERS][HEIGHT][WIDTH]);

#endif
