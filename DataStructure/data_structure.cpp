#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "array.h"
#include "stack.h"
#include "linked_list.h"
#include "tree.h"
#include "queue.h"
#include "hashtable.h"
#include "set.h"

// Dynamic Array Done
// Stack Done
// Linked list Done
// Binary Tree Done
// Queue Done
// Deque Done
// Hashtable Done
// Set 

#if 0
int main()
{
	linked_list List = InitLinkedList();
	
	for(int i = 0; i < 5; ++i)
	{
		PushFront(&List, (real32)i);
	}
	
	Insert(&List, 69, 1);
	
	PrintList(List.Head);
	binary_tree Tree = BinaryTree();
	
	Insert(Tree.Root, 3);
	Insert(Tree.Root, 12);
	Insert(Tree.Root, 5);
	
	if(BinarySearch(Tree.Root, 12))
		printf("True");
	else
		printf("False");
	
	queue Queue = InitQueue();
	
	for(int i = 0; i < 5; ++i)
	{
		Enqueue(&Queue, (real32)i);
	}
	
	for(int i = 0; i < 5; ++i) printf("Dequeue: %.1f\n", Dequeue(&Queue));
	
	Enqueue(&Queue, 69);
	Enqueue(&Queue, 420);
	
	printf("Dequeue: %.1f\n", Dequeue(&Queue));
	printf("Dequeue: %.1f\n", Dequeue(&Queue));
	

	hash_table Table = InitTable(15);
	Insert(&Table, "Auto");
	Insert(&Table, "Roido");
	Insert(&Table, "Afb");
	Insert(&Table, "xxx");
	Insert(&Table, "SharpN");
	
	LookUp(&Table, "Auto");
	LookUp(&Table, "SharpN");
	
	return 0;
}
#endif