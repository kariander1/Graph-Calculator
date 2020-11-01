#ifndef AUXILIARIES_H
#define AUXILIARIES_H

#include <string>
#include <vector>



namespace mtm {

	const char WHITE_SPACE =' ';
	const char ARGUEMENT_SEPARATOR = ',';
	const char ARGUEMENTS_PARENTHESES_PREFIX = '(';
	const char ARGUEMENTS_PARENTHESES_SUFFIX = ')';
	const char NEW_GRAPH_OPEN_BRACE ='{';
	const char NEW_GRAPH_CLOSE_BRACE ='}';


	
	/*
		StringSplit : splits the given string by a given delimiter. Return the string in a vector
	*/
	std::vector<std::string> StringSplit(const std::string &str, const char &delimiter=WHITE_SPACE);
	std::vector<std::string> StringSplit(const std::string &str, const std::string &delimiter);
	/*
		StringContains : return whether a string contains a substring
	*/
	bool StringContains(const std::string &str, const char &character);
	/*
		StringStartsWith : returns whether a string starts with a given string/char
	*/
	const bool StringStartsWith(const std::string &str, const std::string &prefix);
	const bool StringStartsWith(const std::string &str, const char &prefix);
	/*
		StringRetrieveInner : return the inner string that is wrapped by a parentheses
	*/
	std::string StringRetrieveInner(const std::string& str, const char &open_par, const char &close_par);

	  /**
        * StringSplitIgnoreBraces: Splits a given string by a set of delimiters given with special parameters.
		* The given braces makes the delimination process halt if the string reach an inside of the braces.
        * @param str - The source string to delimintate
        * @param delimiters - string of delimiters, in which every char is a potential delimiter
        * @param open_braces - string representing all braces open in which the delimination process stops
		* @param close_braces - string representing all bbraces close in which the delimination process can continue
		* @param delimiter_queue - a vector to hold each delimiter in order that delimited the string (used in binary 
		*				operators)
		* @param allow_consecutive_delimiters - consider two consecutive delimiters (ignoring spaces) as a valid
		*										deliminated string (used in unary operators)
		* @param drop_check_on_first_delimiter - after finding a delimiter, stop checking for balanced braces. ( used
		*										in multiple arguements function like save(arg1,arg2) )
        * @return
        * 	Return a vector with all dead characters locations.
        */
	std::vector<std::string> StringSplitIgnoreBraces(const std::string &str, const std::string &delimiters
		, const std::string &open_braces, const std::string &close_braces, std::vector<char> &delimiter_queue
		, const bool allow_consecutive_delimiters=true, const bool drop_check_on_first_delimiter=false); // Main one

	// These are all overloading function of the above
	std::vector<std::string> StringSplitIgnoreBraces(const std::string &str, const char &delimiter
		, const char &open_brace, const char &close_brace, const bool allow_consecutive_delimiters=true);
	std::vector<std::string> StringSplitIgnoreBraces(const std::string &str, const char &delimiter
		, const std::string &open_brace, const std::string &close_brace, const bool allow_consecutive_delimiters=true);
	std::vector<std::string> StringSplitIgnoreBraces(const std::string &str, const char &delimiter
		, const std::string &open_brace	, const std::string &close_brace
		, const bool allow_consecutive_delimiters, const bool drop_check_on_first_delimiter=false);
	std::vector<std::string> StringSplitIgnoreBraces(const std::string &str, const std::string &delimiters
		, const char &open_brace, const char &close_brace
		, const bool allow_consecutive_delimiters=true);
	
	///Checks number of arguements and returns them as a vector.
	std::vector<std::string> CheckNumberOfArguements(const std::string &payload
					, const unsigned int &num_of_arguements=1, const bool contains_graph_name=false);

	///Checks if the given parentheses are balanced and wrapping the given string
	bool CheckWrappingParentheses(const std::string& str, const char &open_par, const char &close_par
		, const bool &allow_throw=true); // Might be used only to check

	/*
		Trim : returns a string with no leading or trailing white spaces
	*/
	const std::string Trim(const std::string& str);
}

#endif
