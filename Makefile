CFLAGS=-ggdb3 -Wall -Werror -std=gnu++11 -pedantic
dedup: dedup.cpp
	g++ $(CFLAGS) -o dedup dedup.cpp

clean:
	rm -f code *~