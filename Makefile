# If DEBUG = 1, then the program will build with debugging symbols.  If DEBUG = 0, then it will not.
DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG -ggdb
else
	CFLAGS += -DNDEBUG -O3
endif

CC=g++ -std=c++11
CFLAGS += -Wall -Werror
NAME=GIS
LINKS=

SRC := gis.cpp FileTokenizer.cpp GeoFeature.cpp ScriptCommand.cpp
OBJ := $(SRC:.cpp=.o)

.PHONY: default clean

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LINKS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	rm -f *.o *.log $(NAME) $(DEBUGNAME)

