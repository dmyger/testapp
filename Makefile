TARGET = $(if $(MAKECMDGOALS),$(basename $(firstword $(MAKECMDGOALS))),test)
# CCACHE = /usr/local/bin/ccache

# For Windows set it as 'exe'
OUT_EXT = $(if ($(OS),Windows_NT),exe,out)

ifneq ($(filter $(MAKECMDGOALS),clean),clean)
SOURCES = $(addsuffix .cpp,$(if $(MAKECMDGOALS),$(basename $(MAKECMDGOALS)),$(TARGET)))
HEADERS = $(wildcard *.h)
OBJS = $(filter %.o, $(SOURCES:.cpp=.o)) $(filter %.o, $(SOURCES:.c=.o))
else
# To clean only target you have to call: make <target.out> <clean>
# Use 'true' to skip compile, so we avoid any errors if firstly need to build target.out file.
CCACHE = @true
endif

# SOURCES += glad.c

CXXFLAGS += -Wall -std=gnu++11 -g
# LDLIBS = -lGL -lX11 -lglfw -lpthread -lXrandr -ldl -lXi -lm

ifeq ($(filter $(OS),Windows_NT),Windows_NT)
RM=del /q /f
endif

%.o: %.cpp $(HEADERS)
	$(CCACHE) $(CXX) -c $(CXXFLAGS) $< -o $@

%.o: %.c $(HEADERS)
	$(CCACHE) $(CC) -c $(CFLAGS) $< -o $@

$(TARGET).$(OUT_EXT): $(OBJS)
	$(CCACHE) $(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@

clean:
	$(RM) $(TARGET).o $(TARGET).$(OUT_EXT)

clean-all:
	$(RM) *.o */*.o *.$(OUT_EXT)
