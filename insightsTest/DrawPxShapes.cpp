#include "DrawPxShapes.h"


DrawPxShapes::DrawPxShapes(void)
{
}


DrawPxShapes::~DrawPxShapes(void)
{
}



void DrawPxShapes::getColumnMajor( physx::PxMat33 m, physx::PxVec3 t, float* mat )
{
	mat[0]  = m.column0[0];
	mat[1]  = m.column0[1];
	mat[2]  = m.column0[2];
	mat[3]  = 0;
	
	mat[4]  = m.column1[0];
	mat[5]  = m.column1[1];
	mat[6]  = m.column1[2];
	mat[7]  = 0;

	mat[8]  = m.column2[0];
	mat[9]  = m.column2[1];
	mat[10] = m.column2[2];
	mat[11] = 0;

	mat[12] = t[0];
	mat[13] = t[1];
	mat[14] = t[2];
	mat[15] = 1;

}


void DrawPxShapes::DrawBox( PxShape* pShape )
{
	PxTransform pT = PxShapeExt::getGlobalPose( *pShape );
	PxBoxGeometry bg;
	pShape->getBoxGeometry( bg );
	PxMat33 m = PxMat33( pT.q );
	float mat[ 16 ];
	getColumnMajor( m, pT.p, mat );
	glPushMatrix();
	glMultMatrixf( mat );
	glutSolidCube( bg.halfExtents.x * 2 );
	glPopMatrix();
}


void DrawPxShapes::DrawSphere( PxShape* pShape )
{
	PxTransform pT = PxShapeExt::getGlobalPose( *pShape );
	PxSphereGeometry sg;
	pShape->getSphereGeometry( sg );
	PxMat33 m = PxMat33( pT.q );
	float mat[ 16 ];
	getColumnMajor( m, pT.p, mat );
	glPushMatrix();
	glMultMatrixf( mat );
	glutSolidSphere( sg.radius, 20, 20 );
	glPopMatrix();
}


void DrawPxShapes::DrawShape( PxShape* shape )
{
	PxGeometryType::Enum type = shape->getGeometryType();

	switch ( type )
	{
	case PxGeometryType::eBOX:
		DrawBox( shape );
		break;

	case PxGeometryType::eSPHERE:
		DrawSphere( shape );
		break;

	default:
		break;
	}
}


void DrawPxShapes::DrawActor( PxRigidActor* actor )
{
	PxU32 nShapes = actor->getNbShapes();
	PxShape** shapes = new PxShape*[ nShapes ];
	actor->getShapes( shapes, nShapes );
	while ( nShapes-- )
	{
		DrawShape( shapes[ nShapes ] );
	}

	delete [] shapes;
}





