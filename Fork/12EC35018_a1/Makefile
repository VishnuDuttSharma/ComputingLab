# Author: Vishnu Dutt Sharma
#			12EC35018
# compile_1a: 	Compile single process module for prime factoriztion
# run_1a: 		Run single process module for prime factorization
# compile_1b: 	Compile multiple process module for prime factorization
# run_1b: 		Run multiple process module for prime factorization
# compile_1c:	Compile single process module for vowel & consonent counting
# run_1c:		Run single process module for vowel & consonent counting
# compile_1d:	Compile multiple process module for vowel & consonent counting
# run_1d:		Run multiple process module for vowel & consonent counting

all: compile_1a run_1a compile_1b run_1b compile_1c run_1c compile_1d run_1d

compile_1a:
	@echo "*** Compiling 1(a) ***"
	@gcc -o forkSingle forkSingle.c -lm
	
run_1a:
	@echo "*** Running 1(a) ***"
	@./forkSingle

compile_1b:
	@echo "*** Compiling 1(b) ***"
	@gcc -o forkMult forkMult.c -lm

run_1b:
	@echo "**** Running 1(b)***"
	@./forkMult

compile_1c:
	@echo "*** Compiling 1(c) ***"
	@gcc -o filSingle fileSingle.c

run_1c:
	@echo "*** Running 1(c) ***"
	@./fileSingle $(INPUT)

compile_1d:
	@echo "*** Compiling 1(d) ***"
	@gcc -o fileMult fileMult.c

run_1d:
	@echo "*** Running 1(d) ***"
	@./fileMult $(INPUT)