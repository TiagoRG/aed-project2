//
// Algoritmos e Estruturas de Dados --- 2023/2024
//
// Topological Sorting
//

#include "GraphTopologicalSorting.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IntegersQueue.h"
#include "instrumentation.h"

struct _GraphTopoSort {
    int *marked;                    // Aux array
    unsigned int *numIncomingEdges; // Aux array
    unsigned int *vertexSequence;   // The result
    int validResult;                // 0 or 1
    unsigned int numVertices;       // From the graph
    Graph *graph;
};

// AUXILIARY FUNCTION
// Allocate memory for the struct
// And for its array fields
// Initialize all struct fields
//
static GraphTopoSort *_create(Graph *g) {
    assert(g != NULL);

    // Allocate memory for the struct
    GraphTopoSort *p = (GraphTopoSort *)malloc(sizeof(GraphTopoSort));
    if (p == NULL)
        abort();

    int numVertices = GraphGetNumVertices(g);

    // Allocate memory for the marked array initialized to 0 and abort if allocation fails
    p->marked = (int *)calloc(numVertices, sizeof(int));
    if (p->marked == NULL)
        abort();

    // Allocate memory for the numIncomingEdges array and abort if allocation fails
    p->numIncomingEdges = (unsigned int *)malloc(numVertices * sizeof(int));
    if (p->numIncomingEdges == NULL)
        abort();

    // Initialize the numIncomingEdges array with the in-degree of each vertex
    for (unsigned int v = 0; v < numVertices; v++)
        v[p->numIncomingEdges] = GraphGetVertexInDegree(g, v);

    // Allocate memory for the vertexSequence array and abort if allocation fails
    p->vertexSequence = (unsigned int *)malloc(numVertices * sizeof(int));
    if (p->vertexSequence == NULL)
        abort();

    // Initialize the other struct fields
    p->validResult = 0;
    p->numVertices = numVertices;
    p->graph = GraphCopy(g);

    return p;
}

//
// Computing the topological sorting, if any, using the 1st algorithm:
// 1 - Create a copy of the graph
// 2 - Successively identify vertices without incoming edges and remove their
//     outgoing edges
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//
GraphTopoSort *GraphTopoSortComputeV1(Graph *g) {
    assert(g != NULL && GraphIsDigraph(g) == 1);

    // Create and initialize the struct

    GraphTopoSort *topoSort = _create(g);

    // Build the topological sorting

    // TO BE COMPLETED
    //...

    return topoSort;
}

//
// Computing the topological sorting, if any, using the 2nd algorithm
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//
GraphTopoSort *GraphTopoSortComputeV2(Graph *g) {
    assert(g != NULL && GraphIsDigraph(g) == 1);

    // Create and initialize the struct

    GraphTopoSort *topoSort = _create(g);

    // Build the topological sorting

    // TO BE COMPLETED
    //...

    return topoSort;
}

//
// Computing the topological sorting, if any, using the 3rd algorithm
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//
GraphTopoSort *GraphTopoSortComputeV3(Graph *g) {
    assert(g != NULL && GraphIsDigraph(g) == 1);

    // Create and initialize the struct

    GraphTopoSort *topoSort = _create(g);

    // Build the topological sorting

    // TO BE COMPLETED
    //...

    return topoSort;
}

void GraphTopoSortDestroy(GraphTopoSort **p) {
    assert(*p != NULL);

    GraphTopoSort *aux = *p;

    free(aux->marked);
    free(aux->numIncomingEdges);
    free(aux->vertexSequence);

    free(*p);
    *p = NULL;
}

//
// A valid sorting was computed?
//
int GraphTopoSortIsValid(const GraphTopoSort *p) { return p->validResult; }

//
// Getting an array containing the topological sequence of vertex indices
// Or NULL, if no sequence could be computed
// MEMORY IS ALLOCATED FOR THE RESULTING ARRAY
//
unsigned int *GraphTopoSortGetSequence(const GraphTopoSort *p) {
    assert(p != NULL);

    // If no valid sorting was computed, return NULL
    if (p->validResult == 0)
        return NULL;

    // Allocate memory for the result array and abort if allocation fails
    unsigned int *result = (unsigned int *)malloc(p->numVertices * sizeof(int));
    if (result == NULL)
        abort();

    // Copy the sequence
    for (unsigned int o = 0; o < p->numVertices; o++)
        o[result] = o[p->vertexSequence];

    return result;
}

// DISPLAYING on the console

//
// The toplogical sequence of vertex indices, if it could be computed
//
void GraphTopoSortDisplaySequence(const GraphTopoSort *p) {
    assert(p != NULL);

    if (p->validResult == 0) {
        printf(" *** The topological sorting could not be computed!! *** \n");
        return;
    }

    printf("Topological Sorting - Vertex indices:\n");
    for (unsigned int i = 0; i < GraphGetNumVertices(p->graph); i++) {
        printf("%d ", p->vertexSequence[i]);
    }
    printf("\n");
}

//
// The toplogical sequence of vertex indices, if it could be computed
// Followed by the digraph displayed using the adjecency lists
// Adjacency lists are presented in topologic sorted order
//
void GraphTopoSortDisplay(const GraphTopoSort *p) {
    assert(p != NULL);

    // The topological order
    GraphTopoSortDisplaySequence(p);

    if (p->validResult == 0) {
        return;
    }

    // The Digraph
    for (unsigned int i = 0; i < GraphGetNumVertices(p->graph); i++) {
        GraphListAdjacents(p->graph, p->vertexSequence[i]);
    }
    printf("\n");
}
