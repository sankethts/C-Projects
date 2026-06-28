#include <stdio.h>
#include <stdlib.h>
#include "encode.h"
#include "common.h"
#include <string.h>

/* Check operation type */
Opr_type check_operation(char *option)
{
    if (strcmp("-e", option) == 0)
     return encode;

    else if (strcmp("-d", option) == 0)
     return decode;
    
    return unsupported;
}

/* Read and validate Encode args from argv */
Status validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // Source file validation

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
        if (ext == NULL || strcmp(ext, ".bmp") != 0)
        {
            printf("Encode argument validation failed\n");
            printf("ERROR: %s must have .bmp extension\n", argv[2]);
            return failure; // No need to check file existence
        }

        // Check whether file exists
        encInfo->src_image_fptr = fopen(argv[2], "r");

        if (encInfo->src_image_fptr == NULL)
        {
            printf("Encode argument validation failed\n");
            printf("Error: %s such file is not there in pwd\n", argv[2]);
            return failure;
        }
        
        fclose(encInfo->src_image_fptr);
        encInfo->src_image_fname = argv[2];
    
    
    // Secret file validation
    
        // Validate file extension is .txt  .c  .cpp
        //char *ext = strrchr(argv[3], '.'); // finds last '.' in argv
        ext = NULL;
        for(int i = 0; argv[3][i] != '\0'; i++)
        {
            if(argv[3][i] == '.')
            {
                ext = &argv[3][i];
            }
        }
        if (ext == NULL)
        {
            printf("Encode argument validation failed\n");
            printf("ERROR: %s file has extension missing\n", argv[3]);
            return failure;
        }
        // if (strcmp(ext, ".txt") != 0 && strcmp(ext, ".c") != 0 && strcmp(ext, ".cpp") != 0)
        // {
        //     printf("Encode argument validation failed\n");
        //     printf("ERROR: %s file must have .txt /.c /.cpp extension\n", argv[3]);
        //     return failure; // No need to check file existence
        // }
        
        // Secret file extension extracted and stored in struct secret_extn[]
        strcpy(encInfo->secret_extn, ext);

        // secret_extn_size using length of secret_extn
        encInfo->secret_extn_size = strlen(encInfo->secret_extn);

        // Check whether file exists
        encInfo->secret_fptr = fopen(argv[3], "r");

        if (encInfo->secret_fptr == NULL)
        {
            printf("Encode argument validation failed\n");
            printf("Error: %s such file is not there in pwd\n", argv[3]);
            return failure;
        }

        fclose(encInfo->secret_fptr);
        encInfo->secret_fname = argv[3];


    // Output file validation
    if (argv[4] != NULL)
    {
        // Validate file extension is .bmp
        //char *ext = strrchr(argv[4], '.'); // finds last '.' in argv
        ext = NULL;
        for(int i = 0; argv[4][i] != '\0'; i++)
        {
            if(argv[4][i] == '.')
            {
                ext = &argv[4][i];
            }
        }
        if (ext == NULL || strcmp(ext, ".bmp") != 0 )
        {
            printf("Encode argument validation failed\n");
            printf("ERROR: %s file must have .bmp extension\n", argv[4]);
            return failure; // No need to check file existence
        }

        // Generate a file
        // encInfo->output_image_fptr = fopen(argv[4], "w");

        // if (encInfo->output_image_fptr == NULL)
        // {
        //     printf("Encode argument validation failed\n");
        //     printf("Unable to open %s file\n", encInfo->output_image_fname);
        //     //printf("Error: %s such file is not there in pwd\n", argv[4]);
        //     return failure;
        // }

        // fclose(encInfo->output_image_fptr);
        encInfo->output_image_fname = argv[4];    
    }
    else
    {
        encInfo->output_image_fname = "default.bmp";
        printf("Output image filename is not mentioned, hence named as %s\n", encInfo->output_image_fname);
        return success;
    }
    return success;
}

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo)
{
    // Call open_files()
    if (open_files(encInfo) == failure)
    {
        //printf("Error: Unable to open files\n");
        return failure;
    }

    // Call check_capacity()
    if (check_capacity(encInfo) == failure)
    {
        printf("ERROR: Insufficient image capacity\n");
        return failure;
    }

    // Call copy_bmp_header()
    if (copy_bmp_header(encInfo) == failure)
    {
        printf("ERROR: Unable to copy 54 bytes of header of %s file\n", encInfo->src_image_fname);
        return failure;
    }

    // Call encode_magic_string()
    if (encode_magic_string(MAGIC_STRING, encInfo) == failure)
    {
        printf("ERROR: Unable to encode magic string\n");
        return failure;
    }

    // Call encode_secret_file_extn()
    if (encode_secret_file_extn(encInfo) == failure)
    {
        printf("ERROR: Unable to encode %s_file_extn\n", encInfo->secret_fname);
        return failure;
    }

    // Call encode_secret_file_data()
    if (encode_secret_file_data(encInfo) == failure)
    {
        printf("ERROR: Unable to encode %s_file_data\n", encInfo->secret_fname);
        return failure;
    }

    // Call copy_remaining_img_data()
    if (copy_remaining_img_data(encInfo) == failure)
    {
        printf("ERROR: Unable to copy remaining %s data\n", encInfo->src_image_fname);
        return failure;
    }

    printf("Encoded Successfully\n");
    return success;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: success or failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    printf("Opening source image file...\n");
    encInfo->src_image_fptr = fopen(encInfo->src_image_fname, "rb");

    if (encInfo->src_image_fptr == NULL)
    {
    	fprintf(stderr, "ERROR: Unable to open source image file %s\n", encInfo->src_image_fname);

    	return failure;
    }
    printf("Source image file opened successfully\n");

    printf("Opening secret file...\n");
    encInfo->secret_fptr = fopen(encInfo->secret_fname, "r");

    if (encInfo->secret_fptr == NULL)
    {
    	fprintf(stderr, "ERROR: Unable to open secret file %s\n", encInfo->secret_fname);

    	return failure;
    }
    printf("Secret file opened successfully\n");

    printf("Opening output image file...\n");
    encInfo->output_image_fptr = fopen(encInfo->output_image_fname, "wb");

    if (encInfo->output_image_fptr == NULL)
    {
    	fprintf(stderr, "ERROR: Unable to open output image file %s\n", encInfo->output_image_fname);

    	return failure;
    }
    printf("Output image file opened successfully\n");
    return success;
}

/* check capacity */
Status check_capacity(EncodeInfo *encInfo)
{
    printf("Checking capacity of source image file to encode secret file data\n");

    // Get image capacity
    unsigned int img_capacity = get_image_size_for_bmp(encInfo->src_image_fptr);
    printf("Size of %s: %u\n", encInfo->src_image_fname, img_capacity);

    // Get secret file size
    encInfo->secret_file_size = get_file_size(encInfo->secret_fptr);
    printf("Size of %s: %lu\n", encInfo->secret_fname, encInfo->secret_file_size);

    // Required bytes
    unsigned long int required_size = (strlen(MAGIC_STRING) + 4 + strlen(encInfo->secret_extn) + 4 + encInfo->secret_file_size) * 8;
    printf("Required size to encode: %lu\n", required_size);

    if (img_capacity > required_size)
    {
        printf("Sufficient space is there to encode required size...\n");
        return success;
    }

    return failure;
}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
unsigned int get_image_size_for_bmp(FILE *fptr_image)
{
    unsigned int width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}


/* Get secret file size */
unsigned long int get_file_size(FILE *fptr)
{
    unsigned long int size;

    // Move to end
    fseek(fptr, 0, SEEK_END);

    // Get current position
    size = ftell(fptr);

    // Bring back to beginning
    rewind(fptr);

    return size;
}

/* Copy bmp image header */
Status copy_bmp_header(EncodeInfo *encInfo)
{
    printf("Copying BMP header(54 bytes)...\n");
    // Move file offset of source_file.bmp to 0th index
    rewind(encInfo->src_image_fptr);

    char header[54];
    // Read 54 bytes from source file
    if (fread(header, 54, 1, encInfo->src_image_fptr) != 1)
    {
        return failure;
    }

    // Write 54 bytes to the destination file
    if (fwrite(header, 54, 1, encInfo->output_image_fptr) != 1)
    {
        return failure;
    }

    printf("BMP header copied successfully\n");
    return success;
}

/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    printf("Encoding magic string...\n");
    char magic_buf[8];

    int i;
    for(i = 0; magic_string[i] != '\0'; i++)
    {
        // Read 8 bytes from source image
        if (fread(magic_buf, 8, 1, encInfo->src_image_fptr) != 1)
        {
            return failure;
        }
        
        // Call encode_1byte_to_lsb() to change lsb of magic_buf according to magic_string[i]
        if (encode_1byte_to_lsb(magic_string[i], magic_buf) == failure)
        {
            return failure;
        }

        // Write modified bytes into output image
        if (fwrite(magic_buf, 8, 1, encInfo->output_image_fptr) != 1)
        {
            return failure;
        }
    }

    printf("Magic string encoded successfully\n");
    return success;
}

/* Encode 1 byte into LSB of image data array */
Status encode_1byte_to_lsb(char data, char *buf)
{
    int i, bit;
    for(i = 7; i >= 0; i--)
    {
        // Clear lsb from magic_buf[i]
        buf[7-i] = buf[7-i] & ~1;

        // Extract lsb bit from data
        bit = (data >> i) & 1;

        // Store bit into lsb
        buf[7-i] = buf[7-i] | bit;
    }
 
    return success;
}

/* Encode secret file extenstion */
// In this function itself, we encode secret_file extn size and extn
Status encode_secret_file_extn(EncodeInfo *encInfo)
{
    printf("Encoding %s file extension size...\n", encInfo->secret_fname);
    // Encode secret_file extn size
    char extn_size_buf[32];

    int i;
    //for(i = 0; i < (encInfo->secret_extn_size); i++)
    //{
        // Read 32 bytes from source image
        if (fread(extn_size_buf, 32, 1, encInfo->src_image_fptr) != 1)
        {
            return failure;
        }
        
        // Call encode_4byte_to_lsb() to change lsb of extn_size_buf according to encInfo->secret_extn_size
        if (encode_4byte_to_lsb(encInfo->secret_extn_size, extn_size_buf) == failure)
        {
            return failure;
        }

        // Write modified bytes into output image
        if (fwrite(extn_size_buf, 32, 1, encInfo->output_image_fptr) != 1)
        {
            return failure;
        }
    //}
    printf("Encoded %s file extension size successfully\n", encInfo->secret_fname);

    printf("Encoding %s file extension...\n", encInfo->secret_fname);
    // Encode secret_file extn, for example .txt
    char extn_buf[8];

    //  For each character: Read 8 bytes from source image
    for(i = 0; encInfo->secret_extn[i] != '\0'; i++)
    {
        // Read 32 bytes from source image
        if (fread(extn_buf, 8, 1, encInfo->src_image_fptr) != 1)
        {
            return failure;
        }
        
        // Call encode_1byte_to_lsb() to change lsb of extn_buf according to encInfo->secret_extn
        if (encode_1byte_to_lsb(encInfo->secret_extn[i], extn_buf) == failure)
        {
            return failure;
        }

        // Write modified bytes into output image
        if (fwrite(extn_buf, 8, 1, encInfo->output_image_fptr) != 1)
        {
            return failure;
        }
    }
    printf("Encoded %s file extension successfully\n", encInfo->secret_fname);
    //printf("Current file offset: %ld\n", ftell(encInfo->output_image_fptr));
    return success;
}


/* Encode 4byte(int) into LSB of image data array */
Status encode_4byte_to_lsb(int data, char *extn_size_buf)
{
    int i, bit;
    for(i = 31; i >= 0; i--)
    {
        // Clear lsb from magic_buf[i]
        extn_size_buf[31-i] = extn_size_buf[31-i] & ~1;

        // Extract lsb bit from data
        bit = (data >> i) & 1;

        // Store bit into lsb
        extn_size_buf[31-i] = extn_size_buf[31-i] | bit;
    }
    return success;
}

/* Encode secret file data*/
// In this function itself, we encode secret_file size and data
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("Encoding %s file data size\n", encInfo->secret_fname);
    // secret file size

    // 32 byte image buffer
    char sec_size_buf[32];

    // Read 32 bytes from source image
    if (fread(sec_size_buf, 32, 1, encInfo->src_image_fptr) != 1)
    {
        return failure;
    }

    // Call encode_4byte_to_lsb() to change lsb of sec_size_buf according to encInfo->secret_file_size
    if (encode_4byte_to_lsb(encInfo->secret_file_size, sec_size_buf) == failure)
    {
        return failure;
    }

    // Write modified bytes into output image fptr
    if (fwrite(sec_size_buf, 32, 1, encInfo->output_image_fptr) != 1)
    {
        return failure;
    }
    printf("Encoded %s file data size successfully\n", encInfo->secret_fname);

    // secret file data
    printf("Encoding %s file data\n", encInfo->secret_fname);
    // One-byte buffer for secret character
    char ch;

    // 8-byte image buffer
    char sec_data_buf[8];

    // Read one character from secret file, loop until EOF
    while((ch = fgetc(encInfo->secret_fptr)) != EOF)
    {
        // Read 8 bytes from source image
        if (fread(sec_data_buf, 8, 1, encInfo->src_image_fptr) != 1)
        {
            return failure;
        }

        // Call encode_1byte_to_lsb() to change lsb of sec_data_buf according to ch
        if (encode_1byte_to_lsb(ch, sec_data_buf) == failure)
        {
            return failure;
        }

        // Write modified bytes into output image fptr
        if (fwrite(sec_data_buf, 8, 1, encInfo->output_image_fptr) != 1)
        {
            return failure;
        }
    }
    printf("Encoded %s file data successfully\n", encInfo->secret_fname);
    //printf("File offset: %ld\n", ftell(encInfo->output_image_fptr));
    return success;
}

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(EncodeInfo * encInfo)
{
    printf("Encoding %s remaining data as it is\n", encInfo->src_image_fname);
    char remain_buf[1024];
    int bytes_read;

    // Copy remaining bytes from source image to output image until EOF
    while((bytes_read = fread(remain_buf, 1, sizeof(remain_buf), encInfo->src_image_fptr)) > 0)                           
    {
        if(fwrite(remain_buf, 1, bytes_read, encInfo->output_image_fptr) != bytes_read)               
        {
            return failure;
        }
    }
    printf("Encoded %s remaining data as it is successfully\n", encInfo->src_image_fname);
    return success;
}
