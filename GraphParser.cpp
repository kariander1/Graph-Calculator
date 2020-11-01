#include "GraphParser.h"
#include "Auxiliaries.h"
#include "Exceptions.h"
#include "IOManager.h"
#include <algorithm>
#include <stack>

using std::string;
using std::vector;
using std::map;
using std::set;
namespace mtm
{


	/*
        ParseBinaryOperators - Parses binary operators and evaluates the result of the operands
    */
	bool ParseBinaryOperators(const std::string &payload, DiGraph &graph_to_change
												,const std::map<const std::string,DiGraph> &saved_graphs);
	/*
        ParseUnaryOperators - Parses unary operators and evaluates the result of the operands
    */
	bool ParseUnaryOperators(const std::string &payload, DiGraph &graph_to_change
												,const std::map<const std::string,DiGraph> &saved_graphs);
	/*
        ParseFunctionOperands - Parses functions operators (load).
    */
	bool ParseFunctionOperands(const std::string &payload, DiGraph &graph_to_change);
	/*
        OperationLoad - Performs a load from file graph and return the associated graph to it.
    */
	DiGraph OperationLoad(const std::string &payload);
	/*
        NewGraphParser - Parses a new graph string :{x,y.... | <x,y>....}
    */
	DiGraph NewGraphParser(const std::string &payload); // For parsing {} graphs
	
	std::vector<std::string> VerticesParser(const std::string &payload);
	std::vector<std::string> EdgesParser(const std::string &payload);



/**
* ParseBinaryOperators: parses the given string according to binary operators and predecendance.
*/
	bool ParseBinaryOperators(const std::string &payload, DiGraph &graph_to_change
		, const std::map<const std::string, DiGraph> &saved_graphs)
	{		
		std::vector<char> detected_operators;
		vector<string> operands= StringSplitIgnoreBraces(payload, BINARY_OPERATOR
			, std::string() +PREDECENDENCE_BRACE_OPEN+NEW_GRAPH_OPEN_BRACE
			, std::string() +PREDECENDENCE_BRACE_CLOSE+ NEW_GRAPH_CLOSE_BRACE
			, detected_operators);

		if (operands.size()>1)
		{

			DiGraph operand_graph;

			for (size_t i = 0; i < operands.size(); i++)
			{
				const string &graphString = operands[i];
				if (i==0)
				{ // It is the first operand
					operand_graph=ParseGraphString(graphString, saved_graphs);
				}
				else switch (detected_operators[i-1])
					{
					case UNION_OPERATOR:
						operand_graph = operand_graph + ParseGraphString(graphString, saved_graphs);
						break;
					case INTERSECTION_OPERATOR:
						operand_graph = operand_graph ^ ParseGraphString(graphString, saved_graphs);
						break;
					case DIFFERENCE_OPERATOR:
						operand_graph = operand_graph - ParseGraphString(graphString, saved_graphs);
						break;
					case PRODUCT_OPERATOR:
						operand_graph = operand_graph * ParseGraphString(graphString, saved_graphs);
						break;
					default:
						throw UnrecognizedOperator(detected_operators[i-1]);
						break;

					}
			}
			graph_to_change =operand_graph;
			return true;
		}
		//}
		return false;
	}
/**
* ParseUnaryOperators: parses the given string according to unary operators and predecendance (Complement operator).
*/
	bool ParseUnaryOperators(const std::string &payload, DiGraph &graph_to_change
															, const std::map<const std::string, DiGraph> &saved_graphs)
	{
		for (std::size_t i = 0; i < UNARY_OPERATORS.size(); i++)
		{
			const char current_operator_char  = UNARY_OPERATORS[i];
			vector<string> operands= StringSplitIgnoreBraces(payload, current_operator_char
				, PREDECENDENCE_BRACE_OPEN, PREDECENDENCE_BRACE_CLOSE, false);

			if (operands.size()>1)
			{
				if(Trim(operands[0]) != string() ) // If left hand is not empty, then this was used as a bin opeartor
				{
					throw MisusedOperator(current_operator_char);
				}
				DiGraph operand_graph;
				switch (current_operator_char)
				{
				case COMPLEMENT_OPERATOR:
					operand_graph = !ParseGraphString(operands[1], saved_graphs);
					break;
				default:
					throw UnrecognizedOperator(current_operator_char);
					break;
				}


				graph_to_change =operand_graph;
				return true;
			}

		}
		return false;
	}

	DiGraph ParseGraphString(const std::string &payload, const std::map<const std::string, DiGraph> &saved_graphs)
	{
		// +,-,*,^ Has same predecendence
		string trimmed = Trim(payload);
		if (CheckWrappingParentheses(trimmed, PREDECENDENCE_BRACE_OPEN, PREDECENDENCE_BRACE_CLOSE, false))
		{
			return ParseGraphString(StringRetrieveInner(trimmed, PREDECENDENCE_BRACE_OPEN
				, PREDECENDENCE_BRACE_CLOSE), saved_graphs);
		}

		DiGraph graph_to_return;
		if (ParseBinaryOperators(trimmed, graph_to_return, saved_graphs)
			|| ParseUnaryOperators(trimmed, graph_to_return, saved_graphs))
		{
			return graph_to_return;
		}

		// If reached here then there is no more operators
		// Distinguish if new graph or existing
		if (CheckWrappingParentheses(trimmed, NEW_GRAPH_OPEN_BRACE, NEW_GRAPH_CLOSE_BRACE, false))
		{ // This is a new graph expression
			return NewGraphParser(StringRetrieveInner(trimmed, NEW_GRAPH_OPEN_BRACE, NEW_GRAPH_CLOSE_BRACE));
		}
		else if (ParseFunctionOperands(trimmed, graph_to_return))
		{
			return graph_to_return;
		}

		else
		{
			CheckGraphName(trimmed);
			CheckGraphExistence(trimmed, saved_graphs);
			return saved_graphs.at(trimmed);
		}


	}
	DiGraph OperationLoad(const std::string &payload)
	{
		const string trimmed = Trim(payload);
		const string inner = StringRetrieveInner(trimmed, ARGUEMENTS_PARENTHESES_PREFIX
																			, ARGUEMENTS_PARENTHESES_SUFFIX);
		std::vector<string> arguements = CheckNumberOfArguements(inner,1,true); // Only 1 arguements

		const string file_name = Trim(arguements[0]);
		return ReadGraphFromFile(file_name);

	}
	bool ParseFunctionOperands(const std::string &payload, DiGraph &graph_to_change)
	{
		for (const string &operand_function : OPERAND_FUNCTION)
		{

			if (StringStartsWith(payload, operand_function))  // Expression starts with function
			{
				const string arguements =Trim(payload.substr(operand_function.length()
																,payload.length() -operand_function.length()) );
				if (StringStartsWith(arguements, OPERAND_FUNCTION_BRACE_OPEN))
				{// Safe to assume it is function
					graph_to_change = OperationLoad(arguements);
					return true;
				}
			}
		}
		return false;
	}
	// New function {}
	DiGraph NewGraphParser(const std::string &payload)
	{
		const string trimmed = Trim(payload);
		vector<string> segments = StringSplit(trimmed, VERTEX_EDGE_SEPARATOR);
		if (segments.size()<1 || segments.size()>2)
		{
			throw NewGraphBadFormat();
		}
		vector<string> vertices = VerticesParser(segments[0]);
		vector<string> edges;
		if (segments.size()==2)
		{
			edges = EdgesParser(segments[1]);
		}

		return DiGraph(vertices, edges);
	}
	vector<string> VerticesParser(const std::string &payload)
	{
		vector<string> vertices;
		if(Trim(payload)!=string())
		{		
			vector<string> vertices_name = StringSplit(payload, GRAPH_ELEMENT_DELIMITER);
			for (string name : vertices_name)
			{
				const string trimmed_name =Trim(name);
					
					vertices.push_back(trimmed_name);
				
			}
		}
		return vertices;
	}

	vector<std::string> EdgesParser(const std::string &payload)
	{
		vector<string> edges;
		vector<string> edges_name = StringSplitIgnoreBraces(payload, GRAPH_ELEMENT_DELIMITER
																		, EDGE_BRACE_OPEN, EDGE_BRACE_CLOSE);
		for (const string &name : edges_name)
		{
			const string trimmed_name = Trim(name);
			CheckWrappingParentheses(trimmed_name, EDGE_BRACE_OPEN, EDGE_BRACE_CLOSE);
			const string inner_name =Trim(StringRetrieveInner(trimmed_name, EDGE_BRACE_OPEN, EDGE_BRACE_CLOSE));
			edges.push_back(inner_name);
		}
		return edges;
	}
	void CheckGraphExistence(const std::string &graph_name, const std::map<const std::string, DiGraph> &saved_graphs)
	{
		map<const std::string, mtm::DiGraph>::const_iterator it = saved_graphs.find(graph_name);

		if (it == saved_graphs.end())
		{
			throw UndefinedVariable(graph_name);
		}
	}
	void CheckGraphName(const std::string &name)
	{

		if (name.empty())
		{
			throw IllegalGraphNameEmpty();
		}
		else if (std::find(FORBIDDEN_NAMES.begin(), FORBIDDEN_NAMES.end(), name) != FORBIDDEN_NAMES.end())
		{
			throw IllegalGraphNamePreserved(name);
		}
		for (size_t i = 0; i < name.size(); i++)
		{
			char current_char = name[i];
			if (i == 0)
			{
				if (!isalpha(current_char))
				{
					throw IllegalGraphNameHead(name);
				}
			}
			else if (!isalnum(current_char))
			{
				throw IllegalGraphNameAlphaNum(name, i+1);
			}
		}

	}
} // namespace mtm
