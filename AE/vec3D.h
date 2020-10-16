#ifndef VEC3D_H
#define VEC3D_H

union v3
{
	struct
	{
		real32 x, y, z;
	};
	real32 E[3];
};

inline v3 
V3(real32 x = 0, real32 y = 0, real32 z = 0)
{
	v3 Ret = {x, y, z};
	return Ret;
}

inline v3 
IV3()
{
	v3 Result = {1, 1, 1};
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

inline bool 
operator==(v3 A, v3 B)
{
	return (A.x == B.x && A.y == B.y && A.z == B.z);
}

inline bool 
operator!=(v3 A, v3 B)
{
	return A.x != B.x || A.y != B.y || A.z != B.z;
}

inline v3 
operator-(v3 A)
{
	v3 Ret = V3(-A.x, -A.y, -A.z);
	return Ret;
}

inline v3 
operator+(v3 A, v3 B)
{
	v3 Ret = V3(A.x + B.x, A.y + B.y, A.z + B.z);
	return Ret;
}

inline v3
operator-(v3 A, v3 B)
{
	v3 Ret = A + (-B);
	return Ret;
}

inline v3
operator*(v3 A, real32 k)
{
	v3 Ret = V3(A.x * k, A.y * k, A.z * k);
	return Ret;
}

inline v3
operator*(real32 k, v3 A)
{
	v3 Ret = V3(A.x * k, A.y * k, A.z * k);
	return Ret;
}

inline v3
operator/(v3 A, real32 k)
{
	Assert(k != 0);
	
	real32 OneOverK = 1.0f / k;
	v3 Ret = A * OneOverK;
	return Ret;
}

inline v3& 
operator+=(v3 &A, v3 B)
{
	A = A + B;
	return A;
}

inline v3&
operator-=(v3 &A, v3 B)
{
	A = A - B;
	return A;
}

v3 operator*(v3 A, v3 B)
{
	v3 Result = {A.x * B.x, A.y * B.y, A.z * B.z};
	return Result;
}

v3 operator/(v3 A, v3 B)
{
	v3 Result = {A.x / B.x, A.y / B.y, A.z / B.z};
	return Result;
}

inline real32 
Length(v3 A)
{
	real32 Ret = (real32)sqrt(A.x*A.x + A.y*A.y + A.z*A.z);
	return Ret;
}

real32 Distance(v3 A, v3 B)
{
	real32 Ret = Length(A - B);
	return Ret;
}

inline v3 
Normalize(v3 A)
{
	Assert(A != V3());
	v3 Ret = A / Length(A);
	return Ret;
}

inline real32 
Dot(v3 A, v3 B)
{
	real32 Ret = A.x*B.x + A.y*B.y + A.z*B.z;
	return Ret;
}

inline v3
Cross(v3 A, v3 B)
{
	v3 Ret = V3(A.z*B.y - A.y*B.z, 
				A.z*B.x - A.x*B.z, 
				A.x*B.y - A.y*B.x);
	return Ret;
}

#endif