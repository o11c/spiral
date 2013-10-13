prefix=/usr/local
bindir=${prefix}/bin

SOURCES := $(wildcard src/*.cpp)
HEADERS := $(wildcard src/*.hpp)
DEPENDS := $(patsubst src/%.cpp,obj/%.d,${SOURCES})
ALL_OBJECTS := $(patsubst %.d,%.o,${DEPENDS})
MAIN_OBJECTS := $(filter %-main.o,${ALL_OBJECTS})
NORM_OBJECTS := $(filter-out %-main.o,${ALL_OBJECTS})
PROGRAMS := $(patsubst obj/%-main.o,bin/%,${MAIN_OBJECTS})

CXXFLAGS = -g -O2 -march=native
CXXFLAGS += ${WARNINGS}

override CXX += -std=c++0x
override CXXFLAGS += -fvisibility=hidden

WARNINGS := -Wall -Wextra -Wformat -Wunused
WARNINGS += -Werror=missing-declarations -Werror=redundant-decls

override LDLIBS += -lglut `pkg-config --libs gl`

MKDIR_FIRST = @mkdir -p ${@D}

.DELETE_ON_ERROR:
.SUFFIXES:

all: ${PROGRAMS}

clean:
	rm -r obj/ bin/
obj/%.d: src/%.cpp
	$(MKDIR_FIRST)
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -MG -MP -MM $< \
	    -MT '$(patsubst %.d,%.o,$@) $@' \
	    | sed -e 's:[^ ]*\.glsl\.\(h\|def\):src/&:g;s:src/src/:src/:g' \
	    > $@

obj/%.o: src/%.cpp
	$(MKDIR_FIRST)
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o $@ $<

%.glsl.h: %.glsl
	sed -e 's/^/"/;s/$$/\\n"/' < $< > $@
%.glsl.def: %.glsl
	sed -ne 's/^uniform \([^ ]*\) \([^ ]*\);$$/UNIFORM(\2, \1)/;s/^attribute \([^ ]*\) \([^ ]*\);$$/ATTRIBUTE(\2, \1)/;T;p' < $< > $@

include ${DEPENDS}

bin/%: obj/%-main.o ${NORM_OBJECTS}
	$(MKDIR_FIRST)
	${CXX} ${LDFLAGS} $^ ${LDLIBS} -o $@

install:
	install -d ${DESTDIR}${bindir}
	install -t ${DESTDIR}${bindir} ${PROGRAMS}

tags: ${SOURCES} ${HEADERS}
	ctags --totals --c-kinds=+px -f $@ $^
