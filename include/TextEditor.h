#ifndef SPIN_TEXT_EDITOR_H
#define SPIN_TEXT_EDITOR_H

#include <string>
#include <vector>

#include <Controls.h>
#include <Vector.h>

namespace spin {

	enum EditorMode { ED_COMMAND, ED_INSERT, ED_COMMAND_TEXT };
	enum EditorAction { ED_WRITE, ED_QUIT };

	class TextEditor : public Controls {
		private:
		std::vector<std::string> buffer;
		Vector character_size;
		Vector cursor_location;
		EditorMode mode;
		std::string command_string;

		void RenderPane();
		void RenderText();
		void RenderCursor();
		void RenderCommandString();

		void ProcessControlEvent_Insert( ControlEvent event );
		void ProcessControlEvent_Command( ControlEvent event );
		void ProcessControlEvent_CommandText( ControlEvent event );

		void ProcessRawKeyEvent_Insert( RawKeyEvent event );
		void ProcessRawKeyEvent_Command( RawKeyEvent event );
		void ProcessRawKeyEvent_CommandText( RawKeyEvent event );

		bool TextAtCursor();
		void ExecuteCommandString();

		protected:
		virtual void ProcessRawKeyEvent( RawKeyEvent event );
		virtual void ProcessControlEvent( ControlEvent event );

		public:
		TextEditor();

		void SetText( std::string text );
		std::string GetText();
		void SetCharAtCursor( char c );
		void Render();
	};
}

#endif
