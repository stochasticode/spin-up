total_tick = 0
press_key_id = 0;

function Init()
	-- set up camera
	SetCameraMode( "stationary" );
	SetCameraPosition( 0, -50 );
	SetCameraZoom( 0.01 )

	press_key_id = GetEntityID( "press_any_key_text" )
end

function Tick( milliseconds )
	total_tick = total_tick + milliseconds;

	-- zoom camera
	ZoomCamera()
	FadeText()
end

function ZoomCamera()
	if( total_tick < 1000 ) then
		new_zoom = total_tick / 200;
		SetCameraZoom( new_zoom )
	end
end

function FadeText()
	new_alpha = (math.cos( total_tick * 2 * 3.14159 / 1000 ) + 1) / 2;
	QuadEntitySetColorAll( press_key_id, 1.0, 1.0, 1.0, new_alpha )
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
