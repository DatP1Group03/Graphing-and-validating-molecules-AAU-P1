//
// Created by jonas on 09-12-2025.
//

#include "Graph_representation.h"

void draw_molecule(char *SMILES, int atom_count, int adjacency_matrix[atom_count][atom_count], int ring_count) {
    int SMILES_length = strlen(SMILES);
    S_list sideChains;
    sideChains.head = NULL;

    int rings[ring_count];
    for (int i = 0; i<ring_count; i++) {
        rings[i]=1;
    }

    ringSize(SMILES,ring_count, rings);
for (int i = 0; i<ring_count;i++) {
    printf("%d\n",rings[i]);
}

    int current_atom = 0;
    int atoms_in_current_ring = 0;
    short in_ring = 0;
    int ring_counter = 0;
    int started_rings[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int start_of_sidechain = 0;

    Vector2 positions[atom_count];

    positions[0].x = 200;
    positions[0].y = 200;

    Vector2 position_increment;
    position_increment.x = RADIUS * 2 + BOND_LENGTH;
    position_increment.y = 0;

    for (int i = 0; i < SMILES_length; i++) {
        if (isalpha(SMILES[i])) {
            if (current_atom == 0) {
            } else if (start_of_sidechain) {
                positions[current_atom] = Vector2Add(positions[start_of_sidechain], position_increment);
                start_of_sidechain = 0;
                if (in_ring) {
                    ring_rotation(&position_increment, atoms_in_current_ring);
                }
            } else if (in_ring) {
                positions[current_atom] = Vector2Add(positions[current_atom - 1], position_increment);
                ring_rotation(&position_increment, atoms_in_current_ring);
            } else {
                positions[current_atom] = Vector2Add(positions[current_atom - 1], position_increment);
            }
            current_atom++;
        }
        if (isdigit(SMILES[i])) {
            if (started_rings[SMILES[i] - '0']) {
                in_ring--;
                if (in_ring == 0) {
                    ring_rotation(&position_increment, -1 * atoms_in_current_ring);
                }
            } else if (ring_counter != 0) {
                if (ring_counter % 2 != 0) {
                    atoms_in_current_ring = -rings[ring_counter];
                } else {
                    atoms_in_current_ring = rings[ring_counter];
                }
                started_rings[SMILES[i] - '0'] = 1;
                in_ring++;
                ring_counter++;
            } else {
                atoms_in_current_ring = rings[ring_counter];
                started_rings[SMILES[i] - '0'] = 1;
                in_ring++;
                ring_counter++;
            }
        }
        if (SMILES[i] == '(') {
            add_side_chain(&sideChains, current_atom - 1, position_increment);
            //if (in_ring) {
            //} else {
            rotate_Vector(&position_increment, PI / 2);
            // }
        }
        if (SMILES[i] == ')') {
            if (SMILES[i + 1] == '(') {
                if (in_ring) {
                    rotate_Vector(&position_increment, -2 * PI / 3);
                } else {
                    rotate_Vector(&position_increment, PI);
                }
            } else {
                start_of_sidechain = sideChains.head->side_chain_head;
                position_increment = sideChains.head->position_increment;
                remove_side_chain(&sideChains);
            }
        }
    }
    Draw_bonds(atom_count, positions, adjacency_matrix);
    Draw_atoms(atom_count, positions, adjacency_matrix, SMILES);
}

void ring_rotation(Vector2 *vector, int size_of_ring) {
    double radians = 2 * PI / (double) size_of_ring;
    rotate_Vector(vector, radians);
}

void ringSize(char *SMILES, int ring_count, int ring[ring_count]) {
    int visited_rings_numbers[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int visited_rings[ring_count];

    for (int i = 0; i<ring_count;i++) {
        visited_rings[i]=0;
    }

    int count = 0;
    for (int i = 0; i < strlen(SMILES); i++) {
        if (isdigit(SMILES[i])) {
            if (visited_rings_numbers[SMILES[i] - '0']) {
                visited_rings[visited_rings_numbers[SMILES[i] - '0']-1] = 0;
            } else {
                visited_rings_numbers[SMILES[i] - '0'] = count+1;
                visited_rings[count] = 1;
                count++;
            }
        }
        if (isalpha(SMILES[i])) {
            for (int j = 0; j < ring_count; j++) {
                if (visited_rings[j]) {
                    ring[j] += 1;
                }
            }
        }
        if (SMILES[i] == '(') {
            while (SMILES[i] != ')') {
                if (isdigit(SMILES[i])) {
                    i-=1;
                    break;
                }
                i++;
            }
        }
    }
}


void rotate_Vector(Vector2 *vector, double radians) {
    double length = sqrt(pow(vector->x, 2) + pow(vector->y, 2));

    double angle = atan(vector->y / vector->x);
    if (vector->x < 0) {
        angle += PI;
    } else if (vector->x > 0 && vector->y < 0) {
        angle += 2 * PI;
    }
    angle += radians;

    vector->x = cos(angle) * length;
    vector->y = sin(angle) * length;
}

void Draw_atoms(int atom_count, Vector2 positions[atom_count], int adj[atom_count][atom_count], char *SMILES) {
    Color color;
    for (int i = 0; i < atom_count; i++) {
        switch (adj[i][i]) {
            case 6:
                color = BLACK;
                DrawCircle(positions[i].x, positions[i].y,RADIUS, color);
                break;
            case 8:
                color = RED;
                DrawCircle(positions[i].x, positions[i].y,RADIUS, color);
                break;
            case 7:
                color = BLUE;
                DrawCircle(positions[i].x, positions[i].y,RADIUS, color);
                break;
            default:
                color = BLACK;
                DrawCircle(positions[i].x, positions[i].y,RADIUS, color);
                int atom_count2 = 0;
                int j = 0;
                while (atom_count2 != i) {
                    if (isalpha(SMILES[j])) {
                        atom_count2++;
                    }
                }
                char atom_name = SMILES[j];
                DrawText(&atom_name, positions[i].x - 4, positions[i].y - 4, 15,WHITE);
                break;
        }
    }
}

void Draw_bonds(int atom_count, Vector2 positions[atom_count], int adj[atom_count][atom_count]) {
    for (int i = 0; i < atom_count; i++) {
        for (int j = i + 1; j < atom_count; j++) {
            switch (adj[i][j]) {
                case 1:
                    DrawLineEx(positions[i], positions[j], 3,WHITE);
                    DrawLineEx(positions[i], positions[j], 1, BLACK);
                    break;
                case 2:
                    DrawLineEx(positions[i], positions[j], 11,WHITE);
                    DrawLineEx(positions[i], positions[j], 1, BLACK);
                    Vector2 orthagonal_vector;
                    orthagonal_vector.x = -((positions[i].y - positions[j].y) / 75 * 5);
                    orthagonal_vector.y = (positions[i].x - positions[j].x) / 75 * 5;
                    DrawLineEx(Vector2Add(positions[i], orthagonal_vector), Vector2Add(positions[j], orthagonal_vector),
                               1, BLACK);
                    break;
                case 3:
                    DrawLineEx(positions[i], positions[j], 11,WHITE);
                    DrawLineEx(positions[i], positions[j], 1, BLACK);
                    Vector2 orthagonal_vector2;
                    orthagonal_vector2.x = -((positions[i].y - positions[j].y) / 75 * 5);
                    orthagonal_vector2.y = (positions[i].x - positions[j].x) / 75 * 5;
                    DrawLineEx(Vector2Add(positions[i], orthagonal_vector2),
                               Vector2Add(positions[j], orthagonal_vector2), 1, BLACK);
                    DrawLineEx(Vector2Subtract(positions[i], orthagonal_vector2),
                               Vector2Subtract(positions[j], orthagonal_vector2), 1, BLACK);
                    break;
                default:
                    break;
            }
        }
    }
}


void add_side_chain(S_list *Sidechains, int current_atom, Vector2 position_increment) {
    S_node *new_side_chain = (S_node *) malloc(sizeof(S_node));
    new_side_chain->next = Sidechains->head;
    new_side_chain->side_chain_head = current_atom;
    new_side_chain->position_increment = position_increment;
    Sidechains->head = new_side_chain;
}

void remove_side_chain(S_list *Sidechains) {
    S_node *current = Sidechains->head;
    Sidechains->head = current->next;
    free(current);
}
