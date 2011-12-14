#include <GrappleGun.h>
#include <cstdio>

namespace spin
{
	GrappleHook::GrappleHook( int new_life_left ): life_left( new_life_left )
	{
		InitBodyCircle( 0, 1, 0 );
		texture_key = "burst";
	}

	void GrappleHook::Tick( int milliseconds )
	{
		BodyEntity::Tick( milliseconds );
	}

	GrappleGun::GrappleGun( int new_max_hooks ): max_hooks( new_max_hooks ), hooks( new_max_hooks )
	{
		for( int i = 0; i < max_hooks; i++ )
			hooks[i] = 0;
	}

	void GrappleGun::ActivateHook( int hook_index )
	{
		if( hook_index >= max_hooks || hook_index < 0 )
			fprintf( stderr, "GrappleGun::ActivateHook -> invalid hook_index: %d (max_hooks=%d)!\n", hook_index, max_hooks );
		else
		{
		}
	}

	void GrappleGun::DeactivateHook( int hook_index )
	{
		if( hook_index >= max_hooks || hook_index < 0 )
			fprintf( stderr, "GrappleGun::DeactivateHook -> invalid hook_index: %d (max_hooks=%d)!\n", hook_index, max_hooks );
		else
		{
		}
	}

	void GrappleGun::Render()
	{
	}

	void GrappleGun::Tick( int milliseconds )
	{
	}

}
