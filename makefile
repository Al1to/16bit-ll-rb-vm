CC = g++
MAIN = ./src/main.cpp
VM_SOURCE = ./src/source_files/VM.cpp ./src/source_files/CPU.cpp ./src/source_files/MEM.cpp 
NAME = vm

make:
	$(CC) $(MAIN) $(VM_SOURCE) -o $(NAME)

test:
	$(CC) $(MAIN) $(VM_SOURCE) -o $(NAME)
	./$(NAME) -i ./test.asm
	rm -f $(NAME)

clean:
	rm -f $(NAME)
