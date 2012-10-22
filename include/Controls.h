#ifndef SPIN_CONTROLS_H
#define SPIN_CONTROLS_H

#include <map>
#include <vector>
#include <string>

namespace spin
{
	enum ControlFunction { 
		CTRL_UNDEFINED,
		CTRL_LEFT,
		CTRL_RIGHT,
		CTRL_UP,
		CTRL_DOWN,
		CTRL_PRIMARY,
		CTRL_SECONDARY, 
		CTRL_INSERT,
		CTRL_ACCEPT,
		CTRL_DELETE,
		CTRL_EXIT,
	};

	class ControlEvent
	{
		public:
		ControlEvent( ControlFunction new_function, bool new_is_down ): function( new_function ), is_down( new_is_down ) {}

		ControlFunction function;
		bool is_down;
	};

	class RawKeyEvent
	{
		public:
		RawKeyEvent( char new_key, bool new_is_down ): key( new_key ), is_down( new_is_down ) {}

		char key;
		bool is_down;
	};

	class Controls
	{
		public:
		void SetScheme( std::string scheme );

		void MouseDown( int button );
		void MouseUp( int button );
		void KeyDown( char key );
		void KeyUp( char key );

		protected:
		virtual void ProcessControlEvent( ControlEvent event ) = 0;
		virtual void ProcessRawKeyEvent( RawKeyEvent event ) = 0;

		private:
		std::map<char,ControlFunction> key_map;
		std::map<int,ControlFunction> mouse_button_map;

		ControlFunction GetFunction( char key );
	};
}

#endif
