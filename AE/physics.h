#ifndef PHYSICS_H

#include "polygon.h"

struct rigid_body
{
	polygon Shape;
	b_box AABB;
	
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

rigid_body CreateBody(polygon Shape, v2 Vel, real32 Mass, real32 Angle)
{
	rigid_body Body = {};
	Body.Shape = Shape;
	Body.Pos = Shape.Center;
	Body.Dim = Shape.Dim;
	Body.Vel = Vel;
	Body.Mass = Mass;
	Body.Orientation = Angle;
	Body.MomentOfInertia = 
	(Mass * (Body.Dim.x * Body.Dim.x) * (Body.Dim.y * Body.Dim.y)) / 12;
	Body.AABB = ComputeAABB(Shape.Center, Shape.Vertices, Shape.Count);
	
	return Body;
}

void ComputeTorque(rigid_body* Body, v2 R = {})
{
	Body->Torque = (R.x * Body->Force.y) - (R.y * Body->Force.x);
}

void IntegrateLinearMotion(rigid_body* Body, real32 dt, real32 Drag = 0)
{
	if(Body)
	{
		Body->Accel = Body->Force / Body->Mass;
		Body->Accel -= Drag * Body->Vel;
		Body->Vel += Body->Accel * dt;
		
		v2 Delta = {};
		Delta += Body->Vel * dt;
		
		Body->Pos += Delta;
		UpdateVertices(&Body->Shape, Delta);
		Body->AABB.Center = Body->Pos;
	}
}

void IntegrateAngularMotion(rigid_body* Body, real32 t)
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
	
	polygon* Shape = &Body->Shape;
	RotatePolygon(Shape, Radians(Theta));
	Body->AABB = ComputeAABB(Shape->Center, Shape->Vertices, Shape->Count);
}

// This function is incomplete, it doesn't work. It's for testing purposes.
void ResolveCollision(rigid_body* BodyA, rigid_body* BodyB, 
					  collide_result* CollideResult, real32 dt)
{
	v2 n = CollideResult->Normal;
								
	int Index = FurthestPointInDirection(&BodyB->Shape, CollideResult->d);
	
	BodyA->Pos += CollideResult->MTV;
	UpdateVertices(&BodyA->Shape, CollideResult->MTV);
		
	real32 e = 1.0f;
	
	v2 ContactPoint = BodyB->Shape.Vertices[Index];
	v2 CMToCornerPerp = Perp_v2(ContactPoint - BodyB->Shape.Center);
	
	v2 Vel = BodyA->Vel + BodyA->AngularVelocity * CMToCornerPerp;
	
	real32 PerpDot = Dot(CMToCornerPerp, n);
	
	real32 ImpulseNumerator = -(1 + e) * Dot(Vel, n);
	real32 ImpulseDenominator = (1/BodyB->Mass + 1/BodyB->MomentOfInertia) * 
								PerpDot * PerpDot;
	printf("Numerator: %f Denominator: %f\n", ImpulseNumerator, ImpulseDenominator);
	assert(BodyB->Mass);
	assert(BodyB->MomentOfInertia);
	
	real32 j = ImpulseNumerator / ImpulseDenominator;
	printf("Impulse: %f\n", j);
	
	BodyB->Vel += j * 1/BodyB->Mass * n;
	PrintV2(BodyB->Vel);
	BodyB->AngularVelocity += j * BodyB->MomentOfInertia * PerpDot;
	
	// v2 DeltaA = BodyA->Vel * dt;
	// v2 DeltaB = BodyB->Vel * dt;
	// BodyA->Pos += DeltaA;
	// BodyB->Pos += DeltaB;
	// UpdateVertices(&BodyA->Shape, DeltaA);
	// UpdateVertices(&BodyB->Shape, DeltaB);
	
	//BodyA->Vel += -Project(BodyA->Vel, n);
}

void WarpBodyOutOfBound(rigid_body* Body, v2 Min, v2 Max)
{
	if(Body->Pos.x > Max.x)
	{
		v2 NewPos = Body->Pos;
		Body->Pos.x = Min.x;
		v2 Delta = Body->Pos - NewPos;
		UpdateVertices(&Body->Shape, Delta);
		Body->AABB.Center = Body->Pos;
	}
	
	if(Body->Pos.x < Min.x)
	{
		v2 NewPos = Body->Pos;
		Body->Pos.x = Max.x;
		v2 Delta = Body->Pos - NewPos;
		UpdateVertices(&Body->Shape, Delta);
		Body->AABB.Center = Body->Pos;
	}
	
	if(Body->Pos.y < Min.y)
	{
		v2 NewPos = Body->Pos;
		Body->Pos.y = Max.y;
		v2 Delta = Body->Pos - NewPos;
		UpdateVertices(&Body->Shape, Delta);
		Body->AABB.Center = Body->Pos;
	}
	
	if(Body->Pos.y > Max.y)
	{
		v2 NewPos = Body->Pos;
		Body->Pos.y = Min.y;
		v2 Delta = Body->Pos - NewPos;
		UpdateVertices(&Body->Shape, Delta);
		Body->AABB.Center = Body->Pos;
	}
}

#define PHYSICS_H
#endif
