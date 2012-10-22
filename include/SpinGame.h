#ifndef SPIN_GAME_H
#define SPIN_GAME_H

#include <Camera.h>
#include <Controls.h>
#include <Kevin.h>
#include <Resources.h>
#include <TextEditor.h>
#include <World.h>

#define SPIN SpinGame::Instance()

namespace spin 
{
	class SpinGame
	{
		public:
		enum CameraMode { CAMERA_STATIONARY, CAMERA_FOLLOWING }; 

		static SpinGame& Instance();

		bool Init( int argc, char** argv );
		bool InitGraphics();
		bool LoadResources();

		int GetDisplayWidth();
		int GetDisplayHeight();

		void Render();
		void Reshape( int width, int height );
		void Idle();
		void Keyboard( unsigned char key, int x, int y );
		void KeyboardUp( unsigned char key, int x, int y );
		void Motion( int x, int y );
		void Mouse( int button, int state, int x, int y );

		World world;
		Resources resources;
		TextEditor text_editor;
		Camera camera;
		CameraMode camera_mode;
		Kevin* kevin;

		private:
		float mouse_x;
		float mouse_y;
		int display_width;
		int display_height;

		SpinGame() {}
		SpinGame( SpinGame const& game );
		void operator = ( SpinGame const& game );
	};
}

#endif
