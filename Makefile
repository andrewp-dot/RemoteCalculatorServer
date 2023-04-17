MAIN=ipkcpd

HEADERS_PATH=modules/headers
CFILES_PATH=modules/cfiles

HEADERS=$(CFILES_PATH)/*.h
CFILES=$(CFILES_PATH)/*.c
OBJDIR=obj

LOGIN=xponec01

DEBUG_MACRO=NONE
# DEBUG_MACRO=TEST_CALC
# DEBUG_MACRO=TEST_ARGS

# compiler settings
CC=gcc
CFLAGS=-Wall -Werror -pedantic

OBJS=$(OBJDIR)/fraction.o $(OBJDIR)/calculator.o $(OBJDIR)/tcp_module.o $(OBJDIR)/udp_module.o $(OBJDIR)/$(MAIN).o $(OBJDIR)/calc_stack.o

all: directories $(MAIN)

directories:
	mkdir -p $(OBJDIR)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS)  $(OBJS) -o $@

$(OBJDIR)/%.o: $(CFILES_PATH)/%.c  $(HEADERS_PATH)/%.h
	${CC} -c $< -o $@

$(OBJDIR)/$(MAIN).o: $(MAIN).c
	${CC} -D$(DEBUG_MACRO) -c $< -o $@

run: all
	./$(MAIN) $(ARGS)

.PHONY: clean
clean: 
	rm -rf *.log $(MAIN) $(OBJDIR)

zip:
	zip -r $(LOGIN).zip Makefile LICENSE README.md ipkcpd.c modules/* tests/*

arg_test:
	make clean
	make DEBUG_MACRO="TEST_ARGS" 
	python3 test_inputs.py

calc_test:
	make clean
	make run DEBUG_MACRO="TEST_CALC" ARGS="-h 127.0.0.1 -p 2023 -m udp"

