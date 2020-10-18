// Ruany Maia
// 26 May 2019
// Problem Set 1 - Mario - feeling less comfortable version
// This program asks the user for an integer between 1 and 8 and prints a pyramid with the integer's height

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Asks for user input
    int height;
    do
    {
      height = get_int("Height: ");
    }
    while (height <= 0 || height > 8);

    // This loop prints rows according to the height chosen by the user
    for (int row = 0; row < height; row++)
    {
        // This loop prints the spaces needed in each row to make the pyramid
        for (int space = height - 1; space > row; space--)
        {
            printf(" ");
        }

        // This loop prints the hashes needed in each row to make the pyramid
        for (int hash = 0; hash <= row; hash++)
        {
            printf("#");
        }
        printf("\n");
    }
}
