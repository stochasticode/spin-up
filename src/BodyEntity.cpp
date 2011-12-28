#include <GL/glut.h>
#include <BodyEntity.h>
#include <Entity.h>
#include <SpinGame.h>
#include <SpinXML.h>
#include <SpinUtil.h>
#include <chipmunk.h>
#include <chipmunk_unsafe.h>
#include <tinyxml.h>

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

bool BodyEntity::render_shapes = false;

BodyEntity::BodyEntity(): QuadEntity()
{
	body = cpBodyNew( 1.0, 1.0 );
	cpBodySetPos( body, cpv( 0, 0 ) );
	cpSpaceAddBody( SPIN.world.GetCPSpace(), body );
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

void BodyEntity::AddShapeRect( float width, float height, float friction )
{
	// set up shape
	cpShape* shape = cpBoxShapeNew( body, width, height );
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
	}
}

float BodyEntity::MomentForCircle( float mass, float outer_radius, float inner_radius )
{
	return cpMomentForCircle( mass, outer_radius, inner_radius, cpvzero );
}

bool BodyEntity::LoadXML( const char* xml_path )
{
	// load document
	TiXmlDocument doc( xml_path );
	if( !doc.LoadFile() )
	{
		fprintf( stderr, "BodyEntity::LoadXML -> unable to load entity xml: %s\n", xml_path );
		return false;
	}

	// find root
	TiXmlElement* root = doc.FirstChildElement( "entity" );
	if( !root )
	{
		fprintf( stderr, "BodyEntity::LoadXML -> unable to find root tag 'entity' in xml: %s\n", xml_path );
		return false;
	}

	return LoadXML( root );
}

bool BodyEntity::LoadXML( TiXmlElement* element )
{
	// iterate through children
	TiXmlElement* child = element->FirstChildElement();
	while( child != 0 )
	{
		// param
		if( strcmp( "param", child->Value() ) == 0 )
		{
			std::string name = "";
			std::string value = "";
			SpinXML::ReadParam( child, name, value );

			// mass
			if( name.compare( "mass" ) == 0 )
			{
				float new_mass;
				if( SpinUtil::ToFloat( value.c_str(), new_mass ) )
					SetMass( new_mass );
				else
				{
					fprintf( stderr, "BodyEntity::LoadXML -> invalid mass value: %s\n", value.c_str() );
					return false;
				}
			}
			// moment
			else if( name.compare( "moment" ) == 0 )
			{
				float new_moment;
				if( SpinUtil::ToFloat( value.c_str(), new_moment ) )
					SetMoment( new_moment );
				else
				{
					fprintf( stderr, "BodyEntity::LoadXML -> invalid moment value: %s\n", value.c_str() );
					return false;
				}
			}
			// unsupported
			else
			{
				fprintf( stderr, "BodyEntity::LoadXML -> unsupported param: %s\n", name.c_str() );
			}
		}
			
		// shape
		else if( strcmp( "shape", child->Value() ) == 0 )
		{
			const char* type = child->Attribute( "type" );
			if( type == 0 )
			{
				fprintf( stderr, "BodyEntity::LoadXML -> no 'type' attribute in shape tag!\n" );
				return false;
			}

			// polygon
			if( strcmp( type, "poly" ) == 0 )
			{
				if( !LoadPolyElement( child ) )
				{
					fprintf( stderr, "BodyEntity::LoadXML -> LoadPolyElement failed!\n" );
					return false;
				}
			}
			// circle
			else if( strcmp( type, "circle" ) == 0 )
			{
				// friction and radius
				float friction = 1.0;
				float radius = 1.0;
				if( !SpinUtil::ToFloat( child->Attribute( "friction" ), friction ) || !SpinUtil::ToFloat( child->Attribute( "radius" ), radius ) )
				{
					fprintf( stderr, "BodyEntity::LoadXML -> invalid or missing friction or radius attribute for shape type='circle'!\n" );
					return false;
				}

				// offset vector
				Vector offset( 0.0, 0.0 );
				TiXmlElement* circle_child = child->FirstChildElement();
				if( circle_child != 0 || strcmp( "vec2d", child->Value() ) )
				{
					std::string name;
					if( !SpinXML::ReadVec2D( circle_child, name, offset ) || name.compare( "offset" ) != 0 )
					{
						fprintf( stderr, "BodyEntity::LoadXML -> invalid child in circle shape, expecting a single vec2d with name='offset'!\n" );
						return false;
					}
				}
				AddShapeCircle( radius, offset, friction );
			}
			else
			{
				fprintf( stderr, "BodyEntity::LoadXML -> unsupported 'type' attribute ('%s') in shape tag!\n", type );
				return false;
			}
		}

		// quad 
		else if( strcmp( "quad", child->Value() ) == 0 )
		{
			Quad new_quad;
			if( !new_quad.LoadXML( child ) )
			{
				fprintf( stderr, "BodyEntity::LoadXML -> Quad::LoadXML failed!\n" );
				return false;
			}
			quads.push_back( new_quad );
		}
		child = child->NextSiblingElement();
	}
	return true;
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
	shapes[0]->collision_type = World::COL_TYPE_PROP;
	return true;
}
