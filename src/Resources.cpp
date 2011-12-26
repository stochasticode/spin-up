//#include <GL/glew.h>
#include <GL/glut.h>
#include <Resources.h>
#include <SOIL.h>
#include <cstdio>

using namespace spin;

GLuint Resources::LoadPNG(const char *filename, std::string key )
{
	GLuint tex_ID = SOIL_load_OGL_texture(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_POWER_OF_TWO
		| SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_MULTIPLY_ALPHA
		//| SOIL_FLAG_COMPRESS_TO_DXT
		| SOIL_FLAG_DDS_LOAD_DIRECT
		//| SOIL_FLAG_NTSC_SAFE_RGB
		//| SOIL_FLAG_CoCg_Y
		//| SOIL_FLAG_TEXTURE_RECTANGLE
	);
	textures[key] = tex_ID;
	return  tex_ID;
}

bool Resources::BindTexture( std::string key )
{
	// dont re-bind, could be expensive in some implementations
	if( current_texture_key != key )
	{
		// put in place hold texture if  key doesn't exist
		if( textures.find( key ) == textures.end() )
		{
			return false;
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, textures[key]);
			current_texture_key = key;
		}
	}
	return true;
}
