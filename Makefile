MAIN=ipkcpd

HEADERS_PATH=modules/headers
CFILES_PATH=modules/cfiles

HEADERS=$(CFILES_PATH)/*.h
CFILES=$(CFILES_PATH)/*.c
OBJDIR=obj
# compiler settings
CC=gcc
CFLAGS=-Wall -Werror -pedantic 

OBJS=$(OBJDIR)/calculator.o $(OBJDIR)/tcp_module.o $(OBJDIR)/udp_module.o $(OBJDIR)/$(MAIN).o

all: directories $(MAIN)

directories:
	mkdir -p $(OBJDIR)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJDIR)/%.o: $(CFILES_PATH)/%.c  $(HEADERS_PATH)/%.h
	${CC} -c $< -o $@

$(OBJDIR)/$(MAIN).o: $(MAIN).c
	${CC} -c $< -o $@

run: all
	./$(MAIN) $(ARGS)

.PHONY: clean
clean: 
	rm -rf *.log $(MAIN) $(OBJDIR)

tests:
	echo Under constraction...

