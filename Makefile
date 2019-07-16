
MAIN_SRC = main.c

SOURCES := ${MAIN_SRC}

MAIN_NAME = sndmanip

.PHONY: all clean lint

all: $(MAIN_NAME)

$(MAIN_NAME):
	gcc $(MAIN_SRC) -Wall -o $(MAIN_NAME)

clean:
	rm $(MAIN_NAME) $(SOURCES_O) $(SOURCES_GCH)

run:
	./$(MAIN_NAME)

memtest:
	valgrind --track-origins=yes --leak-check=yes\
 ./$(MAIN_NAME)

lint:
	cppcheck --language=c --enable=warning,style,performance,portability,unusedFunction --template=gcc\
 $(SOURCES)
