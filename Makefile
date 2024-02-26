say_hello:
	@echo "Hello world"

say_bye:
	@echo "Bye"

CC = gcc

cold_run: ./src/main.c
	@${CC} ./src/main.c ./src/erow/erow.c  ./src/keypress/keypress.c -o cold -lcurses
	@./cold ./src/main.c

.DEFAULT_GOAL := cold_run
# .DEFAULT_GOAL := say_bye
