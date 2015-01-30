CC=gcc
CXX=g++
CPPFLAGS=-g
LDFLAGS=-g
LDLIBS=

# add any addition source files here (space seperated)
SRCS=SIMPLESEM.cpp

OBJS=$(subst .cpp,.o,$(SRCS))

all: SIMPLESEM

SIMPLESEM: $(OBJS)
	$(CXX) $(LDFLAGS) -o SIMPLESEM $(OBJS) $(LDLIBS)

# dependency rules go here, if needed

clean:
	$(RM) $(OBJS) SIMPLESEM
