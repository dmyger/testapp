CXXFLAGS += -Wall -std=gnu++11 -g

ifeq ($(filter $(OS),Windows_NT),Windows_NT)
RM=del /q /f
endif
OUT_EXT = $(if ($(OS),Windows_NT),.exe,) # For Windows set it as 'exe'

all: make_list$(OUT_EXT) load_list$(OUT_EXT)

make_list$(OUT_EXT): make_list.cpp list_node.h
	$(CXX) $(CXXFLAGS) $^ -o $@

load_list$(OUT_EXT): load_list.cpp list_node.h
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	$(RM) *.o make_list$(OUT_EXT) load_list$(OUT_EXT)

