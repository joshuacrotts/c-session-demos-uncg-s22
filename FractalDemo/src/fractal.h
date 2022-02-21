#ifndef FRACTAL_H
#define FRACTAL_H

#include <complex.h>
#include <stdio.h>
#include <stdlib.h>

//=========================== Preprocessor macros and definitions. ============================== //
#define GET_X(idx, width) ((idx) % (width))
#define GET_Y(idx, height) ((idx) / (height))
#define GET_RED(color) (((color) >> 16) & 0xff)
#define GET_GREEN(color) (((color) >> 8) & 0xff)
#define GET_BLUE(color) ((color) & 0xff)
#define PACK_COLOR24(r, g, b) (((r) << 16) | ((g) << 8) | (b))

//=========================== Typedef/struct declarations. ============================== //
typedef struct fractal {
    // Pixel data for the fractal.
    unsigned int *pixels;

    // Number of threads used to render the fractal.
    size_t num_threads;

    // Maximum number of iterations for the escape algorithm.
    size_t max_iterations;

    // Width and height in pixels.
    size_t width;
    size_t height;

    // Bounds of the scene in pixels. The default is (-2, -2), (2, 2).
    long double min_complex_x;
    long double min_complex_y;
    long double max_complex_x;
    long double max_complex_y;

    // c for the formula z^2 + c
    long double complex c;
} fractal;

fractal *fractal_init(size_t width, size_t height, size_t max_iterations, size_t num_threads, 
                      long double complex c, long double min_complex_x, long double min_complex_y,
                      long double max_complex_x, long double max_complex_y);
void *fractal_generate_multithreaded(void *args);
void fractal_destroy(fractal *fr);

#endif  // FRACTAL_H