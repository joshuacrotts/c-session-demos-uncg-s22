#include <stdio.h>
#include <stdlib.h>

#include "fractal_task.h"
#include "fractal.h"
#include "utils.h"

/**
 * Creates a "fractal task". A fractal task is a set of pixels that a thread generates and renders.
 * 
 * @param pthread_t * pointer to thread.
 * @param fractal * pointer to fractal.
 * @param const unsigned int starting pixel index.
 * @param const unsigned int ending pixel index.
 */
void fractal_task_create(pthread_t *thread, fractal *fr, const unsigned int start_pixel, const unsigned int end_pixel) {
    fractal_task *ft = NULL;
    if ((ft = malloc(sizeof(fractal_task))) == NULL) {
        fprintf(stderr, "Could not allocate memory for fractal task.");
        exit(EXIT_FAILURE);
    }

    ft->fractal = fr;
    ft->start_pixel = start_pixel;
    ft->end_pixel = end_pixel;

    pthread_create(thread, NULL, fractal_generate_multithreaded, (void *) ft);
}
