#include "DiGraph.h"
#include "Auxiliaries.h"
#include "Exceptions.h"
#include <algorithm>

using std::string;
using std::ifstream;
using std::ofstream;
using std::endl;
using std::set;
using std::string;

#define WRITE_INT(file_out,num)                             \
                                                            \
    if(!file_out.write((const char*)&(num),sizeof(int)))    \
    {                                                       \
        throw mtm::FileWriteError()  ;                      \
    }                                                       \
                                                            \

#define WRITE_CHARACTERS(file_out,characters,size)          \
                                                            \
     if(!file_out.write((characters),(size)))               \
         {                                                  \
        throw mtm::FileWriteError() ;                       \
    }                                                       \
                                                            \

namespace mtm
{
    const unsigned int VERTICES_COUNT_IN_EDGE = 2;

    DiGraph::DiGraph(const std::vector<std::string> vertices, const std::vector<std::string> edges)
        // : vertices(vertices), edges(edges)
    {
        for (const string &vertex :vertices)
        {
            AddVertex(vertex);
        }
        for (const string &edge :edges)
        {
            AddEdge(edge);
        }
        ValidateEdges();
    }
    DiGraph::DiGraph(const std::set<std::string> vertices, const std::set<std::string> edges)
        : vertices(vertices), edges(edges)
    {
        ValidateEdges();
    }

    void DiGraph::ValidateEdges() const
    {
        for (const string &edge_name : edges)
        {
            ValidateEdge(edge_name);
        }

    }
    void DiGraph::ValidateEdge(const string &edge_name) const
    {
        const std::vector<string> vertices = GetEdgeVertices(edge_name);
        string previous_vertex_name = string();
        for (const string &vertex_name: vertices)
        {
            if (this->vertices.find(vertex_name) == this->vertices.end()) // Not found
            {
                throw NonexistentVertex(vertex_name);
            }
            if (vertex_name == previous_vertex_name)
            {
                throw GraphSelfLoop(vertex_name);
            }
            previous_vertex_name = vertex_name;
        }
    }
    const std::vector<std::string> DiGraph::GetEdgeVertices(const std::string &edge)
    {
        return StringSplit(edge, EDGE_VERTEX_DELIMITER);
    }
    DiGraph DiGraph::operator+(const DiGraph &graph) const // Union
    {

        set<string> new_vertices;
        new_vertices.insert(vertices.begin(), vertices.end());             //Uniqueness remains
        new_vertices.insert(graph.vertices.begin(), graph.vertices.end()); //Uniqueness remains
        set<string> new_edges;
        new_edges.insert(edges.begin(), edges.end());             //Uniqueness remains
        new_edges.insert(graph.edges.begin(), graph.edges.end()); //Uniqueness remains

        return DiGraph(new_vertices, new_edges);
    }

    DiGraph DiGraph::operator^(const DiGraph &graph) const // Intersection
    {
        set<string> new_vertices;
        set_intersection(vertices.begin(), vertices.end(), graph.vertices.begin(), graph.vertices.end()
                                                                , std::inserter(new_vertices, new_vertices.end()));
        set<string> new_edges;
        set_intersection(edges.begin(), edges.end(), graph.edges.begin(), graph.edges.end()
                                                                , std::inserter(new_edges, new_edges.end()));

        return DiGraph(new_vertices, new_edges);
    }
    DiGraph DiGraph::operator-(const DiGraph &graph) const // Difference
    {
        set<string> new_vertices;
        set_difference(vertices.begin(), vertices.end(), graph.vertices.begin(), graph.vertices.end()
                                                                , std::inserter(new_vertices, new_vertices.end()));
        DiGraph intersection = (*this) ^ graph;

        set<string> new_edges = this->edges; // Is this copied by value?
        for (string vertex_name : intersection.vertices)
        { // All vertices that are both in graph and in graph b
            set<string> containing_edges = GetEdgesContaining(vertex_name);
            for (string edge : containing_edges)
            {
                new_edges.erase(edge);
            }
        }

        return DiGraph(new_vertices, new_edges);
    }
    DiGraph DiGraph::operator*(const DiGraph &graph) const
    {
        set<string> new_vertices;
        for (string source :vertices)
        {
            for (string destination : graph.vertices)
            {
                new_vertices.emplace(BuildProductVertex(source, destination));
            }
        }
        set<string> new_edges;
        for (string source: edges)
        {
            for (string destination :graph.edges)
            {
                std::vector<string> vertices_source =GetEdgeVertices(source);
                std::vector<string> vertices_dest =GetEdgeVertices(destination);

                string source_vertex = BuildProductVertex(vertices_source[0], vertices_dest[0]);
                string dest_vertex = BuildProductVertex(vertices_source[1], vertices_dest[1]);
                new_edges.emplace(BuildEdge(source_vertex, dest_vertex));
            }
        }
        return DiGraph(new_vertices, new_edges);
    }
    DiGraph DiGraph::operator!() const // complement
    {
        set<string> all_edges;
        for (string source : vertices)
        {
            for (string destination : vertices)
            {
                if (source!=destination)
                    all_edges.emplace(BuildEdge(source, destination));
            }
        }
        set<string> new_edges;
        set_difference(all_edges.begin(), all_edges.end(), edges.begin(), edges.end()
                                                                    , std::inserter(new_edges, new_edges.end()));
        return DiGraph(vertices, new_edges);
    }
    std::set<std::string> DiGraph::GetEdgesContaining(const std::string &vertex_name) const
    {
        set<string> containing_edges;
        for (string edge : edges)
        {
            if (edge.find(vertex_name) != std::string::npos)
            {
                containing_edges.emplace(edge);
            }
        }
        return containing_edges;
    }
    const std::string DiGraph::BuildProductVertex(const std::string &vertex_a, const std::string &vertex_b)
    {
        return PRODUCT_VERTEX_PREFIX + vertex_a + PRODUCT_VERTEX_DELIMITER + (vertex_b)+PRODUCT_VERTEX_SUFFIX;
    }
    const std::string DiGraph::BuildEdge(const std::string &vertex_a, const std::string &vertex_b)
    {
        return vertex_a + EDGE_VERTEX_DELIMITER + vertex_b;
    }
    std::ostream &operator<<(std::ostream &os, const DiGraph &graph)
    {

        for (string vertex : graph.vertices)
        {
            os << vertex << endl;
        }
        os << OUTPUT_VERTEX_EDGE_SEPARATOR << endl;
        for (string edge : graph.edges)
        {
            const std::vector<string> vertices =DiGraph::GetEdgeVertices(edge);
            os << vertices[0] << OUTPUT_EDGE_DELIMITER<< vertices[1]<< endl;
        }
        return os;
    }
    void DiGraph::AddVertex(const std::string &vertex_name)
    {
        CheckVertexName(vertex_name);
        if (this->vertices.find(vertex_name) !=  this->vertices.end())
        {
            throw VertexExists(vertex_name);
        }
        this->vertices.emplace(vertex_name);
    }
    void DiGraph::AddEdge(const std::string &edge_name)
    {
        const string refined_name = CheckEdgeName(edge_name);
        if (this->edges.find(refined_name) !=  this->edges.end())
        { // Edge already exists
            throw EdgeExists(refined_name);
        }
        ValidateEdge(refined_name);
        this->edges.emplace(refined_name);
    }
    void DiGraph::AddEdge(const std::string &vertex_name1, const std::string &vertex_name2)
    {
        AddEdge(BuildEdge(vertex_name1, vertex_name2));
    }
    void DiGraph::CheckVertexName(const std::string &name)
    {
        if (name.empty())
        {
            throw IllegalVertexName(name);
        }
        unsigned int count=0;
        for (const char &c: name)
        {
            if (c==PRODUCT_VERTEX_PREFIX)
            {
                count++;
            }
            else if (c==PRODUCT_VERTEX_SUFFIX)
            {
                if (count==0)
                {
                    throw UnmatchedParentheses(PRODUCT_VERTEX_PREFIX, PRODUCT_VERTEX_SUFFIX);
                }
                count--;

            }
            else if (!(isalnum(c) || (c==PRODUCT_VERTEX_DELIMITER && count >0)))
            {
                throw IllegalVertexName(name);
            }

        }
        if (count>0)
        {
            throw UnmatchedParentheses(PRODUCT_VERTEX_PREFIX, PRODUCT_VERTEX_SUFFIX);
        }

    }
    string DiGraph::CheckEdgeName(const std::string &name)
    {
        if (name.empty())
        {
            throw IllegalEdgeName(name);
        }
        const std::vector<string> vertices = GetEdgeVertices(name);
        if (vertices.size()!=VERTICES_COUNT_IN_EDGE)
        {
            throw IllegalEdgeName(name);
        }

        unsigned int count=0;
        string edge_name;
        for (const string &vertex : vertices)
        {
            const string trimmed_name = Trim(vertex);
            CheckVertexName(trimmed_name);
            if (count>0)
            {
                edge_name+=EDGE_VERTEX_DELIMITER;
            }
            edge_name +=trimmed_name;
            count++;
        }

        return edge_name;
    }
    void DiGraph::WriteVertex(std::ofstream &file_out, const string &vertex)
    {
        const unsigned int vertex_length = vertex.length();
        const char* vertex_name = vertex.c_str();

        WRITE_INT(file_out, vertex_length);
        WRITE_CHARACTERS(file_out, vertex_name, vertex_length);
    }
    void DiGraph::ToBytes(std::ofstream &file_out) const
    {

        const unsigned int num_vertices = vertices.size();
        WRITE_INT(file_out, num_vertices);

        const unsigned int num_edges = edges.size();
        WRITE_INT(file_out, num_edges);

        for (const string &vertex: vertices)
        {
            WriteVertex(file_out, vertex);


        }
        for (const string &edge: edges)
        {
            std::vector<string> vertices = GetEdgeVertices(edge);
            for (const string &vertex : vertices)
            {
                WriteVertex(file_out, vertex);
            }
        }

    }


} // namespace mtm

