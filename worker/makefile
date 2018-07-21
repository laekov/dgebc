CC ?= gcc
CXX ?= g++

CFLAGS = -lpthread -ldl

OBJS = obj/worker.o
LIBS = lib/libcivetweb.a

default : bin/main

bin/% : src/%.cc bin ${LIBS} ${OBJS}
	${CXX} -Iinclude -o $@ $< ${OBJS} ${LIBS} ${CFLAGS}

obj/%.o : src/%.cc include/http obj
	${CXX} ${CFLAGS} -Iinclude -c -o $@ $<

lib/libcivetweb.a : thirdparty/libhttp-1.8 lib
	make -C thirdparty/libhttp-1.8 lib
	ln -f thirdparty/libhttp-1.8/libcivetweb.a lib/libcivetweb.a

include/http : thirdparty/libhttp-1.8
	ln -f thirdparty/libhttp-1.8/include/* include/

thirdparty/libhttp-1.8 : thirdparty/v1.8.tar.gz 
ifeq (,$(wildcard thirdparty/thirdparty/libhttp-1.8))
	cd thirdparty && tar -xf v1.8.tar.gz
endif

thirdparty/v1.8.tar.gz : 
ifeq (,$(wildcard thirdparty/v1.8.tar.gz))
	cd thirdparty && wget https://github.com/lammertb/libhttp/archive/v1.8.tar.gz
endif

lib : 
	mkdir -p lib

bin : 
	mkdir -p bin

obj : 
	mkdir -p obj

clean : 
	rm -rf bin obj
