//
// Created by velocitatem on 10/6/24.
//
#include <stdlib.h>
#include "kernels.h"


double ** get_kernel(enum Kernels kernel_type, int &kernel_size) {
    double ** kernel;
    switch (kernel_type) {
        case SHARPEN:
            kernel_size = 3;
            kernel = (double **)malloc(kernel_size * sizeof(double *));
            for (int i = 0; i < kernel_size; i++) {
                kernel[i] = (double *)malloc(kernel_size * sizeof(double));
            }

            kernel[0][0] = 0; kernel[0][1] = -1; kernel[0][2] = 0;
            kernel[1][0] = -1; kernel[1][1] = 4; kernel[1][2] = -1;
            kernel[2][0] = 0; kernel[2][1] = -1; kernel[2][2] = 0;
            break;
        case BLUR: // a simple uniform blur kernel
            //kernel_size = 20;
            kernel = (double **)malloc(kernel_size * sizeof(double *));
            for (int i = 0; i < kernel_size; i++) {
                kernel[i] = (double *)malloc(kernel_size * sizeof(double));
                for (int j = 0; j < kernel_size; j++) {
                    kernel[i][j] = 1.0 / (kernel_size * kernel_size);
                }
            }
            break;

        case EDGE: // an edge detection kernel

        //kernel_size = 10;
        kernel = (double **)malloc(kernel_size * sizeof(double *));
        for (int i = 0; i < kernel_size; i++) {
            kernel[i] = (double *)malloc(kernel_size * sizeof(double));
        }
        // fill borders with -1
        for (int i = 0; i < kernel_size; i++) {
            for (int j = 0; j < kernel_size; j++) {
                if (i == 0 or j == 0 or i == kernel_size-1 or j == kernel_size - 1) kernel[i][j] = -1; // borders -1 for edges
                else if (i == kernel_size/2 and j == kernel_size/2) kernel[i][j] = (kernel_size * 4) - 4; // center to compensate and make it sum 0
                else kernel[i][j] = 0; // ignore
            }
        }

        break;

        default:
            kernel_size = 0;
            break;

    }
    return kernel;
};
