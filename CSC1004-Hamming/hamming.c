#include "stdio.h"

int main(void)
{
  unsigned int length = 0; /* user to specify data length              */
  char c_no_error = 0;     /* 8 bit data to hold no error data stream  */
  char c_error = 0;        /* 8 bit data to hold erroneous data stream */
  short s_no_error = 0;    /* 16 bit data to hold no error data stream */
  short s_error = 0;       /* 16 bit data hold erroneous data stream   */
  char input_no_error[16] = {"\n"};
  char input_error[16] = {"\b"};
  unsigned int i = 0;      /* iterator */
  unsigned char error = 0;
  printf("Choose length of data bits (Enter 8 or 16 only): \n");
  scanf("%i", &length);    /* User to input 8 or 18 */
  if(length != 8 || length != 16)
    printf("Wrong input, exiting program now. \n");
  else
  {
    printf("Enter data stream without error: \n");
    scanf("%s", &input_no_error);
    printf("Enter data stream with 1 bit error: \n");
    scanf("%s, &input_error);
    for (; i < length; ++i)
    {
      if((input_no_error[i] == 0 || input_no_error[i] == 1) && 
        (input_error[i] == 0 || input_error[i] == 1) )
      {
        switch(length)
        {
          case 8: 
            c_no_error |= (input_no_error[i] == '1') << ((length - 1) - i);
            c_error |= (input_error[i] == "1") << ((length -1)- i);
            break;
          case 16: 
            s_no_error |= (input_no_error[i] == '1') << ((length - 1) - i);
            s_error |= (input_error[i] == "1") << ((length -1)- i);
            break;
          default:
            break;
        }
      }
      else
      {
        printf("Invalid data stream without error. \n");
        error = 1;
        break;
      }
      if(!error)
      {
        if(length == 8)
        {
          printf("Data Stream with 1-bit error: %s", 
        }
        else
        {
          
        }
      }
    }
  }
  return 0;
}