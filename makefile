OBJS := \
	build/SpinGame.o \
	build/Resources.o \
	build/World.o \
	build/Entity.o \
	build/Prop.o \
	build/GrappleGun.o \
	build/SnapConstraint.o \
	build/Kevin.o \
	build/Camera.o \

# chipmunk doesn't trust us...
CXX_ARGS := -DCP_ALLOW_PRIVATE_ACCESS

INCLUDES := -Iinclude/ -Iinclude/soil -Iinclude/chipmunk -Iinclude/tinyxml

bin/spin: src/spin.cpp ${OBJS}
	${CXX} ${CXX_ARGS} -Llib/ ${INCLUDES} -o $@ $< ${OBJS} -lGL -lGLU -lGLEW -lglut -ltinyxml -lsoil -lchipmunk

build/%.o: src/%.cpp
	${CXX} ${CXX_ARGS} ${INCLUDES} -c -o $@ $<

run: bin/spin
	./run-spin

clean:
	rm -f build/* bin/*
