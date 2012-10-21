#include <cmath>
#include <GL/glut.h>
#include <TextEditor.h>
#include <SpinGame.h>
#include <TextEntity.h>
#include <iostream>

using namespace spin;
using namespace std;

TextEditor::TextEditor() 
{
	character_size = Vector( 10, 15 );
	cursor_location = Vector( 0, 0 );
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
		// another character for current line
		else
			current_line += current_char;
	}
}

void TextEditor::AddChar( char c )
{
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
