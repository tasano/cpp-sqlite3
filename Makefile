CC		= g++
CFLAGS	= -g
TATGET	= cpp-sqlite3
SRCS	= main.cpp sqlite.cpp
OBJS	= $(SRCS:.cpp=.o)
INCDIR	= -I.
LIBS	= -lsqlite3

$(TATGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCDIR) -o $@ -c $<

setup :
	sqlite3 sample.db < schema.sql

run :
	./$(TATGET)

all : clean $(OBJS) $(TATGET)

clean:
	rm -f $(OBJS) $(TATGET) *.db