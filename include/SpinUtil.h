#ifndef SPIN_UTIL_H
#define SPIN_UTIL_H

namespace spin
{
	class SpinUtil
	{
		public:
		static bool ToFloat( const char* string_in, float& float_out );
		static bool ToInt( const char* string_in, int& int_out );
	};
}

#endif
