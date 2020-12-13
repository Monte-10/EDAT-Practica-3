#include "utils.h"

int no_deleted_registers = NO_DELETED_REGISTERS;

void replaceExtensionByIdx(const char *fileName, char * indexName) {
    return;
}

/*Crea la tabla*/
bool createTable(const char * tableName) {
    FILE *f;
    int num = -1;
    /*Comprueba que se abre bien*/
    if (!fopen(tableName, "r")) {
        f = fopen(tableName, "w");
        fwrite(&num, sizeof(int), 1, f);
    }
    fclose(f);

    return true;
}

/*Crea el index*/
bool createIndex(const char *indexName) {
    FILE *f;
    int num = -1;
    /*Comprueba que se abre bien*/
    if (!fopen(indexName, "r")) {
        f = fopen(indexName, "w");

        fwrite(&num, sizeof(int), 1, f);
        fwrite(&num, sizeof(int), 1, f);
    }
    fclose(f);
    return true;
}

/*Printea nodo*/
void printnode(size_t _level, size_t level, FILE * indexFileHandler, int node_id, char side) {
    int posicion = 0, izq = 0, der = 0, offset = 0, i = 0;
    char book_id[PK_SIZE];

    if (_level > level) {
        return;
    }
    if (node_id == -1) {
        return;
    }
    /*Busca posición del nodo*/
    posicion = node_id * (4 + 4 * 4) + 8;

    fseek(indexFileHandler, posicion, SEEK_SET);

    fread(book_id, sizeof(book_id), 1, indexFileHandler);

    fread(&izq, sizeof(int), 1, indexFileHandler);
    fread(&der, sizeof(int), 1, indexFileHandler);

    fread(&offset, sizeof(int), 1, indexFileHandler);
    fread(&offset, sizeof(int), 1, indexFileHandler);

    for (i=0; (size_t) i < _level; i++ )
        printf("     ");
    printf("%c %s (%d): %d\n", side, book_id, node_id, offset);
    /*Printea nodos a los lados*/
    printnode(_level+1, level, indexFileHandler, izq, 'i');
    printnode(_level+1, level, indexFileHandler, der, 'd');

    return;
}

/*Printea el árbol*/
void printTree(size_t level, const char * indexName) {
    FILE *f;
    int raiz;

    if (!(f = fopen(indexName, "r+"))) {
        return;
    }

    fread(&raiz, sizeof(int), 1 , f);
    printnode(0, level, f, raiz, '  ');

    fclose(f);

    return;
}

/*Encuentra la clave*/
bool findKey(const char * book_id, const char *indexName, int * nodeIDOrDataOffset) {
    char book_read[PK_SIZE];
    int node, res, i;
    FILE *f;
    *nodeIDOrDataOffset = -1;

    if (!book_id || !indexName) {
        return false;
    }
    if (!(f=fopen(indexName,"r+"))) {
        return false;
    }
    fread(&node,sizeof(int), 1, f);
    while (node != -1) {
        *nodeIDOrDataOffset = node;
        fseek(f, node*20+8, SEEK_SET);
        fread(book_read, sizeof(book_read), 1, f);
        res = strncmp(book_id,book_read,PK_SIZE);
        if (res == 0) {
            for (i=0; i<4; i++) {
                fread(nodeIDOrDataOffset, sizeof(int), 1, f);
            }
            fclose(f);
            return true;
        }
        else if (res < 0) {
            fread(&node, sizeof(int), 1, f);
        }
        else {
            for (i=0; i<2; i++) {
                fread(&node,sizeof(int), 1, f);
            }
        }
    }
    fclose(f);
    return false;
}

/*Añade el índice*/
bool addIndexEntry(char * book_id,  int bookOffset, char const * indexName) {
    FILE *f;
    int nodeIDOrDataOffset, numnodes, raiz, borrado, a = -1, siguienteBorrado;
    char padre[PK_SIZE];

    if (!(f=fopen(indexName,"r+"))) {
        return false;
    }
    if (findKey(book_id,indexName,&nodeIDOrDataOffset) == true) {
        fclose(f);
        return false;
    }

    fread(&raiz, sizeof(int), 1, f);
    fread(&borrado, sizeof(int), 1, f);

    if (borrado != -1) {

        fseek(f,nodeIDOrDataOffset*20+8, SEEK_SET);
        fread(padre, sizeof(padre), 1, f);
        
        if (strncmp(book_id,padre,PK_SIZE) < 0) {
            fwrite(&borrado, sizeof(int), 1, f);
        }

        else {
            fseek(f,ftell(f)+4,SEEK_SET);
            fwrite(&borrado, sizeof(int), 1, f);
        }

        fseek(f, borrado*20+12, SEEK_SET);

        fread(&siguienteBorrado, sizeof(int), 1, f);

        fseek(f, 4, SEEK_SET);

        fwrite(&siguienteBorrado, sizeof(int), 1, f);

        fseek(f, borrado*20+8, SEEK_SET);

        fwrite(book_id, PK_SIZE, 1, f);
        fwrite(&a, sizeof(int), 1, f);
        fwrite(&a, sizeof(int), 1, f);
        fwrite(&nodeIDOrDataOffset, sizeof(int), 1, f);
        fwrite(&bookOffset, sizeof(int), 1, f);
    }
    
    else {
        fseek(f, 0, SEEK_END);
        numnodes = (ftell(f)-8)/20;
        if (raiz == -1) {
            fseek(f, 0, SEEK_SET);
            fwrite(&numnodes, sizeof(int), 1, f);
        }

        else {
            fseek(f, nodeIDOrDataOffset*20+8, SEEK_SET);
            fread(padre, sizeof(padre), 1, f);
            if (strncmp(book_id, padre, PK_SIZE) < 0) {
                fwrite(&numnodes, sizeof(int), 1, f);
            }

            else {
                fseek(f,ftell(f)+4,SEEK_SET);
                fwrite(&numnodes, sizeof(int), 1, f);
            }
        }
            fseek(f, 0, SEEK_END);

            fwrite(book_id, PK_SIZE, 1, f);
            fwrite(&a, sizeof(int), 1, f);
            fwrite(&a, sizeof(int), 1, f);
            fwrite(&nodeIDOrDataOffset, sizeof(int), 1, f);
            fwrite(&bookOffset, sizeof(int), 1, f);
        
    }

    fclose(f);
    return true;
}

/*Añade la tabla*/
bool addTableEntry(Book * book, const char * dataName, const char * indexName) {
    FILE *f;
    int borradoActual, nodeIDOrDataOffset;

    if (!(f=fopen(dataName,"r+"))) {
        fprintf(f,"Fallo al leer la clave");
        return false;
    }

    if(findKey(book->book_id, indexName, &nodeIDOrDataOffset)==true){
        fclose(f);
        return false;
    }

    /*Aquí iría el borrado, pero como no tengo el tester no se como hacerlo sin ver los fallos y si dejo el código de prueba me da muchos errores*/

    fseek(f, 0, SEEK_END);
    borradoActual=ftell(f);
    fwrite(book->book_id, 4, 1, f);
    fwrite(&book->title_len, 4, 1, f);
    fwrite(book->title, book->title_len, 1, f);

    addIndexEntry(book->book_id, borradoActual, indexName);
    fclose(f);
    return true;
}