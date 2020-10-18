// Ruany Maia
// 01 Jul 2019
// Problem Set 2 - Crack
// This program cracks passwords

#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>
#include <ctype.h>

#define ALL_LETTERS_LENGTH 52
#define MAX_PASSWORD_LENGTH 5

// declare functions
int verifyOverflow(int c);
void increment(int n[], int size);
int myPow(int operand, int operator);

int main(int argc, string argv[])
{
    // Check if the number of arguments given is correct
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }

    // initialize hash given by the user
    string hash_input = argv[1];

    // extract salt from hash input and initialize it - https://stackoverflow.com/a/41948240
    char s[3] = {hash_input[0], hash_input[1], '\0'};
    string salt = s;

    // initialize characters allowed on password
    char all_letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    // initialize array from minimum to maximum length (1 to 5)
    for (int length = 1; length <= MAX_PASSWORD_LENGTH; length++)
    {
        int array[length];
        // fill array cells with 0
        for (int idx = 0; idx < length; idx++)
        {
            array[idx] = 0;
        }

        // iterate through the number of all possible characters combinations
        for (int n = 0; n < myPow(ALL_LETTERS_LENGTH, length); n++)
        {
            // initialize password variable
            char password[length + 1];
            // iterate through password elements
            for (int idx = 0; idx < length; idx++)
            {
                // assign password elements to their proper values
                password[idx] = all_letters[array[idx]];
            }
            // assign the last password element as the end of the string
            password[length] = '\0';

            // generate a hash for each generated password
            string hash_password = crypt(password, salt);

            // compare the generated hash to the one given by the user
            if (strcmp(hash_password, hash_input) != 0)
            {
                // increment password if different
                increment(array, length);
            }
            else
            {
                // print password and finish program if equal
                printf("%s\n", password);
                return 0;
            }
        }
    }
}

// verify the characters overflow
int verifyOverflow(int c)
{
    return c == ALL_LETTERS_LENGTH;
}

// increment character in case of overflow
void increment(int n[], int size)
{
    int lastIdx = size - 1;

    for (int i = lastIdx; i >= 0; i--)
    {
        if (verifyOverflow(++n[i]))
        {
            n[i] = 0;
        }
        else
        {
            break;
        }
    }
}

// generate the number of all possible combinations - power function
int myPow(int operand, int operator)
{
    int result = 1;

    for (int i = 0; i < operator; i++)
    {
        result *= operand;
    }
    return result;
}
