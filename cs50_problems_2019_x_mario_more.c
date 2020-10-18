// Ruany Maia
// 26 May 2019
// Problem Set 1 - Credit
// This program asks the user for a credit card number and checks if it's a valid American Express, Mastercard or Visa number

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

        // Prints the two spaces span between the pyramids
        printf(" ");
        printf(" ");

        // This loop mirrors the pyramid
        for (int hash = 0; hash <= row; hash++)
        {
            printf("#");
        }
        printf("\n");
    }
}
