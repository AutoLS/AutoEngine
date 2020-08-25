#ifndef AE_POLYGON_H
#define AE_POLYGON_H

enum AE_polygon_type
{
	AE_POLYGON_TRIANGLE,
	AE_POLYGON_RECTANGLE,
	AE_POLYGON_CUSTOM,
};

struct AE_polygon
{
	v2* Vertices;
	v2 Center;
	
	v2 Dim;

	int Count;
	
	real32 Theta;
	AE_polygon_type Type;
};

AE_polygon AE_CreatePolygon(AE_polygon_type Type, v2 Pos, v2 Dim, 
					  int Count = 0, v2* Vertices = 0)
{
	AE_polygon Polygon = {};
	Polygon.Dim = Dim;
	Polygon.Type = Type;
	switch(Type)
	{
		case AE_POLYGON_TRIANGLE:
		{
			Polygon.Count = 3;
			Polygon.Vertices = (v2*)malloc(sizeof(v2) * Polygon.Count);

			Polygon.Vertices[0] = V2(0, Dim.y*0.5f) + Pos;
			Polygon.Vertices[1] = V2(-Dim.x*0.5f, -Dim.y*0.5f) + Pos;
			Polygon.Vertices[2] = V2(Dim.x*0.5f, -Dim.y*0.5f) + Pos;
			
			Polygon.Center += Pos;
			Polygon.Theta = 0;
			
		} break;
		case AE_POLYGON_RECTANGLE:
		{
			Polygon.Count = 4;

			Polygon.Vertices = (v2*)malloc(sizeof(v2) * Polygon.Count);
			//TOP LEFT
			Polygon.Vertices[0] = V2(-Dim.x * 0.5f, Dim.y  * 0.5f) + Pos; 
			//BOTTOM LEFT
			Polygon.Vertices[1] = V2(-Dim.x * 0.5f, -Dim.y * 0.5f) + Pos; 
			//BOTTOM RIGHT
			Polygon.Vertices[2] = V2(Dim.x  * 0.5f, -Dim.y * 0.5f) + Pos; 
			//TOP RIGHT
			Polygon.Vertices[3] = V2(Dim.x  * 0.5f, Dim.y  * 0.5f) + Pos; 
			
			Polygon.Center += Pos;
			Polygon.Theta = 0;
		} break;
		case AE_POLYGON_CUSTOM:
		{
			Polygon.Count = Count;
			Polygon.Vertices = (v2*)malloc(sizeof(v2) * Count);
			memcpy((void*)Polygon.Vertices, (void*)Vertices, sizeof(v2) * Count);
			for(int i = 0; i < Count; ++i)
			{
				Polygon.Vertices[i] += Pos;
			}
			Polygon.Center += Pos;
		} break;
	}
	return Polygon;
}

void AE_UpdateVertices(AE_polygon* Shape, v2 Delta)
{
	for(int i = 0; i < Shape->Count; ++i)
	{
		Shape->Vertices[i] += Delta;
	}
	Shape->Center += Delta;
}

void AE_RotatePolygon(AE_polygon* Shape, real32 Theta)
{
	for(int i = 0; i < Shape->Count; ++i)
	{
		Shape->Vertices[i] -= Shape->Center;
		Shape->Vertices[i] = Rotate(Shape->Vertices[i], Theta);
		Shape->Vertices[i] += Shape->Center;
	}
}

#endif