#ifndef SPIN_LUA_ENTITY
#define SPIN_LUA_ENTITY

#include <Entity.h>

struct lua_State;

namespace spin
{
	class LuaEntity: public Entity
	{
		public:
		LuaEntity();
		~LuaEntity();

		virtual void SetID( int new_id );

		bool LoadScript( const char* script );
		virtual void Tick( int milliseconds );

		virtual bool TryLoadElement( TiXmlElement* element, bool& error );

		protected:
		lua_State* lua_state;

		static int lua_KillEntity( lua_State *L );
		static int lua_LoadEntity( lua_State *L );
		static int lua_QuadEntitySetPosition( lua_State *L );
	};
}


#endif
