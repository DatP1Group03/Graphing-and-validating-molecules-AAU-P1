
#ifndef PROJEKT_KODE_SMILESNODE_H
#define PROJEKT_KODE_SMILESNODE_H

#include <stdio.h>
#include <string.h>

#define MAX_ATOMS 100
#define MAX_FEATURES 3 // 3 propeties: valence, atom number and aromatic


// Here it defines the functions to be used in the node feature matrix part in our program. Ready for use in SMILES NODE.c


int parse_SMILES (const char *smiles, char atoms[MAX_ATOMS][3]);

void build_node_matrix(char atoms[MAX_ATOMS][3], int n_atoms, double matrix[MAX_ATOMS][MAX_FEATURES]);

int validate_features(double matrix[MAX_ATOMS][MAX_FEATURES], int n_atoms);


void print_node_matrix(char atoms[MAX_ATOMS][3], double matrix[MAX_ATOMS][MAX_FEATURES], int n_atoms);

#endif //PROJEKT_KODE_SMILESNODE_H