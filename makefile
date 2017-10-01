TARGET= parchis.exe
OBJFILES= board.o dice.o core.c
TESTFILES= ./unitTest/mainTest.o

default: $(TARGET)

test: $(OBJFILES) $(TESTFILES)
	gcc -o $@ $^ -lncurses -lcunit -g

$(TARGET): $(OBJFILES) main.o
	gcc -o $@ $^ -lncurses -g

%.o: %.c
	gcc -c -o $@ $< -lncurses -g

unitTest/%.o: unitTest/%.c
	gcc -c -o $@ $< -lcunit -g

.phony:
clean:
	@rm ./*.o -f
	@rm ./test/*.o -f
	@rm parchis.exe -f
	@rm test.exe -f
