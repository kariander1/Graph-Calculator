#include "Auxiliaries.h"
#include "Exceptions.h"
#include <stack>
#include <algorithm>

using std::stack;
using std::string;
using std::vector;
namespace mtm
{
    
    std::vector<std::string> StringSplit(const std::string &str, const char &delimiter)
    {

        return StringSplit(str, string(1, delimiter));
    }
    vector<string> StringSplit(const string &str, const string &delimiter)
    {        
        vector<string> sub_strings;
        std::size_t current, previous = 0;
        current = str.find(delimiter) +delimiter.length()-1;
        while (current != string::npos)
        {
            sub_strings.push_back(str.substr(previous, current-(delimiter.length()-1) - previous));
            previous = current + 1;
            current = str.find(delimiter, previous);
        }
        sub_strings.push_back(str.substr(previous, current - previous));
        return sub_strings;
    }

    string StringRetrieveInner(const string& str, const char &open_par, const char &close_par)
    {
        const string trimmed = Trim(str);
        CheckWrappingParentheses(trimmed, open_par, close_par);

        return trimmed.substr(1, trimmed.size()-2);

    }
    std::vector<std::string> StringSplitIgnoreBraces(const std::string &str, const char &delimiter
                , const char &open_brace , const char &close_brace, const bool allow_consecutive_delimiters)
    {
        std::vector<char> queue;
        return StringSplitIgnoreBraces(str, string(1, delimiter), string(1, open_brace), string(1, close_brace)
                                                                                , queue, allow_consecutive_delimiters);
    }
    std::vector<std::string> StringSplitIgnoreBraces(const std::string &str
                                                            , const std::string &delimiters, const char &open_brace
        , const char &close_brace, const bool allow_consecutive_delimiters)
    {
        std::vector<char> queue;
        return StringSplitIgnoreBraces(str, delimiters, string(1, open_brace), string(1, close_brace)
                                                                        , queue, allow_consecutive_delimiters);
    }
    std::vector<std::string> StringSplitIgnoreBraces(const std::string &str, const char &delimiter
        , const std::string &open_brace, const std::string &close_brace, const bool allow_consecutive_delimiters)
    {
        std::vector<char> queue;
        return StringSplitIgnoreBraces(str, string(1, delimiter), open_brace, close_brace, queue
                                                                                    , allow_consecutive_delimiters);
    }

    std::vector<std::string> StringSplitIgnoreBraces(const std::string &str, const char &delimiter
                                , const std::string &open_brace , const std::string &close_brace
                                , const bool allow_consecutive_delimiters, const bool drop_check_on_first_delimiter)
    {
        std::vector<char> queue;
        return StringSplitIgnoreBraces(str, string(1, delimiter), open_brace, close_brace, queue
                                                        , allow_consecutive_delimiters,drop_check_on_first_delimiter);
    }
    std::vector<std::string> CheckNumberOfArguements(const std::string &payload, const unsigned int &num_of_arguements
        , const bool contains_graph_name)
    {
        // This expects to get the inside of the parentheses
        const string trimmed = Trim(payload);
        std::vector<string> segments;

        if (num_of_arguements>1)// Assuming graph parameter is always first
        {
            segments  = StringSplitIgnoreBraces(payload, ARGUEMENT_SEPARATOR
                , string()+ NEW_GRAPH_OPEN_BRACE + ARGUEMENTS_PARENTHESES_PREFIX
                , string()+ NEW_GRAPH_CLOSE_BRACE + ARGUEMENTS_PARENTHESES_SUFFIX, true, true);
        }
        else if (!contains_graph_name)
        {
            segments  = StringSplitIgnoreBraces(payload, ARGUEMENT_SEPARATOR
                , string()+ NEW_GRAPH_OPEN_BRACE + ARGUEMENTS_PARENTHESES_PREFIX
                , string()+ NEW_GRAPH_CLOSE_BRACE + ARGUEMENTS_PARENTHESES_SUFFIX);
        }
        else
        {
            segments =StringSplit(payload, ARGUEMENT_SEPARATOR);
        }

        if (num_of_arguements != segments.size())
        {
            throw ArguementMismatch(num_of_arguements, segments.size());
        }
        return segments;
        // Throw number of arguements mismatch
    }

    const bool StringStartsWith(const std::string &str, const std::string &prefix)
    {
        return (str.rfind(prefix, 0) == 0);
    }
    const bool StringStartsWith(const std::string &str, const char &prefix)
    {
        return StringStartsWith(str, string(1, prefix));
    }
    bool CheckWrappingParentheses(const std::string& str, const char &open_par, const char &close_par
        , const bool &allow_throw)
    {
        bool is_wrapping=true;
        if (str.empty() || !((str[0] ==open_par && str[str.size()-1]==close_par)))
        {
            is_wrapping=false;
        }
        else
        {
            // Check maybe it is  () () 
            unsigned int count=0;
            unsigned int char_pos =0;
            for (const char &character : str)
            {
                if (character == open_par)
                {
                    count++;
                }
                else if (character == close_par)
                {
                    count--;
                    if (count==0 && char_pos<str.size()-1) // Is not last
                    {
                        is_wrapping=false;
                        break;
                    }
                }
                char_pos++;
            }
            /*
            if(count>0)
            { // EG ((   )
                is_wrapping=false;
            }
            */
        }


        if (!is_wrapping && allow_throw)
        {
            throw NoArguements(std::string(1, open_par) +std::string(1, close_par));
        }
        return is_wrapping;

    }

    bool StringContains(const std::string &str, const char &character)
    {
        return (str.find(character) != std::string::npos);

    }
    std::vector<std::string> StringSplitIgnoreBraces(const std::string &str, const string &delimiters, const string &open_braces
        , const string &close_braces, std::vector<char> &delimiter_queue
        , const bool allow_consecutive_delimiters, const bool drop_check_on_first_delimiter)
    {

        vector<string> sub_strings;
        std::stack<size_t> stk; // will store positions in OPEN/CLOSE parentheses consts
        std::size_t previous =0;
        bool last_is_delimiter=false;
        bool check_braces_balancing=true;
        for (std::size_t i = 0; i < str.length(); i++)
        {

            if (stk.empty() && StringContains(delimiters, str[i]))
            {
                if (allow_consecutive_delimiters || !(last_is_delimiter))
                {
                    sub_strings.push_back(str.substr(previous, i - previous));
                    delimiter_queue.push_back(str[i]); // Added delimiter

                    if (drop_check_on_first_delimiter)
                    {
                        check_braces_balancing=false;
                    }
                    previous=i+1;
                    if (i==str.length()-1)
                    {
                        sub_strings.push_back(string());
                    }
                }

                last_is_delimiter=true;


            }
            else if (!(str[i] == WHITE_SPACE))
            {
                last_is_delimiter=false;
            }


            if (check_braces_balancing)
            {
                std::size_t pos;
                if ((pos = open_braces.find(str[i]))  != string::npos)
                {
                    if (stk.empty())
                    {
                        stk.push(pos);
                    }
                    else if (pos == stk.top()) // new patch
                    {
                        stk.push(pos);
                    }
                    continue;

                }
                else if ((pos = close_braces.find(str[i]))  != string::npos)
                {
                    //   if (stk.empty() || stk.top() != pos)
                     //  {
                         //  throw UnmatchedParentheses(open_braces, close_braces);
                       //  int c=1;
                        // c++;
                      // }
                    if (stk.empty())
                    {
                        throw UnmatchedParentheses(open_braces, close_braces);
                    }
                    else if (stk.top() == pos)
                    {
                        stk.pop();
                    }

                }

            }

        }
        if (!stk.empty())
        {
            throw UnmatchedParentheses(open_braces, close_braces);
        }
        if (previous < str.length())
        {
            sub_strings.push_back(str.substr(previous, str.length() - previous));
        }
        return sub_strings;

    }
    const std::string Trim(const std::string& str)
    {

        size_t first = str.find_first_not_of(WHITE_SPACE);
        size_t last = str.find_last_not_of(WHITE_SPACE);
        if (first== string::npos && last == string::npos)
        { // Either blank or only spaces
            return string(); // return blank
        }
        return str.substr(first, (last - first + 1));
    }
} // namespace mtm