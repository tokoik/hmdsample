TARGET	= hmdsample
SOURCES	= $(wildcard *.cpp)
HEADERS	= $(wildcard *.h)
OBJECTS	= $(patsubst %.cpp,%.o,$(SOURCES))
CXXFLAGS	= --std=c++11 -Wall -g -D_DEBUG -DGL_GLEXT_PROTOTYPES -Iglfw3/include
LDLIBS	= -Lglfw3/lib/linux -lglfw3 -lGL -lXrandr -lXinerama -lXcursor -lXxf86vm -lXi -lX11 -lpthread -lrt -lm -ldl

.PHONY: clean

$(TARGET): $(OBJECTS)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(TARGET).dep: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -MM $(SOURCES) > $@

clean:
	-$(RM) $(TARGET) *.o *~ .*~ a.out core

-include $(TARGET).dep
