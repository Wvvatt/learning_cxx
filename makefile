CXX=g++
FLAG=-std=c++17 -g -Wall
DEP_INCLUDE=-I./includes
DEP_LIB=-L./libs -pthread -lm 

SOURCE = $(shell find . -name '*.cc')
OBJS = $(patsubst %.cc,%.o,${SOURCE})
TARGETS = $(patsubst %.o,%,${OBJS})

$(info $(OBJS))
$(info $(TARGETS))

all:${TARGETS}

${TARGETS}:%:%.cc
	$(CXX) $(FLAG) $(DEP_INCLUDE) $(DEP_LIB) -o $@ $<

# %.o:%.cc
# 	$(CXX) $(FLAG)  -c -o $@ $<

.PHONY:clean
clean:
	rm -rf *.o $(TARGETS) backtrace
