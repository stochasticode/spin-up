#include <GL/gl.h>
#include <Entity.h>
#include <SpinGame.h>
#include <chipmunk.h>
#include <tinyxml.h>
#include <sstream>
#include <cstdio>

using namespace spin;

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

void BodyEntity::InitBodyPoly( float mass, std::vector<Vector>& points, float friction )
{
	ChipmunkCleanup();

	// need data in cpVect*
	cpVect verts[points.size()];
	for( int i = 0; i < points.size(); i++ )
	{
		verts[i].x = points[i].x;
		verts[i].y = points[i].y;
	}

	// set up body
	float moment = cpMomentForPoly( mass, points.size(), verts, cpv( 0, 0 ) );
	body = cpBodyNew( mass, moment );
	cpBodySetPos( body, cpv( 0, 0 ) );
	cpSpaceAddBody( SPIN.world.GetCPSpace(), body );

	// set up shape
	shape = cpPolyShapeNew( body, points.size(), verts, cpv( 0, 0 ) );
	cpShapeSetFriction( shape, friction );
	cpSpaceAddShape( SPIN.world.GetCPSpace(), shape );

	// link body
	shape->data = this;
}

void BodyEntity::Render()
{
	QuadEntity::Render();
	//RenderShape();
}

void BodyEntity::RenderShape()
{
	// some code yanked from ChipmunkDebugDraw.c

	if( shape != 0 && body != 0 )
	{
		glDisable( GL_TEXTURE_2D );
		glColor3f( 1.0, 1.0, 1.0 );
		glPushMatrix();
		glTranslatef( position.x, position.y, 0 );
		glRotatef( rotation, 0, 0, 1 );

		// draw circle shape
		if( shape->klass->type == CP_CIRCLE_SHAPE )
		{
			cpCircleShape *circle = (cpCircleShape*)shape;
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
			//glVertexPointer( 2, GL_FLOAT, 0, poly->verts );
			//glDrawArrays( GL_LINE_LOOP, 0, poly->numVerts );
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
	{
		constraint = cpSpaceAddConstraint( SPIN.world.GetCPSpace(), cpSlideJointNew( new_body_a->body, new_body_b->body, cpvzero, cpvzero, min_length, max_length ) );
		new_body_a->constraint_ids.push_back( entity_id );
		new_body_b->constraint_ids.push_back( entity_id );
	}
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
		constraint = cpSpaceAddConstraint( SPIN.world.GetCPSpace(), cpDampedSpringNew( new_body_a->body, new_body_b->body, cpvzero, cpvzero, length, strength, damping ) );
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

BodyEntity* BodyEntity::LoadEntity( const char* xml_path )
{
	BodyEntity* new_entity = new BodyEntity();
	if( new_entity->LoadXML( xml_path ) )
		return new_entity;
	else
	{
		fprintf( stderr, "BodyEntity::Loadentity -> failed for xml: %s!\n", xml_path );
		delete new_entity;
		return 0;
	}
}

bool BodyEntity::LoadXML( const char* xml_path )
{
	std::stringstream stream;

	// load document
	TiXmlDocument doc( xml_path );
	if( !doc.LoadFile() )
	{
		fprintf( stderr, "Unable to load entity xml: %s\n", xml_path );
		return false;
	}

	// find root
	TiXmlElement* root = doc.FirstChildElement( "spin_entity" );
	if( !root )
	{
		fprintf( stderr, "Unable to find tag 'spin_entity' in entity xml: %s\n", xml_path );
		return false;
	}

	float mass = 1.0;
	float friction = 1.0;
	bool shape_loaded = false;
	float size_x = 1.0;
	float size_y = 1.0;

	// iterate through children
	TiXmlElement* child = root->FirstChildElement();
	while( child != 0 )
	{
		// param
		if( strcmp( "param", child->Value() ) == 0 )
		{
			fflush( stdout );
			const char* name = child->Attribute( "name" );
			const char* value = child->Attribute( "value" );

			printf( "param: %s %s\n", name, value );

			if( name != 0 && value != 0 )
			{
				if( strcmp( name, "texture_key" ) == 0 )
					texture_key = value;
				else if( strcmp( name, "mass" ) == 0 )
				{
					stream << value << std::endl;
					stream >> mass;
					if( shape_loaded && body != 0 )
						cpBodySetMass( body, mass );
				}
				else if( strcmp( name, "size_x" ) == 0 )
				{
					stream << value << std::endl;
					stream >> size.x;
				}
				else if( strcmp( name, "size_y" ) == 0 )
				{
					stream << value << std::endl;
					stream >> size.y;
				}
				else if( strcmp( name, "friction" ) == 0 )
				{
					stream << value << std::endl;
					stream >> friction;
					if( shape_loaded && shape != 0 )
						shape->e = friction;
				}
			}
			else
			{
				fprintf( stderr, "Invalid param tag in entity xml: %s\n", xml_path );
				return false;
			}
		}
			
		// shape
		if( strcmp( "shape", child->Value() ) == 0 )
		{
			printf( "shape\n" );
			fflush( stdout );
			if( shape_loaded )
			{
				fprintf( stderr, "Multiple shape tags in entity xml: %s\n", xml_path );
				return false;
			}

			const char* type = child->Attribute( "type" );
			if( type == 0 )
			{
				fprintf( stderr, "No 'type' attribute in shape tag in entity xml: %s\n", xml_path );
				return false;
			}

			if( strcmp( type, "poly" ) == 0 )
			{
				if( !LoadPolyElement( child, mass, friction ) )
				{
					fprintf( stderr, "LoadPolyElement failed in entity xml: %s\n", xml_path );
					return false;
				}
				printf( "poly loaded\n" );
				fflush( stdout );
			}
			else
			{
				fprintf( stderr, "Unsupported 'type' attribute ('%s') in shape tag in entity xml: %s\n", type, xml_path );
				return false;
			}
			shape_loaded = true;
		}
		child = child->NextSiblingElement();
	}

	return true;
}

bool BodyEntity::LoadPolyElement( TiXmlElement* element, float mass, float friction )
{
	std::stringstream stream;
	std::vector<Vector> points;

	// iterate verticies
	TiXmlElement* child = element->FirstChildElement();
	while( child != 0 )
	{
		if( strcmp( "vertex", child->Value() ) == 0 )
		{
			const char* x = child->Attribute( "x" );
			const char* y = child->Attribute( "y" );
			if( x == 0 || y == 0 )
			{
				fprintf( stderr, "Invalid vertex tag!\n" );
				return false;
			}
			float x_val = 0.0;
			float y_val = 0.0;
			stream << x << std::endl << y << std::endl;
			stream >> x_val >> y_val;
			points.push_back( Vector( x_val, y_val ) );
		}
		child = child->NextSiblingElement();
	}

	InitBodyPoly( mass, points, friction );
	shape->collision_type = World::COL_TYPE_PROP;
	return true;
}
