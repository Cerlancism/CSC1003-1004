#include <stdlib.h> // malloc
#include <stdio.h>  // printf scanf fgets
#include <string.h> // strlen

#define MAX_INPUT 256 // Buffer size for input bits in string

// Parse char '0' and '1' into int 0 and 1, else -1 to show error
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

unsigned int calculateCheckBitLength(int bitLength)
{
    unsigned int k = 0;
    unsigned int m = bitLength;
    while (((1 << k) - 1) < (m + k))
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
    printf("\n");
}

int calculateCheckBit(int *checkBitStream, int length, int checkBitNumber)
{
    int output = -1;                 // Placeholder for uninitialised XOR output
    for (int i = 0; i < length; i++) // Stream positioning is zero-based.
    {
        if (!isCheckBitPosition(i))
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
        output[i] = 0; // Init the int array to 0 to ease debugging.
    }
    return output;
}

int *initBitStreamArray(char *inputBuffer)
{
    int length = strlen(inputBuffer);
    int *output = initIntArray(length);
    for (int i = length - 1, j = 0; i > -1; i--, j++) // Reversing the index as least significant bit is from the right.
    {
        int bit = bitCharToInt(inputBuffer[i]);
        if (bit == -1) // Contains an invalid char.
        {
            return NULL;
        }
        output[i] = bit;
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

    for (int i = 0, s = 0 /*original stream counter*/; i < totalLength; i++)
    {
        if (isCheckBitPosition(i))
        {
            output[i] = -1; // Assign a placeholder for now to represent an uncalculated check bit.
        }
        else
        {
            output[i] = stream[s];
            s++; // Increment original stream counter
        }
    }

    for (int i = 0, cb = 0 /*check bit number*/; i < totalLength; i++)
    {
        if (output[i] == -1) // Is placeholder for unassigned check bit
        {
            output[i] = calculateCheckBit(output, totalLength, cb);
            cb++;
        }
    }
    return output;
}

int *getCheckBits(int *checkBitStreamStream, int length, int checkBitLength)
{
    int *output = malloc(sizeof(int) * checkBitLength);

    for (int i = 0, cb = 0; i < length; i++)
    {
        if (isCheckBitPosition(i))
        {
            output[cb] = checkBitStreamStream[i];
            cb++;
        }
    }
    return output;
}

int *calculateSyndrome(int *correctCheckBitStream, int *errorCheckBitStream, int length, int checkBitLength)
{
    int *output = malloc(sizeof(int) * checkBitLength);

    for (int i = 0, s = 0; i < length; i++)
    {
        if (isCheckBitPosition(i))
        {
            output[s] = correctCheckBitStream[i] != errorCheckBitStream[i];
            s++;
        }
    }
    return output;
}

int bitStreamToInt(int *bitStream, int length)
{
    int output = 0;
    for (int i = 0; i < length; i++)
    {
        output += bitStream[i] * (1 << i);
    }
    return output;
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

    // This program supports any reasonable arbitary bit length.
    if (bitLength < 2 || bitLength > MAX_INPUT)
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

    int *correctStreamArray = initBitStreamArray(inputBuffer);

    if (correctStreamArray == NULL) // Input is invalid bit stream.
    {
        return;
    }

    // Prompt for error bit index
    printf("Enter which data bit has error (one-based index): ");

    unsigned short errorBitindex;
    scanf("%d", &errorBitindex);
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

    int *errorStreamArray = initErrorStreamArray(correctStreamArray, bitLength, errorBitindex);

    printf("Data stream with 1-bit error: ");
    printBitStream(errorStreamArray, bitLength);

    int checkBitLength = calculateCheckBitLength(bitLength);
    int checkBitStreamLength = calculateTotalDataCheckBitLength(bitLength);
    int *correctCheckBitStreamArray = initStreamCheckBitArray(correctStreamArray, checkBitStreamLength);
    int *errorCheckBitStreamArray = initStreamCheckBitArray(errorStreamArray, checkBitStreamLength);

    // printf("Correct Full Check Bit Stream: ");
    // printBitStream(correctCheckBitStreamArray, checkBitStreamLength);

    // printf("Errored Full Check Bit Stream: ");
    // printBitStream(errorCheckBitStreamArray, checkBitStreamLength);

    int *correctDataCheckBits = getCheckBits(correctCheckBitStreamArray, checkBitStreamLength, checkBitLength);
    int *errorDataCheckBits = getCheckBits(errorCheckBitStreamArray, checkBitStreamLength, checkBitLength);

    printf("Check bits of the correct data stream: ");
    printBitStream(correctDataCheckBits, checkBitLength);

    printf("Check bits of the errored data stream: ");
    printBitStream(errorDataCheckBits, checkBitLength);

    int *syndrome = calculateSyndrome(correctCheckBitStreamArray, errorCheckBitStreamArray, checkBitStreamLength, checkBitLength);

    printf("Syndrome Word: ");
    printBitStream(syndrome, checkBitLength);

    printf("Which position in table gets error: %d", bitStreamToInt(syndrome, checkBitLength));

    printf("\n==== Completed Single Error Correction Hamming ====\n");

    free(correctStreamArray);
    free(errorStreamArray);
    free(correctDataCheckBits);
    free(errorDataCheckBits);
    free(correctCheckBitStreamArray);
    free(errorCheckBitStreamArray);
    free(syndrome);
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