.SILENT:

COMPILER=g++

# include directories
INCLUDE_DIR := $(CURDIR)/include
INCLUDE_DIR_DATA := ../data_handler/include

# src directories
SRC_DIR := $(CURDIR)/src

BUILD_DIR := $(CURDIR)/build

OBJ_DIR := $(CURDIR)/obj

CFLAGS := -std=c++11 -g

LIB_KNN := libknn.so

all: libknn

libknn: builddir objdir obj/data.o obj/data_handler.o obj/knn.o 
	printf "Building libknn.so of knn module ...\n"
	$(COMPILER) $(CFLAGS) -o $(BUILD_DIR)/$(LIB_KNN) obj/*.o
	rm -r $(CURDIR)/obj
	printf "Builded successfully.\n"


builddir:
	printf "Creating \"$(BUILD_DIR)\"\n"
	mkdir -p $(BUILD_DIR)

objdir:
	printf "Creating \"$(OBJ_DIR)\"\n"
	mkdir -p $(OBJ_DIR)

obj/data_handler.o:
	printf "Compiling data_handler.c into \"$(OBJ_DIR)/data_handler.o\" ...\n"
	$(COMPILER) -fPIC $(CFLAGS) -o obj/data_handler.o -I$(INCLUDE_DIR_DATA) -c $(CURDIR)/../data_handler/src/data_handler.cc

obj/data.o:
	printf "Compiling data.c into \"$(OBJ_DIR)/data.o\" ...\n"
	$(COMPILER) -fPIC $(CFLAGS) -o obj/data.o -I$(INCLUDE_DIR_DATA) -c $(CURDIR)/../data_handler/src/data.cc


obj/knn.o:
	printf "Compiling knn.c into \"$(OBJ_DIR)/knn.o\" ...\n"
	$(COMPILER) -fPIC $(CFLAGS) -o obj/knn.o -I$(INCLUDE_DIR) -I$(INCLUDE_DIR_DATA) -c $(SRC_DIR)/knn.cc

clean:
	printf "Checking existed ...\n"
	@if [ -d "$(BUILD_DIR)" ]; then \
		printf "Removing \"$(BUILD_DIR)\" directory.\n"; \
		rm -r $(BUILD_DIR); \
	fi
	@if [ -d "$(OBJ_DIR)" ]; then \
		printf "Removing \"$(OBJ_DIR)\" directory.\n"; \
		rm -r $(OBJ_DIR); \
	fi
	printf "Totally remove!\n"