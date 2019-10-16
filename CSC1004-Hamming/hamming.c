#include <stdlib.h> // malloc
#include <stdio.h>  // printf scanf fgets
#include <string.h> // strlen

#define MAX_INPUT 32 // Buffer size for input bits in string

// Parse char '0' and '1' into int 0 and 1, else -1 to indicate it as an invalid bit char.
short bitCharToInt(char input)
{
    switch (input)
    {
    case '0':
        return 0;
    case '1':
        return 1;
    default:
        printf("Data stream contains invalid bit char of '%c'! Please enter either 0 or 1. Exiting...\n", input);
        return -1;
    }
}

int isPowerOfTwo(int x)
{
    return (x & (x - 1)) == 0;
}

// Check the position in a stream is a position to hold check bit, it is zero-based.
int isCheckBitPosition(int position)
{
    return isPowerOfTwo(position + 1); // offset 1 because its zero-based.
}

// Purge the occurance of '\n' to string terminator, such as a string from a fgets.
void terminateAtLineBreak(char *input)
{
    int length = strlen(input);
    for (int i = 0; i < length; i++)
    {
        if (input[i] == '\n')
        {
            input[i] = '\0';
            return;
        }
    }
}

unsigned short calculateCheckBitLength(int bitLength)
{
    unsigned short k = 0, m = bitLength;
    while (((1 << k) - 1) < (m + k))
    {
        k++;
    }
    return k;
}

unsigned short calculateTotalDataCheckBitLength(unsigned int bitLength)
{
    return calculateCheckBitLength(bitLength) + bitLength;
}

// Returns -1 if input has invalid char
int parseBits(char *input, int length)
{
    length--;
    int output = 0;
    for (int i = 0; i < length; i++)
    {
        int bit = bitCharToInt(input[i]);
        if (bit == -1)
        {
            return -1;
        }
        else if (bit == 1)
        {
            output = (bit << (length - i)) | output;
        }
    }
    return output;
}

void printIntAsBinary(int input, int length)
{
    do
    {
        printf("%d", ((input >> (length - 1)) & 1));
    } while (--length);
    puts("");
}

void hamming()
{
    printf("========= Single Error Correction Hamming =========\n");

    // Prompt for bit length
    printf("Enter data length (8 or 16): ");

    unsigned short bitLength;
    char inputBuffer[MAX_INPUT + 2] = {'\0'};

    scanf("%i", &bitLength);
    getchar(); // Flush the trailling line break from scanf to prepare for a fgets call later.

    unsigned short totalDataCheckBitLength = calculateTotalDataCheckBitLength(bitLength);

    // This program supports arbitary bit length where the total data and check bits are less than 32 (int buffer - 1).
    if (bitLength < 2 || totalDataCheckBitLength > 31)
    {
        printf("Unsupported bit length! Exiting...\n");
        return;
    }

    if (bitLength != 8 && bitLength != 16)
    {
        printf("Warning: you have entered a bit length of %d, it may work but this is unofficially supported by the program.\n", bitLength);
    }

    // Prompt for correct data stream
    printf("Enter error free data stream length of %d: ", bitLength);
    fgets(inputBuffer, MAX_INPUT + 2, stdin); // Using scanf seams problematic if user enters an extremely long invalid input.
    terminateAtLineBreak(inputBuffer);

    int inputLength = strlen(inputBuffer);

    if (inputLength != bitLength)
    {
        printf("Entered data stream does not match bit length of %d! Exiting...\n", bitLength);
        return;
    }

    int correctData = parseBits(inputBuffer, bitLength);

    if (correctData == -1)
    {
        return;
    }

    // Prompt for error bit index
    printf("Enter which data bit has error (one-based index): ");

    unsigned short errorBitindex;
    scanf("%i", &errorBitindex);
    getchar();

    if (errorBitindex > bitLength)
    {
        printf("Error bit index exceeds data stream length of %d! Exiting...\n", bitLength);
        return;
    }

    if (errorBitindex == 0)
    {
        printf("Error position is one-based index which cannot be 0! Exiting...\n");
        return;
    }

    int errorData = correctData ^ (1 << (errorBitindex - 1));

    printf("Data with one bit error at position %d: ", errorBitindex);
    printIntAsBinary(errorData, bitLength);

    printf("\n==== Completed Single Error Correction Hamming ====\n");
}

int main()
{
    char rerun[1];
    do
    {
        hamming();
        printf("\n\nDo you want to re-run this program? (Y/N) ");
        scanf("%s", &rerun);
        printf("\n");
    } while (rerun[0] == 'Y' || rerun[0] == 'y');

    return 0;
}