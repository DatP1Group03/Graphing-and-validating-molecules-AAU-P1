#ifndef VALENCE_CHECK_H
#define VALENCE_CHECK_H

typedef struct {
    int bondAmount;
    int implicitH;
    char atomChar;
    int  illegalValence;
} Symbol;

extern Symbol *molecule;
extern int *atomIndices;
extern int atomCount;


int run_valence_check(int atom_size, const char input[], int matrix[atom_size][atom_size]);


#endif
