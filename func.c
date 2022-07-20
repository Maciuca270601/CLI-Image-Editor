#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "func.h"

#define NMAX 1000

/*
is_number primeste un string parametru si verifica daca acesta este numar
sau poate contine si litere -> o folosesc la verificarea parametrilor
functii SELECT.
*/
int is_number(char string[10])
{
	int n = strlen(string);
	for (int i = 0; i < n; i++)
		if (string[i] >= 'a' && string[i] <= 'z')
			return 0;
	return 1;
}

void swap(int *x, int *y)
{
	int aux = *x;
	*x = *y;
	*y = aux;
}

/*Functie pt alocarea matricii care retine matricea imaginii */
matrix_t **alloc_matrix(int h, int w)
{
	matrix_t **mat = (matrix_t **)malloc(h * sizeof(matrix_t *));
	if (!mat)
		printf("Nu s-a putut aloca memoria");

	for (int i = 0; i < h; i++) {
		mat[i] = (matrix_t *)malloc(w * sizeof(matrix_t));
		if (!mat[i]) {
			while (--i != 0)
				free(mat[i]);
			free(mat);
		}
	}

	return mat;
}

/*
read_line primeste fisierul din care citeste date si returneaza cate o linie
din fisier. Scopul este sa sara peste liniile inutile care contin comentarii,
identificate prin '#'
*/
void read_line(FILE *file, char *line)
{
	do {
		fgets(line, 32, file);
		line[strlen(line) - 1] = '\0';
	} while (line[0] == '#');
}

/*
LOAD FUNCTIONS
Urmatoarele 5 functii se ocupa de incarcarea fotografiei in program.

load_image deschide orice fisier primit in format text cu scopul de
a identifica tipul de poza cu care se lucreaza, iar apoi il inchide.

Daca poza este de tipul P5, respectiv P6 vom redeschide fisierul in format
binar pentru a putea citi datele matricii. Pentru pozele de tipul P2, P3 acest
pas este inutil, dar de dragul simetriei am preferat sa redeschid fisierul in
in format text. Trebuie sa am grija ca inainte sa inchid fisierul sa tin minte
unde am ramas cu citirea lucru realizat cu ajutorul functiei ftell si fseek.
Valuarea lui ftell este un int salvat in structura image.position.

Dupa ce s-a ales tipul de imagine se verifica pe ce if sa intre, se copiaza
restul datelor din header in image.width, image.height, image.intensity,
image.position. Se acceseaza functia specifica tipului de imagine si se inchide
fisierul, urmand sa ma intorc in main cu return.

Daca fisierul nu exista se afiseaza mesajul "Failed to load filename"
*/
int load_image(char filename[NMAX], matrix_t ***mat, image_t *image)
{
	FILE *file = fopen(filename, "r");
	char line[32];
	if (!file) {
		printf("Failed to load %s\n", filename);
		return 0;
	}
	read_line(file, line);
	char *token = strtok(line, " ");
	strcpy(image->tip, token);
	read_line(file, line);
	token = strtok(line, " ");

	//check if plain - greyscale
	if (strcmp(image->tip, "P2") == 0) {
		image->width = atoi(token);
		token = strtok(NULL, " ");
		image->height = atoi(token);
		read_line(file, line);
		token = strtok(line, " ");
		image->intensity = atoi(token);
		image->position = ftell(file);
		*mat = load_plain_grey(*image, filename);
		fclose(file);
		return 1;
	}

	//check if plain - rgb
	if (strcmp(image->tip, "P3") == 0) {
		image->width = atoi(token);
		token = strtok(NULL, " ");
		image->height = atoi(token);
		read_line(file, line);
		token = strtok(line, " ");
		image->intensity = atoi(token);
		image->position = ftell(file);
		*mat = load_plain_rgb(*image, filename);
		fclose(file);
		return 1;
	}

	//check if binary - greyscale
	if (strcmp(image->tip, "P5") == 0) {
		image->width = atoi(token);
		token = strtok(NULL, " ");
		image->height = atoi(token);
		read_line(file, line);
		token = strtok(line, " ");
		image->intensity = atoi(token);
		image->position = ftell(file);
		*mat = load_binary_grey(*image, filename);
		fclose(file);
		return 1;
	}

	//check if binary - rgb
	if (strcmp(image->tip, "P6") == 0) {
		image->width = atoi(token);
		token = strtok(NULL, " ");
		image->height = atoi(token);
		read_line(file, line);
		token = strtok(line, " ");
		image->intensity = atoi(token);
		image->position = ftell(file);
		*mat = load_binary_rgb(*image, filename);
		fclose(file);
		return 1;
	}

	fclose(file);
	return 0;
}

/*
load_plain_grey deschide fisierul in format text, pozitioneaza cursorul
unde este nevoie si aloca o matrice de tipul matrix_t , o structura in
care retin 3 valori int : red, green, blue.
Din simplitate am preferat sa tratez imaginile greyscale ca avand
matrix.red = matrix.green = matrix.blue si voi folosi doar campul red
in exercitiile cu acest tip de imagine(P2, P5)
*/
matrix_t **load_plain_grey(image_t image, char filename[NMAX])
{
	FILE *file = fopen(filename, "r");
	fseek(file, image.position, SEEK_SET);

	matrix_t **mat = alloc_matrix(image.height, image.width);

	for (int i = 0; i < image.height; i++) {
		for (int j = 0; j < image.width; j++) {
			fscanf(file, "%d", &mat[i][j].red);
			mat[i][j].green = mat[i][j].red;
			mat[i][j].blue = mat[i][j].red;
		}
	}

	fclose(file);
	printf("Loaded %s\n", filename);

	return mat;
}

/*
load_plain_rgb functioneaza similar cu load_plain_grey doar ca acesta
se va folosi cu adevarat de campurile matrix.green, matrix.blue
*/
matrix_t **load_plain_rgb(image_t image, char filename[NMAX])
{
	FILE *file = fopen(filename, "r");
	fseek(file, image.position, SEEK_SET);

	matrix_t **mat = alloc_matrix(image.height, image.width);

	for (int i = 0; i < image.height; i++) {
		for (int j = 0; j < image.width; j++) {
			fscanf(file, "%d", &mat[i][j].red);
			fscanf(file, "%d", &mat[i][j].green);
			fscanf(file, "%d", &mat[i][j].blue);
		}
	}
	fclose(file);
	printf("Loaded %s\n", filename);

	return mat;
}

/*
load_binary_grey functioneaza similar cu load_plain_grey doar ca deschide
fisierul in mod binar si citeste intr-o variabila auxiliara de tip
unsigned char datele din matrice, ca mai apoi sa fac o conversie la int
pentru a le putea stoca
*/
matrix_t **load_binary_grey(image_t image, char filename[NMAX])
{
	FILE *file = fopen(filename, "rb");
	fseek(file, image.position, SEEK_SET);
	matrix_t **mat = alloc_matrix(image.height, image.width);
	unsigned char aux;

	for (int i = 0; i < image.height; i++) {
		for (int j = 0; j < image.width; j++) {
			fread(&aux, sizeof(unsigned char), 1, file);
			mat[i][j].red = (int)aux;
			mat[i][j].blue = mat[i][j].red;
			mat[i][j].green = mat[i][j].red;
		}
	}

	fclose(file);
	printf("Loaded %s\n", filename);

	return mat;
}

/*
load_binary_rgb functioneaza similar cu load_plain_rgb doar ca deschide
fisierul in mod binar si citestele datele similar cu load_binary_grey,
iar apoi inchide fisierul
*/
matrix_t **load_binary_rgb(image_t image, char filename[NMAX])
{
	FILE *file = fopen(filename, "rb");
	fseek(file, image.position, SEEK_SET);
	matrix_t **mat = alloc_matrix(image.height, image.width);
	unsigned char aux;

	for (int i = 0; i < image.height; i++) {
		for (int j = 0; j < image.width; j++) {
			fread(&aux, sizeof(unsigned char), 1, file);
			mat[i][j].red = (int)aux;

			fread(&aux, sizeof(unsigned char), 1, file);
			mat[i][j].green = (int)aux;

			fread(&aux, sizeof(unsigned char), 1, file);
			mat[i][j].blue = (int)aux;
		}
	}

	fclose(file);
	printf("Loaded %s\n", filename);
	return mat;
}

/*
selectt primeste un set de 4 coordonate si returneaza 1 daca acestea
apartin unei selectii valide, altfel returneaza 0

Deoarece eu scad -1 din x2 si y2 pentru a accesa matricea mea corect
trebuie sa verific in primul rand ca nu exista vreun caz in care
x1 = x2 sau y1 = y2, trebuie verificat deasemeni ca x1 sau y1 sa nu fie
negative( x2, y2 daca sunt negative se va da swap in main si valorile negative
vor ajunge tot la x1 si y1) si mai trebuie verificat ca x2 si y2 sa
nu depaseasca limitele matricii
*/
int selectt(int x1, int y1, int x2, int y2, image_t image)
{
	if ((x1 == x2 + 1) || x1 < 0 || (x2 > image.width - 1))
		return 0;
	if ((y1 == y2 + 1) || y1 < 0 || (y2 > image.height - 1))
		return 0;
	return 1;
}

/*
is_rotate primeste un set de 4 coordonate si un unghi si poate returna
3 tipuri de valori
0 = daca rotatia se poate realiza
1 = the selection must be a square pentru ca selectia nu este pe toata
imaginea sau pe o zona patrata
2 = unghiul este invalid => nu aparetine [-360, 360]
*/
int is_rotate(int x1, int y1, int x2, int y2, int angle, image_t image)
{
	if ((angle >= -360) && angle <= 360 && (angle % 90 == 0)) {
		if ((x2 - x1) == (y2 - y1))
			return 0;
		if ((x2 - x1 == image.width - 1) && (y2 - y1 == image.height - 1))
			return 0;
		return 1;
	}

	return 2;
}

/*
rotate are rolul de a realiza o rotatie in sens trigonometric
echivalenta cu o rotatie cu -90, iar 3 astfel de rotatii vor echivala una cu 90
*/
matrix_t **rotate(int x1, int y1, int x2, matrix_t **mat)
{
	int width = x2 - x1 + 1;
	int newy1 = y1 + width - 1;
	int newx1 = x1 + width - 1;
	for (int i = 0; i < width / 2; i++) {
		for (int j = i; j < width - i - 1; j++) {
			matrix_t aux = mat[y1 + i][x1 + j];

			mat[y1 + i][x1 + j] = mat[y1 + j][newx1 - i];

			mat[y1 + j][newx1 - i] = mat[newy1 - i][newx1 - j];

			mat[newy1 - i][newx1 - j] = mat[newy1 - j][x1 + i];

			mat[y1 + width - 1 - j][x1 + i] = aux;
		}
	}

	return mat;
}

/*
rotate_all are rolul de a realiza o rotatie tot in sens trigonometric
dar pentru o selectie care nu e neaparat patratica(pe toate imaginea)

Daca selectia este dreptunghiulara este nevoie sa alocam o noua matrice,
dupa ce completam aux(new_matrix) cu datele de care avem nevoie, dam free
la mat(old_matrix) si returnam aux(new_matrix)
*/
matrix_t **rotate_all(matrix_t **mat, image_t image)
{
	matrix_t **aux = alloc_matrix(image.height, image.width);

	for (int i = 0; i < image.width; i++) {
		for (int j = 0; j < image.height; j++) {
			aux[image.height - 1 - j][i].red = mat[i][j].red;
			aux[image.height - 1 - j][i].green = mat[i][j].green;
			aux[image.height - 1 - j][i].blue = mat[i][j].blue;
		}
	}

	for (int i = 0; i < image.width; i++)
		free(mat[i]);
	free(mat);

	return aux;
}

/*
crop trebuie sa poata micsora mat(old_matrix) lucru realizat
in aux(new_matrix) pe baza a unui set de 4 coordonate.

Dupa ce sunt copiate valorile necesare se da free la old_matrix
si return la new_matrix
*/
matrix_t **crop(int x1, int y1, int x2, int y2, matrix_t **mat, image_t *image)
{
	int width = x2 - x1 + 1;
	int height = y2 - y1 + 1;
	matrix_t **aux = alloc_matrix(height, width);

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			aux[i][j] = mat[y1 + i][x1 + j];

	for (int i = 0; i < image->height; i++)
		free(mat[i]);
	free(mat);

	image->height = height;
	image->width = width;

	return aux;
}

/*
gray functioneaza similar, acum am realizat ca puteam sa nu mai aloc
inca o matrice but don't fix it if is not broken yet

Copiez in aux(new_matrix) datele din mat(old_matrix) si apoi aplic formula
specifica grayscale-ului dar avand grija sa lucrez doar in zona data de cele 4
coordonate: x1, y1, x2, y2
*/
matrix_t **gray(int x1, int y1, int x2, int y2, matrix_t **mat, image_t image)
{
	matrix_t **aux = alloc_matrix(image.height, image.width);

	for (int i = 0; i < image.height; i++)
		for (int j = 0; j < image.width; j++)
			aux[i][j] = mat[i][j];

	for (int i = y1; i <= y2; i++) {
		for (int j = x1; j <= x2; j++) {
			aux[i][j].red = mat[i][j].red + mat[i][j].green;
			aux[i][j].red += mat[i][j].blue;
			aux[i][j].red = aux[i][j].red / 3;
			aux[i][j].green = aux[i][j].red;
			aux[i][j].blue = aux[i][j].red;
		}
	}

	for (int i = 0; i < image.height; i++)
		free(mat[i]);
	free(mat);

	return aux;
}

/*
sepia este identica cu gray si iarasi puteam sa nu mai aloc alta matrice
*/
matrix_t **sepia(int x1, int y1, int x2, int y2, matrix_t **mat, image_t image)
{
	matrix_t **aux = alloc_matrix(image.height, image.width);

	for (int i = 0; i < image.height; i++)
		for (int j = 0; j < image.width; j++)
			aux[i][j] = mat[i][j];

	for (int i = y1; i <= y2; i++) {
		for (int j = x1; j <= x2; j++) {
			aux[i][j].red = round(mat[i][j].red * 0.393
				+ mat[i][j].green * 0.769 + mat[i][j].blue * 0.189);
			if (aux[i][j].red > image.intensity)
				aux[i][j].red = image.intensity;

			aux[i][j].green = round(mat[i][j].red * 0.349
				 + mat[i][j].green * 0.686 + mat[i][j].blue * 0.168);
			if (aux[i][j].green > image.intensity)
				aux[i][j].green = image.intensity;

			aux[i][j].blue = round(mat[i][j].red * 0.272
				 + mat[i][j].green * 0.534 + mat[i][j].blue * 0.131);
			if (aux[i][j].blue > image.intensity)
				aux[i][j].blue = image.intensity;
		}
	}

	for (int i = 0; i < image.height; i++)
		free(mat[i]);
	free(mat);

	return aux;
}

/*
save_txt trebuie sa deschida fisierul in format text si in caz ca imaginea
este de tipul P5 -> P2 sau pentru P6 -> P3

Dupa ce sunt completate datele cum trebuie se va inchide fisierul
*/
void save_txt(matrix_t **mat, image_t image, char filename[NMAX])
{
	FILE *file = fopen(filename, "w");
	int red, green, blue;

	if ((strcmp(image.tip, "P2") == 0) || (strcmp(image.tip, "P5") == 0)) {
		fprintf(file, "P2\n");
		fprintf(file, "%d %d\n", image.width, image.height);
		fprintf(file, "%d\n", image.intensity);
		for (int i = 0; i < image.height; i++) {
			for (int j = 0; j < image.width; j++)
				fprintf(file, "%d ", mat[i][j].red);
			fprintf(file, "\n");
		}
	}

	if ((strcmp(image.tip, "P3") == 0) || (strcmp(image.tip, "P6") == 0)) {
		fprintf(file, "P3\n");
		fprintf(file, "%d %d\n", image.width, image.height);
		fprintf(file, "%d\n", image.intensity);
		for (int i = 0; i < image.height; i++) {
			for (int j = 0; j < image.width; j++) {
				red = mat[i][j].red;
				blue = mat[i][j].blue;
				green = mat[i][j].green;
				fprintf(file, "%d %d %d ", red, green, blue);
			}
			fprintf(file, "\n");
		}
	}

	fclose(file);
}

/*
save_bin trebuie sa deschida fisierul in format text pentru a completa
tipul de imagine, width-ul, height-ul si intensity-ul

Apoi se inchide fisierul si se redeschide in format binar pentru a completa
datele din matrice si se va inchide din nou.

Fisierului in binar ii este aplicat "ab" pentru a nu pierde datele scrise
in format text
*/
void save_bin(matrix_t **mat, image_t image, char filename[NMAX])
{
	FILE *file = fopen(filename, "w");

	if ((strcmp(image.tip, "P2") == 0) || (strcmp(image.tip, "P5") == 0))
		fprintf(file, "P5\n");
	if ((strcmp(image.tip, "P3") == 0) || (strcmp(image.tip, "P6") == 0))
		fprintf(file, "P6\n");
	fprintf(file, "%d %d\n", image.width, image.height);
	fprintf(file, "%d\n", image.intensity);
	fclose(file);

	if ((strcmp(image.tip, "P2") == 0) || (strcmp(image.tip, "P5") == 0)) {
		FILE *file2 = fopen(filename, "ab");

		for (int i = 0; i < image.height; i++)
			for (int j = 0; j < image.width; j++)
				fwrite(&mat[i][j].red, sizeof(unsigned char), 1, file2);

		fclose(file2);
	}

	if ((strcmp(image.tip, "P3") == 0) || (strcmp(image.tip, "P6") == 0)) {
		FILE *file2 = fopen(filename, "ab");
		for (int i = 0; i < image.height; i++) {
			for (int j = 0; j < image.width; j++) {
				fwrite(&mat[i][j].red, sizeof(unsigned char), 1, file2);
				fwrite(&mat[i][j].green, sizeof(unsigned char), 1, file2);
				fwrite(&mat[i][j].blue, sizeof(unsigned char), 1, file2);
			}
		}
		fclose(file2);
	}
}

