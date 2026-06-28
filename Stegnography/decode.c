#include <stdio.h>
#include <stdlib.h>
#include "decode.h"
#include "encode.h"
//#include "types.h"
#include "common.h"
#include <string.h>

/* Read and validate decode args from argv */
Status validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    // Output .bmp file validation
    
    // Validate file extension is .bmp
    //char *ext = strrchr(argv[2], '.'); // finds last '.' in argv
    char *ext = NULL;
    for(int i = 0; argv[2][i] != '\0'; i++)
    {
        if(argv[2][i] == '.')
        {
            ext = &argv[2][i];
        }
    }
    if (ext == NULL || strcmp(ext, ".bmp") != 0 )
    {
        printf("Decode argument validation failed\n");
        printf("ERROR: %s file must have .bmp extension\n",argv[2]);
        return failure; // No need to check file existence
    }

    // Check whether file exists
    decInfo->output_image_fptr = fopen(argv[2], "r");

    if (decInfo->output_image_fptr == NULL)
    {
        printf("Decode argument validation failed\n");
        printf("Error: %s such file is not there in pwd\n", argv[2]);
        return failure;
    }

    fclose(decInfo->output_image_fptr);
    decInfo->output_image_fname = argv[2];

    // Output file validation
    if (argv[3] != NULL)
    {
        int i = 0;
        // Remove extension if user gives output.txt 
        while (argv[3][i] != '.' && argv[3][i] != '\0')
        {
            decInfo->output_fname[i] = argv[3][i];
            i++;
        }
        decInfo->output_fname[i] = '\0';
    }
    else
    {
        strcpy(decInfo->output_fname, "output");
        printf("Output filename is not mentioned, hence named it as %s\n", decInfo->output_fname);
    }

    return success;
}

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo)
{
    // Call open_image_file()
    if (open_image_file(decInfo) == failure)
    {
        printf("Error: Unable to open %s file\n", decInfo->output_image_fname);
        return failure;
    }

    // Move file offset to 54th byte for magic string
    fseek(decInfo->output_image_fptr, 54, SEEK_SET);

    // Call decode_magic_string()
    if (decode_magic_string(decInfo) == failure)
    {
        printf("ERROR: Unable to decode magic string\n");
        return failure;
    }

    // Call decode_secret_file_extn()
    if (decode_secret_file_extn(decInfo) == failure)
    {
        printf("ERROR: Unable to decode secret file extension\n");
        return failure;
    }

    // Concatenate extension with output file name

    // Store output_fname into temporary buffer
    char buffer[30];
    strcpy(buffer, decInfo->output_fname);

    // Concatenating both file name and extn
    strcat(buffer, decInfo->extn_output_file);

    // Copying back to struct output_fname
    strcpy(decInfo->output_fname, buffer);

    // Call decode_secret_file_data()
    if (decode_secret_file_data(decInfo) == failure)
    {
        printf("ERROR: Unable to decode secret file data\n");
        return failure;
    }

    fclose(decInfo->output_image_fptr);

    fclose(decInfo->fptr_output);

    printf("Decoded Successfully\n");
    return success;
}

/* Get File pointer for o/p file */
Status open_image_file(DecodeInfo *decInfo)
{
    decInfo->output_image_fptr = fopen(decInfo->output_image_fname, "r");

    if (decInfo->output_image_fptr == NULL)
    {
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_image_fname);

    	return failure;
    }

    return success;
}

/* Check Magic String */
Status decode_magic_string(DecodeInfo *decInfo)
{
    printf("Decoding magic string...\n");
    //char magic_string(strlen(MAGIC_STRING)+1);

    // Buffer for 8 image bytes
    char image_buffer[8];

    // Variable for decoded character
    char ch;

    for(int i = 0; i < strlen(MAGIC_STRING); i++)
    {
        // Read 8 bytes from output image
        if (fread(image_buffer, 8, 1, decInfo->output_image_fptr) != 1)
        {
            return failure;
        }
        
        // Call decode_1byte_from_lsb()
        ch = decode_1byte_to_lsb(image_buffer);

        // Compare with original magic string
        if (ch != MAGIC_STRING[i])
        {
            printf("ERROR: Magic string mismatch\n");
            return failure;
        }
    }
    printf("Decoded magic string successfully\n");
    return success;
}

/* Decode 1 byte into LSB of image data array */
char decode_1byte_to_lsb(char *image_buffer)
{
    char data = 0;
    int bit;

    for(int i = 0; i < 8; i++)
    {
        // Extract lsb
        bit = image_buffer[i] & 1;

        // PLace bit in proper position
        data = data | (bit << (7-i));
    }
    return data;
}

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    printf("Decoding secret file extension...\n");
    // Buffer for decoding extension size
    char size_buffer[32];

    // Buffer for decoding one character
    char image_buffer[8];

    // Read 32 bytes for extension size
    if (fread(size_buffer, 32, 1, decInfo->output_image_fptr) != 1)
    {
        return failure;
    }
    printf("Decoded secret file extension successfully\n");

    printf("Decoding secret file extension size...\n");
    // Decode extension size
    int extn_size = decode_4byte_to_lsb(size_buffer);

    //printf("Extension size = %d\n", extn_size);

    // Decode extension characters
    for(int i = 0; i < extn_size; i++)
    {
        // Read 8 image bytes
        if (fread(image_buffer, 8, 1, decInfo->output_image_fptr) != 1)
        {
            return failure;
        }
        
        // Decode one character
        char ch = decode_1byte_to_lsb(image_buffer);

        // Store character
        decInfo->extn_output_file[i] = ch;
    }
    // Add null character
    decInfo->extn_output_file[extn_size] = '\0';
    printf("Decoded secret file extension size successfully\n");
    return success;
}

/* Decode 4byte(int) into LSB of image data array */
int decode_4byte_to_lsb(char *size_buffer)
{
    int size = 0;
    int bit;

    for(int i = 0; i < 32; i++)
    {
        // Extract lsb
        bit = size_buffer[i] & 1;

        // PLace bit in proper position
        size = size | (bit << (31-i));
    }
    return size;
}

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    printf("Decoding secret file data from %s file...\n", decInfo->output_image_fname);
    // Open output secret file in write mode
    decInfo->fptr_output = fopen(decInfo->output_fname, "w");

    if (decInfo->fptr_output == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_fname);

    	return failure;
    }

    printf("Decoding secret file size...\n");
    // Decode secret file size

    // Buffer for decoding secret data size
    char buffer[32];

    // Read 32 bytes
    fread(buffer, 32, 1, decInfo->output_image_fptr);

    // Call decode_4byte_to_lsb and store in struct
    decInfo->size_output_file = decode_4byte_to_lsb(buffer);
    printf("Secret file size: %d\n", decInfo->size_output_file);
    printf("Decoded secret file size successfully\n");

    printf("Decoding secret file data...\n");
    // Buffer for decoding secret data
    char data_buf[8];

    // To store decoded character
    char ch;

    for (int i = 0; i < decInfo->size_output_file; i++)
    {
        // Read 8 image bytes
        fread(data_buf, 8, 1, decInfo->output_image_fptr);

        // Decode one character
        ch = decode_1byte_to_lsb(data_buf);
        printf("%c", ch);

        // Write it into output file
        if (fwrite(&ch, 1, 1, decInfo->fptr_output) != 1)
        {
            return failure;
        }
        
    }
    
    printf("\nDecoded secret file data successfully\n");
    return success;
}