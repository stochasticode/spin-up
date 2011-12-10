#ifndef SPIN_VECTOR_H
#define SPIN_VECTOR_H

namespace spin 
{
	class Vector
	{
		public:
		Vector(): x( 0 ), y( 0 ) {}
		Vector( double new_x, double new_y ): x( new_x ), y( new_y ) {}

		double x;
		double y;
	};
}

#endif
