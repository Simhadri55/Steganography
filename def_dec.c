/*
Documentation
Name        :T Simhadri
Date        :30/07/2025
Description :Steganography Project
*/
#include <stdio.h>
#include "decode.h"
#include <string.h>
#include "types.h"
#include "common.h"
#include <stdlib.h>

// Function to validate decoding command line arguments
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    printf("INFO ℹ️  : Validating arguments\n");
    decInfo->fptr_stego_image = NULL;
    decInfo->fptr_secret = NULL;

    // Validate input image extension
    char *ch = strrchr(argv[2], '.');
    if(ch != NULL && strcmp(ch, ".bmp") == 0)
    {
        printf("INFO ✅ : .bmp file validated\n");
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        printf("ERROR ❌ : Source file must be a .bmp 🖼️\n");
        return e_failure;
    }

    // Check for optional output file name
    if( argv[3] != NULL)
    {
        if(strchr(argv[3], '.') != NULL)
        {
            printf("INFO ⚠️  : <output file> must be without extenion\n");
            printf("ERROR ❌ : <output file> name not validated\n");
            return e_failure;
        }
        strcpy(decInfo->secret_fname, argv[3]);
    }
    else
    {
        strcpy(decInfo->secret_fname, "decode_sec");
    }

    printf("INFO ✅ : Successfully validated all arguments\n\n");
    return e_success;
}

// Function that performs all decoding steps
Status do_decoding(DecodeInfo *decInfo)
{
    printf("INFO 📂 : Opening required files\n");
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
   
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen ");
        printf("ERROR ❌ : Can't Open File at Decoding 📁\n");
        return e_failure; 
    }

    printf("INFO ✅ : Opened .bmp 🖼️\n");

    printf("INFO 🔍 : Decoding Magic String Signature 🔐\n");
    if(decode_magic_string(decInfo->fptr_stego_image, decInfo) == e_failure)
    {
        printf("INFO ❌ : Can't Decode MAGIC STRING 💢\n");
        return e_failure;
    }
    printf("INFO ✅ : Done\n\n");

    printf("INFO 📐 : Decoding Output File Extenstion Size\n");
    if(decode_secret_file_extn_size(decInfo) == e_failure)
    {
        printf("INFO ❌ : Can't Decode Secret File Extenstion Size 🚫\n");
        return e_failure;
    }
    printf("INFO ✅ : Done\n\n");

    printf("INFO 📝 : Decoding secret file Extension\n");
    if (decode_secret_file_extn(decInfo) == e_failure)
    {
        printf("INFO ❌ : Can't Decode secret file Extension 📄\n");
        return e_failure;
    }
    printf("INFO ✅ : Done\n\n");

    printf("INFO 📏 : Decoding File Size\n");
    if(decode_secret_file_size(decInfo) == e_failure)
    {
        printf("INFO ❌ : Can't Decode File Size ❗\n");
        return e_failure;
    }
    printf("INFO ✅ : Done\n\n");
    
    printf("INFO 💾 : Decoding File Data\n");
    if(secret_file_data(decInfo) == e_failure)
    {
        printf("INFO ❌ : Can't Decode File Data 📉\n");
        return e_failure;
    }
    printf("INFO ✅ : Done\n\n");
}

// Decode the magic string from stego image
Status decode_magic_string(FILE *fptr_stego_image, DecodeInfo *decInfo)
{
    char buffer[8] , ch = 0;
    int i;
    char magic_string[strlen(MAGIC_STRING) + 1];

    fseek(decInfo->fptr_stego_image, 54, SEEK_CUR); // Skip BMP header
    
    for (i = 0; i < strlen(MAGIC_STRING); i++)
    {
        if(fread(buffer, 8, 1, decInfo->fptr_stego_image) != 1)
        {
            printf("INFO ❌ : Can't read data at MAGIC STRING 🔐\n");
            return e_failure;
        }
        if(decode_byte_from_lsb(&ch, buffer) == e_failure)
        {
            printf("INFO ⚠️ : Can't do byte from lsb operation 🧬\n");
            return e_failure;
        }
       
        magic_string[i] = ch;
    }

    magic_string[i] = '\0';

    if(strcmp(magic_string, MAGIC_STRING) == 0)
    {
        return e_success;
    }

    return e_failure;
}

// Decode the size of the file extension (stored as int)
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];

    fread(buffer, 32, 1, decInfo->fptr_stego_image);

    decInfo->extn_size = decode_int_from_lsb(buffer);

    return e_success;
}

// Decode the file extension from the image
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[8];
    int i;
    char ch[decInfo->extn_size + 1]; // buffer for extension

    for (i = 0; i < decInfo->extn_size; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);
        if((decode_byte_from_lsb(&ch[i], buffer)) == e_failure)
        {
            printf("INFO ⚠️ : Can't do byte from lsb operation 🧬\n");
            return e_failure;
        }
    }

    ch[i] = '\0';

    int f = 1;

    if(strcmp(decInfo->secret_fname, "decode_sec") == 0)
    {
        f = 0;
        printf("INFO ℹ️  : Output File not mentioned. Creating 'decode_sec%s' as default 📝\n", ch);
    }
        
    strcat(decInfo->secret_fname, ch);

    return e_success;
}

// Decode size of secret file (in bytes)
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];

    if(fread(buffer, 32, 1, decInfo->fptr_stego_image) != 1)
    {
        printf("INFO ⚠️ : Can't read data at secret file size 📏\n");
        return e_failure;
    }

    decInfo->size_secret_file = decode_int_from_lsb(buffer);

    return e_success;
}

// Decode actual data into output file
Status secret_file_data(DecodeInfo *decInfo)
{
    char buffer[8];
    char ch;
    int i;

    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    if(decInfo->fptr_secret == NULL)
    {
        printf("INFO ❌ : Can't open secret file 📂\n");
        return e_failure;
    }

    for (i = 0; i < decInfo->size_secret_file; i++)
    {
        if(fread(buffer, 8, 1, decInfo->fptr_stego_image) != 1)
        {
            printf("INFO ❌ : Can't read secret file data 📥\n");
            fclose(decInfo->fptr_secret);
            return e_failure;
        }

        if(decode_byte_from_lsb(&ch, buffer) == e_failure)
        {
            printf("INFO ❌ : Can't decode data from secret file data 🔍\n");
            fclose(decInfo->fptr_secret);
            return e_failure;
        }

        if(fwrite(&ch, 1, 1, decInfo->fptr_secret) !=1)
        {
            printf("INFO ❌ : Can't write secret file data 📝\n");
            return e_failure;
        }
    }

    return e_success;
}

// Decode a single byte from 8 bytes (1 LSB per byte)
Status decode_byte_from_lsb(char *ch, char *image_buffer)
{
    *ch = 0;
    int get = 0, i, j = 7;
    for (i = 0; i < 8; i++)
    {
        get = image_buffer[i] & 1;
        get = get << j;
        *ch = *ch | get;
        j--;
    }

    return e_success;
}

// Decode a 32-bit integer from LSBs of 32 bytes
int decode_int_from_lsb(const char *buffer)
{
    int size = 0;
    int get = 0, i, j = 31;
    for (i = 0; i < 32; i++)
    {
        get = buffer[i] & 1;
        get = get << j;
        size = size | get;
        j--;
    }

    return size;
}

// Close all decoding-related files
Status close_dec_files(DecodeInfo *decInfo)
{
    int flag = 0;

    if (decInfo->fptr_stego_image != NULL)
    {
        flag = 1;
        fclose(decInfo->fptr_stego_image);
        decInfo->fptr_stego_image = NULL;
        printf("INFO 📁 : Closed %s ✅\n", decInfo->stego_image_fname);
    }

    if(decInfo->fptr_secret != NULL)
    {
        flag = 1;
        fclose(decInfo->fptr_secret);
        decInfo->fptr_secret = NULL;
        printf("INFO 📄 : Closed %s ✅\n", decInfo->secret_fname);
    }

    if(flag)
        printf("INFO ✅ : All files closed 🔒\n\n");
    return e_success;
}
