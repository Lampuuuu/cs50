#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

//defining a BYTE
typedef uint8_t BYTE;

//defining the blocksize of 512
#define blocksize 512


bool JPEGstart(BYTE *buffer);


int main(int argc, char *argv[])
{
    //print error message if there is a wrong input
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    
    //open file on argument 1 and "read" it into the varible "file"
    FILE *file = fopen(argv[1], "r");
    
    //allocate space for buffer
    BYTE *buffer = malloc(blocksize * sizeof(BYTE));
    
    FILE *img = NULL;
    
    int bytesread;
    //find the first jpeg:
    //we need this, since the first JPEG needs 3 loops of this.
    while (!JPEGstart(buffer))
    {
        //reading 1 block from file into the buffer
        bytesread = fread(buffer, sizeof(BYTE), blocksize, file);
    }
    
    
    //counter for created JPEGs
    int jpgnum = -1;
    
    //initialize the string filename, which will store the current 7 characte filename "###.jpg" + /0 terminator
    char filename[8];
    
    
    // as long as fread reads 512 bytes.
    while (bytesread == 512)
    {
        if (JPEGstart(buffer))
        {
            jpgnum += 1;
            if (jpgnum == 0)
            {
                sprintf(filename, "%03i.jpg", jpgnum);

                //open new file with name filename
                img = fopen(filename, "w"); 
                
                //write buffer into file
                fwrite(buffer, sizeof(BYTE), blocksize, img);
            }
            
            else
            {
                //close previous image
                fclose(img);
            
                //increase image counter and rename filename.
                sprintf(filename, "%03i.jpg", jpgnum);
            
                //open new file
                img = fopen(filename, "w");
            
                //write buffer into new file
                fwrite(buffer, sizeof(BYTE), blocksize, img);
            }
        }
        // if there is no new JPGstart, write the block into current image
        else
        {
            fwrite(buffer, sizeof(BYTE), blocksize, img);
        }
    
        //read the next block    
        bytesread = fread(buffer, sizeof(BYTE), blocksize, file);    
    }
    

    //free buffermemory
    free(buffer);
    
    //close the infile
    fclose(file);
    
    //close the outfile
    fclose(img);
    
    return 0;
}

//check if the current buffer starts with the JPEG identifier values
bool JPEGstart(BYTE *buffer)
{
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
    {
        return true;
    }
    return false;
}


