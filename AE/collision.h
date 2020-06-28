#ifndef COLLISION_H
#define COLLISION_H

//AABB Bouding box 2D - Center Radius representation
struct b_box
{
	v2 Center;
	real32 r[2];
};

bool TestAABB_Box(b_box* A, b_box* B)
{
	bool Result = true;
	if(abs(A->Center.x - B->Center.x) > A->r[0] + B->r[0]) Result = false;
	if(abs(A->Center.y - B->Center.y) > A->r[1] + B->r[1]) Result = false;
	return Result;
}

b_box ComputeAABB(v2 Center, v2* Vertices, int Count)
{
	real32 MaxX = -FLT_MAX;
	real32 MaxY = -FLT_MAX;
	for(int i = 0; i < Count; ++i)
	{
		real32 Proj = Dot(Vertices[i], V2(1, 0));
		if(Proj > MaxX)
		{
			MaxX = Proj;
		}
	}
	for(int i = 0; i < Count; ++i)
	{
		real32 Proj = Dot(Vertices[i], V2(0, 1));
		if(Proj > MaxY)
		{
			MaxY = Proj;
		}
	}
	
	b_box Result = {};
	Result.Center = Center;
	Result.r[0] = MaxX;
	Result.r[1] = MaxY;
	
	return Result;
}

struct collide_result
{
	bool Collide;
	v2 MTV;
	real32 Depth;
	v2 Normal;
	v2 d;
	bool Sign;
};

//GJK Algorithm
int FurthestPointInDirection(polygon* Shape, v2 Dir)
{
	real32 Max = -FLT_MAX;
	int Result = 0;
	for(int i = 0; i < Shape->Count; ++i)
	{
		real32 DotProduct = Dot(Shape->Vertices[i], Dir);
		if(DotProduct > Max)
		{
			Max = DotProduct;
			Result = i;
		}
	}
	return Result;
}

v2 SupportGJK(polygon* ShapeA, polygon* ShapeB, v2 Dir)
{
	int IndexA = FurthestPointInDirection(ShapeA, Dir);
	int IndexB = FurthestPointInDirection(ShapeB, -Dir);
	v2 p1 = ShapeA->Vertices[IndexA];
	v2 p2 = ShapeB->Vertices[IndexB];
	
	v2 Result = p1 - p2;
	
	return Result;
}

bool TestGJK(polygon* ShapeA, polygon* ShapeB, collide_result* CollideResult = 0)
{
	int Index = 0;
	real32 Min = FLT_MAX;
	
	v2 Simplex[3] = {};
	
	v2 d = ShapeB->Center - ShapeA->Center;
	
	Simplex[Index] = SupportGJK(ShapeA, ShapeB, d);
	
	v2 ClosestPoint = Simplex[Index];
	real32 Distance = Length(Simplex[Index]);
	if(Distance < Min)
	{
		Min = Distance;
	}
	
	v2 a = Simplex[0];
	if(Dot(a, d) <= 0)
	{
		if(CollideResult)
		{
			CollideResult->Collide = false;
		}
		return false;
	}
	d = -a;
	
	v2 b, c, ao, ab, ac, ab_perp, ac_perp;
	
	while(true)
	{
		a = Simplex[++Index] = SupportGJK(ShapeA, ShapeB, d);
		
		Distance = Length(a);
		if(Distance < Min)
		{
			Min = Distance;
			ClosestPoint = a;
		}
		
		if(Dot(a, d) <= 0)
		{
			if(CollideResult)
			{
				CollideResult->Collide = false;
			}
			return false;
		}
		
		ao = -a;
		
		if(Index < 2)
		{
			b = Simplex[0];
			ab = b - a;
			d = TripleProduct(ab, ao, ab);
			if(d.x * d.x + d.y * d.y == 0)
			{
				d = Perp_v2(ab);
			}
			continue;
		}
		
		b = Simplex[1];
        c = Simplex[0];
        ab = b - a; // from point A to B
        ac = c - a; // from point A to C
        
        ac_perp = TripleProduct(ab, ac, ac);
        
        if (Dot(ac_perp, ao) >= 0) 
		{
            d = ac_perp; // new direction is normal to AC towards Origin
        } 
		else 
		{
            ab_perp = TripleProduct(ac, ab, ab);
            
            if (Dot(ab_perp, ao) < 0)
			{
				if(CollideResult)
				{
					CollideResult->Collide = true;
					CollideResult->MTV = V2() - ClosestPoint;
					CollideResult->Depth = Min;
				}
                return true; // collision
			}
            
            Simplex[0] = Simplex[1]; // swap first element (point C)

            d = ab_perp; // new direction is normal to AB towards Origin
        }
        
        Simplex[1] = Simplex[2]; // swap element in the middle (point B)
        --Index;
	}
	
	return false;
}

//SAT Algorithm

v2 ProjectShapeToAxis(polygon* Shape, v2 Axis)
{
	v2 Result = V2();
	real32 Min = Dot(Shape->Vertices[0], Axis);
	real32 Max = Min;
	
	for(int i = 1; i < Shape->Count; ++i)
	{
		real32 P = Dot(Shape->Vertices[i], Axis);
		if(P < Min)
		{
			Min = P;
		}
		else if(P > Max)
		{
			Max = P;
		}
	}
	Result = V2(Min, Max);
	return Result;
}

bool IsProjectionOverlap(v2 ProjA, v2 ProjB)
{
	return !(ProjB.y < ProjA.x || ProjA.y < ProjB.x);
}

real32 GetOverlap(v2 ProjA, v2 ProjB)
{
	real32 Result = Min(ProjA.y, ProjB.y) - Max(ProjA.x, ProjB.x);
	return Result;
}

collide_result TestSAT(polygon* ShapeA, polygon* ShapeB)
{
	collide_result CollideResult = {};
	CollideResult.Collide = true;
	real32 Overlap = FLT_MAX;
	v2 Smallest = {};
	//NOTE: Remember to free this after function ends
	v2* AxesA = (v2*)malloc(sizeof(v2) * ShapeA->Count);
	v2* AxesB = (v2*)malloc(sizeof(v2) * ShapeB->Count);
	
	//Generate Axes
	for(int Index = 0; Index < ShapeA->Count; ++Index)
	{
		v2 Point1 = ShapeA->Vertices[Index];
		v2 Point2 = ShapeA->Vertices[Index+1 == ShapeA->Count ? 0 : Index+1];
		
		v2 Edge = Point1 - Point2;
		
		v2 Normal = Perp_v2(Edge);
		AxesA[Index] = Normalize(Normal);
	}
	
	for(int Index = 0; Index < ShapeB->Count; ++Index)
	{
		v2 Point1 = ShapeB->Vertices[Index];
		v2 Point2 = ShapeB->Vertices[Index+1 == ShapeB->Count ? 0 : Index+1];
		
		v2 Edge = Point1 - Point2;
		
		v2 Normal = Perp_v2(Edge);
		AxesB[Index] = Normalize(Normal);
	}
	
	//Projecting Axes
	for(int i = 0; i < ShapeA->Count; ++i)
	{
		v2 ProjA = ProjectShapeToAxis(ShapeA, AxesA[i]);
		v2 ProjB = ProjectShapeToAxis(ShapeB, AxesA[i]);
		
		if(!IsProjectionOverlap(ProjA, ProjB))
		{
			CollideResult.Collide = false;
		}
		else
		{
			real32 TempOverlap = GetOverlap(ProjA, ProjB);
			if(TempOverlap < Overlap)
			{
				Overlap = TempOverlap;
				Smallest = AxesA[i];
				CollideResult.Sign = ProjA.x < ProjB.x;
			}
		}
	}
	for(int i = 0; i < ShapeB->Count; ++i)
	{
		v2 ProjA = ProjectShapeToAxis(ShapeA, AxesB[i]);
		v2 ProjB = ProjectShapeToAxis(ShapeB, AxesB[i]);
		
		if(!IsProjectionOverlap(ProjA, ProjB))
		{
			CollideResult.Collide = false;
		}
		else
		{
			real32 TempOverlap = GetOverlap(ProjA, ProjB);
			if(TempOverlap < Overlap)
			{
				Overlap = TempOverlap;
				Smallest = AxesB[i];
				CollideResult.Sign = ProjA.x < ProjB.x;
			}
		}
	}
	
	if(CollideResult.Collide)
	{
		v2 d = ShapeB->Center - ShapeA->Center;
		CollideResult.Normal = Smallest;
		CollideResult.MTV =  -Normalize(d) * Overlap;
		CollideResult.d = -Normalize(d);
	}
	
	free(AxesA);
	free(AxesB);
	
	return CollideResult;
}

edge GetClosestEdge(polygon* Shape, v2 n)
{
	int Index = FurthestPointInDirection(Shape, n);
	v2 v = Shape->Vertices[Index];
	v2 l = Shape->Vertices[Index-1 < 0 ? Shape->Count-1: Index-1];
	v2 r = Shape->Vertices[Index+1 >= Shape->Count ? 0 : Index+1];
	
	v2 EdgeL = Normalize(v - l);
	v2 EdgeR = Normalize(v - r);
	
	edge e1 = {EdgeL, v, l};
	edge e2 = {EdgeR, v, r};
	
	if(Dot(EdgeR, n) <= Dot(EdgeL, n))
	{
		return e1;
	}
	else
	{
		return e2;
	}
}

v2* Clipping(polygon* ShapeA, polygon* ShapeB, collide_result* CollideResult)
{
	v2 n = CollideResult->Normal;
	edge EdgeA = GetClosestEdge(ShapeA, n);
	edge EdgeB = GetClosestEdge(ShapeB, -n);
	
	edge Reference, Incident;
	
	bool Flip = false;
	if(Dot(EdgeA.e, n) <= Dot(EdgeB.e, n))
	{
		Reference = EdgeA;
		Incident = EdgeB;
	}
	else
	{
		Reference = EdgeB;
		Incident = EdgeA;
		Flip = true;
	}
	
	v2 RefV = Normalize(Reference.e);
	
	return 0;
}

#endif