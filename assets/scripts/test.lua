total_tick = 0

function Init()
	-- set up camera
	SetCameraMode( "stationary" );
	SetCameraPosition( 0, -50 );
end

function Tick( milliseconds )
	total_tick = total_tick + milliseconds;
	--print( total_tick );

	-- zoom camera
	ZoomCamera()

	if( total_tick > 7000 ) then
		-- kill constraints
		KillConstraints()
		-- kill self
		KillEntity( entity_id )
	end
end

function ZoomCamera()
	if( total_tick > 3000 and total_tick < 4000 ) then
		new_zoom = (total_tick - 3000) / 200;
		SetCameraZoom( new_zoom )
	end
end

function KillConstraints()
	new_entity_id = GetEntityID( "constraint_s_1" )
	KillEntity( new_entity_id )
	new_entity_id = GetEntityID( "constraint_s_2" )
	KillEntity( new_entity_id )
	new_entity_id = GetEntityID( "constraint_p_1" )
	KillEntity( new_entity_id )
	new_entity_id = GetEntityID( "constraint_p_2" )
	KillEntity( new_entity_id )
	new_entity_id = GetEntityID( "constraint_i_1" )
	KillEntity( new_entity_id )
	new_entity_id = GetEntityID( "constraint_i_2" )
	KillEntity( new_entity_id )
	new_entity_id = GetEntityID( "constraint_i_3" )
	KillEntity( new_entity_id )
	new_entity_id = GetEntityID( "constraint_n_1" )
	KillEntity( new_entity_id )
	new_entity_id = GetEntityID( "constraint_n_2" )
	KillEntity( new_entity_id )
end
