#include <stdio.h>
#include <string.h>
#include "SMILESNODE.h"

#define MAX_ATOMS 100

int main () {
    char smiles[100];
    char atoms[MAX_ATOMS][3];
    double node_matrix[MAX_ATOMS][MAX_FEATURES];


    printf("Indtast SMILES STRINGS!: ");
    scanf("%s", smiles);
    //
// Sætter mit input validation ind her
if (!validate_input(smiles)) {
printf("Return 1: Error in inputvalidation. \n");
    return 1;
}



    // 1. Her identificerer jeg mit input dens noders egenskab osv og jeg skal  på en eller anden måde parse mit input


    int n_atoms = parse_SMILES(smiles, atoms);
    if (n_atoms == 0) {
        printf("Return 1: Error! - No valid atoms found. \n");
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
printf("Return 1: Error in node feature matrix. \n");
    return 1;

}


    // 7. Hvis programmet er kørt uden fejl, så foretager den en "handling action", som så afslutter med en output til vores validation af SMILES ADJ.
print_node_matrix(atoms, node_matrix, n_atoms);

printf("\nReturn 0: Node Feature Matrix validated. Ready for bond-validation.\n");

// Output skal være 2-dimensionelt array hvor den på ene akse indeholder nummer af atommolekylet og den anden er valences.

    return 0;
}
