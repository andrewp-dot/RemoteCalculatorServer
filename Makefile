MAIN=ipkcpd
#TODO rename file based on settings

# compiler settings
CC=gcc
CFLAGS=-Wall -Werror -pedantic -I./modules/headers

make: ${MAIN}
	${CC} ${CFLAGS} ${MAIN}.c -o ${MAIN}

run: make
	./$(MAIN) 

.PHONY: clean
clean: 
	rm -rf *.log $(MAIN)

tests:
	echo Under constraction...

