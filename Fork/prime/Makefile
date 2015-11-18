# Author: Vishnu Dutt Sharma
#			12EC35018
# all:			compiling and running both modules
# all_1a:		Compiling and running module for prime factoriztion using single process
# all_1b:		Compiling and running module for prime factoriztion using multiple process
# compile_1a: 	Compile single process module for prime factoriztion
# run_1a: 		Run single process module for prime factorization
# compile_1b: 	Compile multiple process module for prime factorization
# run_1b: 		Run multiple process module for prime factorization
# clean:		Removes all '.out' files#.If no '.out' file exist in the directory then 'rm' will give error, thus a '.out' file is created
	
all: clean compile_1a run_1a compile_1b run_1b

all_1a: clean compile_1a run_1a 

all_1b: clean compile_1b run_1b 

compile_1a:
	@echo "*** Compiling 1(a) ***"
	@gcc -o forkSingle.out forkSingle.c -lm
	
run_1a:
	@echo "*** Running 1(a) ***"
	@./forkSingle.out

compile_1b:
	@echo "*** Compiling 1(b) ***"
	@gcc -o forkMult.out forkMult.c -lm

run_1b:
	@echo "**** Running 1(b)***"
	@./forkMult.out

clean:
	@echo "*** Cleaning Directory ***"
	@rm -f ./*.out