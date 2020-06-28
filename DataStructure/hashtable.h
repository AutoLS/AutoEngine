#ifndef HASHTABLE_H
#define HASHTABLE_H

struct hash_table
{
	array_lp Internal;
	int Length;
};

hash_table InitTable(int Capacity)
{
	hash_table Table = {};
	Table.Internal = InitArray_lp(Capacity);
	
	return Table;
};

int Hash(char* Key)
{
	int Result = 0;
	for(int i = 0; i < (int)strlen(Key); ++i)
	{
		Result += Key[i] * (i + 1);
	}
	return Result;	
}

void Insert(hash_table* Table, char* Key)
{
	pair Pair = {};
	Pair.Key = Key;
	Pair.Value = Hash(Key);
	
	int Index = Pair.Value % Table->Internal.Capacity;
	PushFront(&Table->Internal.List[Index], Pair);
	++Table->Length;
}

bool MatchKeys(char* KeyA, char* KeyB)
{
	if(strlen(KeyA) != strlen(KeyB))
		return false;
	bool Match = false;
	for(int i = 0; i < (int)strlen(KeyA); ++i)
	{
		Match = KeyA[i] == KeyB[i];
	}
	return Match;
}

bool Traverse(node_p* Node, char* Key)
{
	if(Node->Next == 0)
		return false;
	if(MatchKeys(Node->Data.Key, Key))
		return true;
	else if(Node->Next)
		return Traverse(Node->Next, Key);
	else
		return false;
}

void LookUp(hash_table* Table, char* Key)
{
	int HashKey = Hash(Key);
	int Index = HashKey % Table->Internal.Capacity;
	if(Traverse(Table->Internal.List[Index].Head, Key))
	{
		printf("%s exist in table index %d with value of %d.\n", Key, Index, HashKey);
	}
	else
	{
		printf("Key does not exist.\n");
	}
}

#endif