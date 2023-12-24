# AED trabalho2 - CheatSheet

## Graph structure

```
Graph
├── isDigraph (bool aka int 0/1)
├── isComplete (bool aka int 0/1)
├── isWeighted (bool aka int 0/1)
├── numVertices (unsigned int)
├── numEdges (unsigned int)
└── verticesList (List<Vertex>)
```

```
Vertex
├── id (unsigned int)
├── inDegree (unsigned int)
├── outDegree (unsigned int)
└── edgesList (List<Edge>)
```

```
Edge
├── adjVertex (unsigned int)
└── weight (double)
```
