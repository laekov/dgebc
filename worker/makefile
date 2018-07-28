CC ?= gcc
CXX ?= g++

CFLAGS = -std=c++11 -lpthread -ldl -g -DDGEBC_ENGINE_NO_VISUALIZE

OBJS = obj/worker.o obj/scheduler.o obj/proxy.o
LIBS = lib/libcivetweb.a ../engine/libengine.so ../engine/lib/libBox2D.a

default: bin/main

run: bin/main
	LD_LIBRARY_PATH=../engine bin/main -w 90 

bin/% : src/%.cc bin ${LIBS} ${OBJS} 
	${CXX} -Iinclude -o $@ $< ${OBJS} ${LIBS} ${CFLAGS}

obj/%.o : src/%.cc include/civetweb.h obj
	${CXX} ${CFLAGS} -Iinclude -c -o $@ $<

lib/libcivetweb.a : thirdparty/libhttp-1.8 lib
	make -C thirdparty/libhttp-1.8 lib
	ln -f thirdparty/libhttp-1.8/libcivetweb.a lib/libcivetweb.a

include/civetweb.h: thirdparty/libhttp-1.8
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
ifeq (,$(wildcard $@))
	mkdir -p $@
endif

obj : 
ifeq (,$(wildcard $@))
	mkdir -p $@
endif

bin : 
ifeq (,$(wildcard $@))
	mkdir -p $@
endif
