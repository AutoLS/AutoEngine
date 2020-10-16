#ifndef AUTO_IMAGE
#define AUTO_IMAGE

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#pragma pack(push, 1)
struct bitmap_header
{
	uint16 FileType;
	uint32 FileSize;
	uint16 Reserved1;
	uint16 Reserved2;
	uint32 BitmapOffset;
	uint32 Size;
	int Width;
	int Height;
	uint16 Planes;
	uint16 BitsPerPixel;
	uint32 Compression;
	uint32 SizeOfBitmap;
	int HorResolution;
	int VertResolution;
	uint32 ColorsUsed;
	uint32 ColorsImportant;	
};
#pragma pack(pop)

struct image
{
	int Width;
	int Height;
	uint32* Pixels;
};

void WriteImageBMP(char* FileName, image* Image)
{
	int OutputPixelSize = Image->Width*Image->Height*sizeof(int);
	
	bitmap_header Header = {};
	Header.FileType = 0x4D42;
	Header.FileSize = sizeof(Header) + OutputPixelSize;
	Header.BitmapOffset = sizeof(Header);
	Header.Size = sizeof(Header) - 14;
	Header.Width = Image->Width;
	Header.Height = Image->Height;
	Header.Planes = 1;
	Header.BitsPerPixel = 32;
	Header.Compression = 0;
	Header.SizeOfBitmap = OutputPixelSize;
	Header.HorResolution = 0;
	Header.VertResolution = 0;
	
	FILE* OutFile = fopen(FileName, "wb");
	if(OutFile)
	{
		fwrite(&Header, sizeof(Header), 1, OutFile);
		fwrite(Image->Pixels, OutputPixelSize, 1, OutFile);
		fclose(OutFile);
		printf("Writting image successfully...\n");
	}
	else
	{
		fprintf(stderr, "[ERROR] Unable to write output file %s.\n", FileName);
	}
}

int SavePNG(const char* FileName, int Width, int Height, char* Data)
{
	stbi_flip_vertically_on_write(1);
	
	if(!Data)
	{
		return 0;
	}
	
	int Saved = stbi_write_png(FileName, Width, Height, 4, Data, 0);
	
	return Saved;
}

#endif