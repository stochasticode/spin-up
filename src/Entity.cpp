#include <GL/gl.h>
#include <Entity.h>
#include <SpinGame.h>
#include <chipmunk.h>
#include <cstdio>

using namespace spin;

void QuadEntity::Render()
{
	glPushMatrix();
	glTranslatef( position.x, position.y, 0 );
	glRotatef( rotation, 0, 0, 1 );
	glScalef( size.x*scale, size.y*scale, 1 );

	SPIN.resources.BindTexture( texture_key );
	glBegin(GL_TRIANGLES);
		glColor4f( color.r, color.g, color.b, color.a );
		glTexCoord2d( 0.0, 1.0 ); glVertex3f( -0.5, -0.5, 0.0 );
		glTexCoord2d( 1.0, 1.0 ); glVertex3f(  0.5, -0.5, 0.0 );
		glTexCoord2d( 0.0, 0.0 ); glVertex3f( -0.5,  0.5, 0.0 );

		glTexCoord2d( 1.0, 1.0 ); glVertex3f(  0.5, -0.5, 0 );
		glTexCoord2d( 0.0, 0.0 ); glVertex3f( -0.5,  0.5, 0 );
		glTexCoord2d( 1.0, 0.0 ); glVertex3f(  0.5,  0.5, 0 );
	glEnd();

	glPopMatrix();
}

SurfaceEntity::SurfaceEntity( float x1, float y1, float x2, float y2, float new_radius, float friction ): Entity(), radius( new_radius )
{
	shape = (cpSegmentShape*)cpSegmentShapeNew( 0, cpv( x1, y1 ), cpv( x2, y2 ), radius );
	shape->shape.collision_type = World::COL_TYPE_SURFACE;
	cpShapeSetFriction( (cpShape*)shape, friction );
	shape->shape.body = SPIN.world.GetCPSpace()->staticBody;
	cpSpaceAddShape( SPIN.world.GetCPSpace(), (cpShape*)shape );
}

SurfaceEntity::~SurfaceEntity()
{
	if( shape != 0 )
	{
		cpSpaceRemoveShape( SPIN.world.GetCPSpace(), (cpShape*)shape );
		cpShapeFree( (cpShape*)shape );
		shape = 0;
	}
}

void SurfaceEntity::Render()
{
	glDisable( GL_TEXTURE_2D );
	glBegin(GL_LINES);
		glColor3f( 1, 1, 1 );
		glVertex3f( shape->a.x, shape->a.y, 0 );
		glVertex3f( shape->b.x, shape->b.y, 0 );
	glEnd();
	glEnable( GL_TEXTURE_2D );
}

BodyEntity::BodyEntity(): QuadEntity(), body( 0 ), shape( 0 )
{
}

BodyEntity::~BodyEntity()
{
	ChipmunkCleanup();
	for( int i = 0; i < constraint_ids.size(); i++ )
	{
		ConstraintEntity* constraint = dynamic_cast<ConstraintEntity*>(SPIN.world.GetEntity( constraint_ids[i] ));
		if( constraint != 0 )
			constraint->dead = true;
	}
}

void BodyEntity::ChipmunkCleanup()
{
	if( body != 0 )
	{
		cpSpaceRemoveBody( SPIN.world.GetCPSpace(), body );
		cpBodyFree( body );
		body = 0;
	}
	if( shape != 0 )
	{
		cpSpaceRemoveShape( SPIN.world.GetCPSpace(), shape );
		cpShapeFree( shape );
		shape = 0;
	}
}

void BodyEntity::InitBodyCircle( float mass, float radius, float friction )
{
	ChipmunkCleanup();

	// set up body
	body = cpBodyNew( mass, cpMomentForCircle( mass, 0, radius, cpvzero ) );
	cpBodySetPos( body, cpv( 0, 0 ) );
	cpSpaceAddBody( SPIN.world.GetCPSpace(), body );

	// set up shape
	shape = cpCircleShapeNew( body, radius, cpvzero );
	cpShapeSetFriction( shape, friction );
	cpSpaceAddShape( SPIN.world.GetCPSpace(), shape );

	// link body
	shape->data = this;
}

void BodyEntity::InitBodyRect( float mass, float width, float height, float friction )
{
	ChipmunkCleanup();

	// set up body
	body = cpBodyNew( mass, cpMomentForBox( mass, width, height ) );
	cpBodySetPos( body, cpv( 0, 0 ) );
	cpSpaceAddBody( SPIN.world.GetCPSpace(), body );

	// set up shape
	shape = cpBoxShapeNew( body, width, height );
	cpShapeSetFriction( shape, friction );
	cpSpaceAddShape( SPIN.world.GetCPSpace(), shape );

	// link body
	shape->data = this;
}

void BodyEntity::Tick( int milliseconds )
{
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

ConstraintEntity::ConstraintEntity(): Entity(), constraint( 0 )
{
}

ConstraintEntity::~ConstraintEntity()
{
	ChipmunkCleanup();
}

void ConstraintEntity::ChipmunkCleanup()
{
	if( constraint != 0 )
	{
		cpSpaceRemoveConstraint( SPIN.world.GetCPSpace(), constraint );
		cpConstraintFree( constraint );
		constraint = 0;
	}
}

void ConstraintEntity::Render()
{
}

// slide joint between two bodies
void ConstraintEntity::InitConstraintSlideJoint( BodyEntity* new_body_a, BodyEntity* new_body_b, float min_length, float max_length )
{
	ChipmunkCleanup();
	if( new_body_a != 0 && new_body_a->body != 0 && new_body_b != 0 && new_body_b->body != 0 )
		constraint = cpSpaceAddConstraint( SPIN.world.GetCPSpace(), cpSlideJointNew( new_body_a->body, new_body_b->body, cpvzero, cpvzero, min_length, max_length ) );
	else
		fprintf( stderr, "ConstraintEntity::InitConstraintSlideJoint -> failed due to null pointer!\n" );
}

// slide joint between body and static body
void ConstraintEntity::InitConstraintSlideJoint( BodyEntity* new_body_a, Vector static_anchor, float min_length, float max_length )
{
	ChipmunkCleanup();
	if( new_body_a != 0 && new_body_a->body != 0 )
	{
		constraint = cpSpaceAddConstraint( SPIN.world.GetCPSpace(), cpSlideJointNew( SPIN.world.GetCPSpace()->staticBody, new_body_a->body, cpv( static_anchor.x, static_anchor.y ), cpvzero, min_length, max_length ) );
		new_body_a->constraint_ids.push_back( entity_id );
	}
	else
		fprintf( stderr, "ConstraintEntity::InitConstraintSlideJoint -> failed due to null pointer!\n" );
}

// spring between two bodies
void ConstraintEntity::InitConstraintSpring( BodyEntity* new_body_a, BodyEntity* new_body_b, float length, float strength, float damping )
{
	ChipmunkCleanup();
	if( new_body_a != 0 && new_body_a->body != 0 && new_body_b != 0 && new_body_b->body != 0 )
	{
		constraint = cpSpaceAddConstraint( SPIN.world.GetCPSpace(), cpDampedSpringNew( new_body_a->body, new_body_a->body, cpvzero, cpvzero, length, strength, damping ) );
		new_body_a->constraint_ids.push_back( entity_id );
		new_body_b->constraint_ids.push_back( entity_id );
	}
	else
		fprintf( stderr, "ConstraintEntity::InitConstraintSpring -> failed due to null pointer!\n" );
}

// spring between body and static body
void ConstraintEntity::InitConstraintSpring( BodyEntity* new_body_a, Vector static_anchor, float length, float strength, float damping )
{
	ChipmunkCleanup();
	if( new_body_a != 0 && new_body_a->body != 0 )
	{
		constraint = cpSpaceAddConstraint( SPIN.world.GetCPSpace(), cpDampedSpringNew( SPIN.world.GetCPSpace()->staticBody, new_body_a->body, cpv( static_anchor.x, static_anchor.y ), cpvzero, length, strength, damping ) );
		new_body_a->constraint_ids.push_back( entity_id );
	}
	else
		fprintf( stderr, "ConstraintEntity::InitConstraintSpring -> failed due to null pointer!\n" );
}
