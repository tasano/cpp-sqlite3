CC		= g++
TATGET	= cpp-sqlite3
SRCS	= main.cpp
OBJS	= $(SRCS:.cpp=.o)
LIBS	= -lsqlite3

$(TATGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

$(OBJS): $(SRCS)
	$(CC) -c $(SRCS)

setup :
	sqlite3 sample.db < schema.sql

all : clean $(OBJS) $(TATGET)

clean:
	rm -f $(OBJS) $(TATGET)