#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "kernels.h"
#include <time.h>
#include <opencv2/opencv.hpp>
#include <string.h>


#define WIDTH 1920
#define HEIGHT 1080

using namespace std;
using namespace cv;

void apply_convolution(double **image, double **output, double **kernel, int kernel_size) {
    int offset = kernel_size / 2;


    #pragma omp parallel for collapse(2)
    for (int y = offset; y < HEIGHT - offset; y++) {
        for (int x = offset; x < WIDTH - offset; x++) {
            // here we are at pixel x,y
            double sum = 0.0;
            // we could reduce(+:sum) this, but for the kernel size that we have it is not worth it
            // Further consideration: use the following conditional parallelization in case of having generally higher kernels
            #pragma omp simd reduction(+:sum) if(kernel_size > 10)
            // rather than a parallel optimizatin, SIMD is much more efficient
            for (int ky = 0; ky < kernel_size; ky++) {
                for (int kx = 0; kx < kernel_size; kx++) {
                    // finding the pixel values for each kernel cell
                    int ix = x + kx - offset;
                    int iy = y + ky - offset;
                    sum += image[iy][ix] * kernel[ky][kx];
                }
            }
            output[y][x] = sum;
        }
    }
}

void apply_convolution_serial(double **image, double **output, double **kernel, int kernel_size) {
    int offset = kernel_size / 2;

    for (int y = offset; y < HEIGHT - offset; y++) {
        for (int x = offset; x < WIDTH - offset; x++) {
            // here we are at pixel x,y
            double sum = 0.0;

            // Loop through the kernel
            for (int ky = 0; ky < kernel_size; ky++) {
                for (int kx = 0; kx < kernel_size; kx++) {
                    // finding the pixel values for each kernel cell
                    int ix = x + kx - offset;
                    int iy = y + ky - offset;
                    sum += image[iy][ix] * kernel[ky][kx];
                }
            }
            output[y][x] = sum;
        }
    }
}



void show_image(double **output) {
    // Convert output array to cv::Mat for displaying
    Mat output_img(HEIGHT, WIDTH, CV_64F);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            output_img.at<double>(y, x) = output[y][x] / 255.0;
        }
    }

    namedWindow("Output Image", WINDOW_AUTOSIZE);
    imshow("Output Image", output_img);
    waitKey(0);
}

int main() {
    // Dynamically allocate memory for image and output
    double **image = (double **)malloc(HEIGHT * sizeof(double *));
    double **output = (double **)malloc(HEIGHT * sizeof(double *));

    for (int i = 0; i < HEIGHT; i++) {
        image[i] = (double *)malloc(WIDTH * sizeof(double));
        output[i] = (double *)malloc(WIDTH * sizeof(double));

    }

    string names[3] = {"Sharpening", "Bluring", "Edge Detection"};
    double kernel_sizes[5] = {2, 3, 10, 20, 50};

    // load image
    string path = "/home/velocitatem/CLionProjects/ImageConvolution/test.png";


    Mat img = imread(path, IMREAD_GRAYSCALE);
    if (img.empty()) exit(2);

    // we load the image as grayscale
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            uchar pixelValue = img.at<uchar>(y, x);
            double pixel = static_cast<double>(pixelValue);
            image[y][x] = pixel;
        }
    }




    // test all kernels

    for (int K = 0; K < 3; K += 1) {
        int kernel_size;

        if (K == 0) {
            // since sharpening is fixed in size...
            double start_time = omp_get_wtime();
            double ** kernel = get_kernel(static_cast<Kernels>(K), kernel_size);
            // we get our target kernel
            apply_convolution(image, output, kernel, kernel_size);
            double end_time = omp_get_wtime();

            cout << names[K] << " @ kernel_size=" << kernel_size << " ~ Time(s):" << end_time - start_time << endl;

            continue;
        }

        for (int kernel_s = 0; kernel_s < 5; kernel_s++) {

            kernel_size = kernel_sizes[kernel_s];
            double start_time = omp_get_wtime();
            double ** kernel = get_kernel(static_cast<Kernels>(K), kernel_size);
            // we get our target kernel
            apply_convolution(image, output, kernel, kernel_size);
            double end_time = omp_get_wtime();
            cout << names[K] << " @ kernel_size=" << kernel_size << " ~ Time(s):" << end_time - start_time << endl;

            show_image(output);
            start_time = omp_get_wtime();
            kernel =  get_kernel(static_cast<Kernels>(K), kernel_size);
            apply_convolution_serial(image, output, kernel, kernel_size);
            end_time = omp_get_wtime();
            cout << "SERIAL: " << names[K] << " @ kernel_size=" << kernel_size << " ~ Time(s):" << end_time - start_time << endl;


            {
                for (int i = 0; i < kernel_size; i++) {
                    free(kernel[i]);
                }
                free(kernel);
            }
        }


        //show_image(output);


    }


    // wrapped for cleaner code
    {
    // Free allocated memory
    for (int i = 0; i < HEIGHT; i++) {
        free(image[i]);
        free(output[i]);
    }
    free(image);
    free(output);

    }

    return 0;
}
