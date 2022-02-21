#include <complex.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fractal.h"
#include "fractal_task.h"
#include "gfx.h"
#include "utils.h"

//=========================== Static/local function prototypes. ============================== //
static unsigned int fractal_iterate(const fractal *fr, long double complex *z, long double complex *c);
static inline unsigned int get_rgb(const fractal *fr, int n);

//=========================== Static global variables. ============================== //
static pthread_mutex_t draw_mutex = PTHREAD_MUTEX_INITIALIZER;

//=========================== Function declarations. ============================== //
/**
 * Dynamically allocates a fractal object with a specified width and height. The minimum
 * and maximum pixel bounds of the complex domain are also specified. If you're unsure
 * of what to use for these, just use -2, -2, 2, 2.
 * 
 * @param size_t width of image in pixels.
 * @param size_t height of image in pixels.
 * @param size_t maximum number of iterations to use in escape algorithm.
 * @param size_t number of threads to use. This value should evenly divide the width.
 * @param long double complex to use for c in z^2 + c.
 * @param long double minimum x value to use in the complex domain e.g., [-2, 2].
 * @param long double minimum y value to use in the complex domain e.g., [-2, 2].
 * @param long double maximum x value to use in the complex domain e.g., [-2, 2].
 * @param long double maximum y value to use in the complex domain e.g., [-2, 2].
 * 
 * @return new fractal.
 */
fractal *fractal_init(size_t width, size_t height, size_t max_iterations, size_t num_threads,
                      long double complex c, long double min_complex_x, long double min_complex_y,
                      long double max_complex_x, long double max_complex_y) {
    fractal *fr;
    // First, check to make sure the number of threads is correct.
    if (width % num_threads != 0) {
        fprintf(stderr, "The fractal width must be divisible by the number of threads.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate the fractal pointer.
    if ((fr = malloc(sizeof(fractal))) == NULL) {
        fprintf(stderr, "Could not allocate memory for fractal typedef.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate the one-dimensional pixel array. It should be an image--what are the dimensions?
    if ((fr->pixels = malloc(sizeof(unsigned int) * width * height)) == NULL) {
        fprintf(stderr, "Could not allocate memory for pixel data in fractal.\n");
        exit(EXIT_FAILURE);
    }

    // Set each field in the fractal struct ptr.
    fr->c = c;
    fr->max_iterations = max_iterations;
    fr->num_threads = num_threads;
    fr->width = width;
    fr->height = height;
    fr->min_complex_x = min_complex_x;
    fr->max_complex_x = max_complex_x;
    fr->min_complex_y = min_complex_y;
    fr->max_complex_y = max_complex_y;

    return fr;
}

/**
 * Generates the fractal using multiple threads. Each thread receives
 * a segment of the fractal to render, and this is the callback function
 * pointer for that fractal_task. 
 *
 * @param void* args - fractal_task * struct.
 * 
 * @return void. 
 */
void *fractal_generate_multithreaded(void *args) {
    fractal_task *task = (fractal_task *) args;
    fractal *fr = task->fractal;
    
    for (int i = task->start_pixel; i < task->end_pixel; i++) {
        int x = GET_X(i, fr->width);
        int y = GET_Y(i, fr->height);

        // Convert the coordinates to the bounds of our fractal.
        long double px = normalize(x, 0, fr->width, fr->min_complex_x, fr->max_complex_x);
        long double py = normalize(y, 0, fr->height, fr->min_complex_y, fr->max_complex_y);

        // Use the z = (z_{n-1})^2 + c equation. z is our normalized px and py as a complex number. If we have a predefined c use that.
        long double complex z = CMPLXL(px, py);
        long double complex c = (fr->c != 0 ? (fr->c) : CMPLXL(px, py));

        // Run the escape algorithm.
        int iterations = fractal_iterate(fr, &z, &c);

        // Get the color based on the iterations, then draw it.
        fr->pixels[i] = (iterations < fr->max_iterations ? get_rgb(fr, iterations) : 0);

        // Only one thread can draw to the screen at a time.
        pthread_mutex_lock(&draw_mutex);
        gfx_color(GET_RED(fr->pixels[i]), GET_GREEN(fr->pixels[i]), GET_BLUE(fr->pixels[i]));
        gfx_point(x, y);
        pthread_mutex_unlock(&draw_mutex);
    }

    free(task);
    return NULL;
}

/**
 * Destroys a fractal and the associated memory.
 * 
 * @param fractal * pointer to fractal struct to free.
 * 
 * @return void.
 */
void fractal_destroy(fractal *fr) {
    free(fr->pixels);
    free(fr);
}

/**
 * Runs the escape algorithm for a single pixel in the fractal. The pseudocode is 
 * 
 * WHILE iterations < max_iterations && z.real <= 2.0 DO
 *  z = z^2 + c
 *  iterations += 1
 * RETURN iterations
 * 
 * @param const fractal * pointer to fractal for max_iterations. 
 * @param long double complex * pointer to complex value z.
 * @param long double complex * pointer to complex value c.
 * 
 * @return number of iterations used. If the point is in the fractal, this will be max_iterations.
 */
static unsigned int fractal_iterate(const fractal *fr, long double complex *zp, long double complex *cp) {
    // Normalize the pixel coordinates.
    size_t iterations = 0;
    long double complex z = *zp;
    long double complex c = *cp;

    while (iterations < fr->max_iterations && creall(cabsl(z)) < 2.0) {
        // Uses the z^2 + c equation.
        z = z * z + c;
        iterations++;
    }

    return iterations;
}

/**
 * Uses a modified form of the Bernstein polynomials to generate smooth coloring
 * for a given number of iterations. 
 * 
 * Code from https://solarianprogrammer.com/2013/02/28/mandelbrot-set-cpp-11/. 
 *
 * @param const fractal * pointer to fractal for max_iterations.
 * @param int number of iterations used for a pixel.
 * 
 * @return unsigned int 32-bit color in ARGB.
 */
static inline unsigned int get_rgb(const fractal *fr, int n) {
    int maxi = fr->max_iterations;

    double t = (double)n / (double)maxi;

    unsigned int r = (int)(9 * (1 - t) * t * t * t * 0xff);
    unsigned int g = (int)(15 * (1 - t) * (1 - t) * t * t * 0xff);
    unsigned int b = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 0xff);

    return PACK_COLOR24(r, g, b);
}
