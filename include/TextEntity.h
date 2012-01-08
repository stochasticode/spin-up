#ifndef SPIN_TEXT_ENTITY_H
#define SPIN_TEXT_ENTITY_H

#include <QuadEntity.h>

namespace spin
{
	class TextEntity: public QuadEntity
	{
		public:
		void SetText( const char* new_text );

		private:
		void AddLetter( char character, int index );
	};
}

#endif
