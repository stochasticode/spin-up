#include <cmath>
#include <GL/glut.h>
#include <TextEditor.h>
#include <SpinGame.h>
#include <TextEntity.h>
#include <iostream>

using namespace spin;
using namespace std;

TextEditor::TextEditor() : Controls() 
{
	character_size = Vector( 10, 15 );
	cursor_location = Vector( 0, 0 );
	mode = ED_COMMAND;
	SetScheme( "editor" );
}

void TextEditor::SetText( string text )
{
	// clear buffer
	buffer.clear();

	// make sure string ends in newline or last line won't get rendered
	text += "\n";

	// parse all characters
	int line_index = 0;
	string current_line = "";
	for( int char_index  = 0; char_index < text.size(); char_index++ )
	{
		char current_char = text[char_index];
		// newline
		if( current_char == '\n' )
		{
			buffer.insert( buffer.end(), current_line );
			current_line = "";
			line_index++;
		}
		else if( current_char == '\t' )
		{
			current_line += "    ";
		}
		// another character for current line
		else
			current_line += current_char;
	}
}

void TextEditor::SetCharAtCursor( char c )
{
	// make sure we are at a valid line
	if( cursor_location.y < buffer.size() )
	{
		// make sure we are at a valid column
		string line = buffer[cursor_location.y];
		if( cursor_location.x < line.size() )
		{
			// change the character
			line[cursor_location.x] = c;
			buffer[cursor_location.y] = line;
			cursor_location.x++;
		}
	}
}

bool TextEditor::TextAtCursor()
{
	return cursor_location.y < buffer.size() && cursor_location.x < buffer[cursor_location.y].size();
}

void TextEditor::Render()
{
	glPushMatrix();
	glLoadIdentity();

	int half_width = SPIN.GetDisplayWidth() / 2;
	int half_height = SPIN.GetDisplayHeight() / 2;
	glTranslatef(-half_width,half_height,0.0);

	RenderPane();
	RenderText();
	RenderCursor();
	RenderCommandString();
	
	glPopMatrix();
}

void TextEditor::RenderPane()
{
	int half_width = SPIN.GetDisplayWidth() / 2;
	int half_height = SPIN.GetDisplayHeight() / 2;

	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
		glColor3f(0.05,0.05,0.05);

		glVertex3f( 0, -SPIN.GetDisplayHeight(), 0.0 );
		glVertex3f( 0,  0, 0.0 );
		glVertex3f( half_width,  0, 0.0 );
		glVertex3f(  half_width, -SPIN.GetDisplayHeight(), 0.0 );
	glEnd();

	glEnable(GL_TEXTURE_2D);
}	

void TextEditor::RenderText()
{
	glPushMatrix();
	glScalef(2.0,2.0,2.0);
	glTranslatef(5,-5,0);

	for( vector<string>::iterator it = buffer.begin(); it != buffer.end(); it++ )
	{
		TextEntity text_entity;
		text_entity.SetText( it->c_str() );
		text_entity.Render();
		glTranslatef(0,-10,0);
	}

	glPopMatrix();
}

void TextEditor::RenderCursor()
{
	
	float blink = sin( glutGet( GLUT_ELAPSED_TIME ) / 120.0 ) / 2.0 + 0.5;

	glPushMatrix();
	glScalef(2.0,2.0,2.0);
	glTranslatef(5,-5,0);
	glDisable(GL_TEXTURE_2D);

	float offset_x = cursor_location.x * 4;
	float offset_y = cursor_location.y * -10;
	glTranslatef( offset_x, offset_y, 0 );

	glBegin(GL_LINE_LOOP);
		glColor4f(1.0,0.0,1.0,blink);

		glVertex3f( -2, -4, 0.0 );
		glVertex3f( -2,  4, 0.0 );
		glVertex3f( 2,  4, 0.0 );
		glVertex3f( 2, -4, 0.0 );
	glEnd();

	glEnable(GL_TEXTURE_2D);

	glPopMatrix();
}

void TextEditor::RenderCommandString()
{
	glPushMatrix();
	glTranslatef(10,-SPIN.GetDisplayHeight() + 15,0);
	glScalef(2.0,2.0,2.0);

	TextEntity text_entity;
	text_entity.SetText( command_string.c_str() );
	text_entity.Render();

	glPopMatrix();
}

void TextEditor::ProcessRawKeyEvent( RawKeyEvent event )
{
	if( mode == ED_INSERT )
		ProcessRawKeyEvent_Insert( event );
	else if( mode == ED_COMMAND )
		ProcessRawKeyEvent_Command( event );
	else if( mode == ED_COMMAND_TEXT )
		ProcessRawKeyEvent_CommandText( event );
}

void TextEditor::ProcessRawKeyEvent_Insert( RawKeyEvent event )
{
	if( event.is_down )
	{
		if( event.key == 8 || (event.key > 31 && event.key < 128))
		{
			// delete
			if( event.key == 8 )
			{
				if( cursor_location.x > 0 )
				{
					cursor_location.x--;
					if( TextAtCursor() )
					{
						SetCharAtCursor( ' ' );
						cursor_location.x--;
					}
				}
			}
			// visible character
			else
				SetCharAtCursor( event.key );
		}
	}
}

void TextEditor::ProcessRawKeyEvent_Command( RawKeyEvent event )
{
	if( event.key == ':' )
	{
		command_string = ":";
		mode = ED_COMMAND_TEXT;
	}
}

void TextEditor::ProcessRawKeyEvent_CommandText( RawKeyEvent event )
{
	if( event.is_down )
	{
		// delete
		if( event.key == 8 )
		{
			// don't delete the ':'
			if( command_string.size() > 1 )
				command_string.resize( command_string.size() - 1);
		}
		// visible character
		else if(event.key > 31 && event.key < 128)
			command_string += event.key;
	}
}

void TextEditor::ProcessControlEvent( ControlEvent event )
{
	if( mode == ED_INSERT )
		ProcessControlEvent_Insert( event );
	else if( mode == ED_COMMAND )
		ProcessControlEvent_Command( event );
	else if( mode == ED_COMMAND_TEXT )
		ProcessControlEvent_CommandText( event );
}

void TextEditor::ProcessControlEvent_Insert( ControlEvent event )
{
	if( event.is_down )
	{
		if( event.function == CTRL_EXIT )
		{
			mode = ED_COMMAND;
			command_string = "";
		}
	}
}

void TextEditor::ProcessControlEvent_Command( ControlEvent event )
{
	if( event.is_down )
	{
		if( event.function == CTRL_LEFT && cursor_location.x > 0 )
			cursor_location.x--;
		else if( event.function == CTRL_UP && cursor_location.y > 0 )
			cursor_location.y--;
		else if( event.function == CTRL_RIGHT )
			cursor_location.x++;
		else if( event.function == CTRL_DOWN )
			cursor_location.y++;
		else if( event.function == CTRL_INSERT )
		{
			mode = ED_INSERT;
			command_string = "INSERT";
		}
	}
}

void TextEditor::ProcessControlEvent_CommandText( ControlEvent event )
{
	if( event.is_down )
	{
		if( event.function == CTRL_EXIT )
		{
			command_string = "";
			mode = ED_COMMAND;
		}
		else if( event.function == CTRL_ACCEPT )
		{
			ExecuteCommandString();
			command_string = "";
			mode = ED_COMMAND;
		}
	}
}

void TextEditor::ExecuteCommandString()
{
	cout << "EXECUTING: " << command_string << endl;
	if( command_string == ":w" )
		SPIN.TextEditorAction( ED_WRITE );
	else if( command_string == ":wq" )
	{
		SPIN.TextEditorAction( ED_WRITE );
		SPIN.TextEditorAction( ED_QUIT );
	}
	else if( command_string == ":q" )
		SPIN.TextEditorAction( ED_QUIT );
}

string TextEditor::GetText()
{
	// TODO: could be more efficient
	string text;
	for( int i = 0; i < buffer.size(); i++ )
		text += buffer[i] + "\n";
	
	return text;
}
