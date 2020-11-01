#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H


#include <string>

namespace mtm
{
    // Fatal messages
    const std::string FATAL_MESSAGE ="FATAL: ";
    const std::string FATAL_BAD_ALLOC ="bad allocation";
    const std::string FATAL_INVALID_EXE_ARGS_COUNT ="Invalid number of arguements received:";
    const std::string FATAL_INVALID_EXE_ARG ="Invalid argeument received, check file existence and name of:";

    // Error prefixes
    const std::string ERROR_PREFIX = "Error: ";
    const std::string OPERATION_ERROR_PREFIX = "Operation Parse: ";
    const std::string FILE_MANAGER_ERROR_PREFIX = "File Manager: ";
    const std::string GRAPH_PARSE_ERROR_PREFIX = "Graph Parser: ";
    const std::string GRAPH_ERROR_PREFIX = "DiGraph: ";
    

    // Operation related errors
    const std::string OPERATION_ERROR_UNRECOGNIZED_COMMAND = "Unrecognized command";
    const std::string OPERATION_ERROR_UNDEFINED_VARIABLE = "Undefined variable";
    const std::string OPERATION_ERROR_ARGUEMENT_MISMATCH = "Arguement number mismatch:";
    const std::string OPERATION_ERROR_NO_ARGUEMENTS = "No arguements given, expected:";
    
    // Graph errors
    const std::string GRAPH_NONEXISTENT_VERTEX = "Received nonexistent vertex:";
    const std::string GRAPH_BAD_ALLOC = "Not enough memory!";
    const std::string GRAPH_VERTEX_EXISTS = "Vertex already exists:";
    const std::string GRAPH_EDGE_EXISTS = "Edge already exists:";
    const std::string GRAPH_ILLEGAL_VERTEX_NAME = "Illegal vertex name:";
    const std::string GRAPH_ILLEGAL_EDGE_NAME = "Illegal edge name:";
    const std::string GRAPH_SELF_LOOP_ERROR = "Self loof deteced:";

    // IO errors
    const std::string FILE_ILLEGAL_NAME = "File illegal name, illegal character: ";
    const std::string FILE_NAME_EXCEEDS_MAX = "File exceeds maximum length, length was:";
    const std::string FILE_OPEN_ERROR = "error opening file, maybe it doesn't exist or has an illegal character? :";
    const std::string FILE_WRITE_ERROR = "error writing to file:";
    const std::string FILE_READ_ERROR = "error reading file:";
    const std::string FILE_READ_VERTICES_COUNT = "Error reading vertices count in:";
    const std::string FILE_READ_EDGES_COUNT = "Error reading edges count in:";
    const std::string FILE_READ_VERTEX_LENGTH = "Error reading vertex length num.";
    const std::string FILE_READ_VERTEX = "Error reading vertex num.";
    const std::string FILE_READ_EDGE = "Error reading edge num.";
    const std::string FILE_NOT_EOF = "File still has data left in it:";

    // Graph parser errors
    const std::string GRAPH_ILLEGAL_NAME_ALPHANUM = "Illegal variable name, must contain alphanumerical values:";
    const std::string GRAPH_ILLEGAL_NAME_HEAD = "Illegal variable name, must start with a letter:";
    const std::string GRAPH_ILLEGAL_NAME_EMPTY = "Illegal variable name, must not be empty";
    const std::string GRAPH_ILLEGAL_NAME_PRESERVED = "Illegal variable name, perserved name:";
    const std::string GRAPH_UNRECOGNIZED_OPERATOR = "Unrecognized operator:";
    const std::string GRAPH_UNMATCHED_PARENTHESES = "Unmatched parentheses:";
    const std::string GRAPH_BAD_FORMAT = "New graph bad format. Format is : {vertices|edges}";
    const std::string GRAPH_MISUSED_OPERATOR = "Unary operator used as a binary operator:";

    //Python interface errors


    const std::string INTERFACE_RECEIVED_NULL = "Received null-pointer arguement in:";
    /**
    * Class Exception
    * @exception
    * A class to represent all  exceptions, inherits the std::exception class
    */
    class Exception : public std::exception
    {
    private:
        const std::string error_string;

    public:
        Exception(const std::string &error_string) : error_string(ERROR_PREFIX+error_string) {};

    /**
    * what()
    * Overrides the standard what function from std:exception.
    * Return a char* representing the detailed exception
    */
        const char *what() const noexcept override;
    };

 class FatalException : public std::exception
    {
    private:
        const std::string error_string;

    public:
        FatalException(const std::string &error_string) : error_string(ERROR_PREFIX+FATAL_MESSAGE+error_string) {};

        /**
    * what()
    * Overrides the standard what function from std:exception.
    * Return a char* representing the detailed exception
    */
        const char *what() const noexcept override;
    };
 class BadAlloc : public FatalException
    {
    public:
        BadAlloc() : FatalException(FATAL_BAD_ALLOC) {};
    };

     class FatalExeArgsCount : public FatalException
    {
    public:
        FatalExeArgsCount(const unsigned int argc) 
                                        : FatalException(FATAL_INVALID_EXE_ARGS_COUNT + std::to_string(argc)) {};
    };


     class FatalExeArgs : public FatalException
    {
    public:
        FatalExeArgs(const std::string &file_name) 
                                        : FatalException(FATAL_INVALID_EXE_ARG + " \'" + file_name+ " \'") {};
    };



    class OperationSyntaxException : public mtm::Exception
    {
    public:
        OperationSyntaxException(const std::string &error_string) 
                                        : Exception(OPERATION_ERROR_PREFIX + error_string) {};
    };
    class IOManagerException : public mtm::Exception
    {
    public:
        IOManagerException(const std::string &error_string) : Exception(FILE_MANAGER_ERROR_PREFIX + error_string) {};
    };
    class GraphParseException : public mtm::Exception
    {
    public:
        GraphParseException(const std::string &error_string) : Exception(GRAPH_PARSE_ERROR_PREFIX + error_string) {};
    };
    class GraphException : public mtm::Exception
    {
    public:
        GraphException(const std::string &error_string) : Exception(GRAPH_ERROR_PREFIX + error_string) {};
    };
    class UnrecognizedCommand : public OperationSyntaxException
    {
    public:
        UnrecognizedCommand(const std::string &command) 
                        : OperationSyntaxException(OPERATION_ERROR_UNRECOGNIZED_COMMAND + " \'" + command + "\'") {};
    };
    class UndefinedVariable : public OperationSyntaxException
    {
    public:
        UndefinedVariable(const std::string &variable) 
                        : OperationSyntaxException(OPERATION_ERROR_UNDEFINED_VARIABLE + " \'" + variable + "\'") {};
    };
    class ArguementMismatch : public OperationSyntaxException
    {
    public:
        ArguementMismatch(const unsigned int &expected, const unsigned int &received)
            : OperationSyntaxException(OPERATION_ERROR_ARGUEMENT_MISMATCH
                         + " expected:" + std::to_string(expected) + " received:" + std::to_string(received)) {};
    };
    class NoArguements : public OperationSyntaxException
    {
    public:
        NoArguements(const std::string &expected)
            : OperationSyntaxException(OPERATION_ERROR_NO_ARGUEMENTS + (expected)) {};
    };
    class IllegalGraphNameAlphaNum : public GraphParseException
    {
    public:
        IllegalGraphNameAlphaNum(const std::string &name, const unsigned int &pos)
            : GraphParseException(GRAPH_ILLEGAL_NAME_ALPHANUM 
                                            + " " + name+" character position:"+std::to_string(pos)) {};

    };
    class IllegalGraphNameHead : public GraphParseException
    {
    public:
        IllegalGraphNameHead(const std::string &name) 
                            : GraphParseException(GRAPH_ILLEGAL_NAME_HEAD + " \'"+ name+ "\'") {};
    };
    class IllegalGraphNameEmpty : public GraphParseException
    {
    public:
        IllegalGraphNameEmpty() : GraphParseException(GRAPH_ILLEGAL_NAME_EMPTY) {};
    };
    class IllegalGraphNamePreserved : public GraphParseException
    {
    public:
        IllegalGraphNamePreserved(const std::string &name) 
                                    : GraphParseException(GRAPH_ILLEGAL_NAME_PRESERVED + " \'"+ name+ "\'") {};
    };
    class UnrecognizedOperator : public GraphParseException
    {
    public:
        UnrecognizedOperator(const char &operator_char) 
                            : GraphParseException(GRAPH_UNRECOGNIZED_OPERATOR + " \'"+ operator_char+ "\'") {};
    };
    class MisusedOperator : public GraphParseException
    {
    public:
        MisusedOperator(const char &operator_char) 
                            : GraphParseException(GRAPH_MISUSED_OPERATOR + " \'"+ operator_char+ "\'") {};
    };
    class UnmatchedParentheses : public GraphParseException
    {
    public:
        UnmatchedParentheses(const std::string &open, const std::string &close) 
                                    : GraphParseException(GRAPH_UNMATCHED_PARENTHESES + " \'"+ open+""+close+ "\'") {};
        UnmatchedParentheses(const char &open, const char &close) 
                                    : GraphParseException(GRAPH_UNMATCHED_PARENTHESES + " \'"+ open+""+close+ "\'") {};
    };
    class NewGraphBadFormat : public GraphParseException
    {
    public:
        NewGraphBadFormat() : GraphParseException(GRAPH_BAD_FORMAT) {};
    };
    class IllegalVertexName : public GraphException
    {
    public:
        IllegalVertexName(const std::string &name) : GraphException(GRAPH_ILLEGAL_VERTEX_NAME + " \'"+ name+ "\'") {};
    };

    class GraphSelfLoop : public GraphException
    {
    public:
        GraphSelfLoop(const std::string &name) : GraphException(GRAPH_SELF_LOOP_ERROR + " \'"+ name+ "\'") {};
    };

    class IllegalEdgeName : public GraphException
    {
    public:
        IllegalEdgeName(const std::string &name) : GraphException(GRAPH_ILLEGAL_EDGE_NAME + " \'"+ name+ "\'") {};
    };

    class NonexistentVertex : public GraphException
    {
    public:
        NonexistentVertex(const std::string &name) : GraphException(GRAPH_NONEXISTENT_VERTEX + " \'"+ name+ "\'") {};
    };
    class VertexExists : public GraphException
    {
    public:
        VertexExists(const std::string &name) : GraphException(GRAPH_VERTEX_EXISTS + " \'"+ name+ "\'") {};
    };
    class EdgeExists : public GraphException
    {
    public:
        EdgeExists(const std::string &name) : GraphException(GRAPH_EDGE_EXISTS + " \'"+ name+ "\'") {};
    };

    class GraphOutOfMemory : public GraphException
    {
    public:
        GraphOutOfMemory() : GraphException(GRAPH_BAD_ALLOC) {};
    };
    class IllegalFileName : public IOManagerException
    {
    public:
        IllegalFileName(const char &character) : IOManagerException(FILE_ILLEGAL_NAME + " \'"+ character+ "\'") {};
    };

    class FileNotOpen : public IOManagerException
    {
    public:
        FileNotOpen(const std::string &name) : IOManagerException(FILE_OPEN_ERROR + " \'"+ name+ "\'") {};
    };
    class FileWriteError : public IOManagerException
    {
    public:
        FileWriteError(const std::string &name) : IOManagerException(FILE_WRITE_ERROR + " \'"+ name+ "\'") {};
        FileWriteError() : IOManagerException(FILE_WRITE_ERROR) {};
    };
    class FileReadError : public IOManagerException
    {
    public:
        FileReadError(const std::string &name) : IOManagerException(FILE_READ_ERROR + " \'"+ name+ "\'") {};
    };

 class FileVertexCount : public IOManagerException
    {
    public:
        FileVertexCount(const std::string &name) : IOManagerException(FILE_READ_VERTICES_COUNT+ " \'"+ name+ "\'") {};
    };

 class FileEdgeCount : public IOManagerException
    {
    public:
        FileEdgeCount(const std::string &name) : IOManagerException(FILE_READ_EDGES_COUNT+ " \'"+ name+ "\'") {};
    };

 class FileNameExceedsMax : public IOManagerException
    {
    public:
        FileNameExceedsMax(const unsigned int name_length) 
                            : IOManagerException(FILE_NAME_EXCEEDS_MAX+ " \'"+std::to_string(name_length)+ "\'") {};
    };


 class FileReadVertex : public IOManagerException
    {
    public:
        FileReadVertex(const unsigned int &num) 
                                        : IOManagerException(FILE_READ_VERTEX+ " \'"+ std::to_string(num)+ "\'") {};
    };
    class FileReadVertexLength : public IOManagerException
    {
    public:
        FileReadVertexLength(const unsigned int &num) 
                                    : IOManagerException(FILE_READ_VERTEX_LENGTH+ " \'"+std::to_string(num)+ "\'") {};
    };
     class FileReadEdge : public IOManagerException
    {
    public:
        FileReadEdge(const unsigned int &num) 
                                    : IOManagerException(FILE_READ_EDGE+ " \'"+ std::to_string(num)+ "\'") {};
    };

   class FileNotEOF : public IOManagerException
    {
    public:
        FileNotEOF(const std::string &name) : IOManagerException(FILE_NOT_EOF+ " \'"+ name+ "\'") {};
    };

    class NullArgument : public Exception
    {
    public:
        NullArgument(const std::string &name) : Exception(INTERFACE_RECEIVED_NULL + " \'"+ name+ "\'") {};
    };
} // namespace mtm
#endif