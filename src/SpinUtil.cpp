#include <SpinUtil.h>
#include <sstream>

using namespace spin;

bool SpinUtil::ToFloat( const char* string_in, float& float_out )
{
	std::stringstream stream;
	stream << string_in << std::endl;
	float new_float_out = 0.0;
	stream >> new_float_out;

	if( !stream.fail() )
	{
		float_out = new_float_out;
		return true;
	}
	else
		return false;
}

bool SpinUtil::ToInt( const char* string_in, int& int_out )
{
	std::stringstream stream;
	stream << string_in << std::endl;
	int new_int_out = 0;
	stream >> new_int_out;

	if( !stream.fail() )
	{
		int_out = new_int_out;
		return true;
	}
	else
		return false;
}
