##
#	Author: Vishnu Dutt Sharma
#		12EC35018
#	all : compile run 'myshell' only
#	all_mycd: compile and run 'mycd' only
#	all_myls: compile and run 'myls' only
#	all_mymkdir: compile and run 'mymkdir' only
#	all_myps: compile and run 'myps' only
#	all_mymv: compile and run 'mymv' only
#	all_mypwd: compile and run 'mypwd' only
#	all_myrm: compile and run 'myrm' only
# 	all_mytail: compile and run 'mytail' only
#	clean: remove all executables (.out files) 

all: run_myshell

run_myshell:
	@gcc -o myshell.out myshell.c
	@./myshell.out

all_mycd:
	@echo "Compiling and Running mycd"
	@gcc -o mycd.out mycd.c
	@./mycd.out $(INPUT)

all_myls:
	@echo "Compiling and Running myls"
	@gcc -o myls.out myls.c
	@./myls.out

all_mymkdir:
	@echo "Compiling and Running mymymkdir"
	@gcc -o mymkdir.out mymkdir.c
	@./mymkdir.out $(INPUT)

all_myps:
	@echo "Compiling and Running myps"
	@gcc -o myps.out myps.c
	@./myps.out $(INPUT)

all_mymv:
	@echo "Compiling and Running mymv"
	@gcc -o mymv.out mymv.c
	@./mymv.out $(INPUT)

all_mypwd:
	@echo "Compiling and Running mypwd"
	@gcc -o mypwd.out mypwd.c
	@./mypwd.out

all_myrm:
	@echo "Compiling and Running myrm"
	@gcc -o myrm.out mymv.c
	@./myrm.out $(INPUT)

all_mytail:
	@echo "Compiling and Running mytail"
	@gcc -o mytail.out mytail.c
	@./mytail.out $(INPUT)

clean:
	@echo "Cleaning All .out Files "
	@rm -r ./*.out