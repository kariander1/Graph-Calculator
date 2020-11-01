#include "IOManager.h"
#include "Exceptions.h"

using std::string;
using std::ifstream;
using std::ofstream;

namespace mtm {

    const std::string ILLEGAL_NAME_CHARACTERS ="(),";
    const std::string ILLEGAL_BATCH_NAME_CHARACTERS ="";
    const char NULL_TERMINATOR ='\0';

    const string ReadVertexName(ifstream &infile, const unsigned int &vertex_count);
    void CheckFileName(const std::string &file_name, const string &illegal_characters);
    void CheckOutputFile(const std::string &file_name, const string &illegal_characters, const bool is_binary =false);
    void CheckInputFile(const std::string &file_name, const string &illegal_characters, const bool is_binary =false);


    // Return whether the batch files are valid for reading and opening
    bool CheckBatchFiles(const std::string &in_file, const std::string &out_name)
    {
        try
        {
            CheckInputFile(in_file, ILLEGAL_BATCH_NAME_CHARACTERS);
            CheckOutputFile(out_name, ILLEGAL_BATCH_NAME_CHARACTERS);
        }
        catch (const IOManagerException &e)
        {
            return false;
        }
        return true;
    }
    void CheckOutputFile(const std::string &file_name, const string &illegal_characters, const bool is_binary)
    {
        // ofstream outfile;
        CheckFileName(file_name, illegal_characters);
        if (is_binary)
        {
            ofstream outfile(file_name, std::ios_base::binary);
            if (!outfile)
            {
                throw FileNotOpen(file_name);
            }
        }
        else
        {
            ofstream outfile(file_name);
            if (!outfile)
            {
                throw FileNotOpen(file_name);
            }
        }


    }
    void CheckInputFile(const std::string &file_name, const string &illegal_characters, const bool is_binary)
    {
        CheckFileName(file_name, illegal_characters);
        
        if (is_binary)
        {
            ifstream infile(file_name, std::ios_base::binary);
            if (!infile)
        {
            throw FileNotOpen(file_name);
        }

        }
        else
        {
            ifstream infile(file_name);
if (!infile)
        {
            throw FileNotOpen(file_name);
        }
        }

        
    }
    void SaveGraphToFile(const DiGraph &graph, const std::string &file_name)
    {

        CheckOutputFile(file_name, ILLEGAL_NAME_CHARACTERS, true);

        ofstream outfile(file_name, std::ios_base::binary);
        try
        {
            graph.ToBytes(outfile);            
        }
        catch (const std::exception& e)// Might be issue while writing bytes to file
        {
            throw FileWriteError(file_name);
        }

   
        
    }
    const string ReadVertexName(ifstream &infile, const unsigned int &vertex_count)
    {
        char* vertex_name_char = nullptr;
        string to_return;
        try
        {
            unsigned int vertex_length = 0;
            if (!infile.read((char*)&vertex_length, sizeof(int)))
            {
                throw FileReadVertexLength(vertex_count);
            }
            vertex_name_char = new char[vertex_length+1];
            vertex_name_char[vertex_length]=NULL_TERMINATOR;
            if (!infile.read(vertex_name_char, vertex_length))
            {
                throw FileReadVertex(vertex_count);
            }
            to_return = string(vertex_name_char);
            delete[] vertex_name_char;
        }
        catch (const Exception& e)
        {
            delete[] vertex_name_char;
            throw e;
        }

        return to_return;

    }
    DiGraph ReadGraphFromFile(const std::string &file_name)
    {
        CheckInputFile(file_name, ILLEGAL_NAME_CHARACTERS, true);
        std::vector<string> vertices,edges;
        ifstream infile(file_name, std::ios_base::binary);
        unsigned int num_vertices =0;
        unsigned int num_edges =0;
        if (!infile.read((char*)&num_vertices, sizeof(int)))
        {
            throw FileVertexCount(file_name);
        }
        if (!infile.read((char*)&num_edges, sizeof(int)))
        {
            throw FileEdgeCount(file_name);
        }


        for (unsigned int i = 0; i < num_vertices; i++)
        {

            vertices.push_back(ReadVertexName(infile, i));

        }
        for (unsigned int i = 0; i < num_edges; i++)
        {
            string vertices[2];
            for (string &vertex : vertices)
            {
                try
                {
                    vertex = ReadVertexName(infile, i);
                }
                catch (const std::exception& e)
                {
                    throw FileReadEdge(i);
                }


            }
            edges.push_back(vertices[0] +EDGE_VERTEX_DELIMITER +vertices[1]);

        }
        return DiGraph(vertices, edges);
    }
    void CheckFileName(const std::string &file_name, const string &illegal_characters)
    {
        for (const char &illegal_char : illegal_characters)
        {
            if (file_name.find(illegal_char) !=string::npos)
            {
                throw IllegalFileName(illegal_char);
            }
        }

    }

}


