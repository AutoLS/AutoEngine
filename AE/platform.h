#ifndef PLATFORM_H
#define PLATFORM_H

v2 GlobalWinDim = {};
rect32 GlobalWinRect = {};
SDL_Window* GlobalWindow = 0;
const uint64 PerformanceFrequency = SDL_GetPerformanceFrequency();
const real32 SecPerCount = 1.0f / (real32)PerformanceFrequency;

using namespace std;

enum lib_type
{
	LIB_SDL = 0x1,
	LIB_SDL_FULL = 0x2,
	LIB_OPENGL = 0x4,
};

enum keys
{
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_AC_BACK,
	KEY_AC_BOOKMARKS,
	KEY_AC_FORWARD,
	KEY_AC_HOME,
	KEY_AC_REFRESH,
	KEY_AC_SEARCH,
	KEY_AC_STOP,
	KEY_AGAIN,
	KEY_ALTERASE,
	KEY_QUOTE,
	KEY_APPLICATION,
	KEY_AUDIOMUTE,
	KEY_AUDIONEXT,
	KEY_AUDIOPLAY,
	KEY_AUDIOPREV,
	KEY_AUDIOSTOP,
	KEY_B,
	KEY_BACKSLASH,
	KEY_BACKSPACE,
	KEY_BRIGHTNESSDOWN,
	KEY_BRIGHTNESSUP,
	KEY_C,
	KEY_CALCULATOR,
	KEY_CANCEL,
	KEY_CAPSLOCK,
	KEY_CLEAR,
	KEY_CLEARAGAIN,
	KEY_COMMA,
	KEY_COMPUTER,
	KEY_COPY,
	KEY_CRSEL,
	KEY_CURRENCYSUBUNIT,
	KEY_CURRENCYUNIT,
	KEY_CUT,
	KEY_D,
	KEY_DECIMALSEPARATOR,
	KEY_DELETE,
	KEY_DISPLAYSWITCH,
	KEY_DOWN,
	KEY_E,
	KEY_EJECT,
	KEY_END,
	KEY_EQUALS,
	KEY_ESC,
	KEY_EXE,
	KEY_EXSEL,
	KEY_F,
	KEY_F1,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_F13,
	KEY_F14,
	KEY_F15,
	KEY_F16,
	KEY_F17,
	KEY_F18,
	KEY_F19,
	KEY_F2,
	KEY_F20,
	KEY_F21,
	KEY_F22,
	KEY_F23,
	KEY_F24,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_FIND,
	KEY_G,
	KEY_BACKQUOTE,
	KEY_H,
	KEY_HELP,
	KEY_HOME,
	KEY_I,
	KEY_INSERT,
	KEY_J,
	KEY_K,
	KEY_KBDILLUMDOWN,
	KEY_KBDILLUMTOGGLE,
	KEY_KBDILLUMUP,
	KEY_KP_0,
	KEY_KP_00,
	KEY_KP_000,
	KEY_KP_1,
	KEY_KP_2,
	KEY_KP_3,
	KEY_KP_4,
	KEY_KP_5,
	KEY_KP_6,
	KEY_KP_7,
	KEY_KP_8,
	KEY_KP_9,
	KEY_KP_A,
	KEY_KP_AMPERSAND,
	KEY_KP_AT,
	KEY_KP_B,
	KEY_KP_BACKSPACE,
	KEY_KP_BINARY,
	KEY_KP_C,
	KEY_KP_CLEAR,
	KEY_KP_CLEARENTRY,
	KEY_KP_COLON,
	KEY_KP_COMMA,
	KEY_KP_D,
	KEY_KP_DBLAMPERSAND,
	KEY_KP_DBLVERTICALBAR,
	KEY_KP_DECIMAL,
	KEY_KP_DIVIDE,
	KEY_KP_E,
	KEY_KP_ENTER,
	KEY_KP_EQUALS,
	KEY_KP_EQUALSAS400,
	KEY_KP_EXCLAM,
	KEY_KP_F,
	KEY_KP_GREATER,
	KEY_KP_HASH,
	KEY_KP_HEXADECIMAL,
	KEY_KP_LEFTBRACE,
	KEY_KP_LEFTPAREN,
	KEY_KP_LESS,
	KEY_KP_MEMADD,
	KEY_KP_MEMCLEAR,
	KEY_KP_MEMDIVIDE,
	KEY_KP_MEMMULTIPLY,
	KEY_KP_MEMRECALL,
	KEY_KP_MEMSTORE,
	KEY_KP_MEMSUBTRACT,
	KEY_KP_MINUS,
	KEY_KP_MULTIPLY,
	KEY_KP_OCTAL,
	KEY_KP_PERCENT,
	KEY_KP_PERIOD,
	KEY_KP_PLUS,
	KEY_KP_PLUSMINUS,
	KEY_KP_POWER,
	KEY_KP_RIGHTBRACE,
	KEY_KP_RIGHTPAREN,
	KEY_KP_SPACE,
	KEY_KP_TAB,
	KEY_KP_VERTICALBAR,
	KEY_KP_XOR,
	KEY_L,
	KEY_LALT,
	KEY_LCTRL,
	KEY_LEFT,
	KEY_LEFTBRACKET,
	KEY_LGUI,
	KEY_LSHIFT,
	KEY_M,
	KEY_MAIL,
	KEY_MEDIASELECT,
	KEY_MENU,
	KEY_MINUS,
	KEY_MODE,
	KEY_MUTE,
	KEY_N,
	KEY_NUMLOCKCLEAR,
	KEY_O,
	KEY_OPER,
	KEY_OUT,
	KEY_P,
	KEY_PAGEDOWN,
	KEY_PAGEUP,
	KEY_PASTE,
	KEY_PAUSE,
	KEY_PERIOD,
	KEY_POWER,
	KEY_PRINTSCREEN,
	KEY_PRIOR,
	KEY_Q,
	KEY_R,
	KEY_RALT,
	KEY_RCTRL,
	KEY_ENTER,
	KEY_RETURN2,
	KEY_RGUI,
	KEY_RIGHT,
	KEY_RIGHTBRACKET,
	KEY_RSHIFT,
	KEY_S,
	KEY_SCROLLLOCK,
	KEY_SELECT,
	KEY_SEMICOLON,
	KEY_SEPARATOR,
	KEY_SLASH,
	KEY_SLEEP,
	KEY_SPACE,
	KEY_STOP,
	KEY_SYSREQ,
	KEY_T,
	KEY_TAB,
	KEY_THOUSANDSSEPARATOR,
	KEY_U,
	KEY_UNDO,
	KEY_UNKNOWN,
	KEY_UP,
	KEY_V,
	KEY_VOLUMEDOWN,
	KEY_VOLUMEUP,
	KEY_W,
	KEY_WWW,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_AMPERSAND,
	KEY_ASTERISK,
	KEY_AT,
	KEY_CARET,
	KEY_COLON,
	KEY_DOLLAR,
	KEY_EXCLAIM,
	KEY_GREATER,
	KEY_HASH,
	KEY_LEFTPAREN,
	KEY_LESS,
	KEY_PERCENT,
	KEY_PLUS,
	KEY_QUESTION,
	KEY_QUOTEDBL,
	KEY_RIGHTPAREN,
	KEY_UNDERSCORE,
	KEY_MAX,
};

enum mouse_button
{
	BUTTON_LEFT,
	BUTTON_MIDDLE,
	BUTTON_RIGHT,
	BUTTON_MAX,
};

enum render_flip
{
	FLIP_NONE,
	FLIP_HORIZONTAL,
	FLIP_VERTICAL,
};

struct display
{
	int RefreshRate;
	real32 GameUpdateHZ;
	real32 MsPerFrame;
	real32 TargetSecPerFrame;
	real32 FPS;
};

struct render
{
	SDL_Window* Window;
	SDL_Renderer* Renderer;
	v2i Dim;
	v2 WinDim;
	rect32 WinRect;
	display Display;
	bool MissedFrame;
	v2 Scale;
};

struct debug_file
{
    void* Data;
    uint64 Size;
};

struct image_data
{
	v2i Dim;
	SDL_Rect Rect;
	SDL_Texture* Texture;
	
	uint8 a;
	uint8 r;
	uint8 g;
	uint8 b;
	bool Transparency;
	bool AlphaSwitch;
};

struct glyph_character
{
	int MinX;
	int MaxX;
	int MinY;
	int MaxY;
	int Advance;
	
	int Width;
	int Height;
	
	uint32 Texture;
};

struct game_button_state
{
	bool IsDown;
	bool EndedDown;
	bool Up;
	int HalfTransitions;
};

struct game_controller_input
{
	game_button_state Keys[KEY_MAX];
};

#include "timer.h"

struct text_editing
{
	uint32 Cursor;
	uint32 Length;
	string Text;
};

struct game_text_input
{
	bool Active;
	timer Timer;
	timer TextTimer;
	timer TextTimerHold;
	timer BackSpaceTimer;
};

struct game_input
{
	game_controller_input Keyboard;
	game_controller_input Mouse;
	
	v2i MouseState;
	v2i Scroll;
	bool Scrolling;
	
	bool KeyDown;
	bool KeyWasDown;
	bool KeyEndedDown;
	const Uint8* KeyState;
	
	game_text_input TextInput;
};

#if 1
struct stack
{
	uint32 Size;
	uint32 Capacity;
	int* MyStack;
};
#endif

//Init libraries and graphics
bool InitLib(uint8 Flags);
render InitGraphics(char* Title, v2i Dim, Uint32 Flags);
display InitDisplay(SDL_Window* Window);

//Window
int Win32GetWindowRefreshRate(SDL_Window *Window);
void PrintFPS(real32 FPS);
bool HandleEvents(SDL_Event* Event, game_controller_input* KeyboardInput);

//Inputs
void Win32ProcessKeyPress(game_button_state* NewState, bool IsDown, bool WasDown = false);

//Render
void LoadTexture(image_data* Image, char* Path, SDL_Renderer* Renderer, uint8 Alpha = 255);
void LoadAndUpdateTexture(image_data* Image, char* Path, SDL_Rect* Rect = 0);

//Rects
SDL_Rect SetRectRounded(v2 Pos, v2 Dim);

//Utility

#endif