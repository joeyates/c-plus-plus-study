NAME = main

debug: all
run-debug:
	./${NAME}

all: $(NAME).o
	c++ -lstdc++ -o $(NAME) $(NAME).o

compile: $(NAME).o

clean:
	find . -name '*.o' -exec rm -f {} ';'
	find . -name $(NAME) -exec rm -f {} ';'

$(NAME).o: $(NAME).cpp
	gcc -c -I. -o $(NAME).o $(NAME).cpp

.c++.o:
	gcc -c -I. -o $@ $<
