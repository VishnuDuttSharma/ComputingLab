# 	Author: Vishnu Dutt Sharma 12EC35018
#	run_skiplist: compile and run skip.cpp
#	show_graph: Using graphViz create the graph for myOutput.dot
#	clean: Remove all '.out files'

all: run_list show_graph

run_list:
	@g++ -o skip.out skip.cpp
	@./skip.out

show_graph:
	@dot -Tps myOutput.dot -o outfile.out

clean:
	@rm -r *.out
