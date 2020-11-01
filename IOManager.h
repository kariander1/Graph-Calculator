#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include "DiGraph.h"
#include <fstream>

namespace mtm {
	
		/*
			CheckBatchFiles - checks validity of input/output files for batch mode
		*/
		bool CheckBatchFiles(const std::string &in_file,const std::string &out_name);		
		/*
			SaveGraphToFile - Saves a given graph to a binary name
		*/
		void SaveGraphToFile(const DiGraph &graph, const std::string &file_name);
		/*
			ReadGraphFromFile - returns a new graph loaded from file
		*/
		DiGraph ReadGraphFromFile(const std::string &file_name);		


}

#endif
