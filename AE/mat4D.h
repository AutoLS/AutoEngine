#ifndef MAT4D_H
#define MAT4D_H

//IMPORTANT: This is in column major
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

mat4 Scale(v3 k)
{
	mat4 Result = Mat4();
	
	Result.E[0][0] = k.x;
	Result.E[1][1] = k.y;
	Result.E[2][2] = k.z;
	
	return Result;
}

void Scale(mat4* A, v3 k)
{
	A->E[0][0] *= k.x;
	A->E[1][1] *= k.y;
	A->E[2][2] *= k.z;
}

mat4 ArbitraryAxisScale(v3 Axis, real32 k)
{
	mat4 Result = Mat4();
	
	//p
	Result.E[0][0] = 1 + ((k-1)*Axis.x*Axis.x);
	Result.E[1][0] = (k-1)*Axis.x*Axis.y;
	Result.E[2][0] = (k-1)*Axis.x*Axis.z;
	
	//q
	Result.E[0][1] = (k-1)*Axis.x*Axis.y;
	Result.E[1][1] = 1 + ((k-1)*Axis.y*Axis.y);
	Result.E[2][1] = ((k-1)*Axis.y*Axis.z);

	//r
	Result.E[0][2] = (k-1)*Axis.x*Axis.z;
	Result.E[1][2] = (k-1)*Axis.y*Axis.z;
	Result.E[2][2] = 1 + ((k-1)*Axis.z*Axis.z);
	
	return Result;
}

mat4 Translate(v3 T)
{
	mat4 Result = Mat4();
	
	Result.E[0][3] += T.x;
	Result.E[1][3] += T.y;
	Result.E[2][3] += T.z;

	return Result;
}

void Translate(mat4* A, v3 T)
{
	A->E[0][3] += T.x;
	A->E[1][3] += T.y;
	A->E[2][3] += T.z;
}

mat4 AxisRotate(v3 Axis, real32 Theta)
{
	Axis = Normalize(Axis);
	
	mat4 Result = Mat4();
	
	//p'
	Result.E[0][0] = cosf(Theta) + ((Axis.x * Axis.x) * (1 - cosf(Theta)));
	Result.E[1][0] = (Axis.x * Axis.y * (1 - cosf(Theta))) - (Axis.z*sinf(Theta));
	Result.E[2][0] = (Axis.x * Axis.z * (1 - cosf(Theta))) + (Axis.y*sinf(Theta));
	
	//q'
	Result.E[0][1] = (Axis.y * Axis.x * (1 - cosf(Theta))) + (Axis.z*sinf(Theta));
	Result.E[1][1] = cosf(Theta) + ((Axis.y * Axis.y) * (1 - cosf(Theta)));
	Result.E[2][1] = (Axis.z * Axis.y * (1 - cosf(Theta))) - (Axis.x*sinf(Theta));
	
	//r'
	Result.E[0][2] = (Axis.z * Axis.x * (1 - cosf(Theta))) - (Axis.y*sinf(Theta));
	Result.E[1][2] = (Axis.z * Axis.y * (1 - cosf(Theta))) + (Axis.x*sinf(Theta));
	Result.E[2][2] = cosf(Theta) + ((Axis.z * Axis.z) * (1 - cosf(Theta)));
		
	return Result;
}

mat4 RotateZ(real32 Theta)
{
	mat4 Result = Mat4();
	Result.E[0][0] = Cos(Theta);
	Result.E[0][1] = -Sin(Theta);
	Result.E[1][0] = Sin(Theta);
	Result.E[1][1] = Cos(Theta);
	
	return Result;
}

mat4 RotateX(real32 Theta)
{
	mat4 Result = Mat4();
	Result.E[1][1] = Cos(Theta);
	Result.E[2][1] = Sin(Theta);
	Result.E[1][2] = -Sin(Theta);
	Result.E[2][2] = Cos(Theta);
	
	return Result;
}

mat4 RotateY(real32 Theta)
{
	mat4 Result = Mat4();
	Result.E[0][0] = Cos(Theta);
	Result.E[2][0] = -Sin(Theta);
	Result.E[0][2] = Sin(Theta);
	Result.E[2][2] = Cos(Theta);
	
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
	Rotation.E[1][0] = Right.y;
	Rotation.E[2][0] = Right.z;
	
	Rotation.E[0][1] = CameraUp.x;
	Rotation.E[1][1] = CameraUp.y;
	Rotation.E[2][1] = CameraUp.z;
	
	Rotation.E[0][2] = Dir.x;
	Rotation.E[1][2] = Dir.y;
	Rotation.E[2][2] = Dir.z;
		
	return Translate(-Pos) * Rotation;
}

#endif