#include "arguments.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MODE_READ "rb"
#define MODE_WRITE "wb"
#define CNT_ARGS 4

static const char *const STATUSES_INFO[] = {
        [STATUS_OK] = "",
        [STATUS_ERROR_WRONG_ARGS_COUNT] = "Count arguments is not 4. Expected format: ./image-sepia <source-image> <sepia-image-c> <sepia-image-asm>\n",
        [STATUS_ERROR_OPEN_SOURCE_FILE] = "Couldn't open source (file) image for reading\n",
        [STATUS_ERROR_OPEN_TRANSF_FILE] = "Couldn't open transformer (file) image for writing or create new\n",
};

static bool check_argc(int argc, struct arguments* args){
    if (argc != CNT_ARGS){
        args->status = STATUS_ERROR_WRONG_ARGS_COUNT;
        return false;
    }
    return true;
}

static bool check_source_file(const char* name_source_file, struct arguments* args){
    FILE* file = fopen(name_source_file, MODE_READ);
    if (file){
        args->source_file = file;
        return true;
    }
    args->status = STATUS_ERROR_OPEN_SOURCE_FILE;
    return false;
}

static bool check_transf_file(const char* name_transf_file, struct arguments* args){
    FILE* file = fopen(name_transf_file, MODE_WRITE);
    if (file){
        if (!args->transf_file_c){
            args->transf_file_c = file;
            return true;
        }
        args->transf_file_asm = file;
        return true;
    }
    args->status = STATUS_ERROR_OPEN_TRANSF_FILE;
    return false;
}

struct arguments* convert_arguments(int argc, const char** argv){

    struct arguments* result = malloc(sizeof(struct arguments));

    if (!result){
        return 0;
    }

    result->status = STATUS_OK;

    if (!check_argc(argc, result)){
        return result;
    }

    bool (*checkers[CNT_ARGS - 1])(const char*, struct arguments*) = {check_source_file,
                                                                      check_transf_file,
                                                                      check_transf_file};
 
    for (int i = 0; i < CNT_ARGS - 1; ++i){
        if (!checkers[i](argv[i + 1], result)){
            return result;
        }
    }

    return result;

}

void destroy_arguments(struct arguments* args){
    if (args){
        if (args->source_file)
            fclose(args->source_file);
        if (args->transf_file_c)
            fclose(args->transf_file_c);
        if (args->transf_file_asm)
            fclose(args->transf_file_asm);
        free(args);
    }
}

void print_error(enum status_code status){
    if (status != STATUS_OK)
        fprintf(stderr, "%s", STATUSES_INFO[status]);
}
