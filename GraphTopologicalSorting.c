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
    if (p == NULL) abort();

    const unsigned int numVertices = GraphGetNumVertices(g);

    // Allocate memory for the marked array initialized to 0 and abort if allocation fails
    p->marked = (int *)calloc(numVertices, sizeof(int));
    if (p->marked == NULL) abort();

    // Allocate memory for the numIncomingEdges array and abort if allocation fails
    p->numIncomingEdges = (unsigned int *)malloc(numVertices * sizeof(int));
    if (p->numIncomingEdges == NULL) abort();

    // Initialize the numIncomingEdges array with the in-degree of each vertex
    for (unsigned int v = 0; v < numVertices; v++) v[p->numIncomingEdges] = GraphGetVertexInDegree(g, v);

    // Allocate memory for the vertexSequence array and abort if allocation fails
    p->vertexSequence = (unsigned int *)malloc(numVertices * sizeof(int));
    if (p->vertexSequence == NULL) abort();

    // Initialize the other struct fields
    p->validResult = 0;
    p->numVertices = numVertices;
    p->graph = g;

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

    // Create and initialize the struct and the copy of the graph
    GraphTopoSort *topoSort = _create(g);
    Graph *graph = GraphCopy(g);
    unsigned int *seq_next = topoSort->vertexSequence; // Pointer to the next position in the resulting sequence

    // While there are vertices to process
    while (seq_next < topoSort->vertexSequence + topoSort->numVertices) {
        // Get the vertex with no incoming edges and not yet marked or return invalid sorting if no such vertex exists
        unsigned int v = -1;
        while (++v < topoSort->numVertices && !(*(topoSort->numIncomingEdges + v) == 0 && !*(topoSort->marked + v)));
        if (v == topoSort->numVertices) goto return_result;

        // Add the vertex to the sequence and mark it
        *seq_next++ = v;
        *(topoSort->marked + v) = 1;

        // Remove the edges from the vertex and update the in-degree of the adjacent vertices
        const unsigned int *adj = GraphGetAdjacentsTo(graph, v);
        for (unsigned int o = 1; o <= 0[adj]; o++) {
            GraphRemoveEdge(graph, v, o[adj]);
            o[adj][topoSort->numIncomingEdges]--;
        }
        free((void *)adj);
    }

    // If the algorithm reaches this point, a valid topological sorting was computed
    topoSort->validResult = 1;

return_result:
    // Destroy the copy of the graph and return the result
    GraphDestroy(&graph);
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
    unsigned int *seq_next = topoSort->vertexSequence; // Pointer to the next position in the resulting sequence

    // While there are vertices to process
    while (seq_next < topoSort->vertexSequence + topoSort->numVertices) {
        // Get the vertex with no incoming edges and not yet marked or return invalid sorting if no such vertex exists
        unsigned int v = -1;
        while (++v < topoSort->numVertices && !(*(topoSort->numIncomingEdges + v) == 0 && !*(topoSort->marked + v)));
        if (v == topoSort->numVertices) return topoSort;

        // Add the vertex to the sequence and mark it
        *seq_next++ = v;
        *(topoSort->marked + v) = 1;

        // Remove the edges from the vertex and update the in-degree of the adjacent vertices
        const unsigned int *adj = GraphGetAdjacentsTo(topoSort->graph, v);
        for (unsigned int o = 1; o <= 0[adj]; o++) o[adj][topoSort->numIncomingEdges]--;
        free((void *)adj);
    }

    // If the algorithm reaches this point, a valid topological sorting was computed and return the result
    topoSort->validResult = 1;
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
    unsigned int *seq_next = topoSort->vertexSequence; // Pointer to the next position in the resulting sequence

    // Create a queue with the vertices with no incoming edges
    Queue *queue = QueueCreate(topoSort->numVertices);
    for (unsigned int v = 0; v < topoSort->numVertices; v++)
        if (*(topoSort->numIncomingEdges + v) == 0) QueueEnqueue(queue, v);

    while (!QueueIsEmpty(queue) && topoSort->numVertices-- > 0) {
        // Remove the first vertex from the queue
        unsigned int v = QueueDequeue(queue);

        // Add the vertex to the sequence
        *seq_next++ = v;

        // Update the in-degree of the adjacent vertices and enqueue the ones with no incoming edges
        const unsigned int *adj = GraphGetAdjacentsTo(topoSort->graph, v);
        for (unsigned int o = 1; o <= 0[adj]; o++) if (!--(*(topoSort->numIncomingEdges + o[adj]))) QueueEnqueue(queue, o[adj]);
        free((void *)adj);
    }

    QueueDestroy(&queue);

    // A valid sorting was computed if all vertices were processed
    topoSort->validResult = topoSort->numVertices == 0;
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
