#ifndef GRAPH_PARSER_H
#define GRAPH_PARSER_H

#include "DiGraph.h"
#include <string>
#include <vector>
#include <set>
#include <map>

namespace mtm {

	const char UNION_OPERATOR ='+';
	const char INTERSECTION_OPERATOR ='^';
	const char DIFFERENCE_OPERATOR ='-';
	const char PRODUCT_OPERATOR ='*';
	const std::string BINARY_OPERATOR =
		std::string() +UNION_OPERATOR +
					   INTERSECTION_OPERATOR +
					   DIFFERENCE_OPERATOR +
			           PRODUCT_OPERATOR;
	
	const char COMPLEMENT_OPERATOR ='!';
	const std::string UNARY_OPERATORS = std::string(1,COMPLEMENT_OPERATOR);

	const std::string LOAD_PREFIX = "load";
	const std::vector<std::string> OPERAND_FUNCTION = {LOAD_PREFIX};
	const char OPERAND_FUNCTION_BRACE_OPEN ='(';
	const char EDGE_BRACE_OPEN ='<';
	const char EDGE_BRACE_CLOSE ='>';
	const char PREDECENDENCE_BRACE_OPEN ='(';
	const char PREDECENDENCE_BRACE_CLOSE =')';
	const char VERTEX_EDGE_SEPARATOR = '|';
	const char GRAPH_ELEMENT_DELIMITER = ',';

	const std::vector<std::string> FORBIDDEN_NAMES ={"print","who","delete","save","quit","reset","save","load"};
	
	/*
        CheckGraphName - Checks a name of a graph is valid
    */
	void CheckGraphName(const std::string &name);
	/*
        CheckGraphExistence - Checks the existence of a saved graph
    */
	void CheckGraphExistence(const std::string &graph_name,const std::map<const std::string,DiGraph> &saved_graphs);
	/**
	* ParseGraphString: Main parsing function that runs recursively on the given string payload.
	* The function performs the following:
	*	- Search for binary operators
	*	- Serch for unary operator (complement)
	*	- Dictate if the given string represents a new graph {}
	*	- Search if the given function is a function operand ( load)
	*	- Try fetch a saved graph by the given name
	*
	* Note: this runs recursively, so searching for binary operators first dictates they will be executed last.
	*
	* @param payload - The given sting for parsing as a graph literal
	* @param saved_graphs - The saved graphs in the program so far
	* @return
	* 	Return a graph representing the given data.
	*/	
	DiGraph ParseGraphString(const std::string &payload,const std::map<const std::string,DiGraph> &saved_graphs);
	
	
	
}

#endif
