#ifndef SPIN_TEXT_EDITOR_H
#define SPIN_TEXT_EDITOR_H

#include <vector>
#include <string>
#include <Vector.h>

namespace spin {
	class TextEditor {
		private:
		std::vector<std::string> buffer;
		Vector character_size;
		Vector cursor_location;


		void RenderPane();
		void RenderText();
		void RenderCursor();

		public:
		TextEditor();

		void SetText( std::string text );
		void AddChar( char c );
		void Render();
	};
}

#endif
