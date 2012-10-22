OBJS := \
	build/BodyEntity.o \
	build/Camera.o \
	build/ConstraintEntity.o \
	build/Controls.o \
	build/Entity.o \
	build/GrappleGun.o \
	build/Kevin.o \
	build/LuaBodyEntity.o \
	build/LuaEntity.o \
	build/QuadEntity.o \
	build/Quad.o \
	build/Resources.o \
	build/SnapConstraint.o \
	build/SpinGame.o \
	build/SpinUtil.o \
	build/SpinXML.o \
	build/TextEditor.o \
	build/TextEntity.o \
	build/World.o \

# chipmunk doesn't trust us...
CXX_ARGS := -g -DCP_ALLOW_PRIVATE_ACCESS

INCLUDES := -Iinclude/ -Iinclude/soil -Iinclude/chipmunk -Iinclude/tinyxml

.PHONY: run debug clean

bin/spin: src/spin.cpp ${OBJS}
	${CXX} ${CXX_ARGS} -Llib/ ${INCLUDES} -o $@ $< ${OBJS} -llua5.1 -lGL -lGLU -lGLEW -lglut -ltinyxml -lsoil -lchipmunk

build/%.o: src/%.cpp
	${CXX} ${CXX_ARGS} ${INCLUDES} -c -o $@ $<

run: bin/spin
	./run-spin

debug: bin/spin
	./debug-spin

clean:
	rm -f build/* bin/*
