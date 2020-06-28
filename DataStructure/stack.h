#ifndef STACK_H
#define STACK_H

#include "array.h"

struct stack32
{
	array Stack;
};

stack32 InitStack(int Capacity)
{
	stack32 Stack = {};
	Stack.Stack = InitArray(Capacity);
	return Stack;
}

real32 Push(stack32* Stack, real32 Value)
{
	Append(&Stack->Stack, Value);
	return Value;
}

real32 Pop(stack32* Stack)
{
	assert(Stack->Stack.Length > 0);
	return Stack->Stack.Data[--Stack->Stack.Length];
}

real32 Peek(stack32* Stack)
{
	return Stack->Stack.Data[Stack->Stack.Length - 1];
}

#endif