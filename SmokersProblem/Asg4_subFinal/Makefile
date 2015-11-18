all: compile_all run_reception

compile_all:
	@gcc -o chef.out chef.c -lpthread
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

clean:
	@rm -r *.out
