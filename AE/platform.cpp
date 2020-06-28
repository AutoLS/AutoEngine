#include "platform.h"

bool InitLib(uint8 Flags)
{
	bool Success = true;
	if(Flags == 0)
	{
		printf("Error: Cannot set flags to 0\n");
		return false;
	}
	if(Flags & LIB_SDL)
	{
		if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) 
		{
			printf("Unable to initialize SDL: %s\n", SDL_GetError());
			Success = false;
		}
	}
	if(Flags & LIB_SDL_FULL)
	{
		if(SDL_Init(SDL_INIT_EVERYTHING) != 0) 
		{
			printf("Unable to initialize SDL: %s\n", SDL_GetError());
			Success = false;
		}
		else
		{
			int Flags = IMG_INIT_JPG|IMG_INIT_PNG;
			int Initted = IMG_Init(Flags);
			if((Initted&Flags) != Flags) 
			{
				printf("IMG_Init: Failed to init required jpg and png support!\n");
				printf("IMG_Init: %s\n", IMG_GetError());
				Success = false;
			}
			int MusicFlags = MIX_INIT_OGG | MIX_INIT_MOD;
			int MixInitted = Mix_Init(MusicFlags);
			if(MixInitted && MusicFlags != MusicFlags) 
			{
				printf("Mix_Init: Failed to init required ogg and mod support!\n");
				printf("Mix_Init: %s\n", Mix_GetError());
				Success = false;
			}
			else
			{
				if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) 
				{
					printf("Mix_OpenAudio: %s\n", Mix_GetError());
					Success = false;
				}
			}
			if(TTF_Init() == -1) 
			{
				printf("TTF_Init: %s\n", TTF_GetError());
				Success = false;
			}
				
			if(SDLNet_Init() == -1) 
			{
				printf("SDLNet_Init: %s\n", SDLNet_GetError());
				Success = false;
			}
		}
	}
	if(Flags & LIB_OPENGL)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	}
	return Success;
}

render InitGraphics(char* Title, v2i Dim, Uint32 Flags)
{
	render Graphics = {};
	bool Success = true;
	Graphics.Dim = Dim;
	Graphics.WinDim = V2(Dim);
	Graphics.Window = SDL_CreateWindow(Title, 
										SDL_WINDOWPOS_UNDEFINED, 
										SDL_WINDOWPOS_UNDEFINED, 
										Dim.x, Dim.y,
										Flags);
	if(!Graphics.Window)
	{
		printf("Create window failed: %s\n", SDL_GetError());
		Success = false;
	}
	else
	{
#if PROJECT_GL	
			SDL_GLContext GLContext = SDL_GL_CreateContext(Graphics.Window);
			if(!GLContext)
			{
				printf("Failed to create context: %s", SDL_GetError());
				Success = false;
			}
			else
			{
				if (SDL_GL_MakeCurrent(Graphics.Window, GLContext) != 0)
				{
					printf("Failed to make current context: %s", SDL_GetError());
					Success = false;
				}
			}
#else
			Graphics.Renderer = SDL_CreateRenderer(Graphics.Window, -1, SDL_RENDERER_ACCELERATED);
			if(!Graphics.Renderer)
			{
				printf("Create Renderer failed: %s\n", SDL_GetError());
				Success = false;
			}
#endif
		Graphics.Display = InitDisplay(Graphics.Window);
	}
	return Graphics;
}

int Win32GetWindowRefreshRate(SDL_Window *Window)
{
    SDL_DisplayMode Mode;
    int DisplayIndex = SDL_GetWindowDisplayIndex(Window);
    // If we can't find the refresh rate, we'll return this:
    int DefaultRefreshRate = 60;
    if (SDL_GetDesktopDisplayMode(DisplayIndex, &Mode) != 0)
    {
        return DefaultRefreshRate;
    }
    if (Mode.refresh_rate == 0)
    {
        return DefaultRefreshRate;
    }
    return Mode.refresh_rate;
}

display InitDisplay(SDL_Window* Window)
{
	display Display = {};
	Display.RefreshRate = Win32GetWindowRefreshRate(Window);
	Display.GameUpdateHZ = (real32)Display.RefreshRate;
	Display.MsPerFrame = 1000.0f / Display.GameUpdateHZ;
	Display.TargetSecPerFrame = 1.0f / Display.GameUpdateHZ;
	
	return Display;
}
#if 1

stack CreateStack(int Capacity)
{
	stack Stack = {};
	Stack.Capacity = Capacity;
	Stack.MyStack = (int*)malloc(sizeof(int) * Capacity);
	
	return Stack;
}

void ClearStack(stack* Stack)
{
	Stack->Size = 0;
}

void Push(stack* Stack, int Value)
{
	if(Stack->Size + 1 > Stack->Capacity)
	{
		Stack->Capacity *= 5;
		int* NewStack = (int*)malloc(sizeof(int) * Stack->Capacity);
		memcpy(NewStack, Stack->MyStack, Stack->Size * sizeof(int));
		free(Stack->MyStack);
		Stack->MyStack = NewStack;
		Stack->MyStack[Stack->Size] = Value;
		++Stack->Size;
	}
	else
	{
		Stack->MyStack[Stack->Size] = Value;
		++Stack->Size;
	}
	//printf("Index: %d Push value: %d \n", Stack->Size - 1, Stack->MyStack[Stack->Size - 1]);
}

void Pop(stack* Stack)
{
	if(Stack->Size > 0)
	{
		//printf("Index: %d Pop value: %d\n", Stack->Size, Stack->MyStack[Stack->Size - 1]);
		--Stack->Size;
	}	
	else
		printf("Stack size is 0!\n");
}

int Peek(stack* Stack)
{
	if(Stack->Size == 0)
		return 0;
	//printf("Index: %d Value: %d Capacity: %d\n", Stack->Size - 1, 
			//Stack->MyStack[Stack->Size-1], Stack->Capacity);
	return Stack->MyStack[Stack->Size-1];
}
#endif

bool DoesStrContainsChar(char* str, char ch)
{
	for(int i = 0; i < strlen(str)+1; ++i)
	{
		if(str[i] == ch)
		{
			return true;
		}
	}
	return false;
}

char* GetStrFromChar(char* str, char ch)
{
	for(int i = 0; i < strlen(str)+1; ++i)
	{
		if(str[i] == ch)
		{
			return &str[i+1];
		}
	}
	
	return 0;
}

int GetStrLen(char* str, char* End)
{
	int Result = 0;
	if(strstr(str, End))
	{
		while(str[Result++] != End[0]);
	}
	return Result-1;
}

char* ConcatStr(char* s1, char* s2)
{
	char* Result = (char*)malloc(strlen(s1) + strlen(s2) + 1);
	strcpy(Result, s1);
	strcat(Result, s2);
	
	return Result;
}

char* ConcatStrf(char* s1, real32 n, int Digits = 0)
{
	char Buffer[50] = {};
	switch(Digits)
	{
		case 0: stbsp_sprintf(Buffer, "%.0f", n); break;
		case 1: stbsp_sprintf(Buffer, "%.1f", n); break;
		case 2: stbsp_sprintf(Buffer, "%.2f", n); break;
		case 3: stbsp_sprintf(Buffer, "%.3f", n); break;
		case 4: stbsp_sprintf(Buffer, "%.4f", n); break;
		case 5: stbsp_sprintf(Buffer, "%.5f", n); break;
	}
	
	char* Result = (char*)malloc(strlen(s1) + strlen(Buffer) + 1);
	strcpy(Result, s1);
	strcat(Result, Buffer);
	
	return Result;
}

char* ConcatStri(char* s1, int n)
{
	char Buffer[50] = {};
	itoa(n, Buffer, 10);
	
	char* Result = (char*)malloc(strlen(s1) + strlen(Buffer) + 1);
	strcpy(Result, s1);
	strcat(Result, Buffer);
	
	return Result;
}

char* ConcatStri64(char* s1, int64 n)
{
	char Buffer[50] = {};
	stbsp_sprintf(Buffer, "%ld", n);
	
	char* Result = (char*)malloc(strlen(s1) + strlen(Buffer) + 1);
	strcpy(Result, s1);
	strcat(Result, Buffer);
	
	return Result;
}

bool LockFPS(uint64 LastCount, real32 TargetSecPerFrame)
{
	bool Result = false;
	real32 FrameCompleteTime = Win32GetSecondElapsed(LastCount, SDL_GetPerformanceCounter());
	if(FrameCompleteTime < TargetSecPerFrame)
	{
		int SleepTime = (int)((TargetSecPerFrame - Win32GetSecondElapsed(LastCount, SDL_GetPerformanceCounter())) * 1000) - 1;
		if(SleepTime > 0)
		{
			Sleep(SleepTime);
		}
		//assert(Win32GetSecondElapsed(LastCount, SDL_GetPerformanceCounter()) < TargetSecPerFrame);
		while(Win32GetSecondElapsed(LastCount, SDL_GetPerformanceCounter()) < TargetSecPerFrame);
	}
	else
	{
		Result = true;
		//printf("Missed a frame!\n");
	}
	return Result;
}

void PrintFPS(real32 FPS)
{
	printf("FPS: %.2f\n", FPS);
}

void PlayChannelSafe(Mix_Chunk* WAV, int Loop = 0, int Channel = -1)
{
	if(WAV)
	{
		Mix_PlayChannel(Channel, WAV, Loop);
	}
	else
	{
		printf("Missing audio! Please make sure the audio is load properly!\n");
	}
}

bool HandleEvents(SDL_Event* Event, game_input* Input, render* Graphics)
{
	bool Quit = false;
	switch(Event->type)
	{
		case SDL_QUIT:
		{
			Quit = true;
		} break;
		case SDL_WINDOWEVENT:
		{
			switch(Event->window.event)
			{
				case SDL_WINDOWEVENT_RESIZED:
				{
					SDL_GetWindowSize(Graphics->Window, &Graphics->Dim.x, &Graphics->Dim.y);
					Graphics->WinDim = V2(Graphics->Dim);
#if PROJECT_GL
					glViewport(0, 0, Graphics->Dim.x, Graphics->Dim.y);
#endif
				} break;
			}
		} break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			bool IsDown = (Event->key.state == SDL_PRESSED);
			bool WasDown = false;
			if(Event->key.state == SDL_RELEASED)
			{
				WasDown = true;
			}
			else if(Event->key.repeat != 0)
			{
				WasDown = true;
			}
			
			//bool Down = Event->type == SDL_KEYDOWN;
			Input->KeyDown = IsDown;
			Input->KeyWasDown = WasDown;
			Input->KeyEndedDown = (Event->key.repeat == 0) && IsDown;
			
			if(Event->key.repeat == 0)
			{
				switch(Event->key.keysym.sym)
				{
					case SDLK_0:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_0], IsDown, WasDown); break;   
					case SDLK_1:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_1], IsDown, WasDown); break; 
					case SDLK_2:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_2], IsDown, WasDown); break; 
					case SDLK_3:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_3], IsDown, WasDown); break; 
					case SDLK_4:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_4], IsDown, WasDown); break; 
					case SDLK_5:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_5], IsDown, WasDown); break; 
					case SDLK_6:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_6], IsDown, WasDown); break; 
					case SDLK_7:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_7], IsDown, WasDown); break; 
					case SDLK_8:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_8], IsDown, WasDown); break; 
					case SDLK_9:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_9], IsDown, WasDown); break; 
					case SDLK_a:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_A], IsDown, WasDown); break; 
					case SDLK_AC_BACK:                      Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_AC_BACK], IsDown, WasDown); break; 
					case SDLK_AC_BOOKMARKS:                 Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_AC_BOOKMARKS], IsDown, WasDown); break; 
					case SDLK_AC_FORWARD:                   Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_AC_FORWARD], IsDown, WasDown); break; 
					case SDLK_AC_HOME:                      Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_AC_HOME], IsDown, WasDown); break; 
					case SDLK_AC_REFRESH:                   Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_AC_REFRESH], IsDown, WasDown); break; 
					case SDLK_AC_SEARCH:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_AC_SEARCH], IsDown, WasDown); break; 
					case SDLK_AC_STOP:                      Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_AC_STOP], IsDown, WasDown); break; 
					case SDLK_AGAIN:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_AGAIN], IsDown, WasDown); break; 
					case SDLK_ALTERASE:                     Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_ALTERASE], IsDown, WasDown); break; 
					case SDLK_QUOTE:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_QUOTE], IsDown, WasDown); break; 
					case SDLK_APPLICATION:                  Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_APPLICATION], IsDown, WasDown); break; 
					case SDLK_AUDIOMUTE:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_AUDIOMUTE], IsDown, WasDown); break; 
					case SDLK_AUDIONEXT:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_AUDIONEXT], IsDown, WasDown); break; 
					case SDLK_AUDIOPLAY:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_AUDIOPLAY], IsDown, WasDown); break; 
					case SDLK_AUDIOPREV:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_AUDIOPREV], IsDown, WasDown); break; 
					case SDLK_AUDIOSTOP:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_AUDIOSTOP], IsDown, WasDown); break; 
					case SDLK_b:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_B], IsDown, WasDown); break; 
					case SDLK_BACKSLASH:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_BACKSLASH], IsDown, WasDown); break; 
					case SDLK_BACKSPACE:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_BACKSPACE], IsDown, WasDown); break; 
					case SDLK_BRIGHTNESSDOWN:               Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_BRIGHTNESSDOWN], IsDown, WasDown); break; 
					case SDLK_BRIGHTNESSUP:                 Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_BRIGHTNESSUP], IsDown, WasDown); break; 
					case SDLK_c:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_C], IsDown, WasDown); break; 
					case SDLK_CALCULATOR:                   Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_CALCULATOR], IsDown, WasDown); break; 
					case SDLK_CANCEL:                       Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_CANCEL], IsDown, WasDown); break; 
					case SDLK_CAPSLOCK:                     Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_CAPSLOCK], IsDown, WasDown); break; 
					case SDLK_CLEAR:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_CLEAR], IsDown, WasDown); break; 
					case SDLK_CLEARAGAIN:                   Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_CLEARAGAIN], IsDown, WasDown); break; 
					case SDLK_COMMA:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_COMMA], IsDown, WasDown); break; 
					case SDLK_COMPUTER:                     Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_COMPUTER], IsDown, WasDown); break; 
					case SDLK_COPY:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_COPY], IsDown, WasDown); break; 
					case SDLK_CRSEL:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_CRSEL], IsDown, WasDown); break; 
					case SDLK_CURRENCYSUBUNIT:              Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_CURRENCYSUBUNIT], IsDown, WasDown); break; 
					case SDLK_CURRENCYUNIT:                 Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_CURRENCYUNIT], IsDown, WasDown); break; 
					case SDLK_CUT:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_CUT], IsDown, WasDown); break; 
					case SDLK_d:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_D], IsDown, WasDown); break; 
					case SDLK_DECIMALSEPARATOR:             Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_DECIMALSEPARATOR], IsDown, WasDown); break; 
					case SDLK_DELETE:                       Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_DELETE], IsDown, WasDown); break; 
					case SDLK_DISPLAYSWITCH:                Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_DISPLAYSWITCH], IsDown, WasDown); break; 
					case SDLK_DOWN:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_DOWN], IsDown, WasDown); break; 
					case SDLK_e:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_E], IsDown, WasDown); break; 
					case SDLK_EJECT:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_EJECT], IsDown, WasDown); break; 
					case SDLK_END:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_END], IsDown, WasDown); break; 
					case SDLK_EQUALS:                       Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_EQUALS], IsDown, WasDown); break; 
					case SDLK_ESCAPE:                       Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_ESC], IsDown, WasDown); break; 
					case SDLK_EXECUTE:                      Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_EXE], IsDown, WasDown); break; 
					case SDLK_EXSEL:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_EXSEL], IsDown, WasDown); break; 
					case SDLK_f:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F], IsDown, WasDown); break; 
					case SDLK_F1:                           Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F1], IsDown, WasDown); break; 
					case SDLK_F10:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F10], IsDown, WasDown); break; 
					case SDLK_F11:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F11], IsDown, WasDown); break; 
					case SDLK_F12:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F12], IsDown, WasDown); break; 
					case SDLK_F13:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F13], IsDown, WasDown); break; 
					case SDLK_F14:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F14], IsDown, WasDown); break; 
					case SDLK_F15:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F15], IsDown, WasDown); break; 
					case SDLK_F16:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F16], IsDown, WasDown); break; 
					case SDLK_F17:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F17], IsDown, WasDown); break; 
					case SDLK_F18:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F18], IsDown, WasDown); break; 
					case SDLK_F19:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F19], IsDown, WasDown); break; 
					case SDLK_F2:                           Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F2], IsDown, WasDown); break; 
					case SDLK_F20:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F20], IsDown, WasDown); break; 
					case SDLK_F21:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F21], IsDown, WasDown); break; 
					case SDLK_F22:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F22], IsDown, WasDown); break; 
					case SDLK_F23:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F23], IsDown, WasDown); break; 
					case SDLK_F24:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F24], IsDown, WasDown); break; 
					case SDLK_F3:                           Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F3], IsDown, WasDown); break; 
					case SDLK_F4:                           Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F4], IsDown, WasDown); break; 
					case SDLK_F5:                           Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F5], IsDown, WasDown); break; 
					case SDLK_F6:                           Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F6], IsDown, WasDown); break; 
					case SDLK_F7:                           Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F7], IsDown, WasDown); break; 
					case SDLK_F8:                           Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F8], IsDown, WasDown); break; 
					case SDLK_F9:                           Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_F9], IsDown, WasDown); break; 
					case SDLK_FIND:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_FIND], IsDown, WasDown); break; 
					case SDLK_g:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_G], IsDown, WasDown); break; 
					case SDLK_BACKQUOTE:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_BACKQUOTE], IsDown, WasDown); break; 
					case SDLK_h:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_H], IsDown, WasDown); break; 
					case SDLK_HELP:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_HELP], IsDown, WasDown); break; 
					case SDLK_HOME:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_HOME], IsDown, WasDown); break; 
					case SDLK_i:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_I], IsDown, WasDown); break; 
					case SDLK_INSERT:                       Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_INSERT], IsDown, WasDown); break; 
					case SDLK_j:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_J], IsDown, WasDown); break; 
					case SDLK_k:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_K], IsDown, WasDown); break; 
					case SDLK_KBDILLUMDOWN:                 Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KBDILLUMDOWN], IsDown, WasDown); break; 
					case SDLK_KBDILLUMTOGGLE:               Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KBDILLUMTOGGLE], IsDown, WasDown); break; 
					case SDLK_KBDILLUMUP:                   Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KBDILLUMUP], IsDown, WasDown); break; 
					case SDLK_KP_0:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_0], IsDown, WasDown); break; 
					case SDLK_KP_00:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_00], IsDown, WasDown); break; 
					case SDLK_KP_000:                       Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_000], IsDown, WasDown); break; 
					case SDLK_KP_1:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_1], IsDown, WasDown); break; 
					case SDLK_KP_2:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_2], IsDown, WasDown); break; 
					case SDLK_KP_3:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_3], IsDown, WasDown); break; 
					case SDLK_KP_4:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_4], IsDown, WasDown); break; 
					case SDLK_KP_5:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_5], IsDown, WasDown); break; 
					case SDLK_KP_6:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_6], IsDown, WasDown); break; 
					case SDLK_KP_7:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_7], IsDown, WasDown); break; 
					case SDLK_KP_8:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_8], IsDown, WasDown); break; 
					case SDLK_KP_9:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_9], IsDown, WasDown); break; 
					case SDLK_KP_A:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_A], IsDown, WasDown); break; 
					case SDLK_KP_AMPERSAND:                 Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_AMPERSAND], IsDown, WasDown); break; 
					case SDLK_KP_AT:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_AT], IsDown, WasDown); break; 
					case SDLK_KP_B:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_B], IsDown, WasDown); break; 
					case SDLK_KP_BACKSPACE:                 Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_BACKSPACE], IsDown, WasDown); break; 
					case SDLK_KP_BINARY:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_BINARY], IsDown, WasDown); break; 
					case SDLK_KP_C:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_C], IsDown, WasDown); break; 
					case SDLK_KP_CLEAR:                     Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_CLEAR], IsDown, WasDown); break; 
					case SDLK_KP_CLEARENTRY:                Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_CLEARENTRY], IsDown, WasDown); break; 
					case SDLK_KP_COLON:                     Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_COLON], IsDown, WasDown); break; 
					case SDLK_KP_COMMA:                     Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_COMMA], IsDown, WasDown); break; 
					case SDLK_KP_D:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_D], IsDown, WasDown); break; 
					case SDLK_KP_DBLAMPERSAND:              Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_DBLAMPERSAND], IsDown, WasDown); break; 
					case SDLK_KP_DBLVERTICALBAR:            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_DBLVERTICALBAR], IsDown, WasDown); break; 
					case SDLK_KP_DECIMAL:                   Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_DECIMAL], IsDown, WasDown); break; 
					case SDLK_KP_DIVIDE:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_DIVIDE], IsDown, WasDown); break; 
					case SDLK_KP_E:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_E], IsDown, WasDown); break; 
					case SDLK_KP_ENTER:                     Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_ENTER], IsDown, WasDown); break; 
					case SDLK_KP_EQUALS:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_EQUALS], IsDown, WasDown); break; 
					case SDLK_KP_EQUALSAS400:               Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_EQUALSAS400], IsDown, WasDown); break; 
					case SDLK_KP_EXCLAM:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_EXCLAM], IsDown, WasDown); break; 
					case SDLK_KP_F:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_F], IsDown, WasDown); break; 
					case SDLK_KP_GREATER:                   Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_GREATER], IsDown, WasDown); break; 
					case SDLK_KP_HASH:                      Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_HASH], IsDown, WasDown); break; 
					case SDLK_KP_HEXADECIMAL:               Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_HEXADECIMAL], IsDown, WasDown); break; 
					case SDLK_KP_LEFTBRACE:                 Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_LEFTBRACE], IsDown, WasDown); break; 
					case SDLK_KP_LEFTPAREN:                 Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_LEFTPAREN], IsDown, WasDown); break; 
					case SDLK_KP_LESS:                      Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_LESS], IsDown, WasDown); break; 
					case SDLK_KP_MEMADD:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_MEMADD], IsDown, WasDown); break; 
					case SDLK_KP_MEMCLEAR:                  Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_MEMCLEAR], IsDown, WasDown); break; 
					case SDLK_KP_MEMDIVIDE:                 Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_MEMDIVIDE], IsDown, WasDown); break; 
					case SDLK_KP_MEMMULTIPLY:               Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_MEMMULTIPLY], IsDown, WasDown); break; 
					case SDLK_KP_MEMRECALL:                 Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_MEMRECALL], IsDown, WasDown); break; 
					case SDLK_KP_MEMSTORE:                  Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_MEMSTORE], IsDown, WasDown); break; 
					case SDLK_KP_MEMSUBTRACT:               Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_MEMSUBTRACT], IsDown, WasDown); break; 
					case SDLK_KP_MINUS:                     Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_MINUS], IsDown, WasDown); break; 
					case SDLK_KP_MULTIPLY:                  Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_MULTIPLY], IsDown, WasDown); break; 
					case SDLK_KP_OCTAL:                     Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_OCTAL], IsDown, WasDown); break; 
					case SDLK_KP_PERCENT:                   Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_PERCENT], IsDown, WasDown); break; 
					case SDLK_KP_PERIOD:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_PERIOD], IsDown, WasDown); break; 
					case SDLK_KP_PLUS:                      Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_PLUS], IsDown, WasDown); break; 
					case SDLK_KP_PLUSMINUS:                 Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_PLUSMINUS], IsDown, WasDown); break; 
					case SDLK_KP_POWER:                     Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_POWER], IsDown, WasDown); break; 
					case SDLK_KP_RIGHTBRACE:                Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_RIGHTBRACE], IsDown, WasDown); break; 
					case SDLK_KP_RIGHTPAREN:                Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_RIGHTPAREN], IsDown, WasDown); break; 
					case SDLK_KP_SPACE:                     Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_SPACE], IsDown, WasDown); break; 
					case SDLK_KP_TAB:                       Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_TAB], IsDown, WasDown); break; 
					case SDLK_KP_VERTICALBAR:               Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_VERTICALBAR], IsDown, WasDown); break; 
					case SDLK_KP_XOR:                       Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_KP_XOR], IsDown, WasDown); break; 
					case SDLK_l:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_L], IsDown, WasDown); break; 
					case SDLK_LALT:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_LALT], IsDown, WasDown); break; 
					case SDLK_LCTRL:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_LCTRL], IsDown, WasDown); break; 
					case SDLK_LEFT:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_LEFT], IsDown, WasDown); break; 
					case SDLK_LEFTBRACKET:                  Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_LEFTBRACKET], IsDown, WasDown); break; 
					case SDLK_LGUI:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_LGUI], IsDown, WasDown); break; 
					case SDLK_LSHIFT:                       Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_LSHIFT], IsDown, WasDown); break; 
					case SDLK_m:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_M], IsDown, WasDown); break; 
					case SDLK_MAIL:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_MAIL], IsDown, WasDown); break; 
					case SDLK_MEDIASELECT:                  Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_MEDIASELECT], IsDown, WasDown); break; 
					case SDLK_MENU:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_MENU], IsDown, WasDown); break; 
					case SDLK_MINUS:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_MINUS], IsDown, WasDown); break; 
					case SDLK_MODE:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_MODE], IsDown, WasDown); break; 
					case SDLK_MUTE:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_MUTE], IsDown, WasDown); break; 
					case SDLK_n:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_N], IsDown, WasDown); break; 
					case SDLK_NUMLOCKCLEAR:                 Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_NUMLOCKCLEAR], IsDown, WasDown); break; 
					case SDLK_o:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_O], IsDown, WasDown); break; 
					case SDLK_OPER:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_OPER], IsDown, WasDown); break; 
					case SDLK_OUT:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_OUT], IsDown, WasDown); break; 
					case SDLK_p:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_P], IsDown, WasDown); break; 
					case SDLK_PAGEDOWN:                     Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_PAGEDOWN], IsDown, WasDown); break; 
					case SDLK_PAGEUP:                       Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_PAGEUP], IsDown, WasDown); break; 
					case SDLK_PASTE:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_PASTE], IsDown, WasDown); break; 
					case SDLK_PAUSE:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_PAUSE], IsDown, WasDown); break; 
					case SDLK_PERIOD:                       Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_PERIOD], IsDown, WasDown); break; 
					case SDLK_POWER:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_POWER], IsDown, WasDown); break; 
					case SDLK_PRINTSCREEN:                  Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_PRINTSCREEN], IsDown, WasDown); break; 
					case SDLK_PRIOR:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_PRIOR], IsDown, WasDown); break; 
					case SDLK_q:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_Q], IsDown, WasDown); break; 
					case SDLK_r:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_R], IsDown, WasDown); break; 
					case SDLK_RALT:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_RALT], IsDown, WasDown); break; 
					case SDLK_RCTRL:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_RCTRL], IsDown, WasDown); break; 
					case SDLK_RETURN:                       Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_ENTER], IsDown, WasDown); break; 
					case SDLK_RETURN2:                      Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_RETURN2], IsDown, WasDown); break; 
					case SDLK_RGUI:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_RGUI], IsDown, WasDown); break; 
					case SDLK_RIGHT:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_RIGHT], IsDown, WasDown); break; 
					case SDLK_RIGHTBRACKET:                 Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_RIGHTBRACKET], IsDown, WasDown); break; 
					case SDLK_RSHIFT:                       Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_RSHIFT], IsDown, WasDown); break; 
					case SDLK_s:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_S], IsDown, WasDown); break; 
					case SDLK_SCROLLLOCK:                   Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_SCROLLLOCK], IsDown, WasDown); break; 
					case SDLK_SELECT:                       Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_SELECT], IsDown, WasDown); break; 
					case SDLK_SEMICOLON:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_SEMICOLON], IsDown, WasDown); break; 
					case SDLK_SEPARATOR:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_SEPARATOR], IsDown, WasDown); break; 
					case SDLK_SLASH:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_SLASH], IsDown, WasDown); break; 
					case SDLK_SLEEP:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_SLEEP], IsDown, WasDown); break; 
					case SDLK_SPACE:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_SPACE], IsDown, WasDown); break; 
					case SDLK_STOP:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_STOP], IsDown, WasDown); break; 
					case SDLK_SYSREQ:                       Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_SYSREQ], IsDown, WasDown); break; 
					case SDLK_t:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_T], IsDown, WasDown); break; 
					case SDLK_TAB:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_TAB], IsDown, WasDown); break; 
					case SDLK_THOUSANDSSEPARATOR:           Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_THOUSANDSSEPARATOR], IsDown, WasDown); break; 
					case SDLK_u:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_U], IsDown, WasDown); break; 
					case SDLK_UNDO:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_UNDO], IsDown, WasDown); break; 
					case SDLK_UNKNOWN:                      Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_UNKNOWN], IsDown, WasDown); break; 
					case SDLK_UP:                           Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_UP], IsDown, WasDown); break; 
					case SDLK_v:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_V], IsDown, WasDown); break; 
					case SDLK_VOLUMEDOWN:                   Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_VOLUMEDOWN], IsDown, WasDown); break; 
					case SDLK_VOLUMEUP:                     Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_VOLUMEUP], IsDown, WasDown); break; 
					case SDLK_w:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_W], IsDown, WasDown); break; 
					case SDLK_WWW:                          Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_WWW], IsDown, WasDown); break; 
					case SDLK_x:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_X], IsDown, WasDown); break; 
					case SDLK_y:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_Y], IsDown, WasDown); break; 
					case SDLK_z:                            Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_Z], IsDown, WasDown); break; 
					case SDLK_AMPERSAND:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_AMPERSAND], IsDown, WasDown); break; 
					case SDLK_ASTERISK:                     Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_ASTERISK], IsDown, WasDown); break; 
					case SDLK_AT:                           Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_AT], IsDown, WasDown); break; 
					case SDLK_CARET:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_CARET], IsDown, WasDown); break; 
					case SDLK_COLON:                        Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_COLON], IsDown, WasDown); break; 
					case SDLK_DOLLAR:                       Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_DOLLAR], IsDown, WasDown); break; 
					case SDLK_EXCLAIM:                      Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_EXCLAIM], IsDown, WasDown); break; 
					case SDLK_GREATER:                      Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_GREATER], IsDown, WasDown); break; 
					case SDLK_HASH:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_HASH], IsDown, WasDown); break; 
					case SDLK_LEFTPAREN:                    Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_LEFTPAREN], IsDown, WasDown); break; 
					case SDLK_LESS:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_LESS], IsDown, WasDown); break; 
					case SDLK_PERCENT:                      Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_PERCENT], IsDown, WasDown); break; 
					case SDLK_PLUS:                         Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_PLUS], IsDown, WasDown); break; 
					case SDLK_QUESTION:                     Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_QUESTION], IsDown, WasDown); break; 
					case SDLK_QUOTEDBL:                     Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_QUOTEDBL], IsDown, WasDown); break; 
					case SDLK_RIGHTPAREN:                   Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_RIGHTPAREN], IsDown, WasDown); break; 
					case SDLK_UNDERSCORE:                   Win32ProcessKeyPress(&Input->Keyboard.Keys[KEY_UNDERSCORE], IsDown, WasDown); break; 
				}
			}
		} break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		{
#if 1
			bool IsDown = (Event->button.state == SDL_PRESSED);
			bool WasDown = false;
			if(Event->button.state == SDL_RELEASED)
			{
				WasDown = true;
			}
			else if(Event->key.repeat != 0)
			{
				WasDown = true;
			}
			
			switch(Event->button.button)
			{
				case SDL_BUTTON_LEFT:
				{
					Win32ProcessKeyPress(&Input->Mouse.Keys[BUTTON_LEFT], IsDown, WasDown);
				} break;
				case SDL_BUTTON_RIGHT:
				{
					Win32ProcessKeyPress(&Input->Mouse.Keys[BUTTON_RIGHT], IsDown);
				} break;
			}
#endif
		} break;
		case SDL_MOUSEMOTION:
		{
			SDL_GetMouseState(&Input->MouseState.x, &Input->MouseState.y);
		} break;
		case SDL_MOUSEWHEEL:
		{
			Input->Scroll.y = Event->wheel.y;
		} break;
	}
	
	Input->Scrolling = Event->type == SDL_MOUSEWHEEL;
	Input->TextInput.Active = Event->type == SDL_TEXTINPUT;
	
	return Quit;
}

game_text_input InitTextInput(real32 TextInputTime, 
							  real32 TextTime, 
							  real32 TextTimeHold)
{
	game_text_input TextInput = {};
	TextInput.Timer = CreateTimer(TextInputTime);
	TextInput.TextTimer = CreateTimer(TextTime);
	TextInput.TextTimerHold = CreateTimer(TextTimeHold);
	TextInput.BackSpaceTimer = CreateTimer(0.05f);
	
	return TextInput;
}

text_editing CreateTextEditing()
{
	text_editing TextEditing = {};
	
	return TextEditing;
}

void ProcessTextInput(game_input* Input, text_editing* TextEditing, 
					  SDL_Event* Event, real32 t, int Limit)
{
	ProcessTimer(&Input->TextInput.Timer, t);
	ProcessTimer(&Input->TextInput.TextTimer, t);
	ProcessTimer(&Input->TextInput.TextTimerHold, t);
	ProcessTimer(&Input->TextInput.BackSpaceTimer, t);
	
	if(Input->KeyEndedDown)
	{
		ResetTimer(&Input->TextInput.Timer);
		StartTimer(&Input->TextInput.Timer);
		if(Input->Keyboard.Keys[KEY_BACKSPACE].EndedDown &&
		   Input->TextInput.TextTimer.Complete)
		{
			StartTimer(&Input->TextInput.TextTimer);
			if(TextEditing->Text.length() > 0)
			{
				TextEditing->Text.pop_back();
			}
			if(TextEditing->Text.length() > 0)
			printf("%s\n", TextEditing->Text.c_str());
		}
		else
		{
			if(Input->TextInput.Active && Input->TextInput.TextTimer.Complete
				&& TextEditing->Text.length() < Limit)
			{
				StartTimer(&Input->TextInput.TextTimer);
				TextEditing->Text += Event->text.text;
			}
			if(TextEditing->Text.length() > 0)
			printf("%s\n", TextEditing->Text.c_str());
		}
	}
	if(Input->KeyDown && Input->TextInput.Timer.Complete)
	{
		if(Input->Keyboard.Keys[KEY_BACKSPACE].IsDown &&
		   Input->TextInput.BackSpaceTimer.Complete)
		{
			StartTimer(&Input->TextInput.BackSpaceTimer);
			if(TextEditing->Text.length() > 0)
			{
				TextEditing->Text.pop_back();
			}
			if(TextEditing->Text.length() > 0)
			printf("%s\n", TextEditing->Text.c_str());
		}
		else
		{
			if(Input->TextInput.Active && 
			   Input->TextInput.TextTimerHold.Complete &&
			   TextEditing->Text.length() < Limit)
			{
				StartTimer(&Input->TextInput.TextTimerHold);
				TextEditing->Text += Event->text.text;
			}
		}
		if(TextEditing->Text.length() > 0)
			printf("%s\n", TextEditing->Text.c_str());
	}
}

void ResetKeyState(game_controller_input* KeyInput, int nKeys)
{
	for(int i = 0; i < nKeys; ++i)
	{
		KeyInput->Keys[i].EndedDown = 0;
	}
}

void Win32ProcessKeyPress(game_button_state* NewState, bool IsDown, bool WasDown)
{
	//assert(NewState->EndedDown != IsDown);
	NewState->IsDown = IsDown;
	NewState->EndedDown = IsDown;
	NewState->Up = WasDown;
	++NewState->HalfTransitions;
}

char* GetKeyText(int KeyIndex)
{
	char* KeyText = "";
	switch(KeyIndex)
	{
		case KEY_0:                             KeyText = "0"; break;      
		case KEY_1:                             KeyText = "1"; break; 
		case KEY_2:                             KeyText = "2"; break; 
		case KEY_3:                             KeyText = "3"; break; 
		case KEY_4:                             KeyText = "4"; break; 
		case KEY_5:                             KeyText = "5"; break; 
		case KEY_6:                             KeyText = "6"; break; 
		case KEY_7:                             KeyText = "7"; break; 
		case KEY_8:                             KeyText = "8"; break; 
		case KEY_9:                             KeyText = "9"; break; 
		case KEY_A:                             KeyText = "A"; break; 
		case KEY_AC_BACK:                       KeyText = "AC_BACK"; break; 
		case KEY_AC_BOOKMARKS:                  KeyText = "AC_BOOKMARKS"; break; 
		case KEY_AC_FORWARD:                    KeyText = "AC_FORWARD"; break; 
		case KEY_AC_HOME:                       KeyText = "AC_HOME"; break; 
		case KEY_AC_REFRESH:                    KeyText = "AC_REFRESH"; break; 
		case KEY_AC_SEARCH:                     KeyText = "AC_SEARCH"; break; 
		case KEY_AC_STOP:                       KeyText = "AC_STOP"; break; 
		case KEY_AGAIN:                         KeyText = "AGAIN"; break; 
		case KEY_ALTERASE:                      KeyText = "ALTERASE"; break; 
		case KEY_QUOTE:                         KeyText = "QUOTE"; break; 
		case KEY_APPLICATION:                   KeyText = "APPLICATION"; break; 
		case KEY_AUDIOMUTE:                     KeyText = "AUDIOMUTE"; break; 
		case KEY_AUDIONEXT:                     KeyText = "AUDIONEXT"; break; 
		case KEY_AUDIOPLAY:                     KeyText = "AUDIOPLAY"; break; 
		case KEY_AUDIOPREV:                     KeyText = "AUDIOPREV"; break; 
		case KEY_AUDIOSTOP:                     KeyText = "AUDIOSTOP"; break; 
		case KEY_B:                             KeyText = "B"; break; 
		case KEY_BACKSLASH:                     KeyText = "BACKSLASH"; break; 
		case KEY_BACKSPACE:                     KeyText = "BACKSPACE"; break; 
		case KEY_BRIGHTNESSDOWN:                KeyText = "BRIGHTNESSDOWN"; break; 
		case KEY_BRIGHTNESSUP:                  KeyText = "BRIGHTNESSUP"; break; 
		case KEY_C:                             KeyText = "C"; break; 
		case KEY_CALCULATOR:                    KeyText = "CALCULATOR"; break; 
		case KEY_CANCEL:                        KeyText = "CANCEL"; break; 
		case KEY_CAPSLOCK:                      KeyText = "CAPSLOCK"; break; 
		case KEY_CLEAR:                         KeyText = "CLEAR"; break; 
		case KEY_CLEARAGAIN:                    KeyText = "CLEARAGAIN"; break; 
		case KEY_COMMA:                         KeyText = "COMMA"; break; 
		case KEY_COMPUTER:                      KeyText = "COMPUTER"; break; 
		case KEY_COPY:                          KeyText = "COPY"; break; 
		case KEY_CRSEL:                         KeyText = "CRSEL"; break; 
		case KEY_CURRENCYSUBUNIT:               KeyText = "CURRENCYSUBUNIT"; break; 
		case KEY_CURRENCYUNIT:                  KeyText = "CURRENCYUNIT"; break; 
		case KEY_CUT:                           KeyText = "CUT"; break; 
		case KEY_D:                             KeyText = "D"; break; 
		case KEY_DECIMALSEPARATOR:              KeyText = "DECIMALSEPARATOR"; break; 
		case KEY_DELETE:                        KeyText = "DELETE"; break; 
		case KEY_DISPLAYSWITCH:                 KeyText = "DISPLAYSWITCH"; break; 
		case KEY_DOWN:                          KeyText = "DOWN"; break; 
		case KEY_E:                             KeyText = "E"; break; 
		case KEY_EJECT:                         KeyText = "EJECT"; break; 
		case KEY_END:                           KeyText = "END"; break; 
		case KEY_EQUALS:                        KeyText = "EQUALS"; break; 
		case KEY_ESC:                           KeyText = "ESC"; break; 
		case KEY_EXE:                           KeyText = "EXE"; break; 
		case KEY_EXSEL:                         KeyText = "EXSEL"; break; 
		case KEY_F:                             KeyText = "F"; break; 
		case KEY_F1:                            KeyText = "F1"; break; 
		case KEY_F10:                           KeyText = "F10"; break; 
		case KEY_F11:                           KeyText = "F11"; break; 
		case KEY_F12:                           KeyText = "F12"; break; 
		case KEY_F13:                           KeyText = "F13"; break; 
		case KEY_F14:                           KeyText = "F14"; break; 
		case KEY_F15:                           KeyText = "F15"; break; 
		case KEY_F16:                           KeyText = "F16"; break; 
		case KEY_F17:                           KeyText = "F17"; break; 
		case KEY_F18:                           KeyText = "F18"; break; 
		case KEY_F19:                           KeyText = "F19"; break; 
		case KEY_F2:                            KeyText = "F2"; break; 
		case KEY_F20:                           KeyText = "F20"; break; 
		case KEY_F21:                           KeyText = "F21"; break; 
		case KEY_F22:                           KeyText = "F22"; break; 
		case KEY_F23:                           KeyText = "F23"; break; 
		case KEY_F24:                           KeyText = "F24"; break; 
		case KEY_F3:                            KeyText = "F3"; break; 
		case KEY_F4:                            KeyText = "F4"; break; 
		case KEY_F5:                            KeyText = "F5"; break; 
		case KEY_F6:                            KeyText = "F6"; break; 
		case KEY_F7:                            KeyText = "F7"; break; 
		case KEY_F8:                            KeyText = "F8"; break; 
		case KEY_F9:                            KeyText = "F9"; break; 
		case KEY_FIND:                          KeyText = "FIND"; break; 
		case KEY_G:                             KeyText = "G"; break; 
		case KEY_BACKQUOTE:                     KeyText = "BACKQUOTE"; break; 
		case KEY_H:                             KeyText = "H"; break; 
		case KEY_HELP:                          KeyText = "HELP"; break; 
		case KEY_HOME:                          KeyText = "HOME"; break; 
		case KEY_I:                             KeyText = "I"; break; 
		case KEY_INSERT:                        KeyText = "INSERT"; break; 
		case KEY_J:                             KeyText = "J"; break; 
		case KEY_K:                             KeyText = "K"; break; 
		case KEY_KBDILLUMDOWN:                  KeyText = "KBDILLUMDOWN"; break; 
		case KEY_KBDILLUMTOGGLE:                KeyText = "KBDILLUMTOGGLE"; break; 
		case KEY_KBDILLUMUP:                    KeyText = "KBDILLUMUP"; break; 
		case KEY_KP_0:                          KeyText = "KP_0"; break; 
		case KEY_KP_00:                         KeyText = "KP_00"; break; 
		case KEY_KP_000:                        KeyText = "KP_000"; break; 
		case KEY_KP_1:                          KeyText = "KP_1"; break; 
		case KEY_KP_2:                          KeyText = "KP_2"; break; 
		case KEY_KP_3:                          KeyText = "KP_3"; break; 
		case KEY_KP_4:                          KeyText = "KP_4"; break; 
		case KEY_KP_5:                          KeyText = "KP_5"; break; 
		case KEY_KP_6:                          KeyText = "KP_6"; break; 
		case KEY_KP_7:                          KeyText = "KP_7"; break; 
		case KEY_KP_8:                          KeyText = "KP_8"; break; 
		case KEY_KP_9:                          KeyText = "KP_9"; break; 
		case KEY_KP_A:                          KeyText = "KP_A"; break; 
		case KEY_KP_AMPERSAND:                  KeyText = "KP_AMPERSAND"; break; 
		case KEY_KP_AT:                         KeyText = "KP_AT"; break; 
		case KEY_KP_B:                          KeyText = "KP_B"; break; 
		case KEY_KP_BACKSPACE:                  KeyText = "KP_BACKSPACE"; break; 
		case KEY_KP_BINARY:                     KeyText = "KP_BINARY"; break; 
		case KEY_KP_C:                          KeyText = "KP_C"; break; 
		case KEY_KP_CLEAR:                      KeyText = "KP_CLEAR"; break; 
		case KEY_KP_CLEARENTRY:                 KeyText = "KP_CLEARENTRY"; break; 
		case KEY_KP_COLON:                      KeyText = "KP_COLON"; break; 
		case KEY_KP_COMMA:                      KeyText = "KP_COMMA"; break; 
		case KEY_KP_D:                          KeyText = "KP_D"; break; 
		case KEY_KP_DBLAMPERSAND:               KeyText = "KP_DBLAMPERSAND"; break; 
		case KEY_KP_DBLVERTICALBAR:             KeyText = "KP_DBLVERTICALBAR"; break; 
		case KEY_KP_DECIMAL:                    KeyText = "KP_DECIMAL"; break; 
		case KEY_KP_DIVIDE:                     KeyText = "KP_DIVIDE"; break; 
		case KEY_KP_E:                          KeyText = "KP_E"; break; 
		case KEY_KP_ENTER:                      KeyText = "KP_ENTER"; break; 
		case KEY_KP_EQUALS:                     KeyText = "KP_EQUALS"; break; 
		case KEY_KP_EQUALSAS400:                KeyText = "KP_EQUALSAS400"; break; 
		case KEY_KP_EXCLAM:                     KeyText = "KP_EXCLAM"; break; 
		case KEY_KP_F:                          KeyText = "KP_F"; break; 
		case KEY_KP_GREATER:                    KeyText = "KP_GREATER"; break; 
		case KEY_KP_HASH:                       KeyText = "KP_HASH"; break; 
		case KEY_KP_HEXADECIMAL:                KeyText = "KP_HEXADECIMAL"; break; 
		case KEY_KP_LEFTBRACE:                  KeyText = "KP_LEFTBRACE"; break; 
		case KEY_KP_LEFTPAREN:                  KeyText = "KP_LEFTPAREN"; break; 
		case KEY_KP_LESS:                       KeyText = "KP_LESS"; break; 
		case KEY_KP_MEMADD:                     KeyText = "KP_MEMADD"; break; 
		case KEY_KP_MEMCLEAR:                   KeyText = "KP_MEMCLEAR"; break; 
		case KEY_KP_MEMDIVIDE:                  KeyText = "KP_MEMDIVIDE"; break; 
		case KEY_KP_MEMMULTIPLY:                KeyText = "KP_MEMMULTIPLY"; break; 
		case KEY_KP_MEMRECALL:                  KeyText = "KP_MEMRECALL"; break; 
		case KEY_KP_MEMSTORE:                   KeyText = "KP_MEMSTORE"; break; 
		case KEY_KP_MEMSUBTRACT:                KeyText = "KP_MEMSUBTRACT"; break; 
		case KEY_KP_MINUS:                      KeyText = "KP_MINUS"; break; 
		case KEY_KP_MULTIPLY:                   KeyText = "KP_MULTIPLY"; break; 
		case KEY_KP_OCTAL:                      KeyText = "KP_OCTAL"; break; 
		case KEY_KP_PERCENT:                    KeyText = "KP_PERCENT"; break; 
		case KEY_KP_PERIOD:                     KeyText = "KP_PERIOD"; break; 
		case KEY_KP_PLUS:                       KeyText = "KP_PLUS"; break; 
		case KEY_KP_PLUSMINUS:                  KeyText = "KP_PLUSMINUS"; break; 
		case KEY_KP_POWER:                      KeyText = "KP_POWER"; break; 
		case KEY_KP_RIGHTBRACE:                 KeyText = "KP_RIGHTBRACE"; break; 
		case KEY_KP_RIGHTPAREN:                 KeyText = "KP_RIGHTPAREN"; break; 
		case KEY_KP_SPACE:                      KeyText = "KP_SPACE"; break; 
		case KEY_KP_TAB:                        KeyText = "KP_TAB"; break; 
		case KEY_KP_VERTICALBAR:                KeyText = "KP_VERTICALBAR"; break; 
		case KEY_KP_XOR:                        KeyText = "KP_XOR"; break; 
		case KEY_L:                             KeyText = "L"; break; 
		case KEY_LALT:                          KeyText = "LALT"; break; 
		case KEY_LCTRL:                         KeyText = "LCTRL"; break; 
		case KEY_LEFT:                          KeyText = "LEFT"; break; 
		case KEY_LEFTBRACKET:                   KeyText = "LEFTBRACKET"; break; 
		case KEY_LGUI:                          KeyText = "LGUI"; break; 
		case KEY_LSHIFT:                        KeyText = "LSHIFT"; break; 
		case KEY_M:                             KeyText = "M"; break; 
		case KEY_MAIL:                          KeyText = "MAIL"; break; 
		case KEY_MEDIASELECT:                   KeyText = "MEDIASELECT"; break; 
		case KEY_MENU:                          KeyText = "MENU"; break; 
		case KEY_MINUS:                         KeyText = "MINUS"; break; 
		case KEY_MODE:                          KeyText = "MODE"; break; 
		case KEY_MUTE:                          KeyText = "MUTE"; break; 
		case KEY_N:                             KeyText = "N"; break; 
		case KEY_NUMLOCKCLEAR:                  KeyText = "NUMLOCKCLEAR"; break; 
		case KEY_O:                             KeyText = "O"; break; 
		case KEY_OPER:                          KeyText = "OPER"; break; 
		case KEY_OUT:                           KeyText = "OUT"; break; 
		case KEY_P:                             KeyText = "P"; break; 
		case KEY_PAGEDOWN:                      KeyText = "PAGEDOWN"; break; 
		case KEY_PAGEUP:                        KeyText = "PAGEUP"; break; 
		case KEY_PASTE:                         KeyText = "PASTE"; break; 
		case KEY_PAUSE:                         KeyText = "PAUSE"; break; 
		case KEY_PERIOD:                        KeyText = "PERIOD"; break; 
		case KEY_POWER:                         KeyText = "POWER"; break; 
		case KEY_PRINTSCREEN:                   KeyText = "PRINTSCREEN"; break; 
		case KEY_PRIOR:                         KeyText = "PRIOR"; break; 
		case KEY_Q:                             KeyText = "Q"; break; 
		case KEY_R:                             KeyText = "R"; break; 
		case KEY_RALT:                          KeyText = "RALT"; break; 
		case KEY_RCTRL:                         KeyText = "RCTRL"; break; 
		case KEY_ENTER:                         KeyText = "ENTER"; break; 
		case KEY_RETURN2:                       KeyText = "RETURN2"; break; 
		case KEY_RGUI:                          KeyText = "RGUI"; break; 
		case KEY_RIGHT:                         KeyText = "RIGHT"; break; 
		case KEY_RIGHTBRACKET:                  KeyText = "RIGHTBRACKET"; break; 
		case KEY_RSHIFT:                        KeyText = "RSHIFT"; break; 
		case KEY_S:                             KeyText = "S"; break; 
		case KEY_SCROLLLOCK:                    KeyText = "SCROLLLOCK"; break; 
		case KEY_SELECT:                        KeyText = "SELECT"; break; 
		case KEY_SEMICOLON:                     KeyText = "SEMICOLON"; break; 
		case KEY_SEPARATOR:                     KeyText = "SEPARATOR"; break; 
		case KEY_SLASH:                         KeyText = "SLASH"; break; 
		case KEY_SLEEP:                         KeyText = "SLEEP"; break; 
		case KEY_SPACE:                         KeyText = "SPACE"; break; 
		case KEY_STOP:                          KeyText = "STOP"; break; 
		case KEY_SYSREQ:                        KeyText = "SYSREQ"; break; 
		case KEY_T:                             KeyText = "T"; break; 
		case KEY_TAB:                           KeyText = "TAB"; break; 
		case KEY_THOUSANDSSEPARATOR:            KeyText = "THOUSANDSSEPARATOR"; break; 
		case KEY_U:                             KeyText = "U"; break; 
		case KEY_UNDO:                          KeyText = "UNDO"; break; 
		case KEY_UNKNOWN:                       KeyText = "UNKNOWN"; break; 
		case KEY_UP:                            KeyText = "UP"; break; 
		case KEY_V:                             KeyText = "V"; break; 
		case KEY_VOLUMEDOWN:                    KeyText = "VOLUMEDOWN"; break; 
		case KEY_VOLUMEUP:                      KeyText = "VOLUMEUP"; break; 
		case KEY_W:                             KeyText = "W"; break; 
		case KEY_WWW:                           KeyText = "WWW"; break; 
		case KEY_X:                             KeyText = "X"; break; 
		case KEY_Y:                             KeyText = "Y"; break; 
		case KEY_Z:                             KeyText = "Z"; break; 
		case KEY_AMPERSAND:                     KeyText = "AMPERSAND"; break; 
		case KEY_ASTERISK:                      KeyText = "ASTERISK"; break; 
		case KEY_AT:                            KeyText = "AT"; break; 
		case KEY_CARET:                         KeyText = "CARET"; break; 
		case KEY_COLON:                         KeyText = "COLON"; break; 
		case KEY_DOLLAR:                        KeyText = "DOLLAR"; break; 
		case KEY_EXCLAIM:                       KeyText = "EXCLAIM"; break; 
		case KEY_GREATER:                       KeyText = "GREATER"; break; 
		case KEY_HASH:                          KeyText = "HASH"; break; 
		case KEY_LEFTPAREN:                     KeyText = "LEFTPAREN"; break; 
		case KEY_LESS:                          KeyText = "LESS"; break; 
		case KEY_PERCENT:                       KeyText = "PERCENT"; break; 
		case KEY_PLUS:                          KeyText = "PLUS"; break; 
		case KEY_QUESTION:                      KeyText = "QUESTION"; break; 
		case KEY_QUOTEDBL:                      KeyText = "QUOTEDBL"; break; 
		case KEY_RIGHTPAREN:                    KeyText = "RIGHTPAREN"; break; 
		case KEY_UNDERSCORE:                    KeyText = "UNDERSCORE"; break; 
		default: KeyText = " ";
	};
	
	return KeyText;
}

bool ReadDirectory(char* Path, array_c* List, bool OmitFileExt = false)
{
	DIR* Directory = opendir(Path);
	dirent* Entry = 0;
	
	int Count = 0;
	
	bool Result = false;
	
	if(Directory)
	{
		while((Entry = readdir(Directory)) != 0)
		{
			if(Count > 1)
			{
				Assert(Entry->d_name);
				char* EntryName = Entry->d_name;
				if(OmitFileExt)
				{
					int Len = GetStrLen(EntryName, ".");
					char NewEntryName[100] = {};
					strncpy(NewEntryName, EntryName, Len);
					Append(List, string(NewEntryName));
				}
				else
					Append(List, string(EntryName));
				printf("%s\n", Entry->d_name);
			}
			++Count;
		}
		closedir(Directory);
		
		Result = true;
	}
	else
	{
		printf("Unable to locate directory!\n");
	}
	
	return false;
}

debug_file ReadEntireFile(const char* Path, const char* Mode)
{
	debug_file Result = {};
	
	SDL_RWops* File = SDL_RWFromFile(Path, Mode);
	if(File)
	{
		SDL_RWseek(File, 0L, RW_SEEK_END);
		Result.Size = SDL_RWtell(File);
		SDL_RWseek(File, 0L, RW_SEEK_SET);
		
		Result.Data = calloc(Result.Size, sizeof(char));
		if(Result.Data)
		{
			if(SDL_RWread(File, Result.Data, Result.Size, 1))
			{
				//Successfully read the file
			}
			else
			{
				free(Result.Data);
				Result.Data = 0;
			}
		}
		
		SDL_RWclose(File);
	}
	
	return Result;
}

void FreeFileFromMemory(debug_file* File)
{
	if(File->Data) free(File->Data);
}

void LoadText(SDL_Renderer* Renderer, TTF_Font* Font, image_data* Image, 
				const char* Text, SDL_Color Color, uint8 Alpha = 255)
{
	if(Text)
	{
		SDL_Surface* Surface = TTF_RenderText_Solid(Font, Text, Color);
		if(!Surface)
		{
			printf("Loading text failed! TTF_Error: %s\n", TTF_GetError());
		}
		else
		{
			Image->Dim.x = Surface->w;
			Image->Dim.y = Surface->h;
			Image->Rect = {0, 0, Surface->w, Surface->h};
			Image->Texture = SDL_CreateTextureFromSurface(Renderer, Surface);
			Image->a = Alpha;
			Image->Transparency = true;
			if(!Image->Texture)
			{
				printf("Creating texture failed! SDL_Error: %s\n", SDL_GetError());
			}
			else
			{
				SDL_SetTextureBlendMode(Image->Texture, SDL_BLENDMODE_BLEND);
				SDL_SetTextureAlphaMod(Image->Texture, Alpha);
			}
		}
		SDL_FreeSurface(Surface);
	}
}

void LoadTexture(image_data* Image, char* Path, SDL_Renderer* Renderer, 
				 uint8 Alpha)
{
	SDL_Surface* Surface = IMG_Load(Path);
	if(!Surface)
	{
		printf("Loading image failed! IMG_Error: %s\n", IMG_GetError());
	}
	else
	{
		Image->Dim.x = Surface->w;
		Image->Dim.y = Surface->h;
		Image->Rect = {0, 0, Surface->w, Surface->h};
		Image->Texture = SDL_CreateTextureFromSurface(Renderer, Surface);
		Image->a = Alpha;
		Image->Transparency = true;
		if(!Image->Texture)
		{
			printf("Creating texture failed! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			SDL_GetTextureColorMod(Image->Texture, &Image->r, &Image->g, &Image->b);
			SDL_SetTextureBlendMode(Image->Texture, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(Image->Texture, Alpha);
		}
	}
	SDL_FreeSurface(Surface);
}

void LoadAndUpdateTexture(image_data* Image, char* Path, SDL_Rect* Rect)
{
	SDL_Surface* Surface = IMG_Load(Path);
	if(!Surface)
	{
		printf("Loading image failed! IMG_Error: %s\n", IMG_GetError());
	}
	else
	{
		Image->Dim.x = Surface->w;
		Image->Dim.y = Surface->h;
		Image->Rect = {0, 0, Surface->w, Surface->h};
		if(!SDL_UpdateTexture(Image->Texture, Rect, Surface->pixels, Surface->pitch))
		{
			printf("Update texture failed. Error: %s\n", SDL_GetError());
		}
	}
	SDL_FreeSurface(Surface);
}

void UpdateTextureAlpha(image_data* Texture, real32 Strength)
{
	if(Texture->a >= 255)
	{
		Texture->AlphaSwitch = true;
	}
	else if(Texture->a <= 0)
	{
		Texture->AlphaSwitch = false;
	}
	if(Texture->AlphaSwitch)
		Texture->a -= (uint8)Strength;
	else
		Texture->a += (uint8)Strength;
	SDL_SetTextureAlphaMod(Texture->Texture, Texture->a);
}

SDL_Rect SetRectRounded(v2 Pos, v2 Dim)
{
	SDL_Rect Result = {Round32(Pos.x), Round32(Pos.y), Round32(Dim.x), Round32(Dim.y)};
	return Result;
}

SDL_Rect SetRectRounded(rect32 Rect)
{
	SDL_Rect Result = SetRectRounded(Rect.Pos, Rect.Dim);
	return Result;
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
			src->Pos.y = dst->Pos.y + Offset.y + src->Dim.y;
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
