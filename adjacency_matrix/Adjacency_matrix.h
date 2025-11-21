#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ASCII_ZERO 48

int bond_order(char c);

int get_bond_value(char S[], int index);

int get_atom_count(char S[]);

int* find_adjacency(char S[], int atom_count);

void convert_matrix(int* adj, int atom_count, int adjacency_matrix[atom_count][atom_count]);

void create_adjacency_matrix(char* SMILES, int atom_count, int adjacency_matrix[atom_count][atom_count]);

void print_ADJ(const int *adj, int atom_count) ;