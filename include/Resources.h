#ifndef GAL_RESOURCES_H
#define GAL_RESOURCES_H

#include <GL/glut.h>
#include <string>
#include <map>

namespace spin
{
	class Resources
	{
		public:
		GLuint LoadPNG( const char* file_path, std::string key );
		std::map<std::string,GLuint> textures;

		bool BindTexture( std::string key );

		private:
		std::string current_texture_key;
		//short le_short( unsigned char *bytes );
		//void* read_tga( const char* filename, int& width, int& height );
		//void* file_contents( const char *filename, GLint *length );
		//void* read_tga(const char *filename, int *width, int *height);
	};
}

#endif
