//
// Created by magnus on 29.10.2025.
//

#include "bfs_matrix.h"

/* følgende funktion er vores BFS algoritme. Vi inputter følgende:
 * int n dette er antallet af noder som vi har at gøre med. Det er også derfor vi ser
 * at der skal inputtes et array af størrelse [n][n]. '
 * const int adj[n][n] er vores adjaency matrix som inputtes. den gøres const for at
 * vi ikke kommer til at lave ændringer af den.
 * int src: den node vi gerne vil starte på. Denne kan løbe fra 0 til n-1
 * int bfs[] er vores array som vi inputter i
 * */
int bfs_matrix(int n, const int adj[n][n], int src, int bfs[]) {
    int visited[n]; //matrice med formel at holde styr på besøgte noder. noden er lig med subscript og angives om besøgt med 0 eller 1.
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
    }
    int queue[n]; // queue er et array der består af de elementer vi har set men endnu ikke besøgt
    int front = 0, rear = 0; //
    /* front er hvor mange gange vi har dequeuet et element fra køen. så egentlig hvor langt BFS er kommet i køen
     * rear flytter sig når vi tilføjer til køen. rear  = antal elementer i kø + front */

    int count = 0; // tæller hvor mange noder vi har besøgt.

    visited[src] = 1;
    queue[rear++] = src; // vi sætter queue[back (her 0)] = src, og da vi har tilføjet et element til kø så increments back til 1.

    while ( front < rear ) {
        int u = queue[front]; // første gang sættes queue[0] lig med u. U skal bruges til bestemme det første element i vores array.
        front++; // vi incrementer vores front nu med 1, da vi lige har dequet til u.
        bfs[count] = u; // vores kø element som er i u, er et element i bfs.
        count++; // vi har tilføjet en node og derfor fremrykkes vores count.

        for (int v = 0; v < n; v++) {
            if (adj[u][v] == 1 && visited[v] == 0) { // så her tjekker vi om adj[u][v] har 1 altså om der er forbindelse. og vi tjekker om den allerede er besøgt!
                visited[v] = 1; // vi sætter den som besøgt!
                queue[rear++] = v; // og vi tilføjer den til vores queue. som så automatisk også til bfs når while kører forfra. Fordi vi har tilføjet til kø så incrementer vi vores rear.
            }
        }
    }
    return count; // antal besøgte noder - en hurtig måde at se om besøgt = n, hvis de er så er molekylet helt.


}