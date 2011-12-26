#ifndef SPIN_XML_H
#define SPIN_XML_H

#include <string>

struct TiXmlElement;

namespace spin
{
	class Entity;
	class ConstraintEntity;
	class SnapConstraint;
	class QuadEntity;
	class Vector;

	class SpinXML
	{
		public:
		static bool ReadVec2D( TiXmlElement* element, Vector& vector_out );
		static bool ReadParam( TiXmlElement* element, std::string& value );
		static bool ReadEntity( TiXmlElement* elment, Entity** entity_out  );
		static bool ReadConstraint( TiXmlElement* elment, ConstraintEntity** constraint_out  );

		private:
		static bool ReadQuadEntity( TiXmlElement* element, QuadEntity** entity_out );
		static bool ReadSnapConstraint( TiXmlElement* element, SnapConstraint** entity_out );
	};
}

#endif
