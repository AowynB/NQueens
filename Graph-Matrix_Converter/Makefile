## For debugging purposes, make will do, but if you
## want a "production ready" binary (I.E., no debugging
## info for extra efficiency), you want to do make GMC.
CPPC=g++
CPPARGS=-Ofast -std=c++20 -pedantic -fsanitize=address


GMC_DEBUG: main.cpp
	$(CPPC) -o $@ -g $(CPPARGS) $^

GMC: main.cpp
	$(CPPC) -o $@ $(CPPARGS) $^

