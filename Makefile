CC=g++
INCLUDE_DIR_DATAHANDLER := $(CURDIR)/include/data_handler
SRC_DATAHANDLER := $(CURDIR)/src/data_handler
CFLAGS := -std=c++11 -g
LIB_DATA := libdata.so

# all

$(LIB_DATA) : libdir objdir obj/data_handler.o obj/data.o
	$(CC) $(CFLAGS) -o $(CURDIR)/lib/$(LIB_DATA) obj/*.o
	rm -r $(CURDIR)/obj

libdir:
	mkdir -p $(CURDIR)/lib

objdir:
	mkdir -p $(CURDIR)/obj

obj/data_handler.o:
	$(CC) -fPIC $(CFLAGS) -o obj/data_handler.o -I$(INCLUDE_DIR_DATAHANDLER) -c $(SRC_DATAHANDLER)/data_handler.cc

obj/data.o:
	$(CC) -fPIC $(CFLAGS) -o obj/data.o -I$(INCLUDE_DIR_DATAHANDLER) -c $(SRC_DATAHANDLER)/data.cc

clean:
	rm -r $(CURDIR)/lib
	rm -r $(CURDIR)/obj