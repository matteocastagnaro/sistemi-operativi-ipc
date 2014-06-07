CC:= gcc
LD:= gcc
NAME:= prog
RAM:= ram
CPU:= cpu
CFLAGS:= -c -Wall -Iinclude -Iheaders
LDFLAGS:= -Llib 
LDLIBS:= -lm

SRCS:= src/main.c src/utils.c src/shm.c src/sem.c
SRCS_R:= src/ram.c src/utils.c src/shm.c src/sem.c
SRCS_C:= src/cpu.c src/utils.c src/sem.c src/shm.c

OBJS:= $(SRCS:.c=.o)
OBJS_R:= $(SRCS_R:.c=.o)
OBJS_C:= $(SRCS_C:.c=.o)

all: $(NAME) $(RAM) $(CPU)

$(NAME): $(OBJS)
	@$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
$(RAM): $(OBJS_R)
	@$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
$(CPU): $(OBJS_C)
	@$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ $< 

clean: 
	@rm $(NAME) $(RAM) $(CPU) src/*.o

.PHONY: all clean 
