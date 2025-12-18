#ifndef VALENCE_CHECK_H
#define VALENCE_CHECK_H

typedef enum {
    SYMBOL_ATOM,
    SYMBOL_BOND,
    SYMBOL_BRANCH_OPEN,
    SYMBOL_BRANCH_CLOSE,

} SymbolType;

typedef struct {
    SymbolType type;
    int bondType;
    int maxBonds;
    int bondAmount;
    int implicitH;
    char atomChar;
    int  illegalValence;

} Symbol;


int run_valence_check(Symbol **molecule, int smile_size, int atom_size, const char input[], int matrix[atom_size][atom_size]);
void free_valency_memory(Symbol **molecule);

#endif
