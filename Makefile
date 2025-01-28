CC = clang
CFLAGS = -Og -Wall -Wextra -Wpedantic -fno-omit-frame-pointer
CFLAGS += -fsanitize=undefined -fsanitize=address

%: %.c
	$(CC) $(CFLAGS) -o $@.exe $^

CXX = clang++
CXXFLAGS = -Og -Wall -Wextra -Wpedantic -fno-omit-frame-pointer
CXXFLAGS += -fsanitize=undefined -fsanitize=address
CXXFLAGS += -std=c++23
CXXFLAGS += -Wno-sign-compare

ifeq ($(CXX), clang++)
	CXXFLAGS += -stdlib=libc++ -fuse-ld=lld
endif

%: %.cc
	$(CXX) $(CXXFLAGS) -o $@.exe $^
%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@.exe $^
%: %.cxx
	$(CXX) $(CXXFLAGS) -o $@.exe $^

.PHONY: clean
clean:
	rm -rvf *.s *.o *.exe a.out