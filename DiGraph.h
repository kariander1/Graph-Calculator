#ifndef GRAPH_H
#define GRAPH_H


#include <fstream>
#include <set>
#include <iostream>
#include <vector>

namespace mtm
{
    const char EDGE_VERTEX_DELIMITER = ',';
    const char OUTPUT_EDGE_DELIMITER = ' ';
    const char PRODUCT_VERTEX_DELIMITER = ';';
    const char PRODUCT_VERTEX_PREFIX = '[';
    const char PRODUCT_VERTEX_SUFFIX = ']';
    const char OUTPUT_VERTEX_EDGE_SEPARATOR = '$';

    class DiGraph
    {
    private:
        
        
        
        std::set<std::string> vertices; // EG "x1" ,will sort automatically lexicographically
        std::set<std::string> edges;  // EG "x1 x2", will sort automatically lexicographically


        // This is a private constructor used whitin the class, for building graphs from operators.
        // We don't want to allow external use of this constructor since DiGraph can't guarantee edges/vertices validity
        DiGraph(const std::set<std::string> vertices, const std::set<std::string> edges);

        /*
            GetEdgesContaining - return all edges containing a vertex
        */
        std::set<std::string> GetEdgesContaining(const std::string &vertex_name) const;
         /*
            BuildProductVertex - Constructs a new vertex of a product graph 
        */
        static const std::string BuildProductVertex(const std::string &vertex_a, const std::string &vertex_b);
        /*
            BuildProductVertex - Constructs a new vertex of a product graph 
        */
        static const std::string BuildEdge(const std::string &vertex_a, const std::string &vertex_b);
        /*
            GetEdgeVertices - Gets a vector of vertices from an edge
        */
        static const std::vector<std::string> GetEdgeVertices(const std::string &edge);
        /*
            CheckVertexName - Checks if a vertex name is valid. Might throw an exception
        */
        static void CheckVertexName(const std::string &name);
        /*
            CheckEdgeName - Checks if a edge name is valid. Might throw an exception
        */
	    static std::string CheckEdgeName(const std::string &name);

        /*
            WriteVertex - Writes a binary representation of a vertex
        */
        static void WriteVertex(std::ofstream &file_out,const std::string &vertex);
        /*
            ValidateEdges - Validates each edge has existing vertices
        */
        void ValidateEdges() const;
        void ValidateEdge(const std::string &edge_name) const;


    public:

        DiGraph(const std::vector<std::string> vertices, const std::vector<std::string> edges);
        DiGraph() =default;
        DiGraph(const DiGraph & graph) = default; // Default copy constructor
        DiGraph& operator=(const DiGraph & graph) = default;  // assignment operator
        ~DiGraph() = default; // Default destroyer

        DiGraph operator+(const DiGraph & graph) const; // Union
        DiGraph operator^(const DiGraph & graph) const; // Intersection
        DiGraph operator-(const DiGraph & graph) const; // difference
        DiGraph operator*(const DiGraph & graph) const; // product
        DiGraph operator!() const; // complement
        friend std::ostream &operator<<(std::ostream &, const DiGraph &);

        void AddVertex(const std::string &vertex_name);
        void AddEdge(const std::string &edge_name);
        void AddEdge(const std::string &vertex_name1,const std::string &vertex_name2);
        void ToBytes(std::ofstream &file_out) const; // Write to file

    };

} // namespace mtm

#endif //GRAPH_H