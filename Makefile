BASEPROJ?=.

FILENAME=$(BASEPROJ)/config.mk
ifeq ("$(wildcard $(FILENAME))",)
else
include $(BASEPROJ)/config.mk
endif

CFG_CC?=g++-4.9 -Wall
CFG_CXX?=g++-4.9 -Wall

CFLAGS=

INCLUDE=-I include/
CPPFLAGS=$(CFLAGS) $(INCLUDE)
LIBS:=libutil.a libarray.a libmatrix.a libprofile.a libpbar.a

SOURCES=$(patsubst src/%, %, $(wildcard src/*.cpp) $(wildcard src/*.c))

all: example libs ctags
#=================================================================
vpath %.h include/
vpath %.c src/
vpath %.cpp src/
vpath %.o obj/

OBJ_TEMP=$(SOURCES:.cpp=.o)
OBJ=$(addprefix obj/, $(OBJ_TEMP:.c=.o))
#=================================================================

.PHONY: debug o3
debug: CFG_CXX+=-DDEBUG -g
debug: all
o3: CFG_CXX+=-o3
o3: all

example: example.cpp libs
	$(CFG_CXX) $(CPPFLAGS) -o $@ $< -L lib/ -lmatrix -lpbar

libs: $(OBJ) $(addprefix lib/, $(LIBS))
lib/lib%.a: %.o %.cpp
	ar rcs $@ $<
	ranlib $@
install:
	mkdir -p /usr/local/boton/lib/
	cp lib/* /usr/local/boton/lib/
	mkdir -p /usr/local/boton/include/
	cp include/* /usr/local/boton/include/
ctags:
	@ctags -R *
#=================================================================
obj/%.o: %.c
	$(CFG_CC) $(CFLAGS) $(EXTRA_FLAGS) -o $@ -c $<
obj/%.o: %.cpp
	$(CFG_CXX) $(CPPFLAGS) $(EXTRA_FLAGS) -o $@ -c $<

obj/%.d: %.cpp
	@$(CFG_CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,obj/\1.o $@ : ,g' < $@.$$$$ > $@;\
	rm -f $@.$$$$

-include $(patsubst %.o, %.d, $(OBJ))
#=================================================================

.PHONY: 
clean: ctags
	rm -rf obj/* lib/*.a
	rm -rf example 
cleanest: ctags clean
	rm -rf obj/*
