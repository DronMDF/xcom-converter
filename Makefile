
CXX ?= g++
CXXFLAGS ?= -O2 -g0 -Wall -Wextra -Weffc++ -Werror

OBJDIR = .obj
OBJECTS = ${patsubst %.cpp,${OBJDIR}/%.o,${wildcard *.cpp}}
TEST_OBJECTS = ${patsubst check/%.cpp,${OBJDIR}/%.o,${wildcard check/*.cpp}}
.PHONY: check install

all: xcom-converter

check: test
	./test --gtest_shuffle

clean:
	rm -rf ${OBJDIR}
	rm -f xcom-converter test

xcom-converter: ${OBJECTS}
	${CXX} -pthread -o $@ $^ `libpng-config --ldflags` -lboost_filesystem -lboost_system

test: ${TEST_OBJECTS} ${OBJECTS}
	${CXX} -pthread -o $@ $^ -lgtest_main -lgtest `libpng-config --ldflags`

${OBJDIR}/%.o : %.cpp ${OBJDIR}/.keep
	${CXX} -MMD -std=c++0x ${CXXFLAGS} -I. `libpng-config --cflags` -c -o $@ $<

${OBJDIR}/%.o : check/%.cpp ${OBJDIR}/.keep
	${CXX} -MMD -std=c++0x ${CXXFLAGS} -I. `libpng-config --cflags` -c -o $@ $<

${OBJDIR}/.keep:
	test -d ${OBJDIR} || mkdir ${OBJDIR}
	touch $@

-include ${patsubst %.o,%.d,${OBJECTS}}
-include ${patsubst %.o,%.d,${TEST_OBJECTS}}
