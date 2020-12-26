# change application name here (executable output name)
TARGET=application

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

OBJS=   application.o
OBJS_PATH = Control/client.c
OBJ_SERVER = server.o
OBJ_SERVER_PATH = Control/server.c
OBJ_DATA = object_data.o
OBJ_DATA_PATH = symtab/object_data.c
OBJ_ERROR_INVALID = error_invalid.o
OBJ_ERROR_INVALID_PATH = symtab/error_invalid.c



all: $(OBJS) $(OBJ_DATA) $(OBJ_ERROR_INVALID) server
	$(LD)  $(OBJS) $(OBJ_DATA) $(OBJ_ERROR_INVALID)  $(LDFLAGS) -o $(TARGET)

server: $(OBJ_SERVER_PATH) $(OBJ_DATA) $(OBJ_ERROR_INVALID)
	$(CC) $(OBJ_SERVER_PATH) -ljson-c  $(OBJ_DATA) $(OBJ_ERROR_INVALID)  -o server

#$(OBJ_SERVER): $(OBJ_SERVER_PATH)
#	$(CC) -c $(CCFLAGS) $(OBJ_SERVER_PATH)

$(OBJS): $(OBJS_PATH)
	$(CC) -c $(CCFLAGS) $(OBJS_PATH) $(GTKLIB) -o $(OBJS)

$(OBJ_DATA): $(OBJ_DATA_PATH)
	$(CC) -c $(CCFLAGS) $(OBJ_DATA_PATH)

$(OBJ_ERROR_INVALID): $(OBJ_ERROR_INVALID_PATH)
	$(CC) -c $(CCFLAGS) $(OBJ_ERROR_INVALID_PATH)

clean:
	rm -f *.o $(TARGET) server