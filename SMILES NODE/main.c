#include <stdio.h>
#include <string.h>
#include "SMILESNODE.h"
#include "validation.h"


int main () {
    char smiles[256];
    char atoms[MAX_ATOMS][3];
    double node_matrix[MAX_ATOMS][MAX_FEATURES];


    printf("Indtast SMILES STRINGS!: ");
    if (scanf("%256s", smiles) != 1) {
        fprintf(stderr, "No input.\n");
        return 1;
    }

// Sætter mit input validation ind her
if (!validate_smiles(smiles)) {
fprintf(stderr, "SMILES validation failed. See errors above.\n");
    return 1;
}
    // 1. Her identificerer jeg mit input dens noders egenskab osv og jeg skal  på en eller anden måde parse mit input


    int n_atoms = parse_SMILES(smiles, atoms);
    if (n_atoms == 0) {
        fprintf(stderr, "No atoms parsed from SMILES.\n");
        return 1;
    }
    printf("Identifying %d atoms.\n", n_atoms);


    printf("Identifying %d valence.\n", n_atoms);

    printf("Identifying %d aromatics.\n", n_atoms);

    // 2. Her får jeg programmet til at "extract" egenskaberne såsom valence, atom number og "Aromaticlly". Dette sker i functionerne i h filen.

printf("Propeties analyzed and ready for further action. \n", n_atoms);

    // 4. Programmet bygger nu en node feature matrix

build_node_matrix(atoms, n_atoms, node_matrix);


    // 5. Programmet kører nu en "Validering" af node feature matrix, som afgør om der er fejl i koden eller,
    // - om den kan fortsætte til videre behandling.

    // 6. Hvis programmet fejler, så returnerer den med: "Failed operation error, correct mistake!
    // Check invalid node or missing operators"

if (!validate_features(node_matrix, n_atoms)) {
fprintf(stderr, "Node feature validation failed. See errors above. \n");
    print_errors();
    return 1;
}


    // 7. Hvis programmet er kørt uden fejl, så foretager den en "handling action", som så afslutter med en output til vores validation af SMILES ADJ.
print_node_matrix(atoms, node_matrix, n_atoms);

printf("Done.\n");

// Output skal være 2-dimensionelt array hvor den på ene akse indeholder nummer af atommolekylet og den anden er valences.

    return 0;
}
