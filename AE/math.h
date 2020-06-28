#ifndef MATH_H
#define MATH_H

#include <stdint.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;
typedef float real32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#define PI 3.1415926
#define ONE_OVER_180 0.005555

real32 Cos(real32 Theta);
real32 Sin(real32 Theta);
real32 Rand32(real32 Low, real32 High);
real32 Radians(real32 Theta);

struct v2 
{
	real32 x, y;
};

struct v2i
{
	int x, y;
};

struct v3
{
	real32 x, y, z;
};

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

v2 V2(real32 x = 0, real32 y = 0)
{
	v2 Result = {x, y};
	return Result;
}

v2 V2(v2i A)
{
	v2 Result = {(real32)A.x, (real32)A.y};
	return Result;
}

v2 IV2()
{
	v2 Result = V2(1, 1);
	return Result;
}
	
v2i V2i(int x = 0, int y = 0)
{
	v2i Result = {x, y};
	return Result;
}

v3 V3(real32 x = 0, real32 y = 0, real32 z = 0)
{
	v3 Result = {x, y, z};
	return Result;
}

v3 V3(v2i A, real32 z = 0)
{
	v3 Result = {(real32)A.x, (real32)A.y, z};
	return Result;
}

v3 V3(v2 A, real32 z = 0)
{
	v3 Result = {A.x, A.y, z};
	return Result;
}

v3 V3(v4 A)
{
	v3 Result = {A.x, A.y, A.z};
	return Result;
}

v3 IV3()
{
	v3 Result = {1, 1, 1};
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

v4 Color(real32 r = 255, real32 g = 255, real32 b = 255, real32 a = 1.0f)
{
	v4 Result = {r/255, g/255, b/255, a};
	return Result;
}

v4i V4i(int x = 0, int y = 0, int z = 0, int w = 0)
{
	v4i Result = {x, y, z, w};
	return Result;
}

v2 operator+(v2 A, v2 B)
{
	v2 Result = {A.x + B.x, A.y + B.y};
	return Result;
}

v2 operator+(v2 A, real32 N)
{
	v2 Result = {A.x + N, A.y + N};
	return Result;
}

v2 operator-(v2 A, v2 B)
{
	v2 Result = {A.x - B.x, A.y - B.y};
	return Result;
}

v2 operator-(v2 A)
{
	v2 Result = {-A.x, -A.y};
	return Result;
}

v2 operator*(v2 A, real32 k)
{
	v2 Result = {k * A.x, k * A.y};
	return Result;
}

v2 operator*(real32 k, v2 A)
{
	v2 Result = {k * A.x, k * A.y};
	return Result;
}

v2 operator*(v2 A, v2 B)
{
	v2 Result = {B.x * A.x, B.y * A.y};
	return Result;
}

v2 operator/(v2 A, real32 k)
{
	v2 Result = {A.x/k, A.y/k};
	return Result;
}

v2 operator/(v2 A, v2 B)
{
	v2 Result = {A.x/B.x, A.y/B.y};
	return Result;
}

v2 &operator+=(v2 &A, v2 B)
{
	A = A + B;
	return A;
}

v2 &operator+=(v2 &A, real32 N)
{
	A = A + N;
	return A;
}

v2 &operator-=(v2 &A, v2 B)
{
	A = A - B;
	return A;
}

v2 &operator/=(v2 &A, v2 B)
{
	A = A / B;
	return A;
}

v3 operator+(v3 A, v3 B)
{
	v3 Result = {A.x + B.x, A.y + B.y, A.z + B.z};
	return Result;
}

v3 operator-(v3 A, v3 B)
{
	v3 Result = {A.x - B.x, A.y - B.y, A.z - B.z};
	return Result;
}

v3 operator-(v3 A)
{
	v3 Result = {-A.x, -A.y, -A.z};
	return Result;
}

v3 operator*(v3 A, v3 B)
{
	v3 Result = {A.x * B.x, A.y * B.y, A.z * B.z};
	return Result;
}

v3 operator*(v3 A, real32 k)
{
	v3 Result = {A.x * k, A.y * k, A.z * k};
	return Result;
}

v3 operator*(real32 k, v3 A)
{
	v3 Result = {A.x * k, A.y * k, A.z * k};
	return Result;
}

v3 operator/(v3 A, v3 B)
{
	v3 Result = {A.x / B.x, A.y / B.y, A.z / B.z};
	return Result;
}

v3 operator/(v3 A, real32 k)
{
	v3 Result = {A.x / k, A.y / k, A.z / k};
	return Result;
}

v3 &operator+=(v3 &A, v3 B)
{
	A = A + B;
	return A;
}

v3 &operator-=(v3 &A, v3 B)
{
	A = A - B;
	return A;
}

bool operator>(v2 A, real32 N)
{
	return A.x > N || A.y > N;
}

bool operator<(v2 A, real32 N)
{
	return A.x < N || A.y < N;
}

bool operator<=(v2 A, real32 N)
{
	return A.x <= N || A.y <= N;
}

rect32 Rect32(v2 Pos, v2 Dim)
{
	rect32 Result = {Pos, Dim};
	return Result;
}

rect32 WinRect32(v2 Pos, v2 Dim)
{
	rect32 Result = {Pos - (Dim * 0.5f), Dim};
	return Result;
}

real32 Length(v2 A)
{
	real32 Result = (real32)sqrt((A.x * A.x) + (A.y * A.y));
	return Result;
}

real32 Length(v3 A)
{
	real32 Result = (real32)sqrt((A.x * A.x) + (A.y * A.y) + (A.z * A.z));
	return Result;
}

v2 Rotate(v2 A, real32 Theta)
{
	v2 Result = V2(Cos(Theta)*A.x - Sin(Theta)*A.y, Sin(Theta)*A.x + Cos(Theta)*A.y);
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

v2 Perp_v2(v2 A)
{
	v2 Result = {-A.y, A.x};
	return Result;
}

v3 Perp_v3(v3 A)
{
	v3 Result = {-A.y, A.x};
	return Result;
}

v2 Normalize(v2 A)
{
	v2 Result = {};
	if(Length(A) == 0)
	{
		return V2(1, 1);
	}
	else
		Result = A / Length(A);
	return Result;
}

v3 Normalize(v3 A)
{
	v3 Result = {};
	if(Length(A) == 0)
	{
		return V3();
	}
	else
		Result = A / Length(A);
	return Result;
}

v3 NDC(v3 A, v3 B)
{
	return A / B;
}

real32 Dot(v2 A, v2 B)
{
	real32 Result = {A.x*B.x + A.y*B.y};
	return Result;
}

real32 Dot(v3 A, v3 B)
{
	real32 Result = {A.x*B.x + A.y*B.y + A.z*B.z};
	return Result;
}

real32 GetAngle(v2 A, v2 B)
{
	real32 Result = acosf(Dot(A, B) / (Length(A) * Length(B)));
	return Result;
}

v2 Project(v2 A, v2 B)
{
	v2 Result = Dot(A, B) * Normalize(B); //Project A onto B;
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

v3 Cross(v3 A, v3 B)
{
	v3 Result = {};
	Result.x = A.y*B.z - A.z*B.y;
	Result.y = A.z*B.x - A.x*B.z;
	Result.z = A.x*B.y - A.y*B.x;
	
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

struct mat4
{
	real32 E[4][4];
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

v4 operator*(mat4 A, v4 B)
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

mat4 Translate(mat4 A, v3 T)
{
	mat4 Result = A;
	
	Result.E[0][3] += T.x;
	Result.E[1][3] += T.y;
	Result.E[2][3] += T.z;
#if 0
	real32* Ptr = &T.x;
	for(int r = 0; r < 3; ++r)
	{
		Result.E[r][3] += *(Ptr + r);
	}
#endif
	return Result;
}

mat4 Rotate(mat4 A, v3 Axis, real32 Theta)
{
	if(Length(Axis))
	Axis = Normalize(Axis);
	
	mat4 Result = Mat4();
	Result.E[0][0] = cosf(Theta) + ((Axis.x * Axis.x) * (1 - cosf(Theta)));
	Result.E[0][1] = (Axis.x * Axis.y * (1 - cosf(Theta))) - (Axis.z * sinf(Theta));
	Result.E[0][2] = (Axis.x * Axis.z * (1 - cosf(Theta))) + (Axis.y * sinf(Theta));
	
	Result.E[1][0] = (Axis.y * Axis.x * (1 - cosf(Theta))) + (Axis.z * sinf(Theta));
	Result.E[1][1] = cosf(Theta) + ((Axis.y * Axis.y) * (1 - cosf(Theta)));
	Result.E[1][2] = (Axis.z * Axis.y * (1 - cosf(Theta))) - (Axis.x * sinf(Theta));
	
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

void PrintV2(v2 A)
{
	printf("(%.1f, %.1f)\n", A.x, A.y);
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