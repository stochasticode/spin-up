#ifndef SPIN_CAMERA_H
#define SPIN_CAMERA_H

namespace spin 
{
	class Camera
	{
		public:
		float position_x;
		float position_y;
		float zoom;
		int w_width;
		int w_height;
		int w_half_width;
		int w_half_height;

		void Apply();
		void Reshape( int width, int height );
		void Transform( float& x, float& y );
	};
}

#endif

