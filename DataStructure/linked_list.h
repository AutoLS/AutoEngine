#ifndef LINKLIST_H
#define LINKLIST_H

struct pair
{
	char* Key;
	int Value;
};

struct node
{
	real32 Data;
	node* Next;
};

struct node_p
{
	pair Data;
	node_p* Next;
};

struct linked_list
{
	node* Head;
};

struct linked_list_p
{
	node_p* Head;
};

linked_list InitLinkedList()
{
	return {};
}

linked_list_p InitLinkedList_p()
{
	return {};
}

void PushFront(linked_list* List, real32 Value)
{
	node* NewNode = (node*)malloc(sizeof(node));
	NewNode->Data = Value;
	
	if(List->Head == 0)
	{
		List->Head = NewNode;
	}
	else
	{
		NewNode->Next = List->Head;
		List->Head = NewNode;
	}
}

void PushFront(linked_list_p* List, pair Pair)
{
	node_p* NewNode = (node_p*)malloc(sizeof(node_p));
	*NewNode = {};
	NewNode->Data = Pair;
	
	if(List->Head == 0)
	{
		List->Head = NewNode;
	}
	else
	{
		NewNode->Next = List->Head;
		List->Head = NewNode;
	}
}

node* Traverse(node* Node, int Index, int Position)
{
	if(Index < Position)
	{
		return Traverse(Node->Next, ++Index, Position);
	}
	else if(Index == Position)
	{
		return Node;
	}
	else 
		return Node;
}

void Insert(linked_list* List, real32 Value, int Position)
{
	if(Position == 0)
	{
		PushFront(List, Value);
	}
	else
	{
		node* NewNode = (node*)malloc(sizeof(node));
		NewNode->Data = Value;
		
		int Index = 0;
		node* PtrNode = Traverse(List->Head, Index, Position-1);
		if(PtrNode->Next)
		{
			NewNode->Next = PtrNode->Next;
		}
		PtrNode->Next = NewNode;
	}
}

void PrintList(node* Node)
{
	if(Node->Next)
	{
		printf("%.1f ", Node->Data);
		PrintList(Node->Next);
	}
	else
		printf("\n");
}

struct d_node
{
	real32 Data;
	d_node* Next;
	d_node* Previous;
};

struct d_linked_list
{
	d_node* Head;
	d_node* Tail;
};

d_linked_list dLinkedList()
{
	return {};
}

void PushFront(d_linked_list* List, real32 Value)
{
	d_node* NewNode = (d_node*)malloc(sizeof(d_node));
	NewNode->Data = Value;
	
	if(List->Head == 0)
	{
		List->Head = NewNode;
		List->Tail = NewNode;
	}
	else
	{
		NewNode->Next = List->Head;
		NewNode->Next->Previous = NewNode;
		List->Head = NewNode;
	}
}

void PushBack(d_linked_list* List, real32 Value)
{
	d_node* NewNode = (d_node*)malloc(sizeof(d_node));
	NewNode->Data = Value;
	
	if(List->Tail == 0)
	{
		List->Head = NewNode;
		List->Tail = NewNode;
	}
	else
	{
		NewNode->Previous = List->Tail;
		NewNode->Previous->Next = NewNode;
		List->Tail = NewNode;
	}
}

void PopBack(d_linked_list* List)
{
	if(List->Head == List->Tail)
	{
		List->Head = List->Tail = 0;
	}
	else
	{
		d_node* Last = List->Tail;
		List->Tail = Last->Previous;
		List->Tail->Next = 0;
		free(Last);
	}
}

void PrintList(d_node* Node)
{
	if(Node->Next)
	{
		printf("%.1f ", Node->Data);
		PrintList(Node->Next);
	}
	else
		printf("\n");
}

#endif