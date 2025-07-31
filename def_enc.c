/*
Documentation
Name        :T Simhadri
Date        :30/07/2025
Description :Steganography Project
*/

#include <stdio.h>
#include "encode.h"
#include <string.h>
#include "types.h"
#include "common.h"
#include <stdlib.h>

//Checking the operation type
OperationType check_operation_type(char *argv[])
{
    //Check for encode or decode
    if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }

    else if(strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported; //Other than -e / -d
    }
}

//Read and validate the arguments
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    printf("INFO ℹ️  : Validating arguments\n");
    // Initialize file pointers
    encInfo->fptr_secret = NULL;
    encInfo->fptr_src_image = NULL;
    encInfo->fptr_stego_image = NULL;

    // Check if source image file has .bmp extension
    char *ch = strrchr(argv[2], '.');
    if(ch != NULL && strcmp(ch, ".bmp") == 0)
    {
        encInfo->src_image_fname = argv[2];
        printf("INFO ✅ : source image %s validated\n", argv[2]);
    }
    else
    {
        printf("ERROR ❌ : Source file must be a .bmp file 🖼️\n");
        return e_failure;
    }

    // Check if secret file has valid extension
    ch = strrchr(argv[3], '.');
    if ( ch != NULL && strcmp(ch, ".txt") == 0 ) //check for ".txt"
    {
        encInfo->secret_fname = argv[3];
        strcpy(encInfo->extn_secret_file, ".txt"); //copying ".txt"
    }
    else if(ch != NULL && strcmp(ch, ".c") == 0 ) //check for ".c"
    {
        encInfo->secret_fname = argv[3];
        strcpy(encInfo->extn_secret_file, ".c"); //copying ".c"
    }
    else if(ch != NULL && strcmp(ch, ".sh") == 0 ) //check for ".sh"
    {
        encInfo->secret_fname = argv[3];
        strcpy(encInfo->extn_secret_file, ".sh"); //copying ".sh"
    }
    else
    {
        //other than .txt, .c, or .sh
        printf("ERROR ❌ : Secret file must be .txt, .c, or .sh 📄\n");
        return e_failure;
    }

    printf("INFO ✅ : secret file %s validated\n", argv[3]);

    // Check if output file name is provided
    if(argv[4] == NULL)
    {
        printf("INFO ✅ : Output File not mentioned. Creating stego.bmp as default 🖼️\n");
        encInfo->stego_image_fname = "stego.bmp"; //naming encoded file name
    }
    else
    {
        ch = strrchr(argv[4], '.'); //check for '.'
        if(ch != NULL && strcmp(ch, ".bmp") == 0)
        {
            encInfo->stego_image_fname = argv[4]; //copy argv[4] as encoded file name
        }
        else
        {
            printf("ERROR ❌ : Encoded file must be .bmp file 🖼️\n");
            return e_failure;
        }
    }

    printf("INFO ✅ : Successfully validated all arguments\n\n");
    return e_success;
}

//Encoding process 
Status do_encoding(EncodeInfo *encInfo)
{
    printf("INFO 📂 : Opening required files\n");
    if (open_files(encInfo) == e_failure)
    {
        printf("ERROR ❌ : Failed to open files 📁\n");
        return e_failure;
    }
    printf("INFO ✅ : Done\n\n");

    printf("INFO 🚀 : ## Encoding Procedure Started ##\n\n");

    printf("INFO 📏 : Checking for secret.txt size\n");
    if (check_capacity(encInfo) == e_failure)
    {
        printf("ERROR ❌ : Stego.bmp doesn't have the capacity to encode secret.txt 📄\n");
        return e_failure;
    }
    printf("INFO 🟢 : Done. Found OK ✅\n\n");

    printf("INFO 🧩 : Copying Image Header\n");
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR ❌ : Failed to copy the header 🖼️\n");
        return e_failure;
    }
    printf("INFO ✅ : Done\n\n");

    printf("INFO ✨ : Encoding Magic String Signature\n");
    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        printf("ERROR ❌ : Failed to copy MAGIC STRING 🔐\n");
        return e_failure;
    }
    printf("INFO ✅ : Done\n\n");

    printf("INFO 📦 : Encoding secret File Size\n");
    if (encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_failure)
    {
        printf("ERROR ❌ : Failed to copy file size 📏\n");
        return e_failure;
    }
    printf("INFO ✅ : Done\n\n");

    printf("INFO 📝 : Encoding secret.txt File Extenstion\n");
    if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure)
    {
        printf("ERROR ❌ : Faile to copy secret file extension 📄\n");
        return e_failure;
    }
    printf("INFO ✅ : Done\n\n");

    printf("INFO 📏 : Encoding secret.txt File Size\n");
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_failure)
    {
        printf("ERROR ❌ : Failed to copy secret file size 📐\n");
        return e_failure;
    }
    printf("INFO ✅ : Done\n\n");

    printf("INFO 📂 : Encoding secret.txt File Data\n");
    if (encode_secret_file_data(encInfo) == e_failure)
    {
        printf("ERROR ❌ : Failed to copy the secret file data 💾\n");
        return e_failure;
    }
    printf("INFO ✅ : Done\n\n");

    printf("INFO 🧱 : Copying Left Over Data\n");
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR ❌ : Failed to copy the remaining data 🗃️\n");
        return e_failure;
    }
    printf("INFO ✅ : Done\n\n");
    
    return e_success;
}

// Check if source image can handle the data
Status check_capacity(EncodeInfo *encInfo)
{
    int total_capacity ;
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    // Calculate total bits required
    total_capacity = 54 + ((strlen(MAGIC_STRING) + 4 + strlen(encInfo->extn_secret_file) + 4 + encInfo->size_secret_file) * 8);

    if(total_capacity > 0)
        printf("INFO 🟢 : Done. Not Empty 📁\n\n");

    printf("INFO 📊 : Checking for beautiful.bmp capacity to handle secret.txt 📄\n");
    if (encInfo->image_capacity >= total_capacity)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

// Copy BMP image header
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    rewind(fptr_src_image);
    // Read 54 bytes from the src_img
    fread(buffer, 54, 1, fptr_src_image);
    fwrite(buffer, 54, 1, fptr_dest_image);
    
    return e_success;
}

// Encode predefined magic string
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    //call encode_data_to_image()

    if(encode_data_to_image(magic_string, strlen(magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
        return e_success;
    else 
        return e_failure;
}

// Encode size of file extension
Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encoInfo)
{
    int i;
    char buffer[32];
    
    if(fread(buffer, 32, 1, encoInfo->fptr_src_image) != 1)
        return e_failure;

    encode_int_to_lsb(extn_size, buffer);

    fwrite(buffer, 32, 1, encoInfo->fptr_stego_image);

    return e_success;
}

// Encode actual file extension (.txt, .sh, etc.)
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buffer[strlen(file_extn)];
    
    encode_data_to_image(file_extn, strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image);
    
    return e_success;
}

//Encode Secret File Size
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    char buffer[file_size];
    if(fread(buffer, 32, 1, encInfo->fptr_src_image) != 1)
        return e_failure;

    encode_int_to_lsb(file_size, buffer);

    fwrite(buffer, 32, 1, encInfo->fptr_stego_image);

    return e_success;
}

// Encode the actual content of secret file
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buffer[encInfo->size_secret_file];
    rewind(encInfo->fptr_secret);

    if(fread(buffer, encInfo->size_secret_file, 1, encInfo->fptr_secret) != 1)
    {
        printf("ERROR ❌ : Can't read data in the secret file 📄\n");
        return e_failure;
    }

    if(encode_data_to_image(buffer, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        return e_failure;
    }

    return e_success;
}

// Copy leftover image data
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while (fread(&ch, 1, 1, fptr_src) > 0)
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;
}

// Encode buffer of data into image (bit-by-bit)
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    // call encode_byte_to_lsb
    int i;
    char buffer[8];
    
    for (i = 0; i < size; i++)
    {
        fread(buffer, 8, 1, fptr_src_image);
        encode_byte_to_lsb(data[i], buffer);
        fwrite(buffer, 8, 1, fptr_stego_image);
    }
    return e_success;
}

// Encode a byte to LSBs of image buffer
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    int get = 0, clear = 0, i, j = 7;

    for (i = 0; i < 8; i++)
    {
        get = (data >> j) & 1 ;
        j--;

        clear = (image_buffer[i]) & (~1);

        image_buffer[i] = get | clear;
    }
    
    return e_success;
}

// Encode an integer to LSBs
Status encode_int_to_lsb(int data, char *image_buffer)
{
    int get, clear, i, j = 31;
    for (i = 0; i < 32; i++)
    {
        get = (data >> j) & 1 ;
        j--;

        clear = (image_buffer[i]) & (~1);

        image_buffer[i] = get | clear;
    }
    
    return e_success;
}

//Get file size
uint get_file_size(FILE *fptr)
{
    //get secret file size
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

// Close all opened files
Status close_enc_files(EncodeInfo *encInfo)
{
    int flag = 0;

    if (encInfo->fptr_src_image != NULL)
    {
        flag = 1;
        fclose(encInfo->fptr_src_image);
        encInfo->fptr_src_image = NULL;
        printf("INFO 🔒 : Closed %s 📷\n", encInfo->src_image_fname);
    }
    
    if(encInfo->fptr_secret != NULL)
    {
        flag = 1;
        fclose(encInfo->fptr_secret);
        encInfo->fptr_src_image = NULL;
        printf("INFO 🔒 : Closed %s 📄\n", encInfo->secret_fname);
    }

    if(encInfo->fptr_stego_image != NULL)
    {
        flag = 1;
        fclose(encInfo->fptr_stego_image);
        encInfo->fptr_src_image = NULL;
        printf("INFO 🔒 : Closed %s 🖼️\n", encInfo->stego_image_fname);
    }

    if(flag)
        printf("INFO ✅ : All files closed 🔚\n\n");
    return e_success;
}
