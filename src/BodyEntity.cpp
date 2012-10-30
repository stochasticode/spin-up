#include <iostream>

#include <chipmunk.h>
#include <chipmunk_unsafe.h>
#include <GL/glut.h>
#include <lua5.1/lua.hpp>
#include <tinyxml.h>

#include <BodyEntity.h>
#include <ConstraintEntity.h>
#include <Entity.h>
#include <SpinGame.h>
#include <SpinUtil.h>
#include <SpinXML.h>

using namespace spin;
using namespace std;

// yanked from ChipmunkDebugDraw.c
GLfloat BodyEntity::circle_points[] = {
	 0.0000f,  1.0000f,
	 0.2588f,  0.9659f,
	 0.5000f,  0.8660f,
	 0.7071f,  0.7071f,
	 0.8660f,  0.5000f,
	 0.9659f,  0.2588f,
	 1.0000f,  0.0000f,
	 0.9659f, -0.2588f,
	 0.8660f, -0.5000f,
	 0.7071f, -0.7071f,
	 0.5000f, -0.8660f,
	 0.2588f, -0.9659f,
	 0.0000f, -1.0000f,
	-0.2588f, -0.9659f,
	-0.5000f, -0.8660f,
	-0.7071f, -0.7071f,
	-0.8660f, -0.5000f,
	-0.9659f, -0.2588f,
	-1.0000f, -0.0000f,
	-0.9659f,  0.2588f,
	-0.8660f,  0.5000f,
	-0.7071f,  0.7071f,
	-0.5000f,  0.8660f,
	-0.2588f,  0.9659f,
	 0.0000f,  1.0000f,
	 0.0f, 0.0f, // For an extra line to see the rotation.
};

bool BodyEntity::render_shapes = false;

BodyEntity::BodyEntity() : QuadEntity()
{
	body = cpBodyNew( 1.0, 1.0 );
	cpBodySetPos( body, cpv( 0, 0 ) );
	cpSpaceAddBody( SPIN.world.GetCPSpace(), body );

	// register lua functions
	lua_register( lua_state, "AddImpulse", lua_AddImpulse ); 
}

BodyEntity::~BodyEntity()
{
	ChipmunkCleanup();
	// kill any constraints that may be attached to body
	for( int i = 0; i < constraint_ids.size(); i++ )
	{
		ConstraintEntity* constraint = dynamic_cast<ConstraintEntity*>(SPIN.world.GetEntity( constraint_ids[i] ));
		if( constraint != 0 )
			constraint->dead = true;
	}
}

void BodyEntity::ChipmunkCleanup()
{
	// remove/free shapes
	for( int i = 0; i < shapes.size(); i++ )
	{
		cpSpaceRemoveShape( SPIN.world.GetCPSpace(), shapes[i] );
		cpShapeFree( shapes[i] );
	}
	shapes.clear();

	// remove/free body
	if( body != 0 )
	{
		cpSpaceRemoveBody( SPIN.world.GetCPSpace(), body );
		cpBodyFree( body );
		body = 0;
	}
}

void BodyEntity::AddShapeCircle( float radius, Vector offset, float friction )
{
	// set up shape
	cpShape* shape = cpCircleShapeNew( body, radius, cpv( offset.x, offset.y ) );
	shapes.push_back( shape );
	cpShapeSetFriction( shape, friction );
	cpSpaceAddShape( SPIN.world.GetCPSpace(), shape );

	// link body
	shape->data = this;
}

void BodyEntity::AddShapeRect( float width, float height, Vector offset, float friction )
{
	// set up shape
	cpBB box;
	box.l = offset.x - width/2.0;
	box.b = offset.y - height/2.0;
	box.r = offset.x + width/2.0;
	box.t = offset.y + height/2.0;

	cpShape* shape = cpBoxShapeNew2( body, box );
	shapes.push_back( shape );
	cpShapeSetFriction( shape, friction );
	cpSpaceAddShape( SPIN.world.GetCPSpace(), shape );

	// link body
	shape->data = this;
}

void BodyEntity::AddShapePoly( std::vector<Vector>& points, float friction )
{
	// need data in cpVect*
	cpVect verts[points.size()];
	for( int i = 0; i < points.size(); i++ )
	{
		verts[i].x = points[i].x;
		verts[i].y = points[i].y;
	}

	// set up shape
	cpShape* shape = cpPolyShapeNew( body, points.size(), verts, cpv( 0, 0 ) );
	shapes.push_back( shape );
	cpShapeSetFriction( shape, friction );
	cpShapeSetElasticity( shape, 0.5);
	cpSpaceAddShape( SPIN.world.GetCPSpace(), shape );

	// link body
	shape->data = this;
}

void BodyEntity::Render()
{
	QuadEntity::Render();
	if( render_shapes )
		RenderShapes();
}

void BodyEntity::RenderShapes()
{
	// some code yanked from ChipmunkDebugDraw.c
	for( int i = 0; i < shapes.size(); i++ )
	{
		cpShape* shape = shapes[i];
		glDisable( GL_TEXTURE_2D );
		glColor3f( 1.0, 1.0, 1.0 );
		glPushMatrix();
		glTranslatef( position.x, position.y, 0 );
		glRotatef( rotation, 0, 0, 1 );

		// draw circle shape
		if( shape->klass->type == CP_CIRCLE_SHAPE )
		{
			cpCircleShape *circle = (cpCircleShape*)shape;
			glTranslatef( circle->c.x, circle->c.y, 0 );
			int circle_points_count = sizeof(circle_points)/sizeof(GLfloat)/2;
			glVertexPointer( 2, GL_FLOAT, 0, circle_points );
			glScalef( circle->r, circle->r, 1 );
			glDrawArrays( GL_LINE_STRIP, 0, circle_points_count );
		}
		// draw polygon
		else if( shape->klass->type == CP_POLY_SHAPE )
		{
			cpPolyShape *poly = (cpPolyShape*)shape;
			// couldn't get the vertex array to work ... ?
			glBegin( GL_LINE_LOOP );
				for( int i = 0; i < poly->numVerts; i++ )
					glVertex3f( poly->verts[i].x, poly->verts[i].y, 0.0 );
			glEnd();
		}

		glPopMatrix();
		glEnable( GL_TEXTURE_2D );
	}
}

void BodyEntity::Tick( int milliseconds )
{
	QuadEntity::Tick( milliseconds );

	// update body
	if( body != 0 )
	{
		position.x = cpBodyGetPos( body ).x;
		position.y = cpBodyGetPos( body ).y;
		rotation = body->a * 180 / M_PI;
	}
}

void BodyEntity::SetPosition( Vector new_position )
{
	position.x = new_position.x;
	position.y = new_position.y;
	cpBodySetPos( body, cpv( new_position.x, new_position.y ) );
}

void BodyEntity::SetVelocity( Vector new_velocity )
{
	body->v.x = new_velocity.x;
	body->v.y = new_velocity.y;
}

void BodyEntity::ApplyImpulse( Vector impulse )
{
	cpBodyApplyImpulse( body, cpv( impulse.x, impulse.y ), cpv( 0, 0 ) );
}

void BodyEntity::ApplyForce( Vector force )
{
	cpBodyApplyForce( body, cpv( force.x, force.y ), cpv( 0, 0 ) );
}

void BodyEntity::SetMass( float mass )
{
	cpBodySetMass( body, mass );
}

void BodyEntity::SetMoment( float moment )
{
	cpBodySetMoment( body, moment );
}

void BodyEntity::Scale( float scale_factor )
{
	scale *= scale_factor;

	// change mass
	cpBodySetMass( body, body->m * scale_factor * scale_factor );

	// change moment, this may need to be tweaked...
	cpBodySetMoment( body, body->i * scale_factor * scale_factor * scale_factor * scale_factor );

	for( int i = 0; i < shapes.size(); i++ )
	{
		cpShape* shape = shapes[i];

		// polygon
		if( shape->klass->type == CP_POLY_SHAPE )
		{
			cpPolyShape *poly = (cpPolyShape*)shape;
			cpVect new_verts[poly->numVerts];
			for( int i = 0; i < poly->numVerts; i++ )
			{
				new_verts[i].x = poly->verts[i].x * scale_factor;
				new_verts[i].y = poly->verts[i].y * scale_factor;
			}
			cpPolyShapeSetVerts( shape, poly->numVerts, new_verts, cpvzero );
		}
		//TODO circle
	}
}

float BodyEntity::MomentForCircle( float mass, float outer_radius, float inner_radius )
{
	return cpMomentForCircle( mass, outer_radius, inner_radius, cpvzero );
}

int BodyEntity::lua_AddImpulse( lua_State *L ) {

	// check for right number of arguments
	if( lua_gettop( L ) < 3 )
	{
		fprintf( stderr, "BodyEntity::lua_AddImpulse-> not enough lua arguments!\n" );
		return 0;
	}

	// get entity_id
	int entity_id = lua_tonumber( L, 1 );
	int vel_x = lua_tonumber( L, 2 );
	int vel_y = lua_tonumber( L, 3 );

	// make sure it is a BodyEntity
	BodyEntity* body_entity = dynamic_cast<BodyEntity*>(SPIN.world.GetEntity( entity_id ));
	if( body_entity == 0 )
	{
		fprintf( stderr, "BodyEntity::lua_AddImpulse-> couldn't find BodyEntity for id: %d!\n", entity_id );
		return 0;
	}
	else
	{
		// set velocity
		Vector new_vel( vel_x, vel_y );
		body_entity->SetVelocity( new_vel );
	}
}

void BodyEntity::SetLayers( int layers )
{
	for( int i = 0; i < shapes.size(); i++ )
		shapes[i]->layers = layers;
}

bool BodyEntity::TryLoadElement( TiXmlElement* element, bool& error )
{
	error = false;

	// check for NULL
	if( element == 0 )
	{
		fprintf( stderr, "BodyEntity::TryLoadElement -> element was NULL!\n" );
		error = true;
		return false;
	}

	// param
	if( strcmp( "param", element->Value() ) == 0 )
	{
		std::string name = "";
		std::string value = "";
		SpinXML::ReadParam( element, name, value );

		// external
		if( name.compare( "external" ) == 0 )
		{
			if( !LoadXML( value.c_str() ) )
			{
				fprintf( stderr, "BodyEntity::LoadXML -> unable to load external: %s\n", value.c_str() );
				error = true;
				return false;
			}
		}
		// scale
		else if( name.compare( "scale" ) == 0 )
		{
			float new_scale;
			if( SpinUtil::ToFloat( value.c_str(), new_scale ) )
				Scale( new_scale );
			else
			{
				fprintf( stderr, "BodyEntity::LoadXML -> invalid scale value: %s\n", value.c_str() );
				error = true;
				return false;
			}
		}
		// mass
		else if( name.compare( "mass" ) == 0 )
		{
			float new_mass;
			if( SpinUtil::ToFloat( value.c_str(), new_mass ) )
				SetMass( new_mass );
			else
			{
				fprintf( stderr, "BodyEntity::LoadXML -> invalid mass value: %s\n", value.c_str() );
				error = true;
				return false;
			}
		}
		// moment
		else if( name.compare( "moment" ) == 0 )
		{
			float new_moment;
			if( SpinUtil::ToFloat( value.c_str(), new_moment ) )
				SetMoment( new_moment );
			// INFINITY
			else if( value == "INFINITY" )
				SetMoment( INFINITY );
			else
			{
				fprintf( stderr, "BodyEntity::LoadXML -> invalid moment value: %s\n", value.c_str() );
				error = true;
				return false;
			}
		}
		// unsupported
		else
			return QuadEntity::TryLoadElement( element, error );
	}
	// shape
	else if( strcmp( "shape", element->Value() ) == 0 )
	{
		const char* type = element->Attribute( "type" );
		if( type == 0 )
		{
			fprintf( stderr, "BodyEntity::LoadXML -> no 'type' attribute in shape tag!\n" );
			error = true;
			return false;
		}

		// polygon
		if( strcmp( type, "poly" ) == 0 )
		{
			if( !LoadPolyElement( element ) )
			{
				fprintf( stderr, "BodyEntity::LoadXML -> LoadPolyElement failed!\n" );
				error = true;
				return false;
			}
		}
		// circle
		else if( strcmp( type, "circle" ) == 0 )
		{
			// friction and radius
			float friction = 1.0;
			float radius = 1.0;
			if( !SpinUtil::ToFloat( element->Attribute( "friction" ), friction ) || !SpinUtil::ToFloat( element->Attribute( "radius" ), radius ) )
			{
				fprintf( stderr, "BodyEntity::LoadXML -> invalid or missing friction or radius attribute for shape type='circle'!\n" );
				return false;
			}

			// offset vector
			Vector offset( 0.0, 0.0 );
			TiXmlElement* circle_child = element->FirstChildElement();
			if( circle_child != 0  )
			{
				std::string name;
				if( strcmp( "vec2d", circle_child->Value() ) != 0 || !SpinXML::ReadVec2D( circle_child, name, offset ) || name.compare( "offset" ) != 0 )
				{
					fprintf( stderr, "BodyEntity::LoadXML -> invalid child in circle shape, expecting a single vec2d with name='offset'!\n" );
					error = true;
					return false;
				}
			}
			AddShapeCircle( radius, offset, friction );
		}
		else
		{
			fprintf( stderr, "BodyEntity::LoadXML -> unsupported 'type' attribute ('%s') in shape tag!\n", type );
			error = true;
			return false;
		}
	}
	// unsupported
	else
		return QuadEntity::TryLoadElement( element, error );

	return true;
}

void BodyEntity::FinalizeLoadElements()
{
	// make sure chipmunk body has the same position
	SetPosition( position );
}

bool BodyEntity::LoadPolyElement( TiXmlElement* element )
{
	// friction
	float friction = 1.0;
	SpinUtil::ToFloat( element->Attribute( "friction" ), friction );

	// iterate verticies
	std::vector<Vector> points;
	TiXmlElement* child = element->FirstChildElement();
	while( child != 0 )
	{
		if( strcmp( "vec2d", child->Value() ) == 0 )
		{
			std::string name = "";
			Vector vec2d;
			if( SpinXML::ReadVec2D( child, name, vec2d ) )
				points.push_back( vec2d );
			else
				return false;
		}
		child = child->NextSiblingElement();
	}

	AddShapePoly( points, friction );

	// collision_type will need to be delt with differently eventually...
	shapes[shapes.size()-1]->collision_type = World::COL_TYPE_PROP;
	return true;
}

StaticBody::StaticBody()
{
	body = SPIN.world.GetCPSpace()->staticBody;
}

StaticBody::~StaticBody()
{
	// don't delete body in ~BodyEntity()
	body = 0;
}
