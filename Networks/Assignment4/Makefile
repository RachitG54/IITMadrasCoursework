CFLAGS = -w -pedantic -ansi -g -std=c++11
all: emailclient emailserver

emailclient: emailclient.cpp
	${CXX} -o $@ ${CFLAGS} emailclient.cpp errexit.cpp

emailserver: emailserver.cpp
	${CXX} -o $@ ${CFLAGS} emailserver.cpp errexit.cpp
