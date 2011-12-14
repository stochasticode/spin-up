#include <GrappleGun.h>
#include <SpinGame.h>
#include <cstdio>

namespace spin
{
	GrappleHook::GrappleHook( int new_life_left ): BodyEntity(), life_left( new_life_left )
	{
		InitBodyCircle( 1, 1, 0 );
		size.x = 5;
		size.y = 5;
		texture_key = "burst";
	}

	void GrappleHook::Render()
	{
		Kevin* kevin = SPIN.kevin;
		
		glDisable( GL_TEXTURE_2D );
		glBegin( GL_LINES );
			glColor3f( 1.0, 1.0, 1.0 );
			glVertex3f( position.x, position.y, 0 );
			glVertex3f( kevin->position.x, kevin->position.y, 0 );
			glVertex3f( 1000, 1000, 0 );
		glEnd();
		glEnable( GL_TEXTURE_2D );
		BodyEntity::Render();
	}

	void GrappleHook::Tick( int milliseconds )
	{
		BodyEntity::Tick( milliseconds );
		life_left -= milliseconds;
		if( life_left < 0 )
			dead = true;
	}

	GrappleGun::GrappleGun( int new_max_hooks ): max_hooks( new_max_hooks ), current_hook( 0 )
	{
		if( max_hooks < 0 )
			max_hooks = 0;

		for( int i = 0; i < max_hooks; i++ )
			hooks.push_back( 0 );
	}

	void GrappleGun::ActivateHook( Vector position, Vector direction )
	{
		if( hooks[current_hook] != 0 )
		{
			Entity* old_hook = SPIN.world.GetEntity( hooks[current_hook] );
			if( old_hook != 0 )
				old_hook->dead = true;
		}

		GrappleHook* hook = new GrappleHook( 800 );
		unsigned long hook_id = SPIN.world.AddEntity( hook );
		hook->SetPosition( Vector( position.x, position.y ) );
		hook->SetVelocity( Vector( 300*direction.x, 300*direction.y ) );

		hooks[current_hook] = hook_id;
		/*
		if( hook_index >= max_hooks || hook_index < 0 )
			fprintf( stderr, "GrappleGun::ActivateHook -> invalid hook_index: %d (max_hooks=%d)!\n", hook_index, max_hooks );
		else
		{
		}
		*/
	}

	void GrappleGun::DeactivateHook()
	{
		/*
		if( hook_index >= max_hooks || hook_index < 0 )
			fprintf( stderr, "GrappleGun::DeactivateHook -> invalid hook_index: %d (max_hooks=%d)!\n", hook_index, max_hooks );
		else
		{
		}
		*/
	}

	void GrappleGun::Render()
	{
	}
}
