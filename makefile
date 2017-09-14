TARGET= parchis.exe
OBJFILES= main.o board.o dice.o core.c

default: $(TARGET)

$(TARGET): $(OBJFILES)
	gcc -o $@ $^ -lncurses

%.o: %.c
	gcc -c -o $@ $< -lncurses
