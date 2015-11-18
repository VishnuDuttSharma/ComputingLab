all: run_hotel

run_hotel:
	@gcc -o hotel.out hotel.c -lpthread
	@./hotel.out $(INPUT)

run_chef:
	@gcc -o chef.out chef.c -lpthread
	@./chef.out $(INPUT)

run_waiter:
	@gcc -o waiter.out waiter.c -lpthread
	@./waiter.out
