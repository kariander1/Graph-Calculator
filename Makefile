CC=/usr/local/bin/gcc
CXX=/usr/local/bin/g++
OBJS = Gcalc.o GraphParser.o DiGraph.o Auxiliaries.o Exceptions.o IOManager.o
LIB_OBJS = DiGraph.o Exceptions.o Auxiliaries.o PythonInterface.o
PROJ_FILES = Auxiliaries.cpp Auxiliaries.h DiGraph.cpp DiGraph.h Exceptions.cpp Exceptions.h Gcalc.cpp Gcalc.h \
	GraphParser.cpp GraphParser.h IOManager.cpp IOManager.h PythonInterface.cpp PythonInterface.h graph.i Makefile \
	design.pdf test_in.txt test_out.txt
GRAPHS = graph_empty_file.gc graph_extra_edges.gc graph_extra_vertices.gc graph_illegal_semicolon.gc \
	graph_illegal_vertex_name.gc graph_illegal_vertex_name_in_edge_1.gc graph_illegal_vertex_name_in_edge_2.gc \
	graph_same_vertex.gc graph_self_loop.gc graph_too_long.gc graph_unmatched_parentheses.gc \
	graph_vertex_not_exist.gc graph_wrong_edge_vertex_data_1.gc graph_wrong_edge_vertex_data_2.gc \
	graph_wrong_edge_vertex_length_1.gc graph_wrong_edge_vertex_length_2.gc graph_wrong_edges_count.gc \
	graph_wrong_vertex_data.gc graph_wrong_vertex_length.gc graph_wrong_vertices_count.gc \
	temp.gc graph_}{.gc "graph_.<gc" "graph_.>gc"
EXEC = gcalc
ZIP_NAME = $(EXEC).zip
LIB_NAME = libgraph.a
DEBUG_FLAG = #None
COMP_FLAG = -DNDEBUG --pedantic-errors -Wall -Werror -std=c++11
LIB_COMP_FLAG = -fPIC

$(EXEC) : $(OBJS)
	$(CXX) $(DEBUG_FLAG) $(OBJS) -o $@

$(LIB_NAME): $(LIB_OBJS)
	ar -rs $@ $^

Gcalc.o: Gcalc.cpp Gcalc.h DiGraph.h Auxiliaries.h Exceptions.h GraphParser.h IOManager.h
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

Auxiliaries.o:  Auxiliaries.cpp Auxiliaries.h Exceptions.h
	$(CXX) -c $(LIB_COMP_FLAG) $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

DiGraph.o:  DiGraph.cpp DiGraph.h Auxiliaries.h Exceptions.h
	$(CXX) -c $(LIB_COMP_FLAG) $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

Exceptions.o: Exceptions.cpp Exceptions.h
	$(CXX) -c $(LIB_COMP_FLAG) $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

GraphParser.o: GraphParser.cpp GraphParser.h DiGraph.h Auxiliaries.h Exceptions.h IOManager.h
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

IOManager.o: IOManager.cpp IOManager.h DiGraph.h Exceptions.h
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

PythonInterface.o: PythonInterface.cpp DiGraph.h Exceptions.h
	$(CXX) -c $(LIB_COMP_FLAG) $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

tar:
	zip $(ZIP_NAME) $(PROJ_FILES) $(GRAPHS)
clean:
	rm -f $(OBJS) $(LIB_OBJS) $(EXEC) $(ZIP_NAME) $(LIB_NAME)
