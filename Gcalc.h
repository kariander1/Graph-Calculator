#ifndef GCALC_H
#define GCALC_H

#include "DiGraph.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace mtm
{
    const std::string PROMPT_NAME = "Gcalc> ";
    const char NEW_LINE_CHARACTER = '\r';
    const char GRAPH_INITIALIZATION_IDENTIFIER ='=';

    /*
        PromptLoop - main program loop, prompting input or receiving input from file
    */
    void PromptLoop(std::istream &input_buffer = std::cin,std::ostream &output_buffer= std::cout);

}

#endif