CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		genetic.o RNG.o getch.o

LIBS =

TARGET =	genetic

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
