//
// Algoritmos e Estruturas de Dados --- 2023/2024
//
// Joaquim Madeira, Joao Manuel Rodrigues - June 2021, Nov 2023
//
// Graph - Using a list of adjacency lists representation
//

#include "Graph.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "SortedList.h"
#include "instrumentation.h"

struct _Vertex {
    unsigned int id;
    unsigned int inDegree;
    unsigned int outDegree;
    List *edgesList;
};

struct _Edge {
    unsigned int adjVertex;
    double weight;
};

struct _GraphHeader {
    int isDigraph;
    int isComplete;
    int isWeighted;
    unsigned int numVertices;
    unsigned int numEdges;
    List *verticesList;
};

// The comparator for the VERTICES LIST

int graphVerticesComparator(const void *p1, const void *p2) {
    unsigned int v1 = ((struct _Vertex *)p1)->id;
    unsigned int v2 = ((struct _Vertex *)p2)->id;
    int d = v1 - v2;
    return (d > 0) - (d < 0);
}

// The comparator for the EDGES LISTS

int graphEdgesComparator(const void *p1, const void *p2) {
    unsigned int v1 = ((struct _Edge *)p1)->adjVertex;
    unsigned int v2 = ((struct _Edge *)p2)->adjVertex;
    int d = v1 - v2;
    return (d > 0) - (d < 0);
}

Graph *GraphCreate(unsigned int numVertices, int isDigraph, int isWeighted) {
    Graph *g = (Graph *)malloc(sizeof(struct _GraphHeader));
    if (g == NULL)
        abort();

    g->isDigraph = isDigraph;
    g->isComplete = 0;
    g->isWeighted = isWeighted;

    g->numVertices = numVertices;
    g->numEdges = 0;

    g->verticesList = ListCreate(graphVerticesComparator);

    for (unsigned int i = 0; i < numVertices; i++) {
        struct _Vertex *v = (struct _Vertex *)malloc(sizeof(struct _Vertex));
        if (v == NULL)
            abort();

        v->id = i;
        v->inDegree = 0;
        v->outDegree = 0;

        v->edgesList = ListCreate(graphEdgesComparator);

        ListInsert(g->verticesList, v);
    }

    assert(g->numVertices == ListGetSize(g->verticesList));

    return g;
}

Graph *GraphCreateComplete(unsigned int numVertices, int isDigraph) {
    Graph *g = GraphCreate(numVertices, isDigraph, 0);

    g->isComplete = 1;

    List *vertices = g->verticesList;
    ListMoveToHead(vertices);
    unsigned int i = 0;
    for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
        struct _Vertex *v = ListGetCurrentItem(vertices);
        List *edges = v->edgesList;
        for (unsigned int j = 0; j < g->numVertices; j++) {
            if (i == j) {
                continue;
            }
            struct _Edge *new = (struct _Edge *)malloc(sizeof(struct _Edge));
            if (new == NULL)
                abort();
            new->adjVertex = j;
            new->weight = 1;

            ListInsert(edges, new);
        }
        if (g->isDigraph) {
            v->inDegree = g->numVertices - 1;
            v->outDegree = g->numVertices - 1;
        } else {
            v->outDegree = g->numVertices - 1;
        }
    }
    if (g->isDigraph) {
        g->numEdges = numVertices * (numVertices - 1);
    } else {
        g->numEdges = numVertices * (numVertices - 1) / 2;
    }

    return g;
}

void GraphDestroy(Graph **p) {
    assert(*p != NULL);
    Graph *g = *p;

    List *vertices = g->verticesList;
    if (ListIsEmpty(vertices) == 0) {
        ListMoveToHead(vertices);
        unsigned int i = 0;
        for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
            struct _Vertex *v = ListGetCurrentItem(vertices);

            List *edges = v->edgesList;
            if (ListIsEmpty(edges) == 0) {
                unsigned int i = 0;
                ListMoveToHead(edges);
                for (; i < ListGetSize(edges); ListMoveToNext(edges), i++) {
                    struct _Edge *e = ListGetCurrentItem(edges);
                    free(e);
                }
            }
            ListDestroy(&(v->edgesList));
            free(v);
        }
    }

    ListDestroy(&(g->verticesList));
    free(g);

    *p = NULL;
}

Graph *GraphCopy(const Graph *g) {
    assert(g != NULL);

    // Create an empty graph
    Graph *copy = GraphCreate(g->numVertices, g->isDigraph, g->isWeighted);
    // Mark vertices that have already been copied
    unsigned int *marked = (unsigned int *)calloc(g->numVertices, sizeof(unsigned int));

    // Copy vertices
    // Move to the beggining of the list
    ListMoveToHead(g->verticesList);
    // Foreach vertex in the vertices list
    for (unsigned int i = 0; i < g->numVertices; ListMoveToNext(g->verticesList), i++) {
        // Get pointer to current vertex
        const struct _Vertex *v = ListGetCurrentItem(g->verticesList);
        // Get pointer to current vertex copy
        ListMove(copy->verticesList, v->id);
        struct _Vertex *v_copy = ListGetCurrentItem(copy->verticesList);

        // Copy edges
        // Move to the beggining of the list
        ListMoveToHead(v->edgesList);
        // Foreach edges in the edges list
        for (unsigned int j = 0; j < v->outDegree; ListMoveToNext(v->edgesList), j++) {
            // Get pointer to current edge
            const struct _Edge *e = ListGetCurrentItem(v->edgesList);

            // Check if the edge has already been copied
            if (e->adjVertex[marked]) continue;
            // Mark the edge as copied
            v_copy->id[marked] = 1;

            // Add edge
            if (copy->isWeighted) GraphAddWeightedEdge(copy, v_copy->id, e->adjVertex, e->weight);
            else GraphAddEdge(copy, v_copy->id, e->adjVertex);
        }
    }

    free(marked);
    return copy;
}

Graph *GraphFromFile(FILE *f) {
    assert(f != NULL);

    // Read graph type
    unsigned int isDigraph, isWeighted, numVertices, numEdges;
    fscanf(f, "%d", &isDigraph);
    fscanf(f, "%d", &isWeighted);
    fscanf(f, "%d", &numVertices);
    fscanf(f, "%d", &numEdges);

    // Create an empty graph with the read parameters
    Graph *g = GraphCreate(numVertices, isDigraph, isWeighted);

    // Variable for edge vertices (source and destination)
    unsigned int v, w;
    // Read edges until the end of the file and store into variables
    while (fscanf(f, "%d %d", &v, &w) != EOF) {
        // Ignore laces
        if (v == w)
            continue;

        if (!isWeighted) {
            // If the graph is not weighted, add an unweighted edge
            GraphAddEdge(g, v, w);
        } else {
            // Variable for edge weight
            double weight;
            // Read edge weight
            fscanf(f, "%lf", &weight);
            // Add weighted edge
            GraphAddWeightedEdge(g, v, w, weight);
        }
    }

    return g;
}

// Graph

int GraphIsDigraph(const Graph *g) { return g->isDigraph; }

int GraphIsComplete(const Graph *g) { return g->isComplete; }

int GraphIsWeighted(const Graph *g) { return g->isWeighted; }

unsigned int GraphGetNumVertices(const Graph *g) { return g->numVertices; }

unsigned int GraphGetNumEdges(const Graph *g) { return g->numEdges; }

//
// For a graph
//
double GraphGetAverageDegree(const Graph *g) {
    assert(g->isDigraph == 0);
    return 2.0 * (double)g->numEdges / (double)g->numVertices;
}

static unsigned int _GetMaxDegree(const Graph *g) {
    List *vertices = g->verticesList;
    if (ListIsEmpty(vertices))
        return 0;

    unsigned int maxDegree = 0;
    ListMoveToHead(vertices);
    unsigned int i = 0;
    for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
        struct _Vertex *v = ListGetCurrentItem(vertices);
        if (v->outDegree > maxDegree) {
            maxDegree = v->outDegree;
        }
    }
    return maxDegree;
}

//
// For a graph
//
unsigned int GraphGetMaxDegree(const Graph *g) {
    assert(g->isDigraph == 0);
    return _GetMaxDegree(g);
}

//
// For a digraph
//
unsigned int GraphGetMaxOutDegree(const Graph *g) {
    assert(g->isDigraph == 1);
    return _GetMaxDegree(g);
}

// Vertices

//
// returns an array of size (outDegree + 1)
// element 0, stores the number of adjacent vertices
// and is followed by indices of the adjacent vertices
//
unsigned int *GraphGetAdjacentsTo(const Graph *g, unsigned int v) {
    assert(v < g->numVertices);

    // Node in the list of vertices
    List *vertices = g->verticesList;
    ListMove(vertices, v);
    struct _Vertex *vPointer = ListGetCurrentItem(vertices);
    unsigned int numAdjVertices = vPointer->outDegree;

    unsigned int *adjacent =
        (unsigned int *)calloc(1 + numAdjVertices, sizeof(unsigned int));

    if (numAdjVertices > 0) {
        adjacent[0] = numAdjVertices;
        List *adjList = vPointer->edgesList;
        ListMoveToHead(adjList);
        for (unsigned int i = 0; i < numAdjVertices;
             ListMoveToNext(adjList), i++) {
            struct _Edge *ePointer = ListGetCurrentItem(adjList);
            adjacent[i + 1] = ePointer->adjVertex;
        }
    }

    return adjacent;
}

//
// returns an array of size (outDegree + 1)
// element 0, stores the number of adjacent vertices
// and is followed by the distances to the adjacent vertices
//
double *GraphGetDistancesToAdjacents(const Graph *g, unsigned int v) {
    assert(v < g->numVertices);

    // Node in the list of vertices
    List *vertices = g->verticesList;
    ListMove(vertices, v);
    struct _Vertex *vPointer = ListGetCurrentItem(vertices);
    unsigned int numAdjVertices = vPointer->outDegree;

    double *distance = (double *)calloc(1 + numAdjVertices, sizeof(double));

    if (numAdjVertices > 0) {
        distance[0] = numAdjVertices;
        List *adjList = vPointer->edgesList;
        ListMoveToHead(adjList);
        for (unsigned int i = 0; i < numAdjVertices;
             ListMoveToNext(adjList), i++) {
            struct _Edge *ePointer = ListGetCurrentItem(adjList);
            distance[i + 1] = ePointer->weight;
        }
    }

    return distance;
}

//
// For a graph
//
unsigned int GraphGetVertexDegree(Graph *g, unsigned int v) {
    assert(g->isDigraph == 0);
    assert(v < g->numVertices);

    ListMove(g->verticesList, v);
    struct _Vertex *p = ListGetCurrentItem(g->verticesList);

    return p->outDegree;
}

//
// For a digraph
//
unsigned int GraphGetVertexOutDegree(Graph *g, unsigned int v) {
    assert(g->isDigraph == 1);
    assert(v < g->numVertices);

    ListMove(g->verticesList, v);
    struct _Vertex *p = ListGetCurrentItem(g->verticesList);

    return p->outDegree;
}

//
// For a digraph
//
unsigned int GraphGetVertexInDegree(Graph *g, unsigned int v) {
    assert(g->isDigraph == 1);
    assert(v < g->numVertices);

    ListMove(g->verticesList, v);
    struct _Vertex *p = ListGetCurrentItem(g->verticesList);

    return p->inDegree;
}

// Edges

static int _addEdge(Graph *g, unsigned int v, unsigned int w, double weight) {
    struct _Edge *edge = (struct _Edge *)malloc(sizeof(struct _Edge));
    edge->adjVertex = w;
    edge->weight = weight;

    ListMove(g->verticesList, v);
    struct _Vertex *vertex = ListGetCurrentItem(g->verticesList);
    int result = ListInsert(vertex->edgesList, edge);

    if (result == -1) {
        free(edge);
        return 0;
    } else {
        g->numEdges++;
        vertex->outDegree++;

        ListMove(g->verticesList, w);
        struct _Vertex *destVertex = ListGetCurrentItem(g->verticesList);
        destVertex->inDegree++;
    }

    if (g->isDigraph == 0) {
        // Bidirectional edge
        struct _Edge *edge = (struct _Edge *)malloc(sizeof(struct _Edge));
        edge->adjVertex = v;
        edge->weight = weight;

        ListMove(g->verticesList, w);
        struct _Vertex *vertex = ListGetCurrentItem(g->verticesList);
        result = ListInsert(vertex->edgesList, edge);

        if (result == -1) {
            free(edge);
            return 0;
        } else {
            // g->numEdges++; // Do not count the same edge twice on a
            // undirected graph !!
            vertex->outDegree++;
        }
    }

    return 1;
}

int GraphAddEdge(Graph *g, unsigned int v, unsigned int w) {
    assert(g->isWeighted == 0);
    assert(v != w);
    assert(v < g->numVertices);
    assert(w < g->numVertices);

    return _addEdge(g, v, w, 1.0);
}

int GraphAddWeightedEdge(Graph *g, unsigned int v, unsigned int w, double weight) {
    assert(g->isWeighted == 1);
    assert(v != w);
    assert(v < g->numVertices);
    assert(w < g->numVertices);

    return _addEdge(g, v, w, weight);
}

int _removeEdge(Graph *g, struct _Vertex *origin, struct _Vertex *dest, int secondCall) {
    // Move to the beggining of the origin's edges list
    ListMoveToHead(origin->edgesList);
    for (unsigned int i = 0; i < ListGetSize(origin->edgesList); i++) {
        // Check if origin adjVertex is the destination
        if (((struct _Edge *)ListGetCurrentItem(origin->edgesList))->adjVertex == dest->id) {
            // Remove the edge and free it from the memory
            free(ListRemoveCurrent(origin->edgesList));
            // Decrease vertices degrees
            origin->outDegree--;
            if (!secondCall) dest->inDegree--;
            // Decrease number of edges
            if (!secondCall) g->numEdges--;
            // Run addicional code if the graph is not a digraph or return success if it is
            if (g->isDigraph || secondCall) return 1;
            else return _removeEdge(g, dest, origin, 1);
        }
        ListMoveToNext(origin->edgesList);
    }

    return 0;
}

int GraphRemoveEdge(Graph *g, unsigned int v, unsigned int w) {
    assert(g != NULL);

    // Get pointer to origin vertex
    ListMove(g->verticesList, v);
    struct _Vertex *origin = ListGetCurrentItem(g->verticesList);
    // Get pointer to destination vertex
    ListMove(g->verticesList, w);
    struct _Vertex *dest = ListGetCurrentItem(g->verticesList);

    // Call and return _removeEdge
    return _removeEdge(g, origin, dest, 0);
}

// CHECKING

int ArrayCheckDuplicates(const unsigned int *array) {
    assert(array != NULL);

	for (unsigned int o = 1; o < 0[array]; o++)
        if (o[array] == (o+1)[array])
            return 1;

	return 0;
}

int ArrayContains(const unsigned int *array, unsigned int value) {
    assert(array != NULL);

	for (unsigned int o = 1; o <= 0[array]; o++)
		if (o[array] == value)
			return 1;

    return 0;
}

int GraphCheckInvariants(const Graph *g) {
	assert(g != NULL);

    // Check if the number of vertices is equal to the number of vertices in the list
    if (ListGetSize(g->verticesList) != g->numVertices) return 0;

    unsigned int graphTotalInDegree = 0;
    unsigned int graphTotalOutDegree = 0;
    for (unsigned int i = 0; i < g->numVertices; i++) {
        ListMove(g->verticesList, i);
        struct _Vertex *v = ListGetCurrentItem(g->verticesList);

        // Check if the number of edges in a vertex isn't bigger than the number of vertices
        if (ListGetSize(v->edgesList) > g->numVertices - 1) return 0;
        // Check if the number of edges in a vertice isn't bigger than the number of edges
        if (ListGetSize(v->edgesList) > g->numEdges) return 0;

        const unsigned int *adj = GraphGetAdjacentsTo(g, i);
        // Check if the number of adjacent vertices is equal to the outDegree
        if (*adj != v->outDegree) return 0;
        // Check for laces
        if (ArrayContains(adj, i)) return 0;
        // Check for duplicates
        if (ArrayCheckDuplicates(adj)) return 0;

        // Free the array of adjacent vertices
        free((void *)adj);

        graphTotalInDegree += v->inDegree;
        graphTotalOutDegree += v->outDegree;
    }

    if (!GraphIsDigraph(g)) { // Graph is not a digraph
        // Check if the total outDegree is equal to the twice the number of edges
        return graphTotalOutDegree == 2 * g->numEdges;
    } else { // Graph is a digraph
        // Check if the total inDegree is equal to the total outDegree and if it is equal to the number of edges
        return graphTotalInDegree == graphTotalOutDegree && graphTotalOutDegree == g->numEdges;
    }
}

// DISPLAYING on the console

void GraphDisplay(const Graph *g) {
    printf("---\n");
    if (g->isWeighted) {
        printf("Weighted ");
    }
    if (g->isComplete) {
        printf("COMPLETE ");
    }
    if (g->isDigraph) {
        printf("Digraph\n");
        printf("Max Out-Degree = %d\n", GraphGetMaxOutDegree(g));
    } else {
        printf("Graph\n");
        printf("Max Degree = %d\n", GraphGetMaxDegree(g));
    }
    printf("Vertices = %2d | Edges = %2d\n", g->numVertices, g->numEdges);

    List *vertices = g->verticesList;
    ListMoveToHead(vertices);
    unsigned int i = 0;
    for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
        printf("%2d ->", i);
        struct _Vertex *v = ListGetCurrentItem(vertices);
        if (ListIsEmpty(v->edgesList)) {
            printf("\n");
        } else {
            List *edges = v->edgesList;
            unsigned int i = 0;
            ListMoveToHead(edges);
            for (; i < ListGetSize(edges); ListMoveToNext(edges), i++) {
                struct _Edge *e = ListGetCurrentItem(edges);
                if (g->isWeighted) {
                    printf("   %2d(%4.2f)", e->adjVertex, e->weight);
                } else {
                    printf("   %2d", e->adjVertex);
                }
            }
            printf("\n");
        }
    }
    printf("---\n");
}

void GraphListAdjacents(const Graph *g, unsigned int v) {
    printf("---\n");

    unsigned int *array = GraphGetAdjacentsTo(g, v);

    printf("Vertex %d has %d adjacent vertices -> ", v, array[0]);

    for (unsigned int i = 1; i <= array[0]; i++) {
        printf("%d ", array[i]);
    }

    printf("\n");

    free(array);

    printf("---\n");
}
