#ifndef SET_H
#define SET_H

#include "linked_list.h"
#include "hashtable.h"

struct pair32
{
	char* Key;
	real32 Value;
};

struct set
{
	array Internal;
};

set InitSet(int Capacity)
{
	set Set = {};
	Set.Internal = InitArray(Capacity);
	return Set;
}

void Insert(set* Set, pair32 Pair)
{
	int Index = Hash(Pair.Key) % Set->Internal.Capacity;
	Set->Internal.Data[Index] = Pair.Value;
}

real32 Retrieve(set* Set, char* Key)
{
	int Index = Hash(Key) % Set->Internal.Capacity;
	return Set->Internal.Data[Index];
}

#endif