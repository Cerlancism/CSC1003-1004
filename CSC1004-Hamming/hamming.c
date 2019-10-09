#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int bitCharToInt(char input)
{
    switch (input)
    {
    case '0':
        return 0;
    case '1':
        return 1;
    default:
        printf("Data stream invalid bit char! Please enter either 0 or 1. Exiting...");
        exit(0);
    }
}

int isCheckBitPosition(int position, int checkBitNumber)
{
    return position == (pow(2, checkBitNumber) - 1);
}

void removeLineBreak(char *input)
{
    int length = strlen(input);

    for (int i = 0; i < length; i++)
    {
        if (input[i] == '\n')
        {
            input[i] = '\0';
        }
    }
}

int main()
{
    printf("==== Single Error Correction Hamming ====\n");
    printf("Please enter data length (8 or 16): ");

    unsigned short bitLength;
    char inputBuffer[18] = {'\0'};

    scanf("%i", &bitLength);
    getchar();

    if (bitLength != 8 && bitLength != 16)
    {
        printf("Unsupport bit length! exiting...\n");
        exit(0);
    }

    printf("Please enter data stream length of %d: ", bitLength);
    fgets(inputBuffer, 18, stdin);
    removeLineBreak(inputBuffer);

    int inputLength = strlen(inputBuffer);

    if (inputLength != bitLength)
    {
        printf("Entered data stream does not match bit length of %d, exiting...\n", bitLength);
        exit(0);
    }

    int dataArray[21] = {0};

    for (int i = 0, j = 0, currentCheckBit = 0; i < 21; i++)
    {
        if (isCheckBitPosition(i, currentCheckBit))
        {
            currentCheckBit++;
            dataArray[i] = -1;
        }
        else
        {
            dataArray[i] = bitCharToInt(inputBuffer[j]);
            j++;
            if (j >= bitLength)
            {
                break;
            }
        }
    }

    printf("Data Array: ");
    for (int i = 20; i > -1; i--)
    {
        printf("%d,", dataArray[i]);
    }

    return 0;
}