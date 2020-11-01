

#include "PythonInterface.h"
#include "Exceptions.h"
#define TRY_OP_ONE_ARG(OP,ARG)                              \
                                                            \
    try                                                     \
    {                                                       \
        if(!ARG)                                            \
            {                                               \
                throw mtm::NullArgument(#OP);               \
            }                                               \
            OP;                                             \
    }                                                       \
    catch (std::exception & e)                              \
    {                                                       \
        std::cout << e.what() << '\n';                      \
    }                                                       \
                                                            \

#define TRY_OP_TWO_ARGS(OP,GRAPH_RETURNED,ARG2)             \
                                                            \
    Graph returned_graph=nullptr;                           \
    try                                                     \
    {                                                       \
        if(!GRAPH_RETURNED || !ARG2)                        \
            {                                               \
                throw mtm::NullArgument(#OP);               \
            }                                               \
            OP;                                             \
            returned_graph = GRAPH_RETURNED;                \
    }                                                       \
    catch (std::exception & e)                              \
    {                                                       \
        std::cout << e.what() << '\n';                      \
    }                                                       \
    return returned_graph;                                  \
                                                            \

#define TRY_OP_THREE_ARGS(OP,GRAPH_RETURNED,ARG2,ARG3)      \
                                                            \
    Graph returned_graph=nullptr;                           \
    try                                                     \
    {                                                       \
        if(!GRAPH_RETURNED || !ARG2 || !ARG3)               \
            {                                               \
                throw mtm::NullArgument(#OP);               \
            }                                               \
            OP;                                             \
            returned_graph = GRAPH_RETURNED;                \
    }                                                       \
    catch (std::exception & e)                              \
    {                                                       \
        std::cout << e.what() << '\n';                      \
    }                                                       \
    return returned_graph;                                  \
                                                            \

Graph create()
{

    try
    {
        return (new mtm::DiGraph());
    }
    catch(std::exception & e) 
    {
        std::cerr << e.what() << '\n';
    }
    
   return nullptr;

}
void destroy(Graph graph)
{
    TRY_OP_ONE_ARG(delete(graph),graph)
}
Graph addVertex(Graph graph, const  char* v)
{
    TRY_OP_TWO_ARGS(graph->AddVertex(v), graph, v);
}
Graph addEdge(Graph graph, const  char* v1, const char* v2)
{
    TRY_OP_THREE_ARGS(graph->AddEdge(v1, v2),graph,v1,v2)
}
void disp(Graph graph)
{
    TRY_OP_ONE_ARG( std::cout << (*graph),graph)
  
}
Graph graphUnion(Graph graph_in1, Graph graph_in2, Graph graph_out)
{
   TRY_OP_THREE_ARGS((*graph_out) = (*graph_in1) + (*graph_in2) ,graph_out,graph_in1,graph_in2)
}
Graph graphIntersection(Graph graph_in1, Graph graph_in2, Graph graph_out)
{
    TRY_OP_THREE_ARGS((*graph_out) = (*graph_in1) ^ (*graph_in2) ,graph_out,graph_in1,graph_in2)
}
Graph graphDifference(Graph graph_in1, Graph graph_in2, Graph graph_out)
{
    TRY_OP_THREE_ARGS((*graph_out) = (*graph_in1) - (*graph_in2) ,graph_out,graph_in1,graph_in2)
}
Graph graphProduct(Graph graph_in1, Graph graph_in2, Graph graph_out)
{
    TRY_OP_THREE_ARGS((*graph_out) = (*graph_in1) * (*graph_in2) ,graph_out,graph_in1,graph_in2)
}
Graph graphComplement(Graph graph_in, Graph graph_out)
{

    TRY_OP_TWO_ARGS((*graph_out) =!(*graph_in),graph_out, graph_in );


}

