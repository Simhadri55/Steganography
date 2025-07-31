/*
Documentation
Name        :T Simhadri
Date        :30/07/2025
Description :Steganography Project
*/

#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    // Check if the number of arguments is less than 2
    if(argc < 2)
    {
        // Display usage instructions
        printf("➡️  Use -e for encoding or -d for decoding\n");
        printf("📌 Usage:\n");
        printf("📥 To encode : %s -e <input.bmp> <secret.txt|.c|.sh> [optional output.bmp]\n", argv[0]);
        printf("📤 To decode : %s -d <stego_image.bmp> [optional output_file]\n", argv[0]);
        return e_failure;
    }

    // Declare encode and decode info structures
    EncodeInfo encodeInfo; //structure
    DecodeInfo decInfo;

    // Determine operation type using CLA
    if(check_operation_type(argv) == e_encode)
    {
        // For encoding, expect 4 or 5 arguments
        if(argc >= 4 && argc <= 5)
        {
            // Validate encode arguments
            if(read_and_validate_encode_args(argv, &encodeInfo) == e_failure)
            {
                return 1;
            }

            // start the encoding
            if(do_encoding(&encodeInfo) == e_failure)
            {
                close_enc_files(&encodeInfo); // Close files if failure
                
                printf("ERROR ❌ : Can't do encoding\n");
                return 1;
            }

            // Close all opened files after encoding
            close_enc_files(&encodeInfo);
        }
        else
        {
            // Insufficient or too many arguments for encoding
            printf("ERROR ❌ : Invalid number of arguments for encoding\n");
            printf("📌 Usage:\n");
            printf("📥 To encode : %s -e <input.bmp> <secret.txt|.c|.sh> [optional output.bmp]\n", argv[0]);
            return 1;
        }

        // Successful encoding
        printf("INFO ✅ : ## Encoding Done Successfully ## 🎉\n\n");
    }
    else if(check_operation_type(argv) == e_decode)
    {
        if (argc >= 3 && argc <= 4)
        {
            // For decoding, expect 3 or 4 arguments
            if(read_and_validate_decode_args(argv, &decInfo) == e_failure)
                return 1;

            // Begin decoding process
            printf("INFO 🚀 : ## Decoding Procedure Started ## 🔍\n\n");

            if(do_decoding(&decInfo) == e_failure)
            {
                close_dec_files(&decInfo);
                printf("ERROR ❌ : Can't do decoding\n");
                return 1;
            }

            close_dec_files(&decInfo); // Close files if failure
        }
        else
        {
            // Incorrect argument count for decoding
            printf("ERROR ❌ : Invalid number of arguments for decoding\n");
            printf("📌 Usage:\n");
            printf("📤 To decode : %s -d <stego_image.bmp> [optional output_file]\n", argv[0]);
            return 1;
        }

        // Successful decoding
        printf("INFO ✅ : ## Decoding Done Successfully ## 🎉\n\n");
    }
    else
    {
        // Invalid operation type (not -e or -d)
        printf("ERROR ❌ : Unsupported operation\n");
        printf("➡️  Use -e for encoding or -d for decoding\n");
        printf("📌 Usage:\n");
        printf("📥 To encode : %s -e <input.bmp> <secret.txt|.c|.sh> [optional output.bmp]\n", argv[0]);
        printf("📤 To decode : %s -d <stego_image.bmp> [optional output_file]\n", argv[0]);
    }

    return 0;
}