//
// Created by stass on 29.12.2023.
//
#include <stdio.h>
#include "image.h"
#define MAX_VALUE 255
#define MIN(a,b) (((a)<(b))?(a):(b))

static void sepia_one_pixel(struct pixel* pixel){
    struct pixel old = *pixel;
    struct pixel new_pixel;
    new_pixel.r = MIN((old.r * 0.393) + (old.g * 0.769) + (old.b * 0.189), MAX_VALUE);
    new_pixel.g = MIN((old.r * 0.349) + (old.g * 0.686) + (old.b * 0.168), MAX_VALUE);
    new_pixel.b = MIN((old.r * 0.272) + (old.g *0.534)	+ (old.b * 0.131), MAX_VALUE);
    *pixel = new_pixel;
}

void sepia(struct image *img) {
    for (size_t i = 0; i < img->height * img->width; ++i){
        sepia_one_pixel(&img->data[i]);
    }
}
