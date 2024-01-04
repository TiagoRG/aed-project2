//
// Algoritmos e Estruturas de Dados --- 2023/2024
//
// Joaquim Madeira, Joao Manuel Rodrigues - June 2021, Nov 2023
//
// Graph EXAMPLE : Creating and displaying graphs
//

#include "Graph.h"
#include <assert.h>

int main(void) {
    // What kind of graph is g01?
    Graph *g01 = GraphCreate(6, 0, 0);
    GraphAddEdge(g01, 1, 2);
    GraphAddEdge(g01, 1, 4);
    GraphAddEdge(g01, 3, 4);
    printf("The first graph:\n");
    GraphDisplay(g01);
    Graph *g01_copy = GraphCopy(g01);
    printf("Copy of the first graph:\n");
    GraphDisplay(g01_copy);
    GraphDestroy(&g01_copy);
    for (int i = 0; i < 6; i++) {
        GraphListAdjacents(g01, i);
    }
    printf("Remove edge (1,2)\n");
    GraphRemoveEdge(g01, 1, 2);
    assert(GraphCheckInvariants(g01));
    GraphDisplay(g01);
    for (int i = 0; i < 6; i++) {
        GraphListAdjacents(g01, i);
    }

    Graph *dig01 = GraphCreate(6, 1, 0);
    GraphAddEdge(dig01, 1, 2);
    GraphAddEdge(dig01, 1, 4);
    GraphAddEdge(dig01, 3, 4);
    printf("The second graph:\n");
    GraphDisplay(dig01);
    Graph *dig01_copy = GraphCopy(dig01);
    printf("Copy of the second graph:\n");
    GraphDisplay(dig01_copy);
    GraphDestroy(&dig01_copy);
    printf("Remove edge (1,2)\n");
    GraphRemoveEdge(dig01, 1, 2);
    assert(GraphCheckInvariants(dig01));
    GraphDisplay(dig01);

    Graph *g03 = GraphCreate(6, 0, 1);
    GraphAddWeightedEdge(g03, 1, 2, 3);
    GraphAddWeightedEdge(g03, 1, 4, 5);
    GraphAddWeightedEdge(g03, 3, 4, 10);
    printf("The third graph:\n");
    GraphDisplay(g03);
    Graph *g03_copy = GraphCopy(g03);
    printf("Copy of the third graph:\n");
    GraphDisplay(g03_copy);
    GraphDestroy(&g03_copy);
    printf("Remove edge (1,2)\n");
    GraphRemoveEdge(g03, 1, 2);
    assert(GraphCheckInvariants(g03));
    GraphDisplay(g03);

    GraphDestroy(&g01);
    GraphDestroy(&dig01);
    GraphDestroy(&g03);

    return 0;
}
