/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**              Beiqian Liu
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule) {
    int rows = image->rows, cols = image->cols;
    Color **currPix = image->image + row * cols + col;
    Color *newPix = (Color *) malloc(sizeof(Color));
    if (!newPix) exit(-1);

    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int aliveNeighborsR = 0, aliveNeighborsG = 0, aliveNeighborsB = 0;
    for (int i = 0; i < 8; i++) {
        int neighborRow = (row + dx[i] + rows) % rows;
        int neighborCol = (col + dy[i] + cols) % cols;
        Color **neighbor = image->image + neighborRow * cols + neighborCol;
        if ((*neighbor)->R == 255) aliveNeighborsR++;
        if ((*neighbor)->G == 255) aliveNeighborsG++;
        if ((*neighbor)->B == 255) aliveNeighborsB++;
    }

    int maskR = ((*currPix)->R == 255) * 9 + aliveNeighborsR;
    int maskG = ((*currPix)->G == 255) * 9 + aliveNeighborsG;
    int maskB = ((*currPix)->B == 255) * 9 + aliveNeighborsB;
    newPix->R = (rule & (1 << maskR)) ? 255 : 0;
    newPix->G = (rule & (1 << maskG)) ? 255 : 0;
    newPix->B = (rule & (1 << maskB)) ? 255 : 0;

    return newPix;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule) {
    Image *newImage = (Image *) malloc(sizeof(Image));
    if (!newImage) exit(-1);
    newImage->rows = image->rows;
    newImage->cols = image->cols;

    newImage->image = (Color **) malloc(image->cols * image->rows * sizeof(Color *));
    Color **newPixels = newImage->image;
    if (!newPixels) exit(-1);
    for (int i = 0; i < image->rows; i++) {
        for (int j = 0; j < image->cols; j++) {
            *newPixels = evaluateOneCell(image, i, j, rule);
            newPixels++;
        }
    }
    return newImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv) {
    if (argc != 3) {
        printf("usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with maximum value 255.\nrule is a hex number beginning with 0x; Life is 0x1808.\n");
        exit(-1);
    }
    Image *img = readData(argv[1]);
    uint32_t rule = strtol(argv[2], NULL, 16);
    Image *newImg = life(img, rule);
    writeData(newImg);
    freeImage(img);
    freeImage(newImg);
    return 0;
}
