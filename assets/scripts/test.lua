total_tick = 0
entity_created = false
new_entity_id = 0

function Tick( milliseconds )
	total_tick = total_tick + milliseconds;
	print( total_tick );
	if( not entity_created and total_tick > 5000 ) then
		new_entity_id = LoadEntity( "assets/entities/flask.xml" );
		QuadEntitySetPosition( new_entity_id, -20.0, 0.0 );
		entity_created = true
	end

	if( entity_created and total_tick > 10000 ) then
		QuadEntitySetPosition( new_entity_id, -40.0, 0.0 );
		KillEntity( entity_id )
	end
end
