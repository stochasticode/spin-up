#include <QuadEntity.h>
#include <tinyxml.h>
#include <cstdio>

using namespace spin;

bool QuadEntity::TryLoadElement( TiXmlElement* element, bool& error )
{
	error = false;

	// check for NULL
	if( element == 0 )
	{
		fprintf( stderr, "QuadEntity::TryLoadElement -> element was NULL!\n" );
		error = true;
		return false;
	}

	// quad
	if( strcmp( "quad", element->Value() ) == 0 )
	{
		Quad new_quad;
		if( !new_quad.LoadElements( element ) )
		{
			fprintf( stderr, "QuadEntity::LoadXML -> Quad::LoadXML failed!\n" );
			return false;
		}
		quads.push_back( new_quad );
		return true;
	}
	// vec2d
	else if( strcmp( "vec2d", element->Value() ) == 0 )
	{
		std::string name = "";
		Vector vec2d;
		if( !SpinXML::ReadVec2D( element, name, vec2d ) )
		{
			fprintf( stderr, "QuadEntity::LoadXML -> ReadVec2D failed!\n" );
			error = true;
			return false;
		}

		// position
		if( name.compare( "position" ) == 0 )
		{
			position = vec2d;
			return true;
		}
		// unsupported
		else
			return Entity::TryLoadElement( element, error );
	}
	// unsupported
	else
		return Entity::TryLoadElement( element, error );
}

void QuadEntity::Render()
{
	glPushMatrix();
	glTranslatef( position.x, position.y, 0 );
	glRotatef( rotation, 0, 0, 1 );
	glScalef( scale, scale, 1 );

	for( int i = 0; i < quads.size(); i++ )
		quads[i].Render();

	glPopMatrix();
}
