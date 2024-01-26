#include "bmp.h"
#include "arguments.h"
#include "sepia.h"
#include "comp_time.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void clearing_memory(struct image *img_c, struct image *img_asm, struct arguments* args){
    destroy_image(img_c);
    destroy_image(img_asm);
    destroy_arguments(args);
}

int main( int argc, const char** argv ) {
    struct arguments* args = convert_arguments(argc, argv);

    if (args == 0){
        fprintf(stderr, "Couldn't allocate memory for converted arguments");
        exit(EXIT_FAILURE);
    }
    if (args->status != STATUS_OK){
        print_error(args->status);
        destroy_arguments(args);
        exit(EXIT_FAILURE);
    }

    struct image img_c;

    enum read_status rs = from_bmp(args->source_file, &img_c);

    if (rs != READ_OK){
        print_read_error(rs);
        clearing_memory(&img_c, 0, args);
        exit(EXIT_FAILURE);
    }

    struct image img_asm = create_image(img_c.width, img_c.height);
    memcpy(img_asm.data, img_c.data, img_c.width * img_c.height * sizeof(struct pixel));

    comp_time(&img_c, &img_asm);

    enum write_status ws_c = to_bmp(args->transf_file_c, &img_c);
    enum write_status ws_asm = to_bmp(args->transf_file_asm, &img_asm);

    if (ws_c != WRITE_OK || ws_asm != WRITE_OK){
        if (ws_c != WRITE_OK)
            print_write_error(ws_c);
        if (ws_asm != WRITE_OK)
            print_write_error(ws_asm);
        clearing_memory(&img_c, &img_asm, args);
        exit(EXIT_FAILURE);
    }

    clearing_memory(&img_c, &img_asm, args);

    return 0;
}
