CXX=g++
FLAG=-std=c++11 -g -Wall
DEP_INCLUDE=-I./includes
DEP_LIB=-L./libs -pthread -lm 

SOURCE = $(wildcard *.cc)
OBJS = $(patsubst %.cc,%.o,${SOURCE})
TARGET = $(patsubst %.o,%,${OBJS})

all:${TARGET}

${TARGET}:${OBJS}
	$(CXX) $^ $(FLAG) $(DEP_LIB) -o $@

%.o:%.cc
	$(CXX) $(FLAG) $(DEP_INCLUDE) -c $< -o $@

.PHONY:clean
clean:
	rm -rf *.o $(TARGET)
