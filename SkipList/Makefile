# 	Author: Vishnu Dutt Sharma 12EC35018
#	run_skiplist: compile and run skipList.c
#	show_graph: Using graphViz create the graph for myOutput.dot
#	clean: Remove all '.out files'

all: run_skiplist show_graph

run_skiplist:
	@g++ -o skipList.out skipList.cpp
	@./skipList.out

show_graph:
	@dot -Tps myOutput.dot -o outfile.out

clean:
	@rm -r *.out
