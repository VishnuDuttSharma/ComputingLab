all: compile_all run_reception

compile_all:
	@gcc -o chef.out chef.c -lpthread
	@gcc -o chefBinary.out chefBinary.c -lpthread
	@gcc -o waiter.out waiter.c -lpthread
	@gcc -o serviceRoom.out serviceRoom.c -lpthread


run_reception:
	@gcc -o reception.out reception.c -lpthread
	@./reception.out $(INPUT)

run_chef:
	@gcc -o chef.out chef.c -lpthread
	@./chef.out $(INPUT)

run_waiter:
	@gcc -o waiter.out waiter.c -lpthread
	@./waiter.out $(INPUT)

run_service:
	@gcc -o serviceRoom.out serviceRoom.c -lpthread
	@./serviceRoom.out

run_test:
	@gcc -o tests.out tests.c -pthread
	@./tests.out

run_testC:
	@gcc -o testC.out testC.c -pthread
	@./testC.out

clean:
	@rm -r *.out
