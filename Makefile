
# Makefile for clang/libc++ projects
# For libc++ on Mac OS X 10.6:
# http://thejohnfreeman.com/blog/2012/11/07/building-libcxx-on-mac-osx-10.6.html

#******************************************************************************

LINK=llvm-link
CC=clang
CXX=clang++
AR=llvm-ar
AS=llvm-as
NM=llvm-nm

#******************************************************************************

EXECNAME = vectest

INCLUDES += -Isrc

VPATH = src

SOURCE = main.cpp
SOURCE += vectest.cpp

DEFINES = 

LIBS += -lc++ -lc++abi

# -U__STRICT_ANSI__ required for math.h bug on OS X 10.6
CFLAGS = -stdlib=libc++ -U__STRICT_ANSI__ -g -O3 -ffast-math -msse4.1
CFLAGS += $(DEFINES) $(INCLUDES)

CXXFLAGS = -std=c++11 $(CFLAGS)

#******************************************************************************
# Generate lists of object and dependency files
#******************************************************************************
CSOURCES = $(filter %.c,$(SOURCE))
CLSOURCES = $(filter %.cl,$(SOURCE))
CPPSOURCES = $(filter %.cpp,$(SOURCE))

BITCODE = $(addprefix bc/, $(CSOURCES:.c=.c.bc)) \
          $(addprefix bc/, $(CLSOURCES:.cl=.cl.bc)) \
          $(addprefix bc/, $(CPPSOURCES:.cpp=.cpp.bc))

#******************************************************************************
# Dependency rules
#******************************************************************************

.PHONY: all default clean depend echo none disasm

default: $(EXECNAME) Makefile

install:

clean:
	rm -rf obj
	rm -rf disasm
	rm -rf bc
	rm -f $(EXECNAME)


$(EXECNAME): $(BITCODE)
	$(CC) $(CFLAGS) $(BITCODE) $(LIBS) -o $@


bc/%.c.bc: %.c
	$(shell if [ ! -d $(addprefix bc/, $(dir $<)) ]; then mkdir -p $(addprefix bc/, $(dir $<)); fi )
	$(CC) -emit-llvm $(CFLAGS) -c $< -o $@

bc/%.cl.bc: %.cl
	$(shell if [ ! -d $(addprefix bc/, $(dir $<)) ]; then mkdir -p $(addprefix bc/, $(dir $<)); fi )
	$(CC) -x cl -emit-llvm $(CFLAGS) -c $< -o $@

bc/%.cpp.bc: %.cpp
	$(shell if [ ! -d $(addprefix bc/, $(dir $<)) ]; then mkdir -p $(addprefix bc/, $(dir $<)); fi )
	$(CXX) -emit-llvm $(CXXFLAGS) -c $< -o $@


#******************************************************************************
# Dependency-generating rules
#******************************************************************************

# ifneq ($(MAKECMDGOALS),clean)
# -include $(addprefix deps/, $(subst .c,.c.d,$(CSOURCES)))
# -include $(addprefix deps/, $(subst .cpp,.cpp.d,$(CPPSOURCES)))
# endif
# 
# deps/%.c.d: %.c
#   $(shell if [ ! -d $(addprefix deps/, $(dir $<)) ]; then mkdir -p $(addprefix deps/, $(dir $<)); fi )
#   $(CC) -M $(CFLAGS) $< > $@.$$$$; \
#   sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
#   rm -f $@.$$$$
# 
# deps/%.cpp.d: %.cpp
#   $(shell if [ ! -d $(addprefix deps/, $(dir $<)) ]; then mkdir -p $(addprefix deps/, $(dir $<)); fi )
#   $(CC) -M $(CFLAGS) $< > $@.$$$$; \
#   sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
#   rm -f $@.$$$$

#******************************************************************************
# End of file
#******************************************************************************