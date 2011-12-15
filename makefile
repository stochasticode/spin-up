OBJS := \
	build/SpinGame.o \
	build/Resources.o \
	build/World.o \
	build/Entity.o \
	build/GrappleGun.o \
	build/GrappleConstraint.o \
	build/Kevin.o \
	build/Camera.o \

INCLUDES := -Iinclude/ -Iinclude/soil -Iinclude/chipmunk -Iinclude/tinyxml

bin/spin: src/spin.cpp ${OBJS}
	${CXX} -Llib/ ${INCLUDES} -o $@ $< ${OBJS} -lGL -lGLU -lGLEW -lglut -ltinyxml -lsoil -lchipmunk

build/%.o: src/%.cpp
	${CXX} ${INCLUDES} -c -o $@ $<

run: bin/spin
	./run-spin

clean:
	rm -f build/*
