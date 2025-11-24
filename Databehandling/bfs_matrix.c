//
// Created by magnus on 29.10.2025.
//

#include "bfs_matrix.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* The following function is our BFS algorithm. We input the following:
 * int n this is the number of nodes that we are dealing with. This is also why we see
 * that an array of size [n][n] must be input.
 * const int adj[n][n] is our adjacency matrix as input. It is made const so that
 * We are not going to make any changes to it.
 * int src:the node we want to start at. This can range from 0 to n-1
 * int bfs[] is our array that we input into
 * */
int bfs_matrix(int n, const int adj[n][n], int src, int bfs[]) {
    int visited[n]; //matrix for keeping track of visited nodes. The node is equal to subscript and is indicated if visited with 0 or 1.
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
    }
    int queue[n]; // queue is an array consisting of the elements we have seen but not yet visited
    int front = 0, rear = 0; //
    /* front is how many times we have dequeued an element from the queue. so actually how far BFS has come in the queue
     * rear moves when we add to the queue. rear = number of elements in queue + front*/

    int count = 0; // counts how many nodes we have visited.
    printf("Our start node is %d \n", src);
    visited[src] = 1;
    queue[rear++] = src; // we set queue[back (here 0)] = src, and since we have added an element to the queue then increments back to 1.

    int dont_print_bfs_list_on_first = 1;
    while ( front < rear ) {
        // the following is simply for printing the bfs.
        int no_comma_on_first = 1;
        if (!dont_print_bfs_list_on_first) {
            printf("} BFS list currently: {");
            for (int j = 0; j < count; j++) {
                if (!no_comma_on_first) {
                    printf(", ");
                }
                printf("%d", bfs[j]);
                no_comma_on_first = 0;
            }
            printf("} ");
        }
        dont_print_bfs_list_on_first = 0;

        // and here we print our queue
        printf("In our queue we currently have the following: {");
        no_comma_on_first = 1;
        for (int i = front; i < rear; i++) {
            if (!no_comma_on_first) {
                printf(", ");
            }
            printf("%d", queue[i]);
            no_comma_on_first = 0;
        }
        printf("} \n");

        int u = queue[front]; // The first time, queue[0] is set equal to u. U will be used to determine the first element in our array.
        front++; //we now increment our front by 1, since we have just dequeued to u.
        bfs[count] = u; // our queue element which is in u is an element in bfs.
        count++; // we have added a node and therefore our count is advanced.
        printf("We are visiting node %d and it is connected to unvisited elements: {", u);
        no_comma_on_first = 1;
        for (int v = 0; v < n; v++) {
            if (adj[u][v] == 1 && visited[v] == 0) { // so here we check if adj[u][v] has 1, i.e. if there is a connection. and we check if it has already been visited!
                visited[v] = 1; // We'll mark it as visited!
                queue[rear++] = v; // and we add it to our queue. which then automatically also to bfs when while runs from the front. Because we have added to the queue, we increment our rear.

                if (!no_comma_on_first) {
                    printf(", ");
                }
                    printf("%d", v);
                    no_comma_on_first = 0;
            }
        }
    }
    return count; // number of visited nodes - a quick way to see if visited = n, if they are then the molecule is complete.


}

void print_definition_of_BFS() {
    printf("BFS starts at a chosen node and visits it. \n");
    printf("Instead of going deep, BFS looks at all the neighbours of that node first. \n");
    printf("After visiting those neighbours, it then visits all the neighbours of those neighbours, and so on. \n");
    printf("You can think of BFS as exploring the graph layer by layer: \n");
    printf("1. First the start node \n");
    printf ("2. Then all nodes directly connected to it \n");
    printf("3. Then all nodes connected to those nodes \n");
    printf("4. And so on \n");
    printf("BFS keeps track of the nodes it still needs to visit in a queue, which ensures that the earliest \n");
    printf("discovered nodes are processed first. \n");
    printf("BFS continues this pattern of moving outward in waves until all reachable nodes have been visited. \n");
}
/*Følgende funktion har et formål og det at bygge bufferen op uden at risikere buffer overflow */
static void append_log(char *buf, size_t buf_size, const char *fmt, ...)
{
    if (buf_size == 0) return; // hvis bufferen er på 0 bytes, giver det ingen mening at skrive noget, vi returner blot.

    size_t len = strlen(buf); // vi deklarer en unsigned int variable ved navn len som er længden af vores buffer.
    if (len >= buf_size - 1) return; // hvis der ikke er plads tilbage så afbryder vi - vi skal have plads til mindst 1 tegn + '\0'

    va_list args;
    va_start(args, fmt);
    vsnprintf(buf + len, buf_size - len, fmt, args);
    va_end(args);
}


int bfs_matrix_gui(int n, const int adj[n][n], int src,
                   int bfs[], char *logbuf, size_t logbuf_size) {
    int visited[n];
    int queue[n];
    int front = 0, rear = 0;
    int count = 0;

    // nulstil log
    if (logbuf_size > 0) logbuf[0] = '\0';

    for (int i = 0; i < n; i++) visited[i] = 0;

    append_log(logbuf, logbuf_size, "Our start node is %d\n", src);
    visited[src] = 1;
    queue[rear++] = src;

    int dont_print_bfs_list_on_first = 1;

    while (front < rear) {
        int no_comma_on_first = 1;

        if (!dont_print_bfs_list_on_first) {
            append_log(logbuf, logbuf_size, "} BFS list currently: {");
            for (int j = 0; j < count; j++) {
                if (!no_comma_on_first) append_log(logbuf, logbuf_size, ", ");
                append_log(logbuf, logbuf_size, "%d", bfs[j]);
                no_comma_on_first = 0;
            }
            append_log(logbuf, logbuf_size, "}\n");
        }
        dont_print_bfs_list_on_first = 0;

        append_log(logbuf, logbuf_size, "In our queue we currently have the following: {");
        no_comma_on_first = 1;
        for (int i = front; i < rear; i++) {
            if (!no_comma_on_first) append_log(logbuf, logbuf_size, ", ");
            append_log(logbuf, logbuf_size, "%d", queue[i]);
            no_comma_on_first = 0;
        }
        append_log(logbuf, logbuf_size, "}\n");

        int u = queue[front++];
        bfs[count++] = u;

        append_log(logbuf, logbuf_size,
                   "We are visiting node %d and it is connected to unvisited elements: {", u);
        no_comma_on_first = 1;

        for (int v = 0; v < n; v++) {
            if (adj[u][v] == 1 && visited[v] == 0) {
                visited[v] = 1;
                queue[rear++] = v;

                if (!no_comma_on_first) append_log(logbuf, logbuf_size, ", ");
                append_log(logbuf, logbuf_size, "%d", v);
                no_comma_on_first = 0;
            }
        }
        append_log(logbuf, logbuf_size, "}\n");
    }

    return count;
}
