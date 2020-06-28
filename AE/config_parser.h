#ifndef CONFIG_PARSE_H

struct attribute
{
	char* Name;
	int Len;
	
	char Value[100];
};

attribute GetAttribute(char* Buffer, char* Name)
{
	attribute Attribute = {};
	Attribute.Name = Name;
	
	char* FieldPtr = strstr(Buffer, Name);
	if(FieldPtr)
	{
		char* Setting = GetStrFromChar(FieldPtr, '=');
		Attribute.Len = GetStrLen(Setting, "\r");
		
		if(Attribute.Len < 100)
		{
			memcpy((void*)Attribute.Value, (void*)Setting, Attribute.Len);			
		}
		else
		{
			printf("ERROR: CONFIG VALUE TOO LONG\n");
		}
	}
	
	return Attribute;
}

void GetAttributeValue_s(attribute* Attribute, char* Dst)
{
	if(Attribute->Value)
	{
		strcpy(Dst, Attribute->Value);
	}
	else
	{
		free(Dst);
		Dst = 0;
	}
}

bool GetAttributeValue_b(attribute* Attribute)
{
	bool Result = false;
	if(Attribute->Value)
	{
		int Value = atoi(Attribute->Value);
		Result = Value > 0;
	}
	
	return Result;
}

int GetAttributeValue_i(attribute* Attribute, int DefRet = 0)
{
	int Result = DefRet;
	if(Attribute->Value)
	{
		Result = atoi(Attribute->Value);
	}
	
	return Result;
}

int64 GetAttributeValue_i64(attribute* Attribute, int64 DefRet = 0)
{
	int64 Result = DefRet;
	if(Attribute->Value)
	{
		Result = atol(Attribute->Value);
	}
	
	return Result;
}

real32 GetAttributeValue_f(attribute* Attribute, real32 DefRet = 0)
{
	real32 Result = DefRet;
	if(Attribute->Value)
	{
		Result = (real32)atof(Attribute->Value);
	}
	
	return Result;
}

#define CONFIG_PARSE_H
#endif