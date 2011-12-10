#ifndef GAL_SURFACE_H
#define GAL_SURFACE_H

struct cpSpace;
struct cpSegmentShape;

namespace spin
{
	class Surface
	{
		public:
		Surface( float new_x1, float new_y1, float new_x2, float new_y2, float new_radius, float new_friction );
		void Render();

		friend class World;

		private:
		cpSegmentShape* shape;
		float radius;
	};
}

#endif
