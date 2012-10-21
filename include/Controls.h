#ifndef SPIN_CONTROLS_H
#define SPIN_CONTROLS_H

#include <map>
#include <vector>

namespace spin
{
	enum ControlFunction { CTRL_LEFT, CTRL_RIGHT, CTRL_UP, CTRL_DOWN, CTRL_PRIMARY, CTRL_SECONDARY };

	class ControlHandler
	{
		public:
		virtual void ControlDown( ControlFunction function ) = 0;
		virtual void ControlUp( ControlFunction function ) = 0;
	};

	class Controls
	{
		public:
		Controls();

		void KeyDown( char key );

		private:
		std::map<char,ControlFunction> key_map;
		std::map<int,ControlFunction> mouse_button_map;
		std::vector<ControlHandler> handlers;
	};
}

#endif
