/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**              Beiqian Liu
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) {
    FILE *fp = fopen(filename, "r");
    Image *img = (Image *) malloc(sizeof(Image));
    int colorRange;
    char header[2];

    fscanf(fp, "%s %d %d %d\n", header, &img->cols, &img->rows, &colorRange);
    if (header[0] != 'P' || header[1] != '3' || colorRange != 255) {
        exit(-1);
    }

    int pixelNum = img->cols * img->rows;
    Color **pixels = (Color **) malloc(pixelNum * sizeof(Color *));
    if (!pixels) {
        exit(-1);
    }

    for (int i = 0; i < pixelNum; i++) {
        Color *pixel = (Color *) malloc(sizeof(Color *));
        if (!pixel) {
            exit(-1);
        }
        fscanf(fp, "%hhu %hhu %hhu\n", &pixel->R, &pixel->G, &pixel->B);
        *(pixels + i) = pixel;
    }

    img->image = pixels;

    fclose(fp);
    return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image) {
    printf("P3\n%d %d\n255\n", image->cols, image->rows);
    Color **pixels = image->image;
    for (int r = 0; r < image->rows; r++) {
        for (int c = 0; c < image->cols - 1; c++) {
            printf("%3d %3d %3d   ", (*pixels)->R, (*pixels)->G, (*pixels)->B);
            pixels++;
        }
        printf("%3d %3d %3d\n", (*pixels)->R, (*pixels)->G, (*pixels)->B);
        pixels++;
    }
}

//Frees an image
void freeImage(Image *image) {
    int pixelNum = image->cols * image->rows;
    for (int i = 0; i < pixelNum; i++) {
        free(*(image->image + i));
    }
    free(image->image);
    free(image);
}