#ifndef DECODE_H
#define DECODE_H

#include <stdio.h> //FILE is defined in stdio.h
#include "common.h" // encode.h function return type depends on common.h
#include "encode.h"

typedef struct DecodeInfo
{
    /* Stego Image Info */
    char *output_image_fname;   // => Store the Output_img_fname
    FILE *output_image_fptr;    // => File pointer for output_Img

    // Output file info
    char output_fname[20]; // store output file name
    FILE *fptr_output; // File pointer for output file
    char extn_output_file[10]; // To store extension of output file
    int size_output_file; // To store size of output file
    
}DecodeInfo;

/* Read and validate decode args from argv */
Status validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointer for o/p file */
Status open_image_file(DecodeInfo *decInfo);

/* Check Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode 1 byte into LSB of image data array */
char decode_1byte_to_lsb(char *image_buffer);

/* Decode 4byte(int) into LSB of image data array */
int decode_4byte_to_lsb(char *image_buffer);

#endif