# Questions

## What's `stdint.h`?

The <stdint.h> header shall declare sets of integer types having specified widths, and shall define corresponding sets of macros. It shall also define macros that specify limits of integer types corresponding to types defined in other standard headers.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

The size of data types may vary between different machines, so specifying it this way helps on the portability of the program and may prevent it from crashing. 
Also, the ranges of each of these types are different. You may choose one of them according to your need. This way you can save memory.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

BYTE - 1 byte - uint8_t 
DWORD - 4 bytes - uint32_t
LONG - 4 bytes - int32_t
WORD - 2 bytes - uint16_t

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

ASCII: BM
Hexadecimal: 0x42 0x4D

## What's the difference between `bfSize` and `biSize`?

bfSize is in the BITMAPFILEHEADER structure. The BITMAPFILEHEADER structure contains information about the type, size, and layout of a file that contains a DIB. bfSize contains the size, in bytes, of the bitmap file.

biSize is in the BITMAPINFOHEADER structure. The BITMAPINFOHEADER structure contains information about the dimensions and color format of a DIB. biSize contains the number of bytes required by the structure. 

## What does it mean if `biHeight` is negative?

biHeight is the height of the bitmap, in pixels. If biHeight is positive, the bitmap is a bottom-up DIB and its origin is the lower-left corner. If biHeight is negative, the bitmap is a top-down DIB and its origin is the upper-left corner.

If biHeight is negative, indicating a top-down DIB, biCompression must be either BI_RGB or BI_BITFIELDS. Top-down DIBs cannot be compressed.

If biCompression is BI_JPEG or BI_PNG, the biHeight member specifies the height of the decompressed JPEG or PNG image file, respectively.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount - the number of bits-per-pixel. The biBitCount member of the BITMAPINFOHEADER structure determines the number of bits that define each pixel and the maximum number of colors in the bitmap. This member must be one of the following values: 0, 1, 4, 8, 16 or 32.

## Why might `fopen` return `NULL` in `copy.c`?

When dealing with I/O operations your code simply has to expect failure. It's not possible to predict the success of I/O operations, and they can always fail. Some possible reasons for failing are: the file doesn't exist; the file is opened in a mode that doesn't allow other accesses; the network is down; the file exists, but you don't have permissions; a file exists with the name you gave, but the current directory of the process is not what you expected so the relative pathname fails to find and open the file.

## Why is the third argument to `fread` always `1` in our code?

Because we want to read just 1 unit of the given data type size.

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

3

## What does `fseek` do?

fseek() is used to move file pointer associated with a given file to a specific position.

## What is `SEEK_CUR`?

Current position of the file pointer.
