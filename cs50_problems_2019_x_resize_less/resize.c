// Ruany Maia
// 25 August 2019
// Problem Set 3 - Resize (less comfortable)
// This program resizes BMPs

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // ensure the given resize factor is a positive integer and assigns it to n variable
    // iterate through the given resize factor chars and check they are all digits
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("n, the resize factor, must be a positive integer.\n");
            return 1;
        }
    }

    // convert the resize factor from string to integer
    int n = atoi(argv[1]);

    // check if the resize factor satisfy 0 < n <= 100
    if (n <= 0 || n > 100)
    {
        printf("n, the resize factor, must satisfy 0 < n <= 100.\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf, bfOut;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    bfOut = bf;

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, biOut;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    biOut = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }

    // determine resized biWidth, biHeight and padding
    biOut.biWidth *= n;
    biOut.biHeight *= n;
    int paddingOut = (4 - (biOut.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // determine resized biSizeImage
    biOut.biSizeImage = ((sizeof(RGBTRIPLE) * biOut.biWidth) + paddingOut) * abs(biOut.biHeight);

    // determine resized bfSize
    bfOut.bfSize = biOut.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfOut, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&biOut, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // loop for vertical resizing
        for (int vertical_resize = 0; vertical_resize < n; vertical_resize++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // loop for horizontal resizing
                for (int k = 0; k < n; k++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // skip over padding from infile, if any
            fseek(inptr, padding, SEEK_CUR);

            // then add padding to the outfile
            for (int l = 0; l < paddingOut; l++)
            {
                fputc(0x00, outptr);
            }

            // check if vertical resizing is needed
            if (vertical_resize < n - 1)
            {
                fseek(inptr, -(sizeof(RGBTRIPLE) * bi.biWidth + padding), SEEK_CUR);
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
