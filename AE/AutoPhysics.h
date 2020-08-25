#ifndef AE_PHYSICS_H
#define AE_PHYSICS_H

#include "AutoPolygon.h"

struct AE_rigid_body
{
	AE_polygon Shape;
	AE_b_box AABB;
	
	v2 Force;
	
	v2 Pos;
	v2 Vel;
	v2 Accel;
	
	v2 Dim;
	
	real32 dAngle;
	real32 Orientation;
	real32 AngularVelocity;
	
	real32 Torque;
	real32 MomentOfInertia;
	
	real32 Mass;
};

AE_rigid_body AE_CreateBody(AE_polygon Shape, v2 Vel, real32 Mass, real32 Angle)
{
	AE_rigid_body Body = {};
	Body.Shape = Shape;
	Body.Pos = Shape.Center;
	Body.Dim = Shape.Dim;
	Body.Vel = Vel;
	Body.Mass = Mass;
	Body.Orientation = Angle;
	Body.MomentOfInertia = 
	(Mass * (Body.Dim.x * Body.Dim.x) * (Body.Dim.y * Body.Dim.y)) / 12;
	Body.AABB = AE_ComputeAABB(Shape.Center, Shape.Vertices, Shape.Count);
	
	return Body;
}

void AE_ComputeTorque(AE_rigid_body* Body, v2 R = {})
{
	Body->Torque = (R.x * Body->Force.y) - (R.y * Body->Force.x);
}

void AE_IntegrateLinearMotion(AE_rigid_body* Body, real32 dt, real32 Drag = 0)
{
	if(Body)
	{
		Body->Accel = Body->Force / Body->Mass;
		Body->Accel -= Drag * Body->Vel;
		Body->Vel += Body->Accel * dt;
		
		v2 Delta = {};
		Delta += Body->Vel * dt;
		
		Body->Pos += Delta;
		AE_UpdateVertices(&Body->Shape, Delta);
		Body->AABB.Center = Body->Pos;
	}
}

void AE_IntegrateAngularMotion(AE_rigid_body* Body, real32 t)
{
	real32 Theta = 0;
	real32 AngularAccel = Body->Torque / Body->MomentOfInertia;
	//Faking air resistence & friction
	real32 Drag = 0.05f;
	AngularAccel -= Body->AngularVelocity * Drag;
	Body->AngularVelocity += AngularAccel;
	Theta += Body->AngularVelocity;
	Body->Orientation += Theta;
	Body->dAngle = Theta;
	
	AE_polygon* Shape = &Body->Shape;
	AE_RotatePolygon(Shape, Radians(Theta));
	Body->AABB = AE_ComputeAABB(Shape->Center, Shape->Vertices, Shape->Count);
}

void AE_WarpBodyOutOfBound(AE_rigid_body* Body, v2 Min, v2 Max)
{
	if(Body->Pos.x > Max.x)
	{
		v2 NewPos = Body->Pos;
		Body->Pos.x = Min.x;
		v2 Delta = Body->Pos - NewPos;
		AE_UpdateVertices(&Body->Shape, Delta);
		Body->AABB.Center = Body->Pos;
	}
	
	if(Body->Pos.x < Min.x)
	{
		v2 NewPos = Body->Pos;
		Body->Pos.x = Max.x;
		v2 Delta = Body->Pos - NewPos;
		AE_UpdateVertices(&Body->Shape, Delta);
		Body->AABB.Center = Body->Pos;
	}
	
	if(Body->Pos.y < Min.y)
	{
		v2 NewPos = Body->Pos;
		Body->Pos.y = Max.y;
		v2 Delta = Body->Pos - NewPos;
		AE_UpdateVertices(&Body->Shape, Delta);
		Body->AABB.Center = Body->Pos;
	}
	
	if(Body->Pos.y > Max.y)
	{
		v2 NewPos = Body->Pos;
		Body->Pos.y = Min.y;
		v2 Delta = Body->Pos - NewPos;
		AE_UpdateVertices(&Body->Shape, Delta);
		Body->AABB.Center = Body->Pos;
	}
}

#endif