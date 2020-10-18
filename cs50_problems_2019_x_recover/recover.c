// Ruany Maia
// 07 September 2019
// Problem Set 3 - Recover
// This program recovers deleted JPEGs from a memory card

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FAT_BLOCK_SIZE 512
#define FILENAME_SIZE 8

int main(int argc, char *argv[])
{
    typedef uint8_t BYTE;

    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // remember filename
    char *infile = argv[1];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    BYTE buffer[FAT_BLOCK_SIZE];
    char filename[FILENAME_SIZE];
    int count = -1;

    // read infile's 512 bytes blocks, byte by byte
    while (fread(&buffer, sizeof(BYTE), FAT_BLOCK_SIZE, inptr) == FAT_BLOCK_SIZE)
    {
        // if it is the start of a new jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // keeping track of the filenames
            count++;
            sprintf(filename, "%03i.jpg", count);

            // open output file
            FILE *img = fopen(filename, "w");
            if (img == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "Could not create %s.\n", filename);
                return 3;
            }

            // write the first 512 bytes block on the output file
            fwrite(&buffer, sizeof(BYTE), FAT_BLOCK_SIZE, img);

            // keep writing the subsequent blocks until a new jpeg is found
            while (fread(&buffer, sizeof(BYTE), FAT_BLOCK_SIZE, inptr) == FAT_BLOCK_SIZE &&
                   !(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0))
            {
                fwrite(&buffer, sizeof(BYTE), FAT_BLOCK_SIZE, img);
            }

            // close the current output file
            fclose(img);

            // go back to the beginning of the block where the new jpeg starts
            fseek(inptr, -(FAT_BLOCK_SIZE), SEEK_CUR);
        }
    }

    // close infile
    fclose(inptr);

    // success
    return 0;
}
