#ifndef MATH_H
#define MATH_H

#include <stdint.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#if DEBUG_MODE
#define Assert(Expression) if(!(Expression)) {*(int*)0 = 0;}
#else 
#define Assert(Expression)
#endif

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;
typedef float real32;
typedef double real64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#define PI 3.1415926f
#define ONE_OVER_180 0.005555

real32 Cos(real32 Theta);
real32 Sin(real32 Theta);
real32 Rand32(real32 Low, real32 High);
real32 Radians(real32 Theta);
// real32 Dot(v2 A, v2 B);
// real32 Dot(v3 A, v3 B);

#include "vec2D.h"
#include "vec3D.h"

union v4 
{
	struct
	{
		real32 x, y, z, w;
	};
	struct
	{
		real32 r, g, b, a;
	};
};

union v4i
{
	struct
	{
		int x, y, z, w;
	};
	struct
	{
		int r, g, b, a;
	};
};

struct coord
{
	v3 X, Y, Z;
};

struct rect32
{
	v2 Pos;
	v2 Dim;
};

struct edge
{
	v2 e;
	v2 a;
	v2 b;
};

v3 V3(v4 A)
{
	v3 Result = {A.x, A.y, A.z};
	return Result;
}

v4 V4(real32 x = 0, real32 y = 0, real32 z = 0, real32 w = 1)
{
	v4 Result = {x, y, z, w};
	return Result;
}

v4 V4(v3 A, real32 w = 1)
{
	v4 Result = {A.x, A.y, A.z, w};
	return Result;
}

inline v4 Color(real32 r = 1, real32 g = 1, real32 b = 1, real32 a = 1)
{
	real32 OneOver255 = 1.0f / 255.0f;
	
	if(r > 1) r *= OneOver255;
	if(g > 1) g *= OneOver255;
	if(b > 1) b *= OneOver255;
	if(a > 1) a *= OneOver255;
	
	v4 Result = {r, g, b, a};
	return Result;
}

v4i V4i(int x = 0, int y = 0, int z = 0, int w = 0)
{
	v4i Result = {x, y, z, w};
	return Result;
}

rect32 Rect32(v2 Pos = {}, v2 Dim = {})
{
	rect32 Result = {Pos, Dim};
	return Result;
}

rect32 WinRect32(v2 Pos, v2 Dim)
{
	rect32 Result = {Pos - (Dim * 0.5f), Dim};
	return Result;
}

v2 GetMidPoint(rect32* A, rect32* B)
{
	v2 Result = {};
	Result.x = A->Pos.x + (((B->Pos.x + B->Dim.x) - A->Pos.x) * 0.5f);
	Result.y = A->Pos.y + A->Dim.y * 0.5f;
	
	return Result;
}

real32 Abs(real32 n)
{
	real32 Result = (real32)fabs(n);
	return Result;
}

v2 Abs(v2 A)
{
	v2 Result = V2(Abs(A.x), Abs(A.y));
	return Result;
}

v3 Abs(v3 A)
{
	v3 Result = V3(Abs(A.x), Abs(A.y), Abs(A.z));
	return Result;
}

v2 AbsDot(v2 A)
{
	v2 Result = Abs(A);
	return Result;
}

v3 AbsDot(v3 A)
{
	v3 Result = Abs(A);
	return Result;
}

real32 GetMin(v3 A)
{
	real32 Result = (real32)fmin(A.x, (real32)fmin(A.y, A.z));
	return Result;
}

real32 GetMax(v3 A)
{
	real32 Result = (real32)fmax(A.x, (real32)fmax(A.y, A.z));
	return Result;
}

int MaxDimension(v3 A)
{
	int Result = (A.x > A.y) ? ((A.x > A.z) ? 0 : 2) : ((A.y > A.z) ? 1 : 2);
	return Result;
}

v3 Permute(v3 A, int x, int y, int z)
{
	Assert(x >= 0 && x <= 2);
	Assert(y >= 0 && y <= 2);
	Assert(z >= 0 && z <= 2);
	v3 Result = V3(A.E[x], A.E[y], A.E[z]);
	
	return Result;
}

coord CoordinateSystem(v3 X)
{
	v3 Y, Z;
	if(Abs(X.x) > Abs(X.y))
	{
		Y = V3(-X.z, 0 , X.x) / sqrtf(X.x * X.x + X.z * X.z);
	}
	else
	{
		Y = V3(0, X.z, -X.y) / sqrtf(X.y * X.y + X.z * X.z);
	}
	Z = Cross(X, Y);
	return {X, Y, Z};
}

v3 FaceForward(v3 N, v3 A)
{
	v3 Result = Dot(N, A) < 1.0f ? -N : N;
	return Result;
}

v2 Rotate(v2 A, real32 Theta)
{
	v2 Result = V2(Cos(Theta)*A.x - Sin(Theta)*A.y, 
				   Sin(Theta)*A.x + Cos(Theta)*A.y);
	return Result;
}

v3 RotateZ(v3 A, real32 Theta)
{
	real32 x = A.x * Cos(Theta) - A.y * Sin(Theta);
	real32 y = A.y * Cos(Theta) + A.x * Sin(Theta);
	v3 Result = V3(x, y, 0);
	return Result;
}

v3 RotateAroundOrigin(v3 A, v3 Origin, real32 Theta)
{
	v3 Result = A;
	
	Result -= Origin;
	RotateZ(Result, Theta);
	Result += Origin;
	
	return Result;
}

v3 Perp_v3(v3 A)
{
	v3 Result = {-A.y, A.x};
	return Result;
}

v3 NDC(v3 A, v3 B)
{
	return A / B;
}

real32 GetAngle(v2 A, v2 B)
{
	real32 Result = acosf(Dot(A, B) / (Length(A) * Length(B)));
	return Result;
}

v3 Project(v3 A, v3 B)
{
	v3 Result = Dot(A, B) * Normalize(B); //Project A onto B;
	return Result;
}

v3 Reflect(v3 A, v3 N)
{
	v3 Result = A - 2.0f * Project(A, N);
	return Result;
}

v2 TripleProduct(v2 A, v2 B, v2 C)
{
	v2 Result;
	
	real32 ac = Dot(A, C);
	real32 bc = Dot(B, C);
	
	Result = V2(B.x * ac - A.x * bc, B.y * ac - A.y * bc);
	
	return Result;
}

v3 Hadamard(v3 A, v3 B)
{
    v3 Result = {A.x*B.x, A.y*B.y, A.z*B.z};

    return(Result);
}

v4 Hadamard(v4 A, v4 B)
{
    v4 Result = {A.x*B.x, A.y*B.y, A.z*B.z, A.w*B.w};

    return(Result);
}

v4 Hadamard(v4 A, real32 k)
{
    v4 Result = {A.x*k, A.y*k, A.z*k, A.w*k};

    return(Result);
}

real32 Lerp(real32 s, real32 e, real32 t)
{
	real32 Result = s*(1-t) + (e * t);
	return Result;
}

v3 Lerp(v3 s, v3 e, real32 t)
{
	v3 Result = s*(1-t) + (e * t);
	return Result;
}

#ifdef USE_LEGACY_MATH
//IMPORTANT: Matrices are construct in row major! Which means that each basis vector of a coordinate space are store in rows!
union mat4
{
	struct
	{
		real32 E[4][4];
	};
	
	struct 
	{
		real32 m[16];
	};
};

mat4 Mat4()
{
	mat4 Result = {};
	for(int r = 0; r < 4; ++r)
	{
		Result.E[r][r] = 1;
	}
	
	return Result;
}

mat4 Mat4Rand()
{
	mat4 Result;
	for(int r = 0; r < 4; ++r)
	{
		for(int c = 0; c < 4; ++c)
		{
			Result.E[r][c] = Rand32(0, 100);
		}
	}
	
	return Result;
}

mat4 operator*(mat4 A, mat4 B)
{
	mat4 Result = {};
	for(int r = 0; r < 4; ++r)
	{
		for(int c = 0; c < 4; ++c)
		{
			Result.E[r][c] = A.E[r][0] * B.E[0][c] +
							 A.E[r][1] * B.E[1][c] +
							 A.E[r][2] * B.E[2][c] +
							 A.E[r][3] * B.E[3][c];
		}
	}
	return Result;
}

v4 operator*(mat4 &A, v4 B)
{
	v4 Result = {};
	real32* PtrResult = &Result.x;
	for(int i = 0; i < 4; ++i)
	{
		*(PtrResult + i) = A.E[i][0] * B.x +
						   A.E[i][1] * B.y +
						   A.E[i][2] * B.z +
						   A.E[i][3] * B.w;
	}
	return Result;
}

v4 operator*(v4 B, mat4 &A)
{
	v4 Result = {};
	real32* PtrResult = &Result.x;
	for(int i = 0; i < 4; ++i)
	{
		*(PtrResult + i) = A.E[i][0] * B.x +
						   A.E[i][1] * B.y +
						   A.E[i][2] * B.z +
						   A.E[i][3] * B.w;
	}
	return Result;
}

mat4 Transpose(mat4 A)
{
	mat4 Result = Mat4();
	
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			Result.E[i][j] = A.E[j][i];
		}
	}
	
	return Result;
}

mat4 Scale(mat4& A, v3 K)
{
	mat4 Result = A;
	real32* Ptr = &K.x;
	for(int r = 0; r < 3; ++r)
	{
		Result.E[r][r] *= *(Ptr + r);
	}
	
	return Result;
}

mat4 ArbitraryAxisScale(v3 Axis, real32 k)
{
	mat4 Result = Mat4();
	
	v3* p = (v3*)&Result.E[0][0];
	p->x = 1 + ((k-1)*Axis.x*Axis.x);
	p->y = (k-1)*Axis.x*Axis.y;
	p->z = (k-1)*Axis.x*Axis.z;
	
	v3* q = (v3*)&Result.E[1][0];
	q->x = (k-1)*Axis.x*Axis.y;
	q->y = 1 + ((k-1)*Axis.y*Axis.y);
	q->z = ((k-1)*Axis.y*Axis.z);
	
	v3* r = (v3*)&Result.E[2][0];
	r->x = (k-1)*Axis.x*Axis.z;
	r->y = (k-1)*Axis.y*Axis.z;
	r->z = 1 + ((k-1)*Axis.z*Axis.z);
	
	return Result;
}

mat4 Translate(mat4 A, v3 T)
{
	mat4 Result = A;
	
	Result.E[0][3] += T.x;
	Result.E[1][3] += T.y;
	Result.E[2][3] += T.z;

	return Result;
}

mat4 Rotate(mat4 A, v3 Axis, real32 Theta)
{
	if(Length(Axis))
	Axis = Normalize(Axis);
	
	mat4 Result = Mat4();
	
	//p'
	Result.E[0][0] = cosf(Theta) + ((Axis.x * Axis.x) * (1 - cosf(Theta)));
	Result.E[0][1] = (Axis.x * Axis.y * (1 - cosf(Theta))) - (Axis.z * sinf(Theta));
	Result.E[0][2] = (Axis.x * Axis.z * (1 - cosf(Theta))) + (Axis.y * sinf(Theta));
	
	//q'
	Result.E[1][0] = (Axis.y * Axis.x * (1 - cosf(Theta))) + (Axis.z * sinf(Theta));
	Result.E[1][1] = cosf(Theta) + ((Axis.y * Axis.y) * (1 - cosf(Theta)));
	Result.E[1][2] = (Axis.z * Axis.y * (1 - cosf(Theta))) - (Axis.x * sinf(Theta));
	
	//r'
	Result.E[2][0] = (Axis.z * Axis.x * (1 - cosf(Theta))) - (Axis.y * sinf(Theta));
	Result.E[2][1] = (Axis.z * Axis.y * (1 - cosf(Theta))) + (Axis.x * sinf(Theta));
	Result.E[2][2] = cosf(Theta) + ((Axis.z * Axis.z) * (1 - cosf(Theta)));
	
	Result = Result * A;
	
	return Result;
}

mat4 RotateZ(mat4& A, real32 Theta)
{
	mat4 Result = Mat4();
	Result.E[0][0] = Cos(Theta);
	Result.E[0][1] = -Sin(Theta);
	Result.E[1][0] = Sin(Theta);
	Result.E[1][1] = Cos(Theta);
	
	return A * Result;
}

mat4 Ortho(real32 Left, real32 Right, 
		   real32 Bottom, real32 Top, 
		   real32 Near, real32 Far)
{
	mat4 Result = Mat4();
	Result.E[0][0] = 2 / (Right - Left);
	Result.E[1][1] = 2 / (Top - Bottom);
	Result.E[2][2] = -2 / (Far - Near);
	
	Result.E[0][3] = -(Right + Left) / (Right - Left);
	Result.E[1][3] = -(Top + Bottom) / (Top - Bottom);
	Result.E[2][3] = -(Far + Near) / (Far - Near);
	
	return Result;
}

mat4 Perspective(real32 FOV, real32 Aspect, real32 Near, real32 Far)
{
	mat4 Result = Mat4();
	
	real32 Top = Near * (real32)tan(Radians(FOV)/2);
	real32 Bottom = -Top;
	real32 Right = Top * Aspect;
	real32 Left = -Right;
	
	Result.E[0][0] = 2 * Near/(Right - Left);
	Result.E[1][1] = 2 * Near/(Top - Bottom);
	Result.E[2][2] = -(Far + Near)/(Far - Near);
	
	Result.E[0][3] = -Near * (Right + Left) / (Right - Left);
	Result.E[1][3] = -Near * (Top + Bottom) / (Top - Bottom);
	Result.E[2][3] = 2 * Far * Near / (Near - Far);
	
	Result.E[3][2] = -1;
	
	return Result;
}

mat4 LookAt(v3 Pos, v3 Target, v3 Up)
{
	v3 Dir = Normalize(Pos - Target);
	v3 Right = Normalize(Cross(Up, Dir));
	v3 CameraUp = Cross(Dir, Right);
	
	mat4 Rotation = Mat4();
	Rotation.E[0][0] = Right.x;
	Rotation.E[0][1] = Right.y;
	Rotation.E[0][2] = Right.z;
	
	Rotation.E[1][0] = CameraUp.x;
	Rotation.E[1][1] = CameraUp.y;
	Rotation.E[1][2] = CameraUp.z;
	
	Rotation.E[2][0] = Dir.x;
	Rotation.E[2][1] = Dir.y;
	Rotation.E[2][2] = Dir.z;
	
	mat4 Translation = Mat4();
	Translation = Translate(Translation, -Pos);
	
	return Rotation * Translation;
}
#else 
#include "mat4D.h"
#endif

int Min(int A, int B, int Equal = 0)
{
	int Result = 0;
	if(A < B)
		Result = A;
	else if(B < A)
		Result = B;
	return Result;
}

real32 Min(real32 A, real32 B)
{
	real32 Result = 0;
	if(A < B)
		Result = A;
	else if(B < A)
		Result = B;
	return Result;
}

real32 Max(real32 A, real32 B)
{
	real32 Result = 0; 
	if(A > B)
		Result = A;
	else if(B > A)
		Result = B;
	else
		Result = A;
	return Result;
}

int Round32(real32 n)
{
	int Result = (int)(n + 0.5f);
	return Result;
}

uint32 RoundU32(real32 n)
{
	uint32 Result = (uint32)(n + 0.5f);
	return Result;
}


v3 SphericalToCartesian(real32 Theta, real32 Phi)
{
	v3 Result = {cosf(Phi) * sinf(Theta), sinf(Phi) * sinf(Theta), cosf(Theta)};
	return Result;
}

real32 Radians(real32 Theta)
{
	real32 Result = (real32)(Theta * (PI/180));
	return Result;
}

real32 Degrees(real32 Theta)
{
	real32 Result = (real32)(Theta * (180/PI));
	return Result;
}

real32 Cos(real32 Theta)
{
	return (real32)cos((double)Theta);
}

real32 Sin(real32 Theta)
{
	return (real32)sin((double)Theta);
}

real32 Rand32(real32 Low, real32 High)
{
	if(Low == High) return Low;
	real32 Result = Low + (real32)(rand() / (real32)(RAND_MAX/(High - Low)));
	return Result;
}

uint32 Rand(uint32 Low, uint32 High)
{
	if(Low == High) return Low;
	uint32 Result = rand() % (High + 1 - Low) + Low;
	return Result;
}

int Rand(int Low, int High)
{
	if(Low == High) return Low;
	int Result = rand() % (High + 1 - Low) + Low;
	return Result;
}

uint32 RGBAPack4x8(v4 Unpacked)
{
	uint32 Result = RoundU32(Unpacked.a) << 24 |
					RoundU32(Unpacked.r) << 16 |
					RoundU32(Unpacked.g) << 8 |
					RoundU32(Unpacked.b) << 0;
					
	return Result;
}

inline v4
Linear1ToSRGB255(v4 C)
{
    v4 Result;

    real32 One255 = 255.0f;

    Result.r = One255*sqrtf(C.r);
    Result.g = One255*sqrtf(C.g);
    Result.b = One255*sqrtf(C.b);
    Result.a = One255*C.a;

    return(Result);
}

//Top left
bool IsPointInRect(v2 Point, rect32* Rect)
{
	if(Point.x < Rect->Pos.x)
	{
		return false;
	}
	if(Point.x > Rect->Pos.x + Rect->Dim.x)
	{
		return false;
	}
	if(Point.y < Rect->Pos.y)
	{
		return false;
	}
	if(Point.y > Rect->Pos.y + Rect->Dim.y)
	{
		return false;
	}
	return true;
}

enum rect_position
{
	POSITION_TOP_LEFT,
	POSITION_TOP_RIGHT,
	POSITION_BOTTOM_LEFT,
	POSITION_BOTTOM_RIGHT,
	POSITION_CENTERED,
	POSITION_CUSTOM
};

v2 GetPos(v2 Offset, v2 DstDim, rect_position Position = POSITION_CENTERED)
{
	v2 Result = {};
	
	switch(Position)
	{
		case POSITION_CENTERED:
		{
			Result = DstDim * 0.5f + Offset;
		};
		
		case POSITION_TOP_RIGHT:
		{
			Result = V2(DstDim.x, 0) + Offset;
		};
	}
	
	return Result;
}

v2 GetMaxPosX(v2 Pos, v2 Dim)
{
	v2 Result = {Pos.x + Dim.x, Pos.y};
	return Result;
}

v2 GetMaxPosFromRectX(rect32* Rect)
{
	v2 Result = {};
	Result = V2(Rect->Pos.x + Rect->Dim.x, Rect->Pos.y);
	
	return Result;
}

void SetRect32ScreenSpace(rect32* src, rect32* dst, v2 Offset, 
						  rect_position Position)
{
	switch(Position)
	{
		case POSITION_CENTERED:
		{
			src->Pos.x = dst->Pos.x + (dst->Dim.x*0.5f) + Offset.x - 
						 (src->Dim.x*0.5f);
			src->Pos.y = dst->Pos.y + (dst->Dim.y*0.5f) + Offset.y - 
						 (src->Dim.y*0.5f);
		} break;
		case POSITION_TOP_RIGHT:
		{
			src->Pos.x = dst->Pos.x + dst->Dim.x + Offset.x - src->Dim.x;
			src->Pos.y = dst->Pos.y + Offset.y;
		} break;
		case POSITION_TOP_LEFT:
		{
			src->Pos.x = dst->Pos.x + Offset.x;
			src->Pos.y = dst->Pos.y + Offset.y;
		} break;
		case POSITION_BOTTOM_LEFT:
		{
			src->Pos.x = dst->Pos.x + Offset.x;
			src->Pos.y = dst->Pos.y + dst->Dim.y + Offset.y - src->Dim.y;
		} break;
		case POSITION_BOTTOM_RIGHT:
		{
			src->Pos.x = dst->Pos.x + dst->Dim.x + Offset.x - src->Dim.x;
			src->Pos.y = dst->Pos.y + dst->Dim.y + Offset.y - src->Dim.y;
		} break;
	}
}

void SetRect32(rect32* src, rect32* dst, v2 Offset, 
			   v2 Scale, v2 Dim, rect_position Position)
{
	real32 w;
	real32 h;
	
	if(Dim.x || Dim.y)
	{
		w = Dim.x;
		h = Dim.y;
	}
	else
	{
		w = src->Dim.x;
		h = src->Dim.y;
	}
	
	w *= Scale.x;
	h *= Scale.y;
	src->Dim.x = w;
	src->Dim.y = h;
	
	switch(Position)
	{
		case POSITION_TOP_RIGHT:
		{
			src->Pos.x = dst->Pos.x + (dst->Dim.x * 0.5f) + Offset.x - (src->Dim.x * 0.5f);
			src->Pos.y = dst->Pos.y - (dst->Dim.y * 0.5f) + Offset.y + (src->Dim.y * 0.5f);
		} break;
		case POSITION_TOP_LEFT:
		{
			src->Pos.x = dst->Pos.x + Offset.x + (src->Dim.x * 0.5f);
			src->Pos.y = dst->Pos.y + Offset.y + (src->Dim.y * 0.5f);
		} break;
		case POSITION_BOTTOM_LEFT:
		{
			src->Pos.x = dst->Pos.x + Offset.x + (src->Dim.x * 0.5f);
			src->Pos.y = dst->Pos.y + (dst->Dim.y) + Offset.y - (src->Dim.y * 0.5f);
		} break;
		case POSITION_BOTTOM_RIGHT:
		{
			src->Pos.x = dst->Pos.x + (dst->Dim.x * 0.5f) + Offset.x - (src->Dim.x * 0.5f);
			src->Pos.y = dst->Pos.y + (dst->Dim.y * 0.5f) + Offset.y - (src->Dim.y * 0.5f);
		} break;
	}
}

void SetRectPosition(SDL_Rect* SrcRect, SDL_Rect* DstRect, 
					 v2 Offset, rect_position Position = POSITION_CENTERED)
{
	switch(Position)
	{
		case POSITION_CENTERED:
		{
			
		} break;
	}
}

void SetRectPosition(SDL_Rect* Rect, v2 Offset, v2 Scale, v2 ScreenDimension, 
						rect_position Postion, v2 Dimension = {})
{
	real32 w;
	real32 h;
	
	if(Dimension.x || Dimension.y)
	{
		w = Dimension.x;
		h = Dimension.y;
	}
	else
	{
		w = (real32)Rect->w;
		h = (real32)Rect->h;
	}
	
	w *= Scale.x;
	h *= Scale.y;
	Rect->w = (int)w;
	Rect->h = (int)h;
	switch(Postion)
	{
		case POSITION_CENTERED:
		{
			Rect->x = (int)((ScreenDimension.x - Rect->w) * 0.5f + Offset.x);
			Rect->y = (int)((ScreenDimension.y - Rect->h) * 0.5f + Offset.y);
		} break;
		case POSITION_BOTTOM_RIGHT:
		{
			Rect->x = (int)(ScreenDimension.x - Rect->w + Offset.x);
			Rect->y = (int)(ScreenDimension.y - Rect->h + Offset.y);
		} break;
		case POSITION_BOTTOM_LEFT:
		{
			Rect->x = (int)(0 + Offset.x);
			Rect->y = (int)(ScreenDimension.y - Rect->h + Offset.y);
		} break;
		case POSITION_TOP_LEFT:
		{
			Rect->x = (int)(0 + Offset.x);
			Rect->y = (int)(0 + Offset.y);
		} break;
		case POSITION_TOP_RIGHT:
		{
			Rect->x = (int)(ScreenDimension.x - Rect->w + Offset.x);
			Rect->y = (int)(0 + Offset.y);
		} break;
		case POSITION_CUSTOM:
		{
			Rect->x = (int)(Offset.x);
			Rect->y = (int)(Offset.y);
		} break;
	}
}

void PrintV2(v2 A)
{
	printf("(%.1f, %.1f)\n", A.x, A.y);
}

void PrintV2(v2i A)
{
	printf("(%d, %d)\n", A.x, A.y);
}

void PrintV3(v3 A)
{
	printf("(%.1f, %.1f, %.1f)\n", A.x, A.y, A.z);
}

void PrintV4(v4 A)
{
	printf("(%.1f, %.1f, %.1f, %.1f)\n", A.x, A.y, A.z, A.w);
}

void PrintMat4(mat4 A)
{
	for(int r = 0; r < 4; ++r)
	{
		printf("[");
		for(int c = 0; c < 4; ++c)
		{
			printf("%.0f", A.E[r][c]);
			if(c < 3)
			printf("\t");
		}
		printf("]\n");
	}
}

#endif