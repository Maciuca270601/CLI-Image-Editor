#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NMAX 1000

typedef struct{
	char tip[3];
	int width;
	int height;
	int intensity;
	long position;
} image_t;

typedef struct{
	int red;
	int green;
	int blue;
} matrix_t;

int is_number(char string[10]);

void swap(int *x, int *y);

matrix_t **alloc_matrix(int h, int w);

void read_line(FILE *file, char *line);

matrix_t **load_plain_grey(image_t image, char filename[NMAX]);

matrix_t **load_plain_rgb(image_t image, char filename[NMAX]);

matrix_t **load_binary_grey(image_t image, char filename[NMAX]);

matrix_t **load_binary_rgb(image_t image, char filename[NMAX]);

int load_image(char filename[NMAX], matrix_t ***mat, image_t *image);

int selectt(int x1, int y1, int x2, int y2, image_t image);

int is_rotate(int x1, int y1, int x2, int y2, int angle, image_t image);

matrix_t **rotate(int x1, int y1, int x2, matrix_t **mat);

matrix_t **rotate_all(matrix_t **mat, image_t image);

matrix_t **crop(int x1, int y1, int x2, int y2, matrix_t **mat, image_t *image);

matrix_t **gray(int x1, int y1, int x2, int y2, matrix_t **mat, image_t image);

matrix_t **sepia(int x1, int y1, int x2, int y2, matrix_t **mat, image_t image);

void save_txt(matrix_t **mat, image_t image, char filename[NMAX]);

void save_bin(matrix_t **mat, image_t image, char filename[NMAX]);

