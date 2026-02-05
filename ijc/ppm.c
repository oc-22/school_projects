// ppm.c
   // Řešení IJC-DU1, příklad b), 24.3.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 14.2.1-7
   // Program na čtení ze souboru
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include "ppm.h"


struct ppm * ppm_read(const char * filename){

    FILE *file = fopen(filename, "rb");
    if (file==NULL) {
        warning("Opening file unsucessful");
        return NULL;
    }
    char type[3];
    int color_value;
    unsigned x, y = 0;

    if (fscanf(file, "%2s",type)!=1 || strcmp(type,"P6")!=0) {
        warning("Invalid ppm file type(P6 required)");
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "%u %u", &x,&y)!=2 || x<=0 || y<=0) {
        warning("Invalid image size");
        fclose(file);
        return NULL;
    }
    if (fscanf(file, "%d", &color_value)!=1 || (color_value>255 || color_value<=0)) {
        warning("Max color value is incorrect (expected 0...255)");
        fclose(file);
        return NULL;
    }

    fgetc(file);

    struct ppm* image_data = malloc(sizeof(struct ppm)+x*y*3*sizeof(char));
    if (image_data==NULL) {
        warning("Memory allocation failed");
        fclose(file);
        return NULL;
    }
    image_data->xsize = x;
    image_data->ysize = y;


    if (fread(image_data->data, sizeof(char), x*y*3, file)!=x*y*3) {
        warning("File read error");
        fclose(file);
        return NULL;
    }
    if (fgetc(file)!=EOF) {
        warning("EOF not found");
        fclose(file);
        return NULL;
    }

    fclose(file);
    return image_data;

}

void ppm_free(struct ppm *p) {
    free(p);
}
