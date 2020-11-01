#include "Gcalc.h"
#include "Auxiliaries.h"
#include "Exceptions.h"
#include "GraphParser.h"
#include "IOManager.h"
#include <set>


using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::set;
using std::string;

namespace mtm
{
    
    // typedef to define a specific operation
    typedef bool (*gcalc_operation)(const std::string &,std::ostream &output_buffer);



    std::map<const std::string, DiGraph> saved_graphs; // <Graph_name,Graph_Object>


    /*
        ParseOperation - parses a string to an operations (excludes "load")
    */
    gcalc_operation ParseOperation(const std::string &raw_command, std::string &payload);

    /*
        Operations - all operations parsed from coomand line
    */
    bool OperationWho(const  std::string &payload,std::ostream &output_buffer);
    bool OperationPrint(const std::string &payload,std::ostream &output_buffer);
    bool OperationDelete(const std::string &payload,std::ostream &output_buffer);
    bool OperationReset(const std::string &payload,std::ostream &output_buffer);
    bool OperationSave(const std::string &payload,std::ostream &output_buffer);
    bool OperationQuit(const std::string &payload,std::ostream &output_buffer);
    bool OperationNOP(const std::string &payload,std::ostream &output_buffer);
    bool OperationInitialization(const std::string &payload,std::ostream &output_buffer);

    /*
        CheckEmptyPayload - Check a given operation has no payload ( = no arguements/parentheses)
    */
    bool CheckEmptyPayload(const std::string &payload);
    /*
        CheckEmptyPayload - Check a given operation has no payload ( = no arguements/parentheses)
    */

    const std::map<const std::string, gcalc_operation> OPERATIONS_FUNCTIONS={
        { "print", OperationPrint }, // With arguement
        { "delete", OperationDelete }, // With arguement
        { "who", OperationWho },
        { "reset", OperationReset },
        { "save", OperationSave }, // With 2 arguements
        { "quit", OperationQuit },
    };



    void PromptLoop(std::istream &input_buffer, std::ostream &output_buffer)
    {

        gcalc_operation chosen_operation;
        bool should_quit=false;
        do
        {
            string input;
            string payload;
            if (&output_buffer == &cout)
            { // If not batch mode
                output_buffer << PROMPT_NAME;
            }


            if (std::getline(input_buffer, input))
            {
                if (!input.empty() && input[input.size() - 1] == NEW_LINE_CHARACTER)
                {
                    input.erase(input.size() - 1);
                }

                try
                {
                    chosen_operation = ParseOperation(input, payload);
                    should_quit= chosen_operation(payload, output_buffer); //Exectue operation
                }
                catch (const Exception &e)
                {
                    output_buffer << e.what() << endl;
                }
            }
            else
            {   //Reached end of file
                chosen_operation = OperationQuit;
                should_quit=true; // Added this line in resubmission, only change!
            }



        } while (!should_quit);
    }
    bool OperationNOP(const std::string &payload,std::ostream &output_buffer)
    {
        return false;   
    }
    bool OperationPrint(const std::string &payload, std::ostream &output_buffer)
    {
        const string trimmed = Trim(payload);
        const string inner = StringRetrieveInner(trimmed, ARGUEMENTS_PARENTHESES_PREFIX
            , ARGUEMENTS_PARENTHESES_SUFFIX);
        std::vector<string> arguements = CheckNumberOfArguements(inner); // Only 1 arguement
        output_buffer << ParseGraphString((arguements[0]), saved_graphs);
        return false;
    }
    bool OperationSave(const std::string &payload, std::ostream &output_buffer)
    {
        const string trimmed = Trim(payload);

        const string inner = StringRetrieveInner(trimmed, ARGUEMENTS_PARENTHESES_PREFIX
            , ARGUEMENTS_PARENTHESES_SUFFIX);
        std::vector<string> arguements = CheckNumberOfArguements(inner, 2); // Only 2 arguements
        DiGraph graph= ParseGraphString((arguements[0]), saved_graphs);
        const string file_name = Trim(arguements[1]);
        SaveGraphToFile(graph, file_name);
        return false;
    }

    bool OperationDelete(const std::string &payload, std::ostream &output_buffer)
    {
        const string trimmed = Trim(payload);
        const string inner = StringRetrieveInner(trimmed, ARGUEMENTS_PARENTHESES_PREFIX, ARGUEMENTS_PARENTHESES_SUFFIX);
        std::vector<string> arguements = CheckNumberOfArguements(inner); // Only 1 arguement
        string graph_name = Trim(arguements[0]);
        CheckGraphName(graph_name);
        CheckGraphExistence(graph_name, saved_graphs);

        saved_graphs.erase(graph_name);
        return false;
    }
    bool OperationInitialization(const std::string &payload, std::ostream &output_buffer)
    {

        std::vector<string> assignment_operands =StringSplitIgnoreBraces(Trim(payload)
                            , GRAPH_INITIALIZATION_IDENTIFIER, PREDECENDENCE_BRACE_OPEN, PREDECENDENCE_BRACE_CLOSE);
        const string graph_name =Trim(assignment_operands[0]);
        CheckGraphName(graph_name);
        saved_graphs[graph_name] = ParseGraphString((assignment_operands[1]), saved_graphs);
        return false;
    }
    bool OperationWho(const std::string &payload, std::ostream &output_buffer)
    { // No use of payload
        CheckEmptyPayload(payload);
        map<const std::string, DiGraph>::const_iterator it;
        for (it = saved_graphs.begin(); it != saved_graphs.end(); it++)
        {
            output_buffer << (*it).first << endl;
        }
        return false;
    }
    bool OperationReset(const std::string &payload, std::ostream &output_buffer)
    { // No use of payload

        CheckEmptyPayload(payload);
        saved_graphs.clear();
        return false;
    }
    bool OperationQuit(const std::string &payload,std::ostream &output_buffer)
    {
        CheckEmptyPayload(payload);
        return true; // If reached here then this is a valid quit operation
    }
    bool CheckEmptyPayload(const std::string &payload)
    {
        const string trimmed =Trim(payload);
        if (!trimmed.empty())
        {
            throw UnrecognizedCommand(trimmed);
        }
        return false;
    }

    gcalc_operation ParseOperation(const std::string &raw_command, std::string &payload)
    {
        map<const std::string, gcalc_operation>::const_iterator it;

        string trimmed_command = Trim(raw_command); // Allow spacing before command E.G: "  print(g1)"
        if (trimmed_command.empty())
        {
            return OperationNOP;
        }
        std::vector<string> command_segments =StringSplitIgnoreBraces(trimmed_command
                            , GRAPH_INITIALIZATION_IDENTIFIER, PREDECENDENCE_BRACE_OPEN, PREDECENDENCE_BRACE_CLOSE);
        if (command_segments.size()==2)
        {
            payload = trimmed_command;
            return OperationInitialization;
            // Try parse graph ini
        }
        else
        {
            for (it = OPERATIONS_FUNCTIONS.begin(); it != OPERATIONS_FUNCTIONS.end(); it++)
            {
                if (StringStartsWith(trimmed_command, it->first)) // If begins with operation
                {
                    payload = trimmed_command.substr(it->first.size(), trimmed_command.size()-it->first.size());
                    return it->second;
                }
            }
        }
        throw UnrecognizedCommand(trimmed_command);
    }
} // namespace mtm

int main(int argc, char* argv[]) {





    try
    {
        if (argc == 1)
        { // No arguements
            mtm::PromptLoop();
        }
        else if (argc ==3) // Batch mode
        {            
            if (!mtm::CheckBatchFiles(argv[1], argv[2]))
            {
                //throw mtm::FatalExeArgs(string(argv[1])+"," +string(argv[2]));
                throw mtm::FatalExeArgs(string()+ argv[1]+","+argv[2]);
            }
            std::ifstream input_file(argv[1]);
            std::ofstream output_file(argv[2]);
            mtm::PromptLoop(input_file, output_file);            
        }
        else
        {
            throw mtm::FatalExeArgsCount(argc-1);
        }
        return 0;

    }
    catch (const mtm::FatalException& e)
    { // Catch fatal exception defined
        std::cerr << e.what() << endl;
        return -1;
    }
    catch (const std::exception& exception)
    { // Catch other exceptions not defined by program. These will be fata; (includes the bad alloc)
        std::cerr <<mtm::ERROR_PREFIX + mtm::FATAL_MESSAGE+  exception.what() << endl;
        return -2;
    }


}
