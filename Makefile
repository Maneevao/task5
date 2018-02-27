PROGNAME = task5

CC      = g++
CFLAGS  = -O3 -std=c++11
#CFLAGS = -g -O0 -std=c++11

SOURCES  = task5.cpp Users.cpp ParseShkib.cpp Ngram.cpp

$(PROGNAME): $(SOURCES)
	$(CC) $(CFLAGS) -o $(PROGNAME) $(SOURCES)

clear:
	rm -f $(PROGNAME) result.txt
