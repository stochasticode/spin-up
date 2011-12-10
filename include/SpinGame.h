#ifndef SPIN_GAME_H
#define SPIN_GAME_H

#include <Camera.h>
#include <Quad.h>
#include <World.h>
#include <Resources.h>

#define SPIN SpinGame::Instance()

namespace spin 
{
	class SpinGame
	{
		public:
		static SpinGame& Instance();

		bool Init( int argc, char** argv );
		bool InitGraphics();
		bool LoadResources();

		void Render();
		void Reshape( int width, int height );
		void Idle();
		void Keyboard( unsigned char key, int x, int y );
		void KeyboardUp( unsigned char key, int x, int y );
		void Motion( int x, int y );
		void Mouse( int button, int state, int x, int y );

		World world;
		Resources resources;
		Camera camera;

		private:
		float mouse_x;
		float mouse_y;
		//float fps;
		//int last_fps_time;
		//int frames_since_last_fps;

		Quad background;

		SpinGame() {}
		SpinGame( SpinGame const& game );
		void operator = ( SpinGame const& game );
	};
}
#endif
