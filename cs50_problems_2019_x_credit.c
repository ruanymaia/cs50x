// Ruany Maia
// 29 May 2019
// Problem set 1 - Credit

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Asks for user input
    long card_number = get_long("Number: ");

    // Declaration of variables
    long current_digit;
    long digits_left = card_number;
    long second_digit;
    int count = 0;
    int checksum = 0;
    int product;

    // This loop iterates through the number given by the user and extracts some informations
    while (digits_left > 0)
    {
        second_digit = current_digit;
        current_digit = digits_left % 10;
        digits_left = digits_left / 10;
        count++;
        // This conditional applies Luhn's Algorithm through the digits
        // Multiplies every other digit by 2, starting with the number’s second-to-last digit
        if (count % 2 == 0)
        {
            product = current_digit * 2;
            int product_digit = product;
            int product_digits_left = product;
            // This loop iterates through the products’ digits and add them together
            while (product_digits_left > 0)
            {
                product_digit = product_digits_left % 10;
                product_digits_left = product_digits_left / 10;
                checksum += product_digit;
            }
        }
        else
        {
            // Sums the digits that were not multiplied by 2
            checksum += current_digit;
        }
    }

    // Checks if the number given by the user can be a valid one
    // The number has to be 13, 15 OR 16 long AND its checksum must be divisible by 10
    if (checksum % 10 == 0)
    {
        // If the requisites are met, checks if it is a valid VISA number
        // The number has to be 13 OR 16 long AND its first digit must be 4
        if ((count == 13 || count == 16)  && current_digit == 4)
        {
            printf("VISA\n");
        }
        // Else, checks if it is a valid AMEX number
        // The number has to be 15 long AND its first digit must be 3 AND the second digit must be 4 OR 7
        else if (count == 15 && current_digit == 3 && (second_digit == 4 || second_digit == 7))
        {
            printf("AMEX\n");
        }
        // Else, checks if it is a valid MASTERCARD number
        // The number has to be 16 long AND its first digit must be 5 AND the second digit must be 1, 2, 3, 4 OR 5
        else if (count == 16 && current_digit == 5 && second_digit >= 1 && second_digit <= 5)
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
