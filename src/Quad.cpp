#include <GL/glut.h>
#include <Quad.h>
#include <SpinGame.h>
#include <SpinXML.h>
#include <tinyxml.h>
#include <cstdio>

using namespace spin;

Quad::Quad(): 
	texture_key( "DEFAULT" ), 
	texture_top_left( 0.0, 0.0 ),
	texture_top_right( 1.0, 0.0 ),
	texture_bottom_left( 0.0, 1.0 ),
	texture_bottom_right( 1.0, 1.0 ),
	position( 0.0, 0.0 ),
	size( 10.0, 10.0 ), 
	rotation( 0.0 ),
	texture_mode( Q_TEXTURE_ABSOLUTE ),
	texture_scale( 1.0, 1.0 ),
	texture_offset( 0.0, 0.0 )
{
}

void Quad::SetTextureModeRelative( Vector new_texture_scale, Vector new_texture_offset )
{
	if( new_texture_scale.x <= 0 || new_texture_scale.y <= 0 )
	{
		fprintf( stderr, "Quad::SetTextureRelative -> invalid texture_scale!\n" );
		return;
	}

	texture_mode = Q_TEXTURE_RELATIVE;
	texture_scale = new_texture_scale;
	texture_offset = new_texture_offset;

	texture_bottom_left =  Vector( texture_offset.x/texture_scale.x, (texture_offset.y + size.y)/texture_scale.y );
	texture_top_left =     Vector( texture_offset.x/texture_scale.x, texture_offset.y/texture_scale.y );
	texture_top_right =    Vector( (texture_offset.x + size.x)/texture_scale.x, texture_offset.y/texture_scale.y );
	texture_bottom_right = Vector( (texture_offset.x + size.x)/texture_scale.x, (texture_offset.y + size.y)/texture_scale.y);
}

void Quad::SetSize( Vector new_size )
{
	size = new_size;
	// reset texture coords if needed
	if( texture_mode == Q_TEXTURE_RELATIVE )
		SetTextureModeRelative( texture_scale, texture_offset );
}

void Quad::Render()
{
	glPushMatrix();
	glTranslatef( position.x, position.y, 0 );
	glRotatef( rotation, 0, 0, 1 );
	glScalef( size.x, size.y, 1 );
	//glScalef( 10, 10, 1 );

	SPIN.resources.BindTexture( texture_key );

	glBegin( GL_QUADS );
		glColor4f( color.r, color.g, color.b, color.a );

		glTexCoord2d( texture_bottom_left.x,     texture_bottom_left.y );     glVertex3f( -0.5, -0.5, 0.0 );
		glTexCoord2d( texture_top_left.x,  texture_top_left.y );  glVertex3f( -0.5,  0.5, 0.0 );
		glTexCoord2d( texture_top_right.x, texture_top_right.y ); glVertex3f(  0.5,  0.5, 0.0 );
		glTexCoord2d( texture_bottom_right.x,    texture_bottom_right.y );    glVertex3f(  0.5, -0.5, 0.0 );
	glEnd();

	glPopMatrix();
}

bool Quad::TryLoadElement( TiXmlElement* element, bool& error )
{
	error = false;

	// param
	if( strcmp( "param", element->Value() ) == 0 )
	{
		std::string name = "";
		std::string value = "";
		if( !ReadParam( element, name, value ) )
		{
			fprintf( stderr, "Quad::TryLoadElement -> ReadParam failed!\n" );
			error = true;
			return false;
		}

		// texture_key
		if( name.compare( "texture_key" ) == 0 )
			texture_key = value;
		// texture mode
		else if( name.compare( "texture_mode" ) == 0 )
		{
			if( value.compare( "relative" ) == 0 )
				texture_mode = Q_TEXTURE_RELATIVE;
			else if( value.compare( "absolute" ) == 0 )
				texture_mode = Q_TEXTURE_ABSOLUTE;
			else
			{
				fprintf( stderr, "Quad::LoadXML -> unsuppored texture_mode: %s!\n", value.c_str() );
				error = true;
				return false;
			}
		}
		// unsupported
		else
			return false;
	}
	// vec2d
	else if( strcmp( "vec2d", element->Value() ) == 0 )
	{
		std::string name = "";
		Vector vec2d;
		if( !SpinXML::ReadVec2D( element, name, vec2d ) )
		{
			fprintf( stderr, "Quad::LoadXML -> ReadVec2D failed!\n" );
			error = true;
			return false;
		}

		// position
		if( name.compare( "position" ) == 0 )
			position = vec2d;
		// size
		else if( name.compare( "size" ) == 0 )
			size = vec2d;
		// texture_scale
		else if( name.compare( "texture_scale" ) == 0 )
			texture_scale = vec2d;
		// texture_offset_
		else if( name.compare( "texture_offset" ) == 0 )
			texture_offset = vec2d;
		// unsupported
		else
			return false;
	}
	// color
	else if( strcmp( "color", element->Value() ) == 0 )
	{
		std::string name = "";
		if( !SpinXML::ReadColor( element, name, color) )
		{
			fprintf( stderr, "Quad::LoadXML -> ReadColor failed!\n" );
			error = true;
			return false;
		}
	}
	// unsupported
	else
		return false;

	return true;
}

void Quad::FinalizeLoadElements()
{
	SetSize( size );
	if( texture_mode == Q_TEXTURE_RELATIVE )
		SetTextureModeRelative( texture_scale, texture_offset );

}
