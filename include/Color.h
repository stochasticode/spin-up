#ifndef SPIN_COLOR_H
#define SPIN_COLOR_H

namespace spin
{
	class Color
	{
		public:
		Color(): r( 1 ), g( 1 ), b( 1 ), a( 1 ) {}
		Color( float new_r, float new_g, float new_b, float new_a ): r( new_r ), g( new_g ), b( new_b ), a( new_a ) {}
		
		float r;
		float g;
		float b;
		float a;
	};
}

#endif
