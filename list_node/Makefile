CXXFLAGS += -Wall -std=gnu++11 -g

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)

ifeq ($(filter $(OS),Windows_NT),Windows_NT)
RM=del /q /f
endif

OUT_EXT = $(if ($(OS),Windows_NT),exe,out) # For Windows set it as 'exe'; for Linux as 'out

%.$(OUT_EXT): %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $< -o $@

all: $(SOURCES:.cpp=.$(OUT_EXT))

clean:
	$(RM) *.$(OUT_EXT) *.bin
