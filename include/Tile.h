#ifndef SPIN_TILE_H
#define SPIN_TILE_H

#include <GL/glut.h>
#include <Surface.h>
#include <Quad.h>
#include <vector>
#include <string>

namespace spin
{
	class Tile: public Quad
	{
		public:
		bool LoadXML( const char* xml_path );
	};
}

#endif
