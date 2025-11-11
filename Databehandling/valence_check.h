#ifndef VALENCE_CHECK_H
#define VALENCE_CHECK_H

typedef struct {
    int bondAmount;
    int implicitH;
    char atomChar;
} Atom;

extern Atom *atoms;

int run_valence_check(int atom_size, const char input[], int matrix[atom_size][atom_size]);
void create_atoms(int smiles_size, const char input[]);
void fillAtoms(int atom_size, int matrix[atom_size][atom_size]);
void fill_implicit_hydrogen(int size);
int valence_check_struct(int size);

#endif
