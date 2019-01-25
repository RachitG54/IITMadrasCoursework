CFLAGS = -w -pedantic -ansi -g -std=c++14
TARGET = ospf

all: ospf

ospf: ospf.cpp
	${CXX} -o $@ ${CFLAGS} ospf.cpp errexit.cpp -lpthread
clean:
	$(RM) $(TARGET)
