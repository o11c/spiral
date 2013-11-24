prefix=/usr/local
bindir=${prefix}/bin

SOURCES := $(shell find src -name '*.cpp')
HEADERS := $(shell find src -name '*.hpp')
DEPENDS := $(patsubst src/%.cpp,obj/%.d,${SOURCES})
ALL_OBJECTS := $(patsubst %.d,%.o,${DEPENDS})
MAIN_OBJECTS := $(filter %/main.o,${ALL_OBJECTS})
NORM_OBJECTS := $(filter-out %/main.o,${ALL_OBJECTS})
PROGRAMS := $(patsubst obj/%/main.o,bin/%,${MAIN_OBJECTS})

CXXFLAGS = -g -O2 -march=native
CXXFLAGS += ${WARNINGS}

override CXX += -std=c++0x
override CXXFLAGS += -fvisibility=hidden

WARNINGS := -Wall -Wextra -Wformat -Wunused
WARNINGS += -Werror=missing-declarations -Werror=redundant-decls


ifeq "$(shell uname)" "Darwin"
GLUT_LIB = -framework GLUT
GL_LIB = -framework OpenGL -framework Cocoa
else
GLUT_LIB = -lglut
GL_LIB = `pkg-config --libs gl`
endif
override LDLIBS += ${GLUT_LIB} ${GL_LIB}

MKDIR_FIRST = @mkdir -p ${@D}

.DELETE_ON_ERROR:
.SUFFIXES:

all: ${PROGRAMS}

clean:
	rm -rf obj/ bin/
ifndef MAKE_RESTARTS
obj/%.d: src/%.cpp
	$(MKDIR_FIRST)
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -MG -MP -MM $< \
	    -MT '$(patsubst %.d,%.o,$@) $@' \
	    | sed -e 's: \([^ /]*\.glsl\.\(h\|def\)\): src/shaders/\1:g' \
	    > $@
endif

obj/%.o: src/%.cpp
	$(MKDIR_FIRST)
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -c -o $@ $<

%.glsl.h: %.glsl
	sed -e 's/^/"/;s/$$/\\n"/' < $< > $@
%.glsl.def: %.glsl
	sed -n	\
	    -e '/^uniform/{s/^uniform \([^ ]*\) \([^ ]*\);$$/UNIFORM(\2, \1)/;p;}' \
	    -e '/^attribute/{s/^attribute \([^ ]*\) \([^ ]*\);$$/ATTRIBUTE(\2, \1)/;p;}' \
	    < $< > $@

include ${DEPENDS}

obj/most.a: ${NORM_OBJECTS}
	rm -f $@
	ar rcs $@ $^

bin/%: obj/%/main.o obj/most.a
	$(MKDIR_FIRST)
	${CXX} ${LDFLAGS} $^ ${LDLIBS} -o $@

install:
	install -d ${DESTDIR}${bindir}
	install -t ${DESTDIR}${bindir} ${PROGRAMS}

tags: ${SOURCES} ${HEADERS}
	ctags --totals --c-kinds=+px -f $@ $^
