#include "utils.h"

int no_deleted_registers = NO_DELETED_REGISTERS;

void replaceExtensionByIdx(const char *fileName, char * indexName) {
    return;
}


bool createTable(const char * tableName) {
    FILE *f;
    int num = -1;
    
    if (!fopen(tableName, "r")) {
        f = fopen(tableName, "w");
        fwrite(&num, sizeof(int), 1, f);
        fclose(f);
    }

    return true;
}

bool createIndex(const char *indexName) {
    FILE *f;
    int num = -1;
    if (!fopen(indexName, "r")) {
        f = fopen(indexName, "w");

        fwrite(&num, sizeof(int), 1, f);
        fwrite(&num, sizeof(int), 1, f);

        fclose(f);
    }
    return true;
}

void printnode(size_t _level, size_t level, FILE * indexFileHandler, int node_id, char side) {
    int posicion = 0, izq = 0, der = 0, offset = 0, i = 0;
    char book_id[PK_SIZE];

    if (_level > level) {
        return;
    }
    if (node_id == -1) {
        return;
    }

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

    printnode(_level+1, level, indexFileHandler, izq, 'i');
    printnode(_level+1, level, indexFileHandler, der, 'd');

    return;
}

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

bool findKey(const char * book_id, const char *indexName,
             int * nodeIDOrDataOffset)
 {
     return true;
 }

bool addIndexEntry(char * book_id,  int bookOffset, char const * indexName) {
    return true;
}

bool addTableEntry(Book * book, const char * dataName,
                   const char * indexName) {
    return true;
}