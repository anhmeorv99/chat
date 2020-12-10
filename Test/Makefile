# change application name here (executable output name)
TARGET=text_file

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

OBJS=   text_file.o

all: $(OBJS)
	$(LD)  $(OBJS) $(LDFLAGS) -o $(TARGET)

text_file.o: src/text_file.c
	$(CC) -c $(CCFLAGS) src/text_file.c $(GTKLIB) -o text_file.o


clean:
	rm -f *.o $(TARGET)