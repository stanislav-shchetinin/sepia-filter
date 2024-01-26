//
// Created by stass on 31.12.2023.
//

#include <stdio.h>
#include "comp_time.h"
#include "sepia.h"
#include "sepia_asm.h"

#define N 100

double calc_time(void (*filter)(struct image *img), struct image *img) {
    clock_t begin = clock();
    for (int32_t i = 0; i < N; ++i){
        sepia(img);
    }
    clock_t end = clock();
    return (double)(end - begin) / CLOCKS_PER_SEC;
}

void comp_time(struct image *img_c, struct image *img_asm) {

    double time_c = calc_time(sepia, img_c);
    double time_asm = calc_time(sepia_asm, img_asm);

    printf("Sepia filter on C: %f seconds\n", time_c);
    printf("Sepia filter on ASM: %f seconds\n", time_asm);

}