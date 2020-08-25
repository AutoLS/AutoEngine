#include <SDL.h>
#include <SDL_image.h>
#include <SDL_TTF.h>
#include <SDL_Mixer.h>
#include <SDL_Net.h>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <dirent.h>
#include <shellapi.h>

#define ArraySize(arr) (sizeof(arr)/sizeof(arr[0]))

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

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

#include <data_structure/data_structure.cpp>
#include "math.h"
#include "platform.h"
#include "platform.cpp"

#include "AutoPolygon.h"
#include "AutoCollision.h"
#include "AutoPhysics.h"

#include "config_parser.h"

#if PROJECT_GL
#include "AutoGL.h"
#include "AutoGL.cpp"
#endif

