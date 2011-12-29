#include <GL/glut.h>
#include <Quad.h>
#include <SpinGame.h>
#include <SpinXML.h>
#include <tinyxml.h>
#include <cstdio>

using namespace spin;

Quad::Quad(): 
	texture_key( "DEFAULT" ), 
	position( 0.0, 0.0 ),
	size( 10.0, 10.0 ), 
	rotation( 0.0 ),
	texture_top_left( 0.0, 1.0 ),
	texture_top_right( 1.0, 1.0 ),
	texture_bottom_left( 0.0, 0.0 ),
	texture_bottom_right( 1.0, 0.0 )
{
}

void Quad::Render()
{
	glPushMatrix();
	glTranslatef( position.x, position.y, 0 );
	glRotatef( rotation, 0, 0, 1 );
	glScalef( size.x, size.y, 1 );

	SPIN.resources.BindTexture( texture_key );

	glBegin(GL_TRIANGLES);
		glColor4f( color.r, color.g, color.b, color.a );
		glTexCoord2d( texture_top_left.x,    texture_top_left.y );    glVertex3f( -0.5, -0.5, 0.0 );
		glTexCoord2d( texture_top_right.x,   texture_top_right.y );   glVertex3f(  0.5, -0.5, 0.0 );
		glTexCoord2d( texture_bottom_left.x, texture_bottom_left.x ); glVertex3f( -0.5,  0.5, 0.0 );

		glTexCoord2d( texture_top_right.x,    texture_top_right.y );    glVertex3f(  0.5, -0.5, 0 );
		glTexCoord2d( texture_bottom_left.x,  texture_bottom_left.y );  glVertex3f( -0.5,  0.5, 0 );
		glTexCoord2d( texture_bottom_right.x, texture_bottom_right.y ); glVertex3f(  0.5,  0.5, 0 );
	glEnd();

	glPopMatrix();
}

bool Quad::LoadXML( TiXmlElement* element )
{
	// iterate through children
	TiXmlElement* child = element->FirstChildElement();
	while( child != 0 )
	{
		// param
		if( strcmp( "param", child->Value() ) == 0 )
		{
			std::string name = "";
			std::string value = "";
			SpinXML::ReadParam( child, name, value );

			// texture_key
			if( name.compare( "texture_key" ) == 0 )
				texture_key = value;
			// unsupported
			else
				fprintf( stderr, "Quad::LoadXML -> unsuppored param: %s!\n", name.c_str() );
		}
		// vec2d
		else if( strcmp( "vec2d", child->Value() ) == 0 )
		{
			std::string name = "";
			Vector vec2d;
			if( !SpinXML::ReadVec2D( child, name, vec2d ) )
			{
				fprintf( stderr, "Quad::LoadXML -> ReadVec2D failed!\n" );
				return false;
			}

			// position
			if( name.compare( "position" ) == 0 )
				position = vec2d;
			// size
			else if( name.compare( "size" ) == 0 )
				size = vec2d;
			// unsupported
			else
				fprintf( stderr, "Quad::LoadXML -> unsuppored vec2d: %s!\n", name.c_str() );
		}
		child = child->NextSiblingElement();
	}
	return true;
}
