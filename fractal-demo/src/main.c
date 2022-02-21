#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "fractal.h"
#include "fractal_task.h"
#include "gfx.h"
#include "utils.h"

struct pair {
    int x;
    int y;
};

/**
 * Create the attributes to store the previous two mouse clicks.
 */
struct zoom {
    struct pair p1;
    struct pair p2;
};

/**
 * Prints example of how to run the program.
 * 
 * @param void.
 * 
 * @return void.
 */
static void usage(void) {
    fprintf(stderr, "Usage: ./fractal width height max_iterations num_threads c1 c2 min_x min_y max_x max_y\n");
    fprintf(stderr, "Example: ./fractal 400 400 80 20 -2 -2 2 2 (note that the mandelbrot only has 9 arguments.)\n");
    fprintf(stderr, "Example: ./fractal 400 400 1000 20 .286682 .014037 .287182 .014537 (note that the mandelbrot only has 9 arguments.)\n");
    fprintf(stderr, "Example: ./fractal 400 400 80 4 -.391 -.587 -2 -2 2 2\n");
}

/**
 * Reads in a fractal from the terminal args.
 * 
 * @param int number of args.
 * @param char *argv[] arguments.
 */
static fractal *read_fractal(int argc, char *argv[]) {
    fractal *fr;
    long double c1, c2, mincx, maxcx, mincy, maxcy;
    size_t width, height, max_iterations, num_threads;
    sscanf(argv[1], "%zu", &width);
    sscanf(argv[2], "%zu", &height);
    sscanf(argv[3], "%zu", &max_iterations);
    sscanf(argv[4], "%zu", &num_threads);
    if (argc == 11) {
        sscanf(argv[5], "%Le", &c1);
        sscanf(argv[6], "%Le", &c2);
        sscanf(argv[7], "%Le", &mincx);
        sscanf(argv[8], "%Le", &mincy);
        sscanf(argv[9], "%Le", &maxcx);
        sscanf(argv[10], "%Le", &maxcy);
        long double complex fn = c1 + c2 * I;
        fr = fractal_init(width, height, max_iterations, num_threads, fn, mincx, mincy, maxcx, maxcy);
    }
    else {
        sscanf(argv[5], "%Le", &mincx);
        sscanf(argv[6], "%Le", &mincy);
        sscanf(argv[7], "%Le", &maxcx);
        sscanf(argv[8], "%Le", &maxcy);
        fr = fractal_init(width, height, max_iterations, num_threads, 0, mincx, mincy, maxcx, maxcy);
    }
    return fr;
}

/**
 * Zooms in on a region of the fractal. The parameters define the mouse 
 * which creates a rectangle (or at least it should). These are then normalized
 * to create the new bounds of the fractal.
 * 
 * @param fractal * pointer to fractal to change.
 * @param const struct zoom * pointer to zoom struct to use.
 */
static void zoom_in(fractal *fr, const struct zoom *z) {
    if (z->p2.x < z->p1.x || z->p2.y < z->p1.y) {
        fprintf(stderr, "Cannot create a rectangle with these bounds.\n");
        exit(EXIT_FAILURE);
    }

    long double new_min_complex_x = normalize(z->p1.x, 0, fr->width, fr->min_complex_x, fr->max_complex_x);
    long double new_min_complex_y = normalize(z->p1.y, 0, fr->height, fr->min_complex_y, fr->max_complex_y);
    long double new_max_complex_x = normalize(z->p2.x, 0, fr->width, fr->min_complex_x, fr->max_complex_x);
    long double new_max_complex_y = normalize(z->p2.y, 0, fr->height, fr->min_complex_y, fr->max_complex_y);

    fr->min_complex_x = new_min_complex_x;
    fr->min_complex_y = new_min_complex_y;
    fr->max_complex_x = new_max_complex_x;
    fr->max_complex_y = new_max_complex_y;
}

/**
 * Spawns a pool of threads for the fractal to use. The threads
 * should be initialized and freed in this function. The number of
 * threads should be stored in the fractal.
 * 
 * @param fractal * pointer to fractal.
 * 
 * @return void.
 */
static void fractal_thread_pool(fractal *fr) {
    struct timespec start_time;
    get_current_time(&start_time);

    // Allocate the correct number of threads.
    pthread_t *threads = NULL;
    if ((threads = malloc(sizeof(pthread_t) * fr->num_threads)) == NULL) {
        fprintf(stderr, "Could not allocate pthreads array.\n");
        exit(EXIT_FAILURE);
    }

    unsigned int delta = (fr->width * fr->height) / fr->num_threads;
    unsigned int start_pixel = 0;
    unsigned int end_pixel = delta - 1;

    // Create the threads and make sure they start and end at the appropriate locations.
    for (int i = 0; i < fr->num_threads; i++) {
        fractal_task_create(&threads[i], fr, start_pixel, end_pixel);
        start_pixel = end_pixel + 1;
        end_pixel += delta;
    }

    for (int i = 0; i < fr->num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);

    // Compute the ending time and delta time.
    struct timespec end_time;
    get_current_time(&end_time);
    printf("Render time: %.2f\n", get_time_diff(&end_time, &start_time));
}

/**
 * Problem: Write a fractal generator that draws to a graphics context (use the gfx.h library).
 * This time, use multithreading (pthreads). Note that the project doesn't even compile right now!
 * Your job is to add the correct code to make it compile.
 */
int main(int argc, char *argv[]) {
    if (argc <= 1) {
        usage();
        return 1;
    } else {
        fractal *fr = read_fractal(argc, argv);

        // Open a new window.
        gfx_open(fr->width, fr->height, "Multithreaded Fractal");

        // Draw black to screen.
        gfx_clear_color(0, 0, 0);
        gfx_clear();

        // Timespecs and zoom structs.
        struct timespec start_time;
        struct timespec end_time;
        struct zoom z = {{-1, -1}, {-1, -1}};

        get_current_time(&start_time);

        // Generate the fractal here.
        fractal_thread_pool(fr);

        // Main loop.
        while (true) {
            // If we haven't started to zoom, print out the time it took to generate and render.
            if (z.p1.x == -1 && z.p2.x == -1) {
                get_current_time(&end_time);
                printf("Time to render: %.2f seconds\n", get_time_diff(&end_time, &start_time));
            }

            // Block until we receive an event.
            int ch = gfx_wait();

            // If the user clicks the left mouse button.
            if (ch == 1) {
                // If the user hasn't clicked at all yet, grab the top-left of the rectangle.
                if (z.p1.x == -1) {
                    z.p1.x = gfx_xpos();
                    z.p1.y = gfx_ypos();
                } else if (z.p2.x == -1) {
                    z.p2.x = gfx_xpos();
                    z.p2.y = gfx_ypos();

                    zoom_in(fr, &z);
                    get_current_time(&start_time);
                    fractal_thread_pool(fr);

                    // Reset the zoom rectangle.
                    z.p1.x = z.p1.y = z.p2.x = z.p2.y = -1;
                }
            }

            if (ch == 'q') break;
        }

        fractal_destroy(fr);
        return 0;
    }
}