#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#define MAX 256

int ShowMainMenu() {
    int nSelected = 0;
    char buf[16]; 

    do {
        printf("\n (1) use\n"
            " (2) insert\n"
            " (3) print\n"
            " (4) exit\n\n"
            "Introduce un número > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) {
            printf("Has introducido un valor no válido. Prueba otra vez\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}

int main(void) {
    int nChoice = 0, altura, use = 0;
    char tablename[MAX], indexname[MAX], reserva[16];
    Book book;
    char titulo[256];

    do {
        nChoice = ShowMainMenu();
        switch (nChoice) {
            case 1: {
                printf ("Introduce el nombre de la tabla: \n");
                scanf("%s",tablename);
                fgets(reserva, 16, stdin);
                strcpy(indexname,tablename);
                strcat(indexname,".idx");
                strcat(tablename,".dat");
                createTable(tablename);
                createIndex(indexname);
                use = 1;
                printf ("\nTabla creada correctamente.");
                printf("\n");
            }
                break;

            case 2: {
                if (use == 0) {
                    printf("No has seleccionado una tabla.\n");
                } 
                else {
                    printf("Introduce book id: \n");
                    scanf("%s",book.book_id);
                    fgets(reserva, 16, stdin);
                    printf("Introduce el título: \n");
                    scanf("%s",titulo);
                    fgets(reserva, 16, stdin);
                    fflush(stdin);

                    if ((book.title = malloc(strlen(titulo))) == NULL) {
                        return false;
                    }

                    book.title_len = strlen(titulo);
                    strncpy(book.title, titulo, book.title_len);

                    addTableEntry(&book, tablename, indexname);
                    free(book.title);
                    printf("\nSe ha realizado correctamente.");
                    printf("\n");
                }
            }
                break;

            case 3: {
                printf("Introduce la altura del árbol: \n");
                scanf("%d",&altura);
                fgets(reserva, 16, stdin);
                printTree(altura,indexname);
                printf("\nÁrbol imprimido correctamente.");
                printf("\n");
            }
                break;

            case 4: {
                printf("Adios\n\n");
            }
                break;
        }
    } while (nChoice != 4);
    return 0;
}