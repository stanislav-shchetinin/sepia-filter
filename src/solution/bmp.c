#include "../include/bmp.h"
#include "../include/image.h"

#define SIGNATURE 19778
#define BITS_COUNT 24
#define padding(x) ((4 - ((x) * 3) % 4) % 4)
#define RESERVED 0
#define SIZE 40
#define PLANES 1
#define COMPRESSION 0

static const char *const READ_STATUSES_INFO[] = {
        [READ_OK] = "",
        [READ_INVALID_SIGNATURE] = "SOURCE FILE: Signature of header is invalid\n",
        [READ_INVALID_BITS] = "SOURCE FILE: Incorrect pixel bitness\n",
        [READ_INVALID_HEADER] = "SOURCE FILE: Incorrect header\n",
        [READ_INVALID_DATA] = "SOURCE FILE: Incorrect pixels\n"
};

static const char *const WRITE_STATUSES_INFO[] = {
        [WRITE_OK] = "",
        [WRITE_ERROR] = "Error when writing pixels to a file\n",
        [WRITE_HEADER_ERROR] = "Error when writing header to a file\n",
};

enum read_status from_bmp( FILE* in, struct image* img ){

    struct bmp_header header = {0};
    if (fread(&header, sizeof(struct bmp_header), 1, in) != 1) {
        return READ_INVALID_HEADER;
    }

    if (header.bfType != SIGNATURE) {
        return READ_INVALID_SIGNATURE;
    }

    if (header.biBitCount != BITS_COUNT) {
        return READ_INVALID_BITS;
    }

    *img = create_image(header.biWidth, header.biHeight);

    int8_t padding = padding(img->width);
    for (uint32_t i = 0; i < header.biHeight; ++i) {
        size_t cnt_read = fread(img->data + i * header.biWidth, sizeof(struct pixel), header.biWidth, in);
        if (cnt_read != header.biWidth) {
            return READ_INVALID_DATA;
        }
        int res_fseek = fseek(in, padding, SEEK_CUR);
        if (res_fseek != 0) {
            return READ_INVALID_DATA;
        }
    }
    return READ_OK;
}

static struct bmp_header create_header(const struct image* img) {
    int8_t padding = padding(img->width);
    struct bmp_header header = {0};
    header.bfType = SIGNATURE;
    header.bfReserved = RESERVED;
    header.biSize = SIZE;
    header.biPlanes = PLANES;
    header.biBitCount = BITS_COUNT;
    header.biCompression = COMPRESSION;
    header.biXPelsPerMeter = 0;
    header.biYPelsPerMeter = 0;
    header.biClrUsed = 0;
    header.biClrImportant = 0;
    header.biWidth = img->width;
    header.biHeight = img->height;
    header.biSizeImage = ((sizeof(struct pixel) * header.biWidth + padding) * header.biHeight);
    header.bOffBits = sizeof(struct bmp_header);
    header.bfileSize = sizeof(struct bmp_header) + header.biSizeImage;
    return header;
}

enum write_status to_bmp(FILE *out, struct image const *img) {
    int8_t padding = padding(img->width);
    struct bmp_header header = create_header(img);

    size_t cnt_write_header = fwrite(&header, sizeof(header), 1, out);

    if (cnt_write_header != 1) {
        return WRITE_HEADER_ERROR;
    }

    for (uint32_t i = 0; i < img->height; i++) {
        size_t cnt_write = fwrite(img->data + i * header.biWidth, sizeof(struct pixel), header.biWidth, out);
        if (cnt_write != header.biWidth) {
            return WRITE_ERROR;
        }
        int res_fseek = fseek(out, padding, SEEK_CUR);
        if (res_fseek != 0) {
            return WRITE_ERROR;
        }
    }
    return WRITE_OK;
}

void print_read_error(enum read_status status){
    if (status != READ_OK){
        fprintf(stderr, "%s\n", READ_STATUSES_INFO[status]);
    }
}

void print_write_error(enum write_status status){
    if (status != WRITE_OK){
        fprintf(stderr, "%s\n", WRITE_STATUSES_INFO[status]);
    }
}
