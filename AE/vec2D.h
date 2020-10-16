#ifndef VEC2D_H
#define VEC2D_H

union v2 
{
	struct 
	{
		real32 x, y;
	};
	real32 E[2];
};

struct v2i
{
	int x, y;
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
	real32 inv = 1/k;
	v2 Result = {A.x * inv, A.y * inv};
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

v2i operator-(v2i A, v2i B)
{
	v2i Result = {A.x - B.x, A.y - B.y};
	return Result;
}

inline bool 
operator==(v2 A, v2 B)
{
	return (A.x == B.x && A.y == B.y);
}

inline bool 
operator!=(v2 A, v2 B)
{
	return A.x != B.x || A.y != B.y;
}

real32 Length(v2 A)
{
	real32 Result = (real32)sqrt((A.x * A.x) + (A.y * A.y));
	return Result;
}

v2 Perp_v2(v2 A)
{
	v2 Result = {-A.y, A.x};
	return Result;
}

real32 Dot(v2 A, v2 B)
{
	real32 Result = {A.x*B.x + A.y*B.y};
	return Result;
}

v2 Normalize(v2 A)
{
	v2 Result = {};
	if(Length(A) == 0)
	{
		return V2();
	}
	else
		Result = A / Length(A);
	return Result;
}

v2 Project(v2 A, v2 B)
{
	v2 Result = Dot(A, B) * Normalize(B); //Project A onto B;
	return Result;
}

#endif