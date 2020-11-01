#ifndef PYTHON_MANAGER_H
#define PYTHON_MANAGER_H

#include "DiGraph.h"


typedef struct mtm::DiGraph* Graph;



Graph create();
void destroy(Graph graph);
Graph addVertex(Graph diGraph,const char* v);
Graph addEdge(Graph diGraph,const char* v1,const char* v2);
void disp(Graph diGraph);
Graph graphUnion(Graph graph_in1, Graph graph_in2, Graph graph_out);
Graph graphIntersection(Graph graph_in1, Graph graph_in2, Graph graph_out);
Graph graphDifference(Graph graph_in1, Graph graph_in2, Graph graph_out);
Graph graphProduct(Graph graph_in1, Graph graph_in2, Graph graph_out);
Graph graphComplement(Graph graph_in, Graph graph_out);


#endif