#ifndef QUEUE_H
#define QUEUE_H

#include "linked_list.h"

struct queue
{
	d_linked_list List;
	int Length;
};

queue InitQueue()
{
	return {};
}

void Enqueue(queue* Queue, real32 Value)
{
	++Queue->Length;
	PushFront(&Queue->List, Value);
}

real32 Dequeue(queue* Queue)
{
	assert(Queue->List.Head);
	real32 Result = Queue->List.Tail->Data;
	--Queue->Length;
	PopBack(&Queue->List);
	
	return Result;
}

#endif 