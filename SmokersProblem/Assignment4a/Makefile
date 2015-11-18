#	Author: Vishnu Dutt Sharma 
#	Roll No. 12EC35018
#	run_chef: Compiles and runs Chef program/process (chef.c)
#	clean: Deletes all '.out' files

all: run_chef 

run_chef:
	@gcc -o chef.out chef.c -lpthread
	@echo "Running Chef program"
	@./chef.out $(INPUT)

clean:
	@echo "Cleaning All .out Files "
	@rm -r ./*.out