total_tick = 0

function Tick( milliseconds )
	if( total_tick > 3000 ) then
		total_tick = 0;
																		
																		
																		
																		
																		
		AddImpulse( entity_id, 0, 100 );									
	else
		total_tick = total_tick + milliseconds;
	end
end
