
#ifndef TOXICPHORE_H
#define TOXICPHORE_H

int fill_atom_symbols_from_smile( const char *smiles, char atom_symbol[], int max_atoms);


int dfs_toxicphore(int t, int n_tox, const int adj_tox[n_tox][n_tox], char tox_symbol[n_tox], int n_main, const int adj_main[n_main][n_main], char main_symbol[n_main], int mapping_tox_to_main[n_tox], int used_main[n_tox], int count);


int find_toxicphore_in_main(
    int n_tox, const int adj_tox[n_tox][n_tox], char tox_symbol[n_tox],
    int n_main, const int adj_main[n_main][n_main], char main_symbol[n_main]);

int toxicphore_function(char *smile, char *toxicphore);

#endif // TOXICPHORE_H 
