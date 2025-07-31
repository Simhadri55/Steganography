#ifndef DECODE_H
#define DECODE_H

#include "types.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    

    //char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char secret_fname[100];
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    //char secret_data[MAX_SECRET_BUF_SIZE];
    long int size_secret_file;
    int extn_size;

} DecodeInfo;


//Function Declarations

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

Status decode_magic_string(FILE *fptr_stego_image, DecodeInfo *decInfo);

Status decode_secret_file_extn_size(DecodeInfo *decInfo);

Status decode_secret_file_extn(DecodeInfo *decInfo);

Status decode_secret_file_size(DecodeInfo *decInfo);

Status secret_file_data(DecodeInfo *decInfo);

Status decode_byte_from_lsb(char *buffer, char *image_buffer);

int decode_int_from_lsb(const char *buffer);

Status close_dec_files(DecodeInfo *decInfo);

#endif