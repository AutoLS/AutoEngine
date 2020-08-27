#include "AutoGL.h"

object_data InitObject(v3* Vertices, int Count, int TotalCount, uint32* Indices, 
						int IndicesCount, GLenum Usage)
{
	object_data Object = {};
	glGenVertexArrays(1, &Object.VAO);
	glGenBuffers(1, &Object.VBO);
	
	if(Indices)
	{
		glGenBuffers(1, &Object.EBO);
		Object.Indices = Indices;
		Object.IndicesCount = IndicesCount;
	}
	
	Object.Vertices = Vertices;
	Object.Count = Count;
	Object.TotalCount = TotalCount;
	Object.Usage = Usage;
	
	return Object;
}

void UploadObjectData(object_data* Object)
{
	glBindVertexArray(Object->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, Object->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(v3) * Object->TotalCount, Object->Vertices, Object->Usage);
	
	if(Object->Indices)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Object->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * Object->IndicesCount, 
						Object->Indices, Object->Usage);
	}

	glBindVertexArray(0);
}

void UpdateObjectData(object_data* Object, v3* Vertices, int Count)
{
	Object->Vertices = Vertices;
	Object->Count = Count;
	glBindVertexArray(Object->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, Object->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(v3) * Count, Vertices, Object->Usage);
}

void AttribPointer(object_data* Object, uint32 MemLoc, int Size, GLsizei Stride, int Offset)
{
	glBindVertexArray(Object->VAO);
	glVertexAttribPointer(MemLoc, Size, GL_FLOAT, GL_FALSE, 
							Stride * sizeof(real32), (void*)(sizeof(real32) * Offset));
	glEnableVertexAttribArray(MemLoc);
	glBindVertexArray(0);
}

simple_renderer InitSimpleRenderer(char* VertPath, char* FragPath)
{
	simple_renderer Renderer = {};
	Renderer.Shader = 
	CreateRenderer(LoadShaderSource(VertPath), 
				   LoadShaderSource(FragPath));
	
	v3 RectObjectVert[] =
	{
		V3(0.5f, 0.5f),   V3(1, 1),	//Top Right		
		V3(0.5f, -0.5f),  V3(1, 0),	//Bottom Right
		V3(-0.5f, -0.5f), V3(),		//Bottom Left
		V3(-0.5f, -0.5f), V3(),		//Bottom Left
		V3(-0.5f, 0.5f),  V3(0, 1),	//Top Left
		V3(0.5f, 0.5f),   V3(1, 1)	//Top Right	
	};
	
	v3 TriObjectVert[] =
	{
		V3(0, 0.5f),  V3(0.5, 1),	//Top Centre
		V3(-0.5f, -0.5f), V3(),		//Bottom Left
		V3(0.5f, -0.5f),  V3(1, 0),	//Bottom Right
	};
	
	Renderer.Rect = InitObject(RectObjectVert, 6, ArraySize(RectObjectVert));
	UploadObjectData(&Renderer.Rect);
	AttribPointer(&Renderer.Rect, 0, 3, 6, 0);
	AttribPointer(&Renderer.Rect, 1, 3, 6, 3);
	
	Renderer.Tri = 
	InitObject(TriObjectVert, 3, ArraySize(TriObjectVert));
	UploadObjectData(&Renderer.Tri);
	AttribPointer(&Renderer.Tri, 0, 3, 6, 0);
	AttribPointer(&Renderer.Tri, 1, 3, 6, 3);
	
	return Renderer;
}

void SetGlobalUniforms(uint32 Renderer, mat4 Projection, mat4 View)
{
	SetMat4(Renderer, "Projection", Projection);
	SetMat4(Renderer, "View", View);
}

void DrawObject(uint32 Renderer, object_data* Object, v3 Pos, v3 Dim, 
					real32 Theta, v3 Axis, v4 Color, uint32 Texture, bool Blend)
{
	Assert(Object);
	
	glUseProgram(Renderer);
	SetVec4(Renderer, "Color", Color);
	
	mat4 Transform = Mat4();
	Transform = Scale(Transform, Dim);
	Transform = Rotate(Transform, Axis, Theta);
	Transform = Translate(Transform, Pos);
	SetMat4(Renderer, "Transform", Transform);

	if(Blend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	if(Texture)
	{
		SetInt(Renderer, "Texture1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
	}
	glBindVertexArray(Object->VAO);
	if(Object->Indices)
	{
		glDrawElements(GL_TRIANGLES, Object->IndicesCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, Object->Count);
	}
	glDisable(GL_BLEND);
	glBindVertexArray(0);
	glUseProgram(0);
}

void BasicDraw(uint32 Renderer, object_data* Object, 
				v3 Pos, v3 Dim,
				v4 Color, 
				real32 Theta, v3 Axis, 
				bool Fill, bool Blend)
{
	glUseProgram(Renderer);
	SetVec4(Renderer, "Color", Color);
	
	mat4 Transform = Mat4();
	Transform = Scale(Transform, Dim);
	Transform = Rotate(Transform, Axis, Theta);
	Transform = Translate(Transform, Pos);
	SetMat4(Renderer, "Transform", Transform);
		
	if(Blend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
		
	glBindVertexArray(Object->VAO);
	if(Object->Indices)
	{
		glDrawElements(GL_TRIANGLES, Object->IndicesCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		if(Fill)
			glDrawArrays(GL_TRIANGLES, 0, Object->Count);
		else
			glDrawArrays(GL_LINE_LOOP, 0, Object->Count);
	}
	
	glDisable(GL_BLEND);
	glBindVertexArray(0);
	glUseProgram(0);
}

void DrawShape(uint32 Renderer, v2* Vertices, int Count, v4 Color, 
			   bool Blend, bool WireFrame)
{
	v3* Vertices_3 = (v3*)malloc(sizeof(v3) * Count);
	for(int i = 0; i < Count; ++i)
	{
		Vertices_3[i] = V3(Vertices[i]);
	}
	object_data Object = InitObject(Vertices_3, Count, Count);
	UploadObjectData(&Object);
	AttribPointer(&Object, 0, 3, 3, 0);
	
	glUseProgram(Renderer);
	SetVec4(Renderer, "Color", Color);
	
	mat4 Transform = Mat4();
	SetMat4(Renderer, "Transform", Transform);
	
	if(Blend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	if(WireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	
	glBindVertexArray(Object.VAO);
	glDrawArrays(GL_LINE_LOOP, 0, Object.Count);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_BLEND);
	glBindVertexArray(0);
	glUseProgram(0);
	
	glDeleteBuffers(1, &Object.VAO);
	glDeleteBuffers(1, &Object.VBO);
}

void RenderSprite(uint32 Renderer, uint32 Texture, rect32* Clips, 
					object_data* Object, render_flip Flip, v3 Pos, v3 Dim, 
					real32 Theta, v3 Axis)
{
	real32 tw = Clips->Dim.x;
	real32 th = Clips->Dim.y;
	
	real32 tx = Clips->Pos.x;
	real32 ty = Clips->Pos.y;
	
	switch(Flip)
	{
		case FLIP_HORIZONTAL:
		{
			tx = tx + tw;
			tw = -tw;
		} break;
		case FLIP_VERTICAL:
		{
			ty = ty + th;
			th = -th;
		} break;
	}
	
	v3 Vertices[] =
	{
		V3(0.5f, 0.5f),   V3(tx + tw, ty),		//Top Right		
		V3(0.5f, -0.5f),  V3(tx + tw, ty + th),	//Bottom Right
		V3(-0.5f, -0.5f), V3(tx, ty + th),		//Bottom Left
		V3(-0.5f, -0.5f), V3(tx, ty + th),		//Bottom Left
		V3(-0.5f, 0.5f),  V3(tx, ty),			//Top Left
		V3(0.5f, 0.5f),   V3(tx + tw, ty)		//Top Right	
	};
	
	UpdateObjectData(Object, Vertices, ArraySize(Vertices));
	
	glUseProgram(Renderer);
	SetVec4(Renderer, "Color", Color());
	
	mat4 Transform = Mat4();
	Transform = Scale(Transform, Dim);
	Transform = Rotate(Transform, Axis, Theta);
	Transform = Translate(Transform, Pos);
	SetMat4(Renderer, "Transform", Transform);
	
	SetInt(Renderer, "Texture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	
	glBindVertexArray(Object->VAO);
	glDrawArrays(GL_TRIANGLES, 0, Object->Count);
	
	glBindVertexArray(0);
	glUseProgram(0);
}

void UpdateTextureGL(uint32 Texture, char* Path, bool Alpha, bool Flip)
{
	stbi_set_flip_vertically_on_load(Flip);
	int Width, Height, Components;
    uint8 *Data = stbi_load(Path, &Width, &Height, &Components, 0);
    if (Data)
    {
        GLenum Format;
        if (Components == 1)
            Format = GL_RED;
        else if (Components == 3)
            Format = GL_RGB;
        else if (Components == 4)
            Format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
		if(Alpha)
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			
		}
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(Data);
    }
    else
    {
        printf("Texture failed to load at path: %s\n", Path);
        stbi_image_free(Data);
    }
}

void UpdateTextureGL(texture_data* Texture, char* Path, bool Alpha, bool Flip)
{
	stbi_set_flip_vertically_on_load(Flip);
	int Width, Height, Components;
    uint8 *Data = stbi_load(Path, &Width, &Height, &Components, 0);
    if (Data)
    {
		Texture->Dim = V2((real32)Width, (real32)Height);
		
        GLenum Format;
        if (Components == 1)
            Format = GL_RED;
        else if (Components == 3)
            Format = GL_RGB;
        else if (Components == 4)
            Format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, Texture->ID);
        glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
		if(Alpha)
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			
		}
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(Data);
		Texture->Success = true;
    }
    else
    {
		Texture->Success = false;
        printf("Texture failed to load at path: %s\n", Path);
        stbi_image_free(Data);
    }
}

uint32 LoadTextureGL(char* Path, bool Alpha, bool Flip)
{
	stbi_set_flip_vertically_on_load(Flip);
    uint32 TextureID;
    glGenTextures(1, &TextureID);
    
    int Width, Height, Components;
    uint8 *Data = stbi_load(Path, &Width, &Height, &Components, 0);
    if (Data)
    {
        GLenum Format;
        if (Components == 1)
            Format = GL_RED;
        else if (Components == 3)
            Format = GL_RGB;
        else if (Components == 4)
            Format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
		if(Alpha)
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			
		}
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(Data);
    }
    else
    {
        printf("Texture failed to load at path: %s\n", Path);
        stbi_image_free(Data);
    }

    return TextureID;
}

texture_data LoadTextureGLEX(char* Path, bool Alpha, bool Flip)
{
	texture_data Texture = {};
	stbi_set_flip_vertically_on_load(Flip);
	
    uint32 TextureID;
    glGenTextures(1, &TextureID);
    
    int Width, Height, Components;
    uint8* Data = 0;
	Data = stbi_load(Path, &Width, &Height, &Components, 0);
    if (Data)
    {
        GLenum Format;
        if (Components == 1)
            Format = GL_RED;
        else if (Components == 3)
            Format = GL_RGB;
        else if (Components == 4)
            Format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
		if(Alpha)
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(Data);
		Texture.Success = true;
    }
    else
    {
        printf("Texture failed to load at path: %s\n", Path);
        stbi_image_free(Data);
    }

    Texture.ID = TextureID;
	Texture.Dim = V2((real32)Width, (real32)Height);
	
	return Texture;
}

void UpdateTextGL(uint32 ID, TTF_Font* Font, char* Text, 
				  SDL_Color Color, 
				  bool FreeText = false,
				  SDL_Color BgColor = {},
			      text_render_type Type = TEXT_RENDER_BLENDED)
{
	SDL_Surface* Surface = 0;
	switch(Type)
	{
		case TEXT_RENDER_SOLID:
		{
			Surface = TTF_RenderText_Solid(Font, Text, Color);
		} break;
		
		case TEXT_RENDER_SHADED:
		{
			Surface = TTF_RenderText_Shaded(Font, Text, Color, BgColor);
		} break;
		
		case TEXT_RENDER_BLENDED:
		{
			Surface = TTF_RenderText_Blended(Font, Text, Color);			
		} break;
	}
	
	if(FreeText)
	{
		free(Text);
	}
	
	if(Surface)
	{
		uint8 BytesPerPixel = Surface->format->BytesPerPixel;
		GLenum Format = (BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
		
		glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, Format, 
					 Surface->w, Surface->h, 0, Format, 
				 	 GL_UNSIGNED_BYTE, Surface->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		printf("Failed to load text\n");
	}
	SDL_FreeSurface(Surface);
}

uint32 LoadTextGL(TTF_Font* Font, char* Text, 
					SDL_Color Color, SDL_Color BgColor = {},
					text_render_type Type = TEXT_RENDER_BLENDED)
{
	uint32 ID;
	glGenTextures(1, &ID);
	
	SDL_Surface* Surface;
	switch(Type)
	{
		case TEXT_RENDER_SOLID:
		{
			Surface = TTF_RenderText_Solid(Font, Text, Color);
		} break;
		
		case TEXT_RENDER_SHADED:
		{
			Surface = TTF_RenderText_Shaded(Font, Text, Color, BgColor);
		} break;
		
		case TEXT_RENDER_BLENDED:
		{
			Surface = TTF_RenderText_Blended(Font, Text, Color);			
		} break;
	}
	if(Surface)
	{
		uint8 BytesPerPixel = Surface->format->BytesPerPixel;
		GLenum Format = (BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
		
		glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, Format, 
					 Surface->w, Surface->h, 0, Format, 
				 	 GL_UNSIGNED_BYTE, Surface->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		printf("Failed to load text\n");
	}
	SDL_FreeSurface(Surface);
	return ID;
}

glyph_character LoadGlyphGL(TTF_Font* Font, uint16 Ch, SDL_Color Color)
{
	glyph_character Result = {};
	
	uint32 ID;
	glGenTextures(1, &ID);
	
	TTF_GlyphMetrics(Font, Ch, 
					&Result.MinX, &Result.MaxX, 
					&Result.MinY, &Result.MaxY,
					&Result.Advance);
	
	Result.Width = Result.MaxX - Result.MinX;
	Result.Height = TTF_FontHeight(Font);
	
	SDL_Surface* Surface = TTF_RenderGlyph_Blended(Font, Ch, Color);
	if(Surface)
	{
		uint8 BytesPerPixel = Surface->format->BytesPerPixel;
		GLenum Format = (BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
		
		glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, Format, 
					 Surface->w, Surface->h, 0, Format, 
				 	 GL_UNSIGNED_BYTE, Surface->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		printf("Failed to load text\n");
	}
	SDL_FreeSurface(Surface);
	
	Result.Texture = ID;
	return Result;
}

char* LoadShaderSource(char* Path)
{
	FILE* ShaderSourceFile = fopen(Path, "rb");
	char* Buffer = 0;
	long Length; 
	
	if(!ShaderSourceFile)
	{
		printf("File does not exist.\n");
	}
	else
	{
		fseek(ShaderSourceFile, 0L, SEEK_END);
		Length = ftell(ShaderSourceFile);
		fseek(ShaderSourceFile, 0L, SEEK_SET);
		
		Buffer = (char*)calloc(Length, sizeof(char));
		if(Buffer)
		{
			fread(Buffer, sizeof(char), Length, ShaderSourceFile);
		}
		else
		{
			printf("Buffer is empty. Error.\n");
		}
	}
	fclose(ShaderSourceFile);
	
	return Buffer;
}

uint32 CreateRenderer(char* VertexShaderSource, char* FragmentShaderSource)
{
	uint32 VertexShader;
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &VertexShaderSource, 0);
	glCompileShader(VertexShader);
	
	bool32 ShaderCompileSuccess;
	char InfoLog[512];
	
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &ShaderCompileSuccess);
	if(!ShaderCompileSuccess)
	{
		glGetShaderInfoLog(VertexShader, 512, 0, InfoLog);
		printf("VERTEX SHADER COMPILATION FAILED: %s", InfoLog);
	}
	
	uint32 FragmentShader;
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FragmentShaderSource, 0);
	glCompileShader(FragmentShader);
	
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &ShaderCompileSuccess);
	if(!ShaderCompileSuccess)
	{
		glGetShaderInfoLog(FragmentShader, 512, 0, InfoLog);
		printf("FRAGMENT SHADER COMPILATION FAILED: %s", InfoLog);
	}
	
	uint32 Program = glCreateProgram();
	glAttachShader(Program, VertexShader);
	glAttachShader(Program, FragmentShader);
	glLinkProgram(Program);
	
	bool32 LinkingSuccess;
	glGetProgramiv(Program, GL_LINK_STATUS, &LinkingSuccess);
	if(!LinkingSuccess)
	{
		glGetProgramInfoLog(Program, 512, 0, InfoLog);
		printf("PROGRAM LINKING FAILED: %s", InfoLog);
	}
	
	free(VertexShaderSource);
	free(FragmentShaderSource);
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);  
	
	return Program;
}

void SetReal32(uint32 Shader, char* Location, real32 A)
{
	glUseProgram(Shader);
	glUniform1f(glGetUniformLocation(Shader, Location), A);
}

void SetVec2(uint32 Shader, char* Location, v2 A)
{
	glUseProgram(Shader);
	glUniform2f(glGetUniformLocation(Shader, Location), A.x, A.y);
}

void SetVec3(uint32 Shader, char* Location, v3 A)
{
	glUseProgram(Shader);
	glUniform3f(glGetUniformLocation(Shader, Location), A.x, A.y, A.z);
}

void SetVec4(uint32 Shader, char* Location, v4 A)
{
	glUseProgram(Shader);
	glUniform4f(glGetUniformLocation(Shader, Location), A.x, A.y, A.z, A.w);
}

void SetMat4(uint32 Shader, char* Location, mat4 A, GLenum IsTranspose)
{
	glUseProgram(Shader);
	glUniformMatrix4fv(glGetUniformLocation(Shader, Location), 1, IsTranspose, (real32*)A.E);
}

void SetInt(uint32 Shader, char* Location, int Entry)
{
	glUseProgram(Shader);
	glUniform1i(glGetUniformLocation(Shader, Location), Entry);
}