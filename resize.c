/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * resizes a BMP piece by piece by n
 */
 
/*
//to look at bmp info/header
./resize 3 small.bmp large.bmp
~cs50/pset4/resize 3 small.bmp staff.bmp
~cs50/pset4/peek small.bmp large.bmp
diff large.bmp staff.bmp
check50 2015.fall.pset4.resize bmp.h resize.c
*/

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    int n = 0;
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    if(n < 0 || n > 100){
        printf("n must be greater than 0 and less than or equal to 100\n");
        return 1;
    }
    
    // remember filenames
    n = atoi(argv[1]);
    
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
   
    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    //save original image height, width and padding
    int oldheight = bi.biHeight;
    int oldwidth = bi.biWidth;
    int oldpadding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
  
    //new image height, width, padding
    bi.biHeight *= n;
    bi.biWidth *= n;
    int newpadding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // edit outfile's BITMAPFILEHEADER to account for resize
    //if resize is not == 1
    if(n!=1){
        bi.biSizeImage = (bi.biWidth*sizeof(RGBTRIPLE)+newpadding)*abs(bi.biHeight);
        bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;
        fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    }
    //if resize is == 1
    else{
        fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    }
    
    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    //create buffer using malloc to be able to store entire new scanline
    RGBTRIPLE *buffer = malloc(sizeof(RGBTRIPLE) * bi.biWidth);

    //iterate over infile's scanlines - oldheight
    for (int i = 0; i < abs(oldheight); i++)
    {
        int element = 0;

        //iterate over each pixel within each scanline - old width
        for (int j = 0; j < oldwidth; j++)
        {
            // temporary storage for reading
            RGBTRIPLE triple;

            //read RGB triple from infile and store in buffer
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            //iterate over each pixel n times
            for (int k = 0; k < n; k++)
            {
                buffer[element] = triple;
                element++;
            }
        }

        // skip over any input padding
        fseek(inptr, oldpadding, SEEK_CUR);

        // print each row from buffer n times
        for (int r = 0; r < n; r++)
        {
            // write RGB triple to outfile
            fwrite(buffer, sizeof(RGBTRIPLE), bi.biWidth, outptr);

            // write padding to outfile
            for (int p = 0; p < newpadding; p++)
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    free(buffer);
    // that's all folks
    return 0;
}
