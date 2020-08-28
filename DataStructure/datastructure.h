#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

namespace ds
{
	template <class T>
	struct array
	{
		int Length;
		int Capacity;
		T* Data;
	};

	template <class T>
	void InitArray(array<T> *Arr, int Capacity)
	{
		Arr->Capacity = Capacity;
		Arr->Data = (T*)malloc(sizeof(T) * Capacity);
	}

	template <class T>
	void Append(array<T> *Arr, T Value)
	{
		if(Arr->Length == Arr->Capacity)
		{
			int NewCapacity = Arr->Capacity * 2;
			T* NewData = (T*)malloc(sizeof(T) * NewCapacity);
			memcpy((void*)NewData, (void*)Arr->Data, Arr->Capacity * sizeof(T));
			free(Arr->Data);
			
			Arr->Data = NewData;
			Arr->Capacity = NewCapacity;
		}
		Arr->Data[Arr->Length++] = Value;
	}

	template <class T>
	void PrintArray(array<T> *Arr)
	{
		for(int i = 0; i < Arr->Length; ++i)
		{
			std::cout << Arr->Data[i] << '\n';
		}
	}
}

#endif