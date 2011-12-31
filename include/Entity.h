#ifndef SPIN_ENTITY_H
#define SPIN_ENTITY_H

#include <GL/gl.h>
#include <Vector.h>
#include <Color.h>
#include <Quad.h>
#include <string>
#include <vector>
#include <chipmunk.h>

//struct cpBody;
//struct cpShape;
//struct cpSegmentShape;
//struct cpConstraint;
struct TiXmlElement;

namespace spin
{
	class Entity
	{
		public:
		virtual ~Entity() {}

		virtual void Tick( int milliseconds ) {}
		virtual void Render() {}

		bool dead;
		int id;
		std::string alias;

		protected:
		Entity(): dead( false ), id( -1 ), alias( "UNNAMED" ) {}
	};
}

#endif
