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
	int Length = (int)strlen(Data);
	
	AE_String Str = {};
	Str.Capacity = Length+1;
	Str.Length = Length;
	Str.Data = (char*)calloc(Str.Capacity, sizeof(char));
	
	memcpy(Str.Data, Data, Length);
	Str.Data[Length] = '\0';
	
	return Str;
}

AE_String& operator+=(AE_String& Str, const char* Data)
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
	
	return Str;
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