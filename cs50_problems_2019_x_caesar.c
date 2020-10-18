// Ruany Maia
// 21 Jun 2019
// Problem Set 2 - Caesar
// This program encrypts a message shifting its content by a number given by the user

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // Check if the number of arguments given is correct
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    // Iterate through the given key characters and check they are all digits
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!isdigit (argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    // Convert the key from string to integer and initialize the variable key
    int key = atoi(argv[1]);

    // Get plaintext from user
    string plaintext = get_string("plaintext: ");
    string ciphertext = plaintext;

    // Iterate through plaintext characters and cipher it
    for (int i = 0; i < strlen(plaintext); i++)
    {
        // Initialize the variable c and assign it to the current plaintext character
        int c = plaintext[i];

        // Check if the current character is alphabetic and cipher it
        if (isalpha(c))
        {
            int remainder = key % 26;
            int wraparound_check = c + remainder;
            // Check if c is an uppercase character and if it is necessary to wraparound the alphabet
            if (isupper(c))
            {
                if (wraparound_check <= 'Z')
                {
                    ciphertext[i] = wraparound_check;
                }
                else
                {
                    int new_key = wraparound_check - 'Z';
                    ciphertext[i] = ('A' - 1) + new_key;
                }

            }
            // Check if c is an uppercase character and if it is necessary to wraparound the alphabet
            if (islower(c))
            {
                 if (wraparound_check <= 'z')
                {
                    ciphertext[i] = wraparound_check;
                }
                else
                {
                    int new_key = wraparound_check - 'z';
                    ciphertext[i] = ('a' - 1) + new_key;
                }
            }
        }
        // If the current character is not part of the alphabet, keep it the same
        else
        {
            ciphertext[i] = plaintext[i];
            printf("cipher = plain\n");
        }
    }
    // Print ciphertext
    printf("ciphertext: ");
    printf("%s\n", ciphertext);
    return 0;
}
