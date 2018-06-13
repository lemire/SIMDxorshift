# minimalist makefile
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h
ifeq ($(DEBUG),1)
CFLAGS = -fPIC  -std=c99 -ggdb -mavx2 -march=native -Wall -Wextra -pedantic -Wshadow -fsanitize=undefined  -fno-omit-frame-pointer -fsanitize=address
else
CFLAGS = -fPIC -std=c99 -O3 -mavx2  -march=native -Wall -Wextra -pedantic -Wshadow
endif # debug

HEADERS=include/simdxorshift128plus.h  include/xorshift128plus.h

OBJECTS= simdxorshift128plus.o xorshift128plus.o

all: $(OBJECTS) fillarray shuffle

xorshift128plus.o: ./src/xorshift128plus.c $(HEADERS)
	$(CC) $(CFLAGS) -c ./src/xorshift128plus.c -Iinclude

simdxorshift128plus.o: ./src/simdxorshift128plus.c $(HEADERS)
	$(CC) $(CFLAGS) -c ./src/simdxorshift128plus.c -Iinclude

fillarray: ./benchmark/fillarray.c $(HEADERS)  $(OBJECTS)
	$(CC) $(CFLAGS) -o fillarray ./benchmark/fillarray.c $(OBJECTS) -Iinclude -flto

shuffle: ./benchmark/shuffle.c $(HEADERS)  $(OBJECTS)
	$(CC) $(CFLAGS) -o shuffle ./benchmark/shuffle.c $(OBJECTS) -Iinclude -flto


clean:
	rm -f $(OBJECTS) fillarray shuffle
