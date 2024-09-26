.SILENT:

COMPILER=g++
INCLUDE_DIR:= $(CURDIR)/include
SRC_DIR := $(CURDIR)/src
BUILD_DIR := $(CURDIR)/build
LIB_DIR := $(CURDIR)/lib
OBJ_DIR := $(CURDIR)/obj
CFLAGS := -std=c++11 -g

# LIB_DATA := libdata.o
# LIB_KNN := libknn.o


all: executable

executable: build
	$(BUILD_DIR)/exec

build: builddir objdir obj/data.o obj/data_handler.o obj/knn.o obj/main.o
	@if [ -f "exec" ]; then \
		printf "Removing \"exec\" directory.\n"; \
		rm -r exec; \
	else \
		$(COMPILER) $(CFLAGS) obj/*.o -o $(BUILD_DIR)/exec; \
	fi

builddir:
	printf "Creating \"$(BUILD_DIR)\"\n"
	mkdir -p $(BUILD_DIR)

# libdata: libdir objdir obj/data.o obj/data_handler.o
# 	printf "Building libdata.so of data_handler module ...\n"
# 	$(COMPILER) $(CFLAGS) -o $(LIB_DIR)/$(LIB_DATA) obj/*.o
# 	rm -r $(CURDIR)/obj
# 	printf "Builded $(LIB_DATA) successfully.\n"

# libknn: libdir objdir obj/data.o obj/data_handler.o obj/knn.o 
# 	printf "Building libknn.so of knn module ...\n"
# 	$(COMPILER) $(CFLAGS) -o $(LIB_DIR)/$(LIB_KNN) obj/*.o
# 	rm -r $(CURDIR)/obj
# 	printf "Builded $(LIB_KNN) successfully.\n"

# libdir:
# 	printf "Creating \"$(LIB_DIR)\"\n"
# 	mkdir -p $(LIB_DIR)

objdir:
	printf "Creating \"$(OBJ_DIR)\"\n"
	mkdir -p $(OBJ_DIR)

obj/data_handler.o:
	printf "Compiling data_handler.c into \"$(OBJ_DIR)/data_handler.o\" ...\n"
	$(COMPILER) $(CFLAGS) -o obj/data_handler.o -I$(INCLUDE_DIR) -c $(SRC_DIR)/data_handler.cc

obj/data.o:
	printf "Compiling data.c into \"$(OBJ_DIR)/data.o\" ...\n"
	$(COMPILER) $(CFLAGS) -o obj/data.o -I$(INCLUDE_DIR) -c $(SRC_DIR)/data.cc

obj/knn.o:
	printf "Compiling knn.c into \"$(OBJ_DIR)/knn.o\" ...\n"
	$(COMPILER) $(CFLAGS) -o obj/knn.o -I$(INCLUDE_DIR) -c $(SRC_DIR)/knn.cc

obj/main.o:
	printf "Compiling main.c into \"$(OBJ_DIR)/main.o\" ...\n"
	$(COMPILER) $(CFLAGS) -o obj/main.o -I$(INCLUDE_DIR) -c $(SRC_DIR)/main.cc

clean:
	# printf "Checking existed ...\n"
	# @if [ -d "$(LIB_DIR)" ]; then \
	# 	printf "Removing \"$(LIB_DIR)\" directory.\n"; \
	# 	rm -r $(LIB_DIR); \
	# fi

	printf "Checking existed ...\n"
	@if [ -d "$(BUILD_DIR)" ]; then \
		printf "Removing \"$(BUILD_DIR)\" directory.\n"; \
		rm -r $(BUILD_DIR); \
	fi
	
	@if [ -d "$(OBJ_DIR)" ]; then \
		printf "Removing \"$(OBJ_DIR)\" directory.\n"; \
		rm -r $(OBJ_DIR); \
	fi

	@if [ -f "exec" ]; then \
		printf "Removing \"exec\" directory.\n"; \
		rm -r exec; \
	fi

	printf "Totally remove!\n"

	