#ifndef VALENCE_CHECK_H
#define VALENCE_CHECK_H

typedef struct {
    int bondAmount;
    int implicitH;
    char atomChar;
} Atom;

extern Atom *atoms;

void create_atoms(int smiles_size, const char input[]);
void fillAtoms(int smiles_size, int matrix[smiles_size][smiles_size]);
void fill_implicit_hydrogen(int size);
int valence_check_struct(int size);
int run_valence_check(int smiles_size, const char input[],int matrix[smiles_size][smiles_size]);

#endif
