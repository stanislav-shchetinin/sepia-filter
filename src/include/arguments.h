#include <ctype.h>
#include <malloc.h>
#include <stdint.h>

enum status_code {
    STATUS_OK = 0,
    STATUS_ERROR_WRONG_ARGS_COUNT,
    STATUS_ERROR_OPEN_SOURCE_FILE,
    STATUS_ERROR_OPEN_TRANSF_FILE,
};

struct arguments {
    FILE* source_file;
    FILE* transf_file_c;
    FILE* transf_file_asm;
    enum status_code status;
}; 

struct arguments* convert_arguments(int argc, const char** argv);
void destroy_arguments(struct arguments* args);
void print_error(enum status_code status);
