#ifndef FRACTAL_TASK_H
#define FRACTAL_TASK_H

#include <pthread.h>

#include "fractal.h"

//=========================== Typedef/struct declarations. ============================== //
typedef struct fractal_task {
    unsigned int start_pixel;
    unsigned int end_pixel;
    fractal *fractal;
} fractal_task;

void fractal_task_create(pthread_t *thread, fractal *fr, const unsigned int start_pixel, const unsigned int end_pixel);

#endif // FRACTAL_TASK_H