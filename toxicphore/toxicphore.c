#include "toxicphore.h"
#include <ctype.h>
#include <stdio.h>
#include "Adjacency_matrix.h"


/*
 * Reads the SMILES string and extracts atom symbols into an array.
 * Each uppercase character is treated as an atom symbol.
 * This allows constant-time lookup of atom type by index during matching.
 *
 * Returns the number of atoms found.
 */
int fill_atom_symbols_from_smile( const char *smiles, char atom_symbol[], int max_atoms){
	int count =0; 

	for (int i=0; smiles[i] != '\0' && count < max_atoms; i++){
      		char c = smiles[i]; 
      		
      		if (isupper(c)){
      			atom_symbol[count++] = c; 
		}
	}
	return count; 
}

/*
 * Edge-based DFS approach for toxicophore matching (legacy attempt).
 *
 * This function performs a depth-first traversal of the toxicophore graph
 * starting from node t and simultaneously attempts to find a corresponding
 * subgraph in the main molecule.
 *
 * The mapping must satisfy:
 *  - Injectivity: no two toxicophore nodes may map to the same main node.
 *  - Atom type consistency: tox_symbol[u] == main_symbol[f(u)].
 *  - Edge preservation: if two nodes are connected in the toxicophore,
 *    their mapped nodes must also be connected with the same bond order.
 *
 * Parameters:
 *  - t: current toxicophore node
 *  - n_tox: number of atoms in the toxicophore
 *  - adj_tox: adjacency matrix of the toxicophore
 *  - tox_symbol: atom symbols of the toxicophore
 *  - n_main: number of atoms in the main molecule
 *  - adj_main: adjacency matrix of the main molecule
 *  - main_symbol: atom symbols of the main molecule
 *  - mapping_tox_to_main: maps toxicophore nodes to main molecule nodes
 *  - used_main: marks which main nodes are already used
 *  - parent: previous toxicophore node (used to avoid trivial backtracking)
 *  - count: number of toxicophore nodes matched so far
 *
 * Note:
 * This approach struggles with side chains because failure in one branch
 * may prematurely terminate the DFS instead of backtracking and trying
 * alternative branches. It is therefore replaced by a node-based approach.
 */
int dfs_toxicphore(int t, int n_tox, const int adj_tox[n_tox][n_tox], char tox_symbol[n_tox], int n_main, const int adj_main[n_main][n_main], char main_symbol[n_main], int mapping_tox_to_main[n_tox], int used_main[n_main], int parent, int count) {
	// vi forsøger at finde f: noder i tox --> noder i main 
	// f er injektiv, ingen to noder i tox må pege på samme node i main
	// Dette betyder følgende
	// 	atomtype skal matche altså tox_symbol(node) == main_symbol(node) 
	// 	hvis der er en kant mellem to noder i tox så skal der også være en kant i main. altså hvis 
	// 	t er en node i tox, og er nabo til u. Så skal i main være f(t)-f(u) være naboer. 
	// Som det første i denne funktion skal vi gå til en nabo fra vores startnode t. og formålet er at for hver nabo i tox skal vi finde en passende nabo i main. 
	// Så det første vi skal det er at finde en nabo til vores startnode t.  D
	//
	if (count == n_tox) {
		printf("Har matchet alle %d tox-noder! \n", n_tox); 
		return count; 
	}
	// vi forsøger at finde en nabo til t (stadig i vores toxicphere molekyle! 
	for (int u = 0; u < n_tox; u++){
		printf("kører loop %d \n", u); 
      	if (adj_tox[t][u] >= 1){ // så en nabo betyder en værdi større 0. 
		if (u == parent) {continue;} // hvis noden er der hvor vi kom fra så vil springe den over. 
		printf("og vi har fundet et match mellem %d %d i toxicphere molekylet \n", t, u); 
      		// hvis vi allerede har mappet denne node så skal der være en kant mellem f(t) og f(u) så dette skal vi tjekke
		if (mapping_tox_to_main[u] != -1){ 
      			// hvis vi har mappet så kan vi fortsætte med dfs fra noden u. 
			// vi tjekker her om der er forbundelse mellem f(t) eller f(u) 
			if (adj_main[mapping_tox_to_main[t]][mapping_tox_to_main[u]] >= 1){
				printf("Allerede matchet! \n"); 
				// hvis allerede et match så skal vi blot fortsætte med at finde noder ud fra uindtil vi ikke længere kan finde et match . 
				if (dfs_toxicphore(u, n_tox, adj_tox, tox_symbol, n_main, adj_main, main_symbol, mapping_tox_to_main, used_main, t, count)) { return 1; 
					} else { 
						printf("fandt ingen match \n"); 
						return 0; 
					}

				// hvis vores dfs fortsætter og finder success så skal vi blot return count så ledes det returneres videre op gennem kald-stacken. Hvis ingen matches fandt så returneres 0 til kald-stacken. Det var den forkerte "vej". 
      				} 
		} else {// ellers har vi inne fundet et match endnu, og så skal vi finde en kandidat der kan være et match 
      			printf("Vi har ikke et match og derfor skal vi søge! \n"); 
			for (int j = 0; j < n_main; j++){
				printf("Vi søger mellem %d %d om der er et match! \n", mapping_tox_to_main[t], j); 
				/** så vi har kun et match idet at der er en forbindelse mellem vores allerede kendte "mappet" atom i main, og så en node derfor skal vi have en betingelse hvor >= 1. Samtidig har vi en betingelse at den ikke allerede må være brugt,. Det skal også være det samme atom, samt så skal binding også være den samme mellem denne forbindelse. */ 
      				if (adj_main[mapping_tox_to_main[t]][j] >= 1 && used_main[j] == 0 && tox_symbol[u] == main_symbol[j] && adj_tox[t][u] == adj_main[mapping_tox_to_main[t]][j] ){
      						printf("fandt et match! \n");
						used_main[j] = 1; 
						mapping_tox_to_main[u] = j; 
						
						if (dfs_toxicphore(u, n_tox, adj_tox, tox_symbol, n_main, adj_main, main_symbol, mapping_tox_to_main, used_main, t, count +1)){
							return 1; }

						// backtrack hvis ikke man kan fortsætte :
						used_main[j] = 0; // vi har alligevel ikke skulle "bruge den" 
						mapping_tox_to_main[u] = -1; // vi unmapper den. 
					}
      		}
			// hvis ingen j virker så hvad der ingen matches. 
			return 0; }
      	}
	}
	printf("og vi returner (fail) fordi count != n_tox hvis vi er nået hertil  \n"); 
	return 0; 
}


/*
 * Node-based DFS approach for toxicophore matching.
 *
 * Instead of traversing edges, this function assigns toxicophore nodes
 * one by one to main molecule nodes.
 *
 * For a given toxicophore node t:
 *  - All unused main molecule nodes are tested as candidates.
 *  - Atom types must match.
 *  - All already-mapped neighbors of t must be connected to the candidate
 *    with the same bond order.
 *
 * The function backtracks if a partial mapping leads to a contradiction.
 *
 * Returns 1 if a complete valid mapping is found, otherwise 0.
 */
int dfs_tox_nodebased(int t, int n_tox, const int adj_tox[n_tox][n_tox], char tox_symbol[n_tox], int n_main, const int adj_main[n_main][n_main], char main_symbol[n_main], int mapping_tox_to_main[n_tox], int used_main[n_main]){
	
	if (t == n_tox){
		return 1; 
	}

	// hvis v allerede er mappet (pga. naboer) spring til næste 
	if (mapping_tox_to_main[t] != -1){
		printf("Allerede matchet så vi fortsætter til næste node \n"); 
		return dfs_tox_nodebased(t + 1, n_tox, adj_tox, tox_symbol, n_main, adj_main, main_symbol, mapping_tox_to_main, used_main); 
	}

	// nodebaseret så vi skal finde en main node 
	for (int j= 0; j < n_main; j++){
		if (tox_symbol[t] != main_symbol[j]) { continue; } // hvis ikke symbolerne er ens så kan vi blot springe over 
		if (used_main[j]){continue;} // hvis den er brugt før så kan vi blot springe over. 

		// at vi er kommet hertil betyder at vi har korrekt atomtype, og at den ikke er brugt. Nu skal vi tjekke om den er kompatibel med det vi allerede har mappet. 
		int no_errors = 1; 
		
		for (int u = 0; u < n_tox; u++){
			if (adj_tox[t][u] >= 1) { // hvis t og u er naboer i toxicphore
      				if (mapping_tox_to_main[u] == -1){ // og u endnu ikke er mappet så skal vi blot fortsætte
      					continue;
				}
				// hvis vi kommer hertil så er u allerede mappet og vi skal tjekke om den er kompatibel 
				int mu = mapping_tox_to_main[u]; // mu er vores map til u, blot forkortelse
				if (adj_main[j][mu] < 1) { // hvis vi ingen kant har i main så kan vi ikke sige at j er tilsvarende node til t. 
					no_errors = 0; 
					break; 
				}
				// hvis vi har en kant i main så skal vi tjekke om bindingstypen er et match
				if (adj_main[j][mu] != adj_tox[t][u]){ //bindingstype mismatch
					no_errors = 0; 
					break; 
				}
				
				// hvis u er mappet så skal vi være sikker på at der er en kant. 
			}
		}
		
		if (!no_errors){ continue;}

		//ellers hvis vi ingen fejl har så kan vi vælge j som et billede af t. 
		mapping_tox_to_main[t] = j; 
		used_main[j] = 1; 

		// vi forsætter med næste node i tox 
		if (dfs_tox_nodebased(t+1, n_tox, adj_tox, tox_symbol, n_main, adj_main, main_symbol, mapping_tox_to_main, used_main)){
			return 1; 
		}

		// hvis ikke grenen lykkes så er noden ikke korrekt og vi skal backtrack
		mapping_tox_to_main[t] = -1; 
		used_main[j] = 0;
	}
	// ingen kandidat virkede
	return 0; 


}

/*
 * Attempts to locate the toxicophore graph as a subgraph of the main molecule.
 *
 * This function initializes the mapping and used-node arrays and then tries
 * all possible starting positions in the main molecule that match the first
 * toxicophore atom.
 *
 * For each valid starting candidate, a node-based DFS search is performed.
 * This ensures that no potential matches are missed by making assumptions
 * about the starting position.
 *
 * Returns 1 if the toxicophore is found in the main molecule, otherwise 0.
 */
int find_toxicphore_in_main(
    int n_tox, const int adj_tox[n_tox][n_tox], char tox_symbol[n_tox],
    int n_main, const int adj_main[n_main][n_main], char main_symbol[n_main]
) {
    int mapping_tox_to_main[n_tox];
    int used_main[n_main];


    // prøv alle main-noder som kandidat for tox-node 0
    for (int j = 0; j < n_main; j++) {
        if (main_symbol[j] == tox_symbol[0]) {
            printf("Prøver startmatch tox 0 (%c) -> main %d (%c)\n",
                   tox_symbol[0], j, main_symbol[j]);

    // init
    for (int i = 0; i < n_tox; i++) mapping_tox_to_main[i] = -1;
    for (int j = 0; j < n_main; j++) used_main[j] = 0;
            mapping_tox_to_main[0] = j;
            used_main[j] = 1;
/*
            int result = dfs_toxicphore(
                0,
                n_tox, adj_tox, tox_symbol,
                n_main, adj_main, main_symbol,
                mapping_tox_to_main, used_main, 0,
                1   // count = 1, fordi vi allerede har matchet node 0
            );
*/
	 if (dfs_tox_nodebased(1, n_tox, adj_tox, tox_symbol, n_main, adj_main, main_symbol, mapping_tox_to_main, used_main)){
                printf("Fandt fuldt toxicphore-match!\n");
                // her kunne du fx printe mappingen også
                return 1;
            }
		}
	}
            // backtrack start-valget
    		//for (int i = 0; i < n_tox; i++) mapping_tox_to_main[i] = -1;
   		//for (int j = 0; j < n_main; j++) used_main[j] = 0;
        // }
    //}

    return 0; //ingen match
}

/*
 * High-level wrapper function for toxicophore detection.
 *
 * This function:
 *  - Builds adjacency matrices for both the main molecule and the toxicophore.
 *  - Extracts atom symbol arrays from both SMILES strings.
 *  - Validates consistency between atom counts and extracted symbols.
 *  - Initiates the toxicophore search.
 *
 * Returns 1 if the toxicophore is found, otherwise 0.
 */
int toxicphore_function(char *smile, char *toxicphore) {
	int n_main = get_atom_count(smile);
	int adj_main[n_main][n_main];
	create_adjacency_matrix(smile, n_main,adj_main); 
		char main_symbol[n_main]; 
	int main_count_check = fill_atom_symbols_from_smile(smile, main_symbol, n_main); 

	if (main_count_check != n_main){ 
		printf("Something went wrong in main_symbol! \n"); 
		return -1; 
	}


	int n_tox = get_atom_count(toxicphore); 
	int adj_tox[n_tox][n_tox];
	create_adjacency_matrix(toxicphore, n_tox, adj_tox);
	char tox_symbol[n_tox]; 
	int tox_count_check = fill_atom_symbols_from_smile(toxicphore, tox_symbol, n_tox);

	if (tox_count_check != n_tox){
		printf("Something went wrong in tox symbol! \n"); 
		return -1; 
	}

	int find_toxic = find_toxicphore_in_main(n_tox, adj_tox, tox_symbol, n_main, adj_main, main_symbol);
	
	if (find_toxic == 1){return 1;} else {return 0;}
}
