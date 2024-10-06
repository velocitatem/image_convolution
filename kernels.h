//
// Created by velocitatem on 10/6/24.
//

#ifndef KERNELS_H
#define KERNELS_H
enum Kernels {
    SHARPEN = 0,
    BLUR = 1,
    EDGE = 2,

};
double ** get_kernel(enum Kernels kernel_type, int &kernel_size);
#endif //KERNELS_H
