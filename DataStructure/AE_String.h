#ifndef AE_STRING
#define AE_STRING

struct AE_String
{
	char* Data;
	int Length;
	int Capacity;
};

AE_String AE_Str(const char* Data)
{
	AE_String Str = {};

	if(Data)
	{
		int Length = (int)strlen(Data);
	
		Str.Capacity = Length+1;
		Str.Length = Length;
		Str.Data = (char*)calloc(Str.Capacity, sizeof(char));
		
		memcpy(Str.Data, Data, Length);
		Str.Data[Length] = '\0';
	}
	else
	{
		Str.Capacity = 1;
		Str.Length = 1;
		Str.Data = (char*)calloc(Str.Capacity, sizeof(char));
	}
	
	return Str;
}

AE_String& operator+=(AE_String& Str, const char* Data)
{
	if(Data)
	{
		int Length = (int)strlen(Data);
		int NewLen = Length + Str.Length;
		
		if(Str.Capacity < NewLen)
		{
			Str.Capacity = NewLen * 2;
			
			char* NewStr = (char*)calloc(Str.Capacity, sizeof(char));
			for(int i = 0; i < Length; ++i)
			{
				NewStr[i] = Str.Data[i];
			}
			
			int Count = 0;
			for(int i = Str.Length; i < NewLen; ++i)
			{
				NewStr[i] = Data[Count++];
			}
			
			free(Str.Data);
			
			Str.Length = NewLen;
			Str.Data = NewStr;
		}
		else
		{
			int Count = 0;
			for(int i = Str.Length; i < NewLen; ++i)
			{
				Str.Data[i] = Data[Count++];
			}
			
			Str.Length = NewLen;
		}
	}
	
	return Str;
}

AE_String AE_Substr(AE_String* Str, int Start, int End)
{
	AE_String NewStr = {};
	int Length = End - Start + 1;
	char* Buffer = (char*)malloc(sizeof(char) * Length);
	memcpy(Buffer, &Str->Data[Start], Length);
	Buffer[Length] = '\0';
	
	NewStr = AE_Str(Buffer);
	free(Buffer);
	
	return NewStr;
}

int AE_IndexOfReverse(AE_String* Str, char c, int Start)
{
	while(Start > 0)
	{
		if(c == Str->Data[Start])
		{
			return Start;
		}
		--Start;
	}
	return -1;
}

AE_String operator+(AE_String StrA, AE_String StrB)
{
	AE_String NewStr = AE_Str(StrA.Data);
	NewStr += StrB.Data;
	
	return NewStr;
}

void AE_PrintStr(AE_String* Str)
{
	printf("%s", Str->Data);
}

void AE_PrintStrLn(AE_String* Str)
{
	printf("%s\n", Str->Data);
}

void AE_FreeString(AE_String* Str)
{
	Assert(Str->Data);
	free(Str->Data);
}

#endif