#include "../include/image.h"
#include <stdlib.h>

struct image create_image(uint64_t width, uint64_t height){
    struct pixel* data = malloc(sizeof(struct pixel) * width * height);
    if (!data){
        return (struct image) {0};
    }
    struct image img;
    img.width = width;
    img.height = height;
    img.data = data;
    return img;
}

void destroy_image(struct image* img){
    if (img){
        if (img->data){
            free(img->data);
        }
    }
}
