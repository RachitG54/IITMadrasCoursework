CFLAGS = -w -pedantic -ansi -g -std=c++11
all: ReceiverGBN SenderGBN

SenderGBN: SenderGBN.cpp
	${CXX} -o $@ ${CFLAGS} SenderGBN.cpp errexit.cpp -lpthread

ReceiverGBN: ReceiverGBN.cpp
	${CXX} -o $@ ${CFLAGS} ReceiverGBN.cpp errexit.cpp

Thread: Thread.cpp
	${CXX} -o $@ ${CFLAGS} Thread.cpp  -lpthread
