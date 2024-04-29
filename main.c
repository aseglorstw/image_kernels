#define _POSIX_C_SOURCE 202001L

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>


#define MAX_COLOR_VALUE 255

int main(int argc, char **argv) {
    
    FILE *file_input = fopen(argv[1], "rb");

    char header[3];
    int width, height, maxval;
    fscanf(file_input, "%s\n%d%d\n%d\n", header, &width, &height, &maxval);
    
    unsigned char *buffer_1 = (unsigned char*)malloc(width * sizeof(unsigned char) * 3);
    fread(buffer_1, sizeof(unsigned char), width * 3, file_input);
    
    unsigned char *buffer_2 = (unsigned char*)malloc(width * sizeof(unsigned char) * 3);
    fread(buffer_2, sizeof(unsigned char), width * 3, file_input);
    
    unsigned char *buffer_3 = (unsigned char*)malloc(width * sizeof(unsigned char) * 3);

    unsigned char *ptr = NULL;

    register short int result = 0;

    int r = 2126;
    int g = 7152;
    int b = 722;
    
    register int histogram = 0;
    int histograms[5] = {0};
    
    for(int x = 0; x < width; x++) {
        
        histogram =  buffer_1[x * 3 + 0] * r + buffer_1[x * 3 + 1] * g + buffer_1[x * 3 + 2] * b;
        histogram = histogram / 10000 + (histogram % 10000 > 4999);
        if(histogram != 255)
            histograms[histogram/51]++;
        else 
            histograms[4]++;
    }

    FILE *file_output = fopen("output.ppm", "wb");
    fprintf(file_output, "P6\n%d\n%d\n%d\n", width, height, MAX_COLOR_VALUE);
    fwrite(buffer_1, sizeof(unsigned char), width * 3, file_output);

    for(int i = 1; i < height - 1; i++) {
        
        fread(buffer_3, sizeof(unsigned char), width * 3 , file_input);
        
        buffer_1[0] = buffer_2[0];
        buffer_1[1] = buffer_2[1];
        buffer_1[2] = buffer_2[2];

        buffer_1[(width - 1 ) * 3] = buffer_2[(width - 1 ) * 3];
        buffer_1[(width - 1 ) * 3 + 1] = buffer_2[(width - 1 ) * 3 + 1];
        buffer_1[(width - 1 ) * 3 + 2] = buffer_2[(width - 1 ) * 3 + 2];

        histogram =  buffer_1[0] * r + buffer_1[1] * g + buffer_1[2] * b;
        histogram = histogram / 10000 + (histogram % 10000 > 4999);
        
        if(histogram != 255)
            histograms[histogram/51]++;
        else 
            histograms[4]++;

        histogram =  buffer_1[(width - 1) * 3 ] * r + buffer_1[(width -1) * 3 + 1] * g + buffer_1[(width - 1) * 3 + 2] * b;
        histogram = histogram / 10000 + (histogram % 10000 > 4999);
        
        if(histogram != 255)
            histograms[histogram/51]++;
        else 
            histograms[4]++;
    
        
        for(int x = 1; x < width - 1; x++) {

            result = 5 * buffer_2[x * 3] - buffer_1[x * 3] - buffer_3[x * 3] - buffer_2[(x - 1) * 3] - buffer_2[(x + 1) * 3];
            if(result < 0)
                result = 0;
            else if(result > 255)
                result = 255;
            buffer_1[x * 3] = result;

            result = 5 * buffer_2[x * 3 + 1] - buffer_1[x * 3 + 1] - buffer_3[x * 3 + 1] - buffer_2[(x - 1) * 3 + 1] - buffer_2[(x + 1) * 3 + 1];
            if(result < 0)
                result = 0;
            else if(result > 255)
                result = 255;
            buffer_1[x * 3 + 1] = result;

            result = 5 * buffer_2[x * 3 + 2] - buffer_1[x * 3 + 2] - buffer_3[x * 3 + 2] - buffer_2[(x - 1) * 3 + 2] - buffer_2[(x + 1) * 3 + 2];
            if(result < 0)
                result = 0;
            else if(result > 255)
                result = 255;
            buffer_1[x * 3 + 2] = result;

            histogram =  buffer_1[x * 3 + 0] * r + buffer_1[x * 3 + 1] * g + buffer_1[x * 3 + 2] * b;
            histogram = histogram / 10000 + (histogram % 10000 > 4999);
            
            if(histogram != 255)
                histograms[histogram/51]++;
            else 
                histograms[4]++;
            
        }
        
        fwrite(buffer_1, sizeof(unsigned char), width * 3, file_output);
        
        ptr = buffer_1;
        
        buffer_1 = buffer_2;

        buffer_2 = buffer_3;

        buffer_3 = ptr;
    }
    
    fwrite(buffer_2, sizeof(unsigned char), width * 3, file_output);
    
    for(int x = 0; x < width; x++) {
        
        histogram =  buffer_2[x * 3 + 0] * r + buffer_2[x * 3 + 1] * g + buffer_2[x * 3 + 2] * b;
        histogram = histogram / 10000 + (histogram % 10000 > 4999);
        
        if(histogram != 255)
            histograms[histogram/51]++;
        else 
            histograms[4]++;
    
    }

    FILE *file_txt = fopen("output.txt", "w");
    fprintf(file_txt, "%d %d %d %d %d", histograms[0], histograms[1], histograms[2], histograms[3], histograms[4]);
    
    free(buffer_1);
    free(buffer_2);
    free(buffer_3);
    
    fclose(file_txt);
    fclose(file_input);
    fclose(file_output);
    
    return 0;
}

