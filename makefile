TARGET= parchis.exe
OBJFILES= board.o dice.o core.c
TESTFILES= ./unitTest/mainTest.o

default: $(TARGET)

test: $(OBJFILES) $(TESTFILES)
	gcc -o $@ $^ -lncurses -lcunit

$(TARGET): $(OBJFILES) main.o
	gcc -o $@ $^ -lncurses

%.o: %.c
	gcc -c -o $@ $< -lncurses

unitTest/%.o: unitTest/%.c
	gcc -c -o $@ $< -lcunit
