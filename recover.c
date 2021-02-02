#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef uint8_t BYTE;


#define buffersize 512

int main(int argc, char *argv[])
{
    int  files_count = 0;
    BYTE buffer[buffersize];
    char filename[8];
    FILE *output = NULL;

    // check command line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover card.raw\n");
        return 1;
    }

    // Open card file for reading
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    //start cycle and check that fread = 1 to make sure there are 512 bytes to read
    while (fread(buffer, sizeof(BYTE), buffersize, card) == buffersize)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // plus 1 to file headers found
            files_count++;

            // if file has already been open
            if (output != NULL)
            {
                fclose(output);
            }

            //create new file name
            sprintf(filename, "%03i.jpg", files_count - 1);

            // create new file to write to
            output = fopen(filename, "w");

            if (output == NULL)
            {
                printf("Could not open file. %s\n", filename);
                return 1;
            }
        }


        if (files_count > 0)
        {
            //write buffer to the existing file
            fwrite(buffer, sizeof(BYTE), buffersize, output);
        }
    }

    if (output != NULL)
    {
        fclose(output);

    }

    if (card != NULL)
    {
        fclose(card);
    }

    return 0;
}
