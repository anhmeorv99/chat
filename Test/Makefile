# change application name here (executable output name)
TARGET=time

# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall

PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic

OBJS=   time.o

all: $(OBJS)
	$(LD)  $(OBJS) $(LDFLAGS) -o $(TARGET)

time.o: src/time.c
	$(CC) -c $(CCFLAGS) src/time.c $(GTKLIB) -o time.o


clean:
	rm -f *.o $(TARGET)