/*
Name: Sanketh T S
Register no: 25048_084
Date: 22/05/2026
Description: 
The Image Steganography project is implemented using the C programming language and is based on the 
Least Significant Bit (LSB) technique for secure data hiding. The main objective of the project is 
to hide a secret file inside a BMP image without causing any noticeable change in the image appearance. 
The project demonstrates how digital images can be used as carrier files to securely transmit confidential 
information while keeping the existence of the hidden data invisible to others.
The project performs two major operations: encoding and decoding. During encoding, the program reads
a source BMP image and a secret file, then hides the secret data inside the image by modifying the 
least significant bits of the image pixel bytes. Along with the secret data, important metadata such 
as the magic string, secret file extension, and secret file size are also encoded to ensure proper 
recovery during decoding. The modified image is then saved as a stego image.
During decoding, the program extracts the hidden information from the stego image by reading the least 
significant bits in the same sequence used during encoding. The magic string is first verified to confirm 
the presence of encoded data. After validation, the program retrieves the secret file extension, file size, 
and actual secret content, and reconstructs the original hidden file successfully.
This project provides practical exposure to important concepts such as file handling, bitwise operations, 
structures, command-line arguments, BMP image processing, and modular programming in C. It also introduces 
fundamental concepts of information security and secure communication. The project serves as an effective 
implementation for understanding how steganography works at the low-level programming stage while maintaining 
the visual quality of the image.
*/

#include <stdio.h>
#include "encode.h"
#include "common.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    EncodeInfo encodeInfo;
    DecodeInfo decodeInfo;

    if( argc == 1 )
    {
        // print -> Error + usage msg
        printf("ERROR: Invalid number of arguments for encoding/decoding\n");
        printf("For encoding, (./a.out -e src_image_fname.bmp secret_fname.txt/.c/.cpp output_image_fname.bmp) or (./a.out -e src_image_fname.bmp secret_fname.txt/.c/.cpp)\n");
        printf("For decoding, (./a.out -d output_image_fname.bmp output_fname) or (./a.out -d output_image_fname.bmp)\n");
        return 0;
    }

    int opr = check_operation(argv[1]);

    if( opr == encode )
    {
        printf("SELECTED ENCODE OPERATION\n\n");

        if( argc < 4 || argc > 5 )
        {
            // print -> Error + usage msg
            printf("Error: For encoding, (./a.out -e src_image_fname.bmp secret_fname.txt/.c/.cpp output_image_fname.bmp) or (./a.out -e src_image_fname.bmp secret_fname.txt/.c/.cpp)\n");
            return 0;
        }

        printf("Validating encode arguments...\n");
        
        opr = validate_encode_args(argv, &encodeInfo);
        
        if (opr == failure)
        {
            //printf("Encode argument validation failed\n");
            //printf("Error: For encoding, (./a.out -e src_image_fname.bmp secret_fname.txt/.c/.cpp output_image_fname.bmp) or (./a.out -e src_image_fname.bmp secret_fname.txt/.c/.cpp)\n");
            return 0;
        }
        
        printf("Encode arguments validated successfully\n");
        
        printf("ENCODING STARTED...\n");

        do_encoding(&encodeInfo);
    }
    else if( opr == decode )
    {
        printf("SELECTED DECODE OPERATION\n\n");

        if( argc < 3 || argc > 4 )	
        {
            // print -> Error + usage msg
            printf("Error: For decoding, (./a.out -d output_image_fname.bmp output_fname) or (./a.out -d output_image_fname.bmp)\n");
            return 0;
        }

        printf("Validating decode arguments...\n");

        opr = validate_decode_args(argv, &decodeInfo);
        
        if (opr == failure)
        {
            //printf("Decode argument validation failed\n");
            //printf("Error: For decoding, (./a.out -d output_image_fname.bmp) or (./a.out -d output_image_fname.bmp output_fname)\n");
            return 0;
        }
        printf("Decode arguments validated successfully\n");

        printf("DECODING STARTED...\n");

        do_decoding(&decodeInfo);
    }
    else
    {
        // Print -> Error + usage msg
        printf("Unsupported operation\n");
        printf("Error: ./a.out -e/-d ......\n");
    }
    return 0;
}
