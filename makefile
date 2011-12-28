OBJS := \
	build/SpinGame.o \
	build/Resources.o \
	build/World.o \
	build/Quad.o \
	build/Entity.o \
	build/BodyEntity.o \
	build/GrappleGun.o \
	build/SnapConstraint.o \
	build/Kevin.o \
	build/Camera.o \
	build/SpinXML.o \
	build/SpinUtil.o \

# chipmunk doesn't trust us...
CXX_ARGS := -g -DCP_ALLOW_PRIVATE_ACCESS

INCLUDES := -Iinclude/ -Iinclude/soil -Iinclude/chipmunk -Iinclude/tinyxml

bin/spin: src/spin.cpp ${OBJS}
	${CXX} ${CXX_ARGS} -Llib/ ${INCLUDES} -o $@ $< ${OBJS} -lGL -lGLU -lGLEW -lglut -ltinyxml -lsoil -lchipmunk

build/%.o: src/%.cpp
	${CXX} ${CXX_ARGS} ${INCLUDES} -c -o $@ $<

run: bin/spin
	./run-spin

debug: bin/spin
	./debug-spin

clean:
	rm -f build/* bin/*
