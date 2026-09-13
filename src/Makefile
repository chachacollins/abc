
ifeq ($(OS),Windows_NT)
  BIN := abc.exe
else
  BIN := abc
endif

all:
	gcc -Ofast -flto *.c -o $(BIN)
