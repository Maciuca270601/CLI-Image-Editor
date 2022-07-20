#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "func.h"

/*
MACIUCA ALEXANDRU PETRU 314CA TEMA-3
*/

#define NMAX 1000

//MAIN
int main(void)
{
	char filename[NMAX], comanda[NMAX];
	matrix_t **matrix;
	image_t image;
	int x1, x2, y1, y2, angle;
	int sem = 1;
	/*
	sem = 1 => pot fi scrise comenzi de la tastatura
	sem = 0 => s-a intrat pe exit si programul va inceta executia
	*/
	int select_type;
	/*
	select_type = 0 => a fost introdusa o selectia cu 4 parametrii
	select_type = 1 => a fost aleasa comanda SELECT ALL
	*/
	int load = 0;
	/*
	verifica daca s-a dat load la un fisier inainte de a executa o comanda
	load = 0 => nu s-a dat load la un fisier
	load = 1 => exista fisier caruia i s-a dat load in memorie
	*/
	int valid;
	/*
	valid = 0 => comanda aleasa nu este una existenta
	valid = 1 => comanda aleasa exista
	*/
	int nulled;
	/*
	nulled = 0 => token-ul meu inca nu este NULL deci pot sa aplic
	strcmp cu el
	nulled = 1 => primesc segmentation fault daca incerc sa lucrez cu token-ul
	*/

	while (sem == 1) {
		/*
		Pana cand se va apasa EXIT citesc comenzi in sirul de caractere
		comanda si scot parametrii din aceasta cu ajutorul lui token
		*/
		valid = 0;
		nulled = 0;
		fgets(comanda, NMAX, stdin);
		if (comanda[strlen(comanda) - 1] == '\n')
			comanda[strlen(comanda) - 1] = '\0';
		char *token = strtok(comanda, " ");

		/*
		EXIT
		Daca load = 1 inseamna ca trebuie sa dau free la matricea din memorie
		altfel printez "No image loaded"
		*/
		if (strcmp(token, "EXIT") == 0) {
			sem = 0;
			valid = 1;
			if (load == 1) {
				for (int i = 0; i < image.height; i++)
					free(matrix[i]);
				free(matrix);
			} else if (load == 0) {
				printf("No image loaded\n");
			}
		}

		/*
		LOAD
		Daca load = 1 trebuie sa dau free la matricea veche, iar apoi
		apelez functia de load_image care intoarce structurile
		matrice si image completate corect si imi auto selecteaza
		intreaga matrice
		*/
		if (strcmp(token, "LOAD") == 0) {
			valid = 1;
			if (load == 1) {
				for (int i = 0; i < image.height; i++)
					free(matrix[i]);
				free(matrix);
			}
			token = strtok(NULL, " ");
			strcpy(filename, token);
			load = load_image(filename, &matrix, &image);
			x1 = 0;
			y1 = 0;
			x2 = image.width - 1;
			y2 = image.height - 1;
			select_type = 1;
		}

		/*
		SELECT
		Daca load = 1 verific daca se doreste SELECT ALL si completez
		x1, y1, x2, y2 si atribui lui select_type = 1(SELECT ALL)

		Altfel realizez un set de verificari pentru a ma asigura
		ca introduc un set de coordonate valid sau nu
		*/
		if (strcmp(token, "SELECT") == 0) {
			valid = 1;
			if (load == 1) {
				token = strtok(NULL, " ");
				if (strcmp(token, "ALL") == 0) {
					x1 = 0;
					x2 = image.width - 1;
					y1 = 0;
					y2 = image.height - 1;
					printf("Selected ALL\n");
					select_type = 1;
				} else {
					int w1, w2, h1, h2;
					int ok = 1;
					if (!token) {
						ok = 0;
						nulled = 1;
					} else {
						ok = is_number(token);
					}
					if (ok) {
						w1 = atoi(token);
						token = strtok(NULL, " ");
						if (!token) {
							ok = 0;
							nulled = 1;
						} else {
							ok = is_number(token);
						}
					}
					if (ok) {
						h1 = atoi(token);
						token = strtok(NULL, " ");
						if (!token) {
							ok = 0;
							nulled = 1;
						} else {
							ok = is_number(token);
						}
					}
					if (ok) {
						w2 = atoi(token);
						token = strtok(NULL, " ");
						if (!token) {
							nulled = 1;
							ok = 0;
						} else {
							ok = is_number(token);
						}
					}
					if (ok)
						h2 = atoi(token);
					if (ok == 0) {
						printf("Invalid command\n");
					} else if (ok == 1) {
						if (w1 == w2 || h1 == h2)
							ok = 0;
						if (ok) {
							if (w1 > w2)
								swap(&w1, &w2);
							if (h1 > h2)
								swap(&h1, &h2);
							w2 = w2 - 1;
							h2 = h2 - 1;
							ok = selectt(w1, h1, w2, h2, image);
						}
						if (ok == 1) {
							w2 = w2 + 1;
							h2 = h2 + 1;
							printf("Selected %d %d %d %d\n", w1, h1, w2, h2);
							x1 = w1;
							y1 = h1;
							x2 = w2 - 1;
							y2 = h2 - 1;
							select_type = 0;
						} else {
							printf("Invalid set of coordinates\n");
						}
					}
				}
			} else if (load == 0) {
				printf("No image loaded\n");
			}
		}

		/*
		ROTATE
		Daca load = 1 atribui lui contor numarul de cicluri necesar
		rotirii cu unghiul respectiv

		Daca rotirea are loc pe o zona delimitata de coordonate aplic
		functia rotate altfel pentru toata matricea aplic functia
		rotate_all
		*/
		if (nulled == 0) {
			if (strcmp(token, "ROTATE") == 0) {
				valid = 1;
				if (load == 1) {
					token = strtok(NULL, " ");
					angle = atoi(token);
					int contor;

					if (angle == 270 || angle == -90)
						contor = 1;
					if (angle == 180 || angle == -180)
						contor = 2;
					if (angle == 90 || angle == -270)
						contor = 3;
					if (angle == 360 || angle == -360 || angle == 0)
						contor = 0;

					int ok = is_rotate(x1, y1, x2, y2, angle, image);
					if (ok == 0) {
						if (x1 * y1 == 0 && x2 == image.width - 1 && y2 == image.height - 1)
							select_type = 1;
						for (int i = 0; i < contor; i++) {
							if (select_type == 0) {
								matrix = rotate(x1, y1, x2, matrix);
							} else if (select_type == 1) {
								swap(&image.width, &image.height);
								matrix = rotate_all(matrix, image);
								x1 = 0;
								y1 = 0;
								x2 = image.width - 1;
								y2 = image.height - 1;
							}
						}
						printf("Rotated %d\n", angle);
					} else if (ok == 1) {
						printf("The selection must be square\n");
					} else if (ok == 2) {
						printf("Unsupported rotation angle\n");
					}
				} else if (load == 0) {
					printf("No image loaded\n");
				}
			}
		}

		/*
		CROP
		Daca load = 1 aplic crop pe imagine si setez noile colturi ale acesteia
		*/
		if (nulled == 0) {
			if (strcmp(token, "CROP") == 0) {
				valid = 1;
				if (load == 1) {
					matrix = crop(x1, y1, x2, y2, matrix, &image);
					printf("Image cropped\n");
					x1 = 0;
					y1 = 0;
					x2 = image.width - 1;
					y2 = image.height - 1;
				} else if (load == 0) {
					printf("No image loaded\n");
				}
			}
		}

		/*
		GRAYSCALE
		Daca load = 1 verific ca imaginea sa fie color <=> tipul P3 sau P6
		altfel printez "Grayscale filter not available"
		*/
		if (nulled == 0) {
			if (strcmp(token, "GRAYSCALE") == 0) {
				valid = 1;
				if (load == 1) {
					if ((strcmp(image.tip, "P3") != 0) && (strcmp(image.tip, "P6") != 0)) {
						printf("Grayscale filter not available\n");
					} else if ((strcmp(image.tip, "P3") == 0) || (strcmp(image.tip, "P6") == 0)) {
						matrix = gray(x1, y1, x2, y2, matrix, image);
						printf("Grayscale filter applied\n");
					}
				} else if (load == 0) {
					printf("No image loaded\n");
				}
			}
		}

		/*
		SEPIA
		Daca load = 1 verific ca imaginea sa fie color <=> tipul P3 sau P6
		altfel printez "Sepia filter not available"
		*/
		if (nulled == 0) {
			if (strcmp(token, "SEPIA") == 0) {
				valid = 1;
				if (load == 1) {
					if ((strcmp(image.tip, "P3") != 0) && (strcmp(image.tip, "P6") != 0)) {
						printf("Sepia filter not available\n");
					} else if ((strcmp(image.tip, "P3") == 0) || (strcmp(image.tip, "P6") == 0)) {
						matrix = sepia(x1, y1, x2, y2, matrix, image);
						printf("Sepia filter applied\n");
					}
				} else if (load == 0) {
					printf("No image loaded\n");
				}
			}
		}

		/*
		SAVE
		Daca load = 1 verific daca primesc ascii caz in care salvez in mod text
		cu ajutorul functiei save_txt, altfel salvez in binar cu ajutorul functiei
		save_bin
		*/
		if (nulled == 0) {
			if (strcmp(token, "SAVE") == 0) {
				valid = 1;
				if (load == 1) {
					token = strtok(NULL, " ");
					strcpy(filename, token);
					token = strtok(NULL, " ");
					if (!token)
						save_bin(matrix, image, filename);
					else if (strcmp(token, "ascii") == 0)
						save_txt(matrix, image, filename);
					printf("Saved %s\n", filename);
				} else if (load == 0) {
					printf("No image loaded\n");
				}
			}
		}
		if (valid == 0)
			printf("Invalid command\n");
	}

	return 0;
}
