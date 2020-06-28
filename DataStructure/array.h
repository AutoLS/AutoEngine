#ifndef ARRAY_H
#define ARRAY_H

typedef float real32;

struct array
{
	real32* Data;
	int Length;
	int Capacity;
};

struct array_c
{
	std::string* Data;
	int Length;
	int Capacity;
};

#include "linked_list.h"
struct array_lp
{
	linked_list_p* List;
	int Length;
	int Capacity;
};

array InitArray(int Capacity)
{
	array NewArr = {};
	NewArr.Data = (real32*)malloc(sizeof(real32) * Capacity);
	NewArr.Capacity = Capacity;
	
	return NewArr;
}

array_c InitArray_c(int Capacity)
{
	array_c NewArr = {};
	NewArr.Data = (std::string*)malloc(sizeof(std::string) * Capacity);
	ZeroMemory((void*)NewArr.Data, sizeof(std::string) * Capacity);
	NewArr.Capacity = Capacity;
	
	return NewArr;
}

array_lp InitArray_lp(int Capacity)
{
	array_lp NewArr = {};
	NewArr.List = (linked_list_p*)malloc(sizeof(linked_list_p) * Capacity);
	NewArr.Capacity = Capacity;
	
	return NewArr;
}

void FreeArray(array_c* Arr)
{
	if(Arr->Data)
	{
		free(Arr->Data);
	}
}

void Grow(array* Arr)
{
	Arr->Capacity *= 2;
	real32* NewArr = (real32*)malloc(sizeof(real32) * Arr->Capacity);
	memcpy(NewArr, Arr->Data, sizeof(real32) * Arr->Length);
	free(Arr->Data);
	
	Arr->Data = NewArr;
}

void Grow(array_c* Arr)
{
	Arr->Capacity *= 2;
	std::string* NewArr = (std::string*)malloc(sizeof(std::string) * 
						  Arr->Capacity);
	ZeroMemory((void*)NewArr, sizeof(std::string) * Arr->Capacity);
	memcpy(NewArr, Arr->Data, sizeof(std::string) * Arr->Length);
	free(Arr->Data);
	
	Arr->Data = NewArr;
}

void Grow(array_lp* Arr)
{
	Arr->Capacity *= 2;
	linked_list_p* NewArr = (linked_list_p*)malloc(sizeof(linked_list_p) * Arr->Capacity);
	memcpy(NewArr, Arr->List, sizeof(linked_list_p) * Arr->Length);
	free(Arr->List);
	
	Arr->List = NewArr;
}

void Append(array* Arr, real32 Value)
{
	if(Arr->Length == Arr->Capacity)
	{
		Grow(Arr);
	}
	Arr->Data[Arr->Length++] = Value;
}

void Append(array_c* Arr, std::string Entry)
{
	if(Arr->Length == Arr->Capacity)
	{
		Grow(Arr);
	}
	Arr->Data[Arr->Length++] = Entry;
}

#endif