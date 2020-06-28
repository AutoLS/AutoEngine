#ifndef TREE_H
#define TREE_H

struct t_node
{
	real32 Data;
	t_node* Left;
	t_node* Right;
};

struct binary_tree
{
	t_node* Root;
};

binary_tree BinaryTree()
{
	binary_tree Tree = {};
	Tree.Root = (t_node*)malloc(sizeof(t_node));
	*Tree.Root = {};
	return Tree;
}

bool BinarySearch(t_node* Node, real32 Value)
{
	if(Node == 0)
	{
		return false;
	}
	
	if(Value > Node->Data)
	{
		return BinarySearch(Node->Right, Value);
	}
	else if(Value < Node->Data)
	{
		return BinarySearch(Node->Left, Value);
	}
	else if(Value == Node->Data)
		return true;
	
	return false;
}

void Insert(t_node* Node, real32 Value)
{
	if(Node->Data == Value)
	{
		return;
	}
	else if(Value > Node->Data)
	{
		if(Node->Right == 0)
		{
			t_node* NewNode = (t_node*)malloc(sizeof(t_node));
			*NewNode = {};
			NewNode->Data = Value;
			Node->Right = NewNode;
		}
		else
		{
			Insert(Node->Right, Value);
		}
	}
	else
	{
		if(Node->Left == 0)
		{
			t_node* NewNode = (t_node*)malloc(sizeof(t_node));
			*NewNode = {};
			NewNode->Data = Value;
			Node->Left = NewNode;
		}
		else
		{
			Insert(Node->Left, Value);
		}
	}
}

#endif 