#ifndef AUTOGL_H
#define AUTOGL_H

#include "stb_image.cpp"

enum text_render_type
{
	TEXT_RENDER_SOLID,
	TEXT_RENDER_SHADED,
	TEXT_RENDER_BLENDED,	
};

struct object_data
{
	uint32 VAO;
	uint32 VBO;
	uint32 EBO;
	v3* Vertices;
	uint32* Indices;
	int Count;
	int TotalCount;
	int IndicesCount;
	GLenum Usage;
};

struct texture_data
{
	uint32 ID;
	v2 Dim;
	bool Success;
};

struct simple_renderer
{
	uint32 Shader;
	object_data Rect;
	object_data Tri;
};

//Functions for loading uniforms to shaders
void SetReal32(uint32 Shader, char* Location, real32 A);
void SetVec3(uint32 Shader, char* Location, v3 A);
void SetVec4(uint32 Shader, char* Location, v4 A);
void SetMat4(uint32 Shader, char* Location, mat4 A, 
			 GLenum IsTranspose = GL_TRUE);
void SetInt(uint32 Shader, char* Location, int Entry);

object_data InitObject( v3* Vertices, 
						int Count, int TotalCount, 
						uint32* Indices = 0, 
						int IndicesCount = 0, 
						GLenum Usage = GL_STATIC_DRAW);

void UploadObjectData(object_data* Object);

void UpdateObjectData(object_data* Object, v3* Vertices, int Count);

void AttribPointer( object_data* Object, uint32 MemLoc, 
					int Size, 
					GLsizei Stride, 
					int Offset);

simple_renderer InitSimpleRenderer();

void SetGlobalUniforms(uint32 Renderer, mat4 Projection, mat4 View);

//Basic rendering functions for openGL
void DrawObject(uint32 Renderer, object_data* Object, 
				v3 Pos, v3 Dim, 
				real32 Theta, v3 Axis = V3(), 
				v4 Color = {}, uint32 Texture = 0, bool Blend = false);

void BasicDraw(uint32 Renderer, object_data* Object, v3 Pos, v3 Dim,
				v4 Color = {}, real32 Theta = 0, v3 Axis = V3(), 
				bool Fill = false, bool Blend = true);
				
void RenderSprite(uint32 Renderer, texture_data Texture, rect32* Clips, 
					object_data* Object, render_flip Flip, v3 Pos, v3 Dim, 
					real32 Theta = 0, v3 Axis = V3());

//Functions for loading textures
uint32 LoadTextureGL(char* Path, bool Alpha = false, bool Flip = false);

texture_data LoadTextureGLEX(char* Path, bool Alpha = false, bool Flip = false);

char* LoadShaderSource(char* Path);

uint32 CreateRenderer(char* VertexShaderSource, char* FragmentShaderSource);

#endif