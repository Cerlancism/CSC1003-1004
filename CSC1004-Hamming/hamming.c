#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Parse char '0' and '1' into int, else throw.
unsigned short bitCharToInt(char input)
{
    switch (input)
    {
    case '0':
        return 0;
    case '1':
        return 1;
    default:
        printf("Data stream contains invalid bit char of '%c'! Please enter either 0 or 1. Exiting...", input);
        exit(1);
    }
}

// Check the position in a stream is a position to hold check bit, it is zero-based.
int isCheckBitPosition(int position, int checkBitNumber)
{
    return position == (pow(2, checkBitNumber) - 1); // minus 1 because its zero-based.
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

unsigned int calculateCheckBitLength(unsigned int bitLength)
{
    unsigned int k = 0;
    unsigned int m = bitLength;
    while ((pow(2, k) - 1) < (m + k))
    {
        k++;
    }
    return k;
}

unsigned int calculateTotalDataCheckBitLength(unsigned int bitLength)
{
    return calculateCheckBitLength(bitLength) + bitLength;
}

void printBitStream(int *bitStream, int length)
{
    for (int i = length - 1; i > -1; i--) // Reversing the index as least significant bit is from the right.
    {
        printf("%d", bitStream[i]);
    }
}

// int bitSteamXOR(int *bitStream, int length)
// {
//     if (length < 2)
//     {
//         printf("Bit Stream XOR requires at least length of 2!");
//         exit(1);
//     }
//     int output = bitStream[0];
//     for (int i = 1; i < length; i++)
//     {
//         output = output != bitStream[i];
//     }
//     return output;
// }

int calculateCheckBit(int *checkBitStream, int length, int checkBitNumber)
{
    int output = -1;                                                              // Placeholder for uninitialised XOR output
    for (int i = 0, cb = 0 /* check bit number is zero based */; i < length; i++) // Stream positioning is zero-based.
    {
        if (isCheckBitPosition(i, cb))
        {
            cb++;
        }
        else
        {
            if (((i + 1) >> checkBitNumber) % 2 == 1) // Is a data bit for the check bit number.
            {
                if (output == -1) // For first data bit.
                {
                    output = checkBitStream[i]; // Init the first data bit for XOR.
                }
                else
                {
                    output = output != checkBitStream[i]; // Bit flip for XOR.
                }
            }
        }
    }
    return output;
}

int *initIntArray(unsigned int length)
{
    int *output = malloc(sizeof(int) * length);
    for (int i = 0; i < length; i++)
    {
        output[i] = 0; // Init the int array to 0
    }
    return output;
}

int *initBitStreamArray(char *inputBuffer)
{
    int length = strlen(inputBuffer);
    int *output = initIntArray(length);
    for (int i = length - 1, j = 0; i > -1; i--, j++) // Reversing the index as least significant bit is from the right.
    {
        output[j] = bitCharToInt(inputBuffer[i]);
    }
    return output;
}

int *initErrorStreamArray(int *correctStream, int length, int errorIndex) // errorIndex is one-based index
{
    int *output = initIntArray(length);
    for (int i = 0; i < length; i++)
    {
        output[i] = i != (errorIndex - 1) ? correctStream[i] : !correctStream[i];
    }
    return output;
}

int *initStreamCheckBitArray(int *stream, int totalLength)
{
    int *output = initIntArray(totalLength);

    for (int i = 0, cb = 0 /*check bit number */, s = 0 /*original stream counter*/; i < totalLength; i++)
    {
        if (isCheckBitPosition(i, cb))
        {
            output[i] = 2; // Assign placeholder 2 for now to represent an uncalculated check bit.
            cb++;          // Increment check bit number
        }
        else
        {
            output[i] = stream[s];
            s++; // Increment original stream counter
        }
    }

    for (int i = 0, cb = 0 /*check bit number */; i < totalLength; i++)
    {
        if (output[i] == 2) // Is placeholder for unassigned check bit
        {
            output[i] = calculateCheckBit(output, totalLength, cb);
            cb++;
        }
    }
    return output;
}

void hamming()
{
    printf("==== Single Error Correction Hamming ====\n");

    // Prompt for bit length
    printf("Enter data length (8 or 16): ");

    unsigned short bitLength;
    char inputBuffer[18] = {'\0'};

    scanf("%i", &bitLength);
    getchar(); // Flush the trailling line break from scanf to prepare for a fgets call later.

    if (bitLength != 8 && bitLength != 16)
    {
        printf("Unsupport bit length! exiting...\n");
        exit(1);
    }

    // Prompt for correct data stream
    printf("Enter error free data stream length of %d: ", bitLength);
    fgets(inputBuffer, 18, stdin); // Using scanf seams problematic if user enters an extremely long invalid input.
    terminateAtLineBreak(inputBuffer);

    int inputLength = strlen(inputBuffer);

    if (inputLength != bitLength)
    {
        printf("Entered data stream does not match bit length of %d, exiting...\n", bitLength);
        exit(1);
    }

    int *correctStreamArray = initBitStreamArray(inputBuffer);

    // Prompt for error bit index
    printf("Enter which data bit has error (one-based index): ");

    unsigned short errorBitindex;
    scanf("%d", &errorBitindex);

    if (errorBitindex > bitLength)
    {
        printf("Error bit index exceeds data stream length of %d, exiting...", bitLength);
        exit(1);
    }

    int *errorStreamArray = initErrorStreamArray(correctStreamArray, bitLength, errorBitindex);

    printf("Data stream with 1-bit error: ");
    printBitStream(errorStreamArray, bitLength);
    printf("\n");

    int checkBitStreamLength = calculateTotalDataCheckBitLength(bitLength);
    int *correctCheckBitStreamArray = initStreamCheckBitArray(correctStreamArray, checkBitStreamLength);
    int *errorCheckBitStreamArray = initStreamCheckBitArray(errorStreamArray, checkBitStreamLength);

    printf("Correct Check Bit Stream: ");
    printBitStream(correctCheckBitStreamArray, checkBitStreamLength);
    printf("\n");

    printf("Errored Check Bit Stream: ");
    printBitStream(errorCheckBitStreamArray, checkBitStreamLength);
    printf("\n");
}

int main()
{
    hamming();

    return 0;
}