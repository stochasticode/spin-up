#include <TextEntity.h>
#include <Vector.h>
#include <string.h>
#include <cctype>
#include <tinyxml.h>

using namespace spin;

void TextEntity::SetText( const char* new_text )
{
	// clear previous quads
	quads.clear();

	// add each letter
	for( int i = 0; i < strlen( new_text ); i++ )
		AddLetter( new_text[i], i );
}

void TextEntity::AddLetter( char character, int index )
{
	// do nothing for space
	if( character == ' ' )
		return;

	Quad new_quad;
	new_quad.texture_key = "alphabet";
	new_quad.SetSize( Vector( 8, 8 ) );
	new_quad.position = Vector( index * 8, 0 );

	// make character upper case
	character = toupper( character );

	int character_index = character - 65;

	if( character_index < 0 || character_index > 25 )
	{
		fprintf( stderr, "TextEntity::AddLetter -> invalid character!\n" );
		return;
	}

	int y_ind = character_index / 8;
	int x_ind = character_index % 8;

	new_quad.SetTextureModeRelative( Vector( 64, 64 ), Vector( x_ind*8, y_ind*8 ) );

	quads.push_back( new_quad );
}

bool TextEntity::TryLoadElement( TiXmlElement* element, bool& error )
{
	error = false;

	// check for NULL
	if( element == 0 )
	{
		fprintf( stderr, "TextEntity::TryLoadElement -> element was NULL!\n" );
		error = true;
		return false;
	}

	// param
	if( strcmp( "param", element->Value() ) == 0 )
	{
		std::string name = "";
		std::string value = "";
		SpinXML::ReadParam( element, name, value );

		// text
		if( name.compare( "text" ) == 0 )
			SetText( value.c_str() );
		// unsupported
		else
			return QuadEntity::TryLoadElement( element, error );
	}
	// unsupported
	else
		return QuadEntity::TryLoadElement( element, error );
	
	return true;
}
