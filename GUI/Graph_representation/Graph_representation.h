//
// Created by jonas on 09-12-2025.
//

#ifndef PROJEKT_KODE_GRAPH_REPRESENTATION_H
#define PROJEKT_KODE_GRAPH_REPRESENTATION_H
#include <string.h>
#include <ctype.h>
#include "raylib.h"
#include "raygui.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BOND_LENGTH 45
#define  RADIUS 15

typedef struct S_node {
    int side_chain_head;
    Vector2 position_increment;
    struct S_node *next;
} S_node;

typedef struct {
    S_node *head;
} S_list;


void rotate_Vector(Vector2 *vector, double radians);

void ring_rotation(Vector2 *vector, int size_of_ring);

void draw_molecule(char *SMILES, int atom_count, int adjacency_matrix[atom_count][atom_count], int ring_count, double node_matrix[atom_count][3]);

void Draw_atoms(int atom_count, Vector2 positions[atom_count], double node_matrix[atom_count][3], char *SMILES);

void Draw_bonds(int atom_count, Vector2 positions[atom_count], int adj[atom_count][atom_count]);

void add_side_chain(S_list *Sidechains, int current_atom, Vector2 position_increment);

void remove_side_chain(S_list *Sidechains);

void ringSize(char *SMILES, int ring_count, int ring[ring_count]);


#endif //PROJEKT_KODE_GRAPH_REPRESENTATION_H
