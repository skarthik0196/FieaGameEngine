#pragma once
#include"Vector.h"
#include<stdexcept>
#include<exception>
#include<algorithm>

namespace FieaGameEngine
{
	template<typename T, typename IncrementFunctor>
	Vector<T, IncrementFunctor>::Vector(const uint32_t& capacity) :Size(0), Data(static_cast<T*>(std::malloc(Capacity * sizeof(T)))), Capacity(capacity), IncrementFunction()
	{

	}

	template<typename T, typename IncrementFunctor = BaseIncrementFunctor<int>>
	Vector<T, IncrementFunctor>::~Vector()
	{
		Clear();
		free(Data);
		Capacity = 0;
		Data = nullptr;
	}

	template<typename T, typename IncrementFunctor>
	Vector<T, IncrementFunctor>::Vector(const Vector<T, IncrementFunctor>& otherVector) :Size(0), Capacity(0), Data(nullptr), IncrementFunction(otherVector.IncrementFunction)
	{
		if (otherVector.Size > 0)
		{
			Reserve(otherVector.Capacity);
			Iterator It = otherVector.begin();
			for (; It != otherVector.end(); ++It)
			{
				PushBack(*It);
			}
		}
	}

	template<typename T, typename IncrementFunctor>
	Vector<T, IncrementFunctor>::Vector(Vector<T, IncrementFunctor>&& otherVector) : Data(otherVector.Data), Size(otherVector.Size), Capacity(otherVector.Capacity), IncrementFunction(otherVector.IncrementFunction)
	{
		otherVector.Size = 0;
		otherVector.Capacity = 0;
		otherVector.Data = nullptr;
	}

	template<typename T, typename IncrementFunctor>
	Vector<T, IncrementFunctor>& Vector<T, IncrementFunctor>::operator=(const Vector<T, IncrementFunctor>& otherVector)
	{
		if (this != &otherVector)
		{
			Clear();
			Shrink();
			Reserve(otherVector.Capacity);
			IncrementFunction = otherVector.IncrementFunction;
			Iterator It = otherVector.begin();
			if (otherVector.Size > 0)
			{
				for (; It != otherVector.end(); ++It)
				{
					PushBack(*It);
				}
			}
		}
		return *this;
	}

	template<typename T, typename IncrementFunctor>
	Vector<T, IncrementFunctor>& Vector<T, IncrementFunctor>::operator=(Vector<T, IncrementFunctor>&& otherVector)
	{
		if (this != &otherVector)
		{
			Clear();
			free(Data);
			Data = otherVector.Data;
			Size = otherVector.Size;
			Capacity = otherVector.Capacity;
			IncrementFunction = otherVector.IncrementFunction;

			otherVector.Size = 0;
			otherVector.Capacity = 0;
			otherVector.Data = nullptr;
		}
		return *this;
	}

	template<typename T, typename IncrementFunctor>
	T& Vector<T, IncrementFunctor>::operator[](const uint32_t& index)
	{
		if (index >= Size)
		{
			throw std::runtime_error("Invalid Index");
		}
		return Data[index];
	}

	template<typename T, typename IncrementFunctor>
	const T& Vector<T, IncrementFunctor>::operator[](const uint32_t& index) const
	{
		if (index >= Size)
		{
			throw std::runtime_error("Invalid Index");
		}
		return Data[index];
	}

	template<typename T, typename IncrementFunctor>
	T& Vector<T, IncrementFunctor>::Front()
	{
		if (Size <= 0)
		{
			throw std::runtime_error("Unable to dereference empty vector");
		}
		return Data[0];
	}

	template<typename T, typename IncrementFunctor>
	 const T & Vector<T, IncrementFunctor>::Front() const
	{
		 if (Size <= 0)
		 {
			 throw std::runtime_error("Unable to dereference empty vector");
		 }
		 return Data[0];
	}

	 template<typename T, typename IncrementFunctor>
	 T & Vector<T, IncrementFunctor>::Back()
	 {
		 if (Size <= 0)
		 {
			 throw std::runtime_error("Unable to dereference empty vector");
		 }
		 return Data[Size - 1];
	 }

	 template<typename T, typename IncrementFunctor>
	 const T & Vector<T, IncrementFunctor>::Back() const
	 {
		 if (Size <= 0)
		 {
			 throw std::runtime_error("Unable to dereference empty vector");
		 }
		 return Data[Size - 1];
	 }

	template<typename T, typename IncrementFunctor>
	T& Vector<T, IncrementFunctor>::At(const uint32_t& index)
	{
		if (index >= Size)
		{
			throw std::runtime_error("Invalid Index");
		}
		return Data[index];
	}

	template<typename T, typename IncrementFunctor>
	const T& Vector<T, IncrementFunctor>::At(const uint32_t& index) const
	{
		if (index >= Size)
		{
		throw std::runtime_error("Invalid Index");
		}
		return Data[index];
	}

	template<typename T, typename IncrementFunctor>
	bool Vector<T, IncrementFunctor>::IsEmpty() const
	{
		return (Size == 0);
	}

	template<typename T, typename IncrementFunctor>
	void Vector<T, IncrementFunctor>::Clear()
	{
		for (uint32_t i = 0; i < Size; ++i)
		{
			Data[i].~T();
		}
		Size = 0;
	}

	template<typename T, typename IncrementFunctor>
	void Vector<T, IncrementFunctor>::PushBack(const T& data)
	{
		if (Size == Capacity)
		{
			Reserve(std::max<uint32_t>(1U, (Capacity + IncrementFunction(Size, Capacity))));
		}
		new(&Data[Size])T(data);
		++Size;
	}

	template<typename T, typename IncrementFunctor>
	void Vector<T, IncrementFunctor>::PopBack()
	{
		if (Size > 0)
		{
			Data[Size - 1].~T();
			--Size;
		}
	}

	template<typename T, typename IncrementFunctor>
	uint32_t Vector<T, IncrementFunctor>::Length() const
	{
		return Size;
	}

	template<typename T, typename IncrementFunctor>
	uint32_t Vector<T, IncrementFunctor>::CurrentCapacity() const
	{
		return Capacity;
	}

	template<typename T, typename IncrementFunctor>
	void Vector<T, IncrementFunctor>::Reserve(const uint32_t& capacity)
	{
		if (capacity > Capacity)
		{
			Data = static_cast<T*>(std::realloc(Data, capacity * sizeof(T)));
			Capacity = capacity;
		}
	}

	template<typename T, typename IncrementFunctor>
	void Vector<T, IncrementFunctor>::Shrink()
	{
		T* newDataAllocation = static_cast<T*>(std::malloc(Size * sizeof(T)));
		std::memcpy(newDataAllocation, Data, Size * sizeof(T));
		std::free(Data);
		Data = newDataAllocation;
		Capacity = Size;
	}

	template<typename T, typename IncrementFunctor>
	void Vector<T, IncrementFunctor>::Remove(const uint32_t& index)
	{
		if (index > (Size - 1))
		{
			throw std::runtime_error("InValid Position provided");
		}
		if (index == (Size - 1))
		{
			PopBack();
		}
		else
		{
			Data[index].~T();
			memmove(Data + index, Data + index + 1, ((Size - index -1) * sizeof(T)));
			--Size;
		}
	}

	template<typename T, typename IncrementFunctor>
	void Vector<T, IncrementFunctor>::Remove(typename const Vector<T, IncrementFunctor>::Iterator& startPoint,typename const Vector<T, IncrementFunctor>::Iterator& endPoint)
	{
		if (endPoint.Index > Size || startPoint.Index > endPoint.Index)
		{
			throw std::runtime_error("Invalid Iterators");
		}
		Iterator It1 = startPoint;
		Iterator It2 = endPoint;

		It2++;
		for (;It1 != It2; ++It1)
		{
			(*It1).~T();
		}
		memmove(Data + startPoint.Index, Data + endPoint.Index + 1, ((Size - It2.Index) * sizeof(T)));
		Size = Size - (endPoint.Index - startPoint.Index + 1);
	}

	template<typename T, typename IncrementFunctor>
	typename Vector<T, IncrementFunctor>::Iterator Vector<T, IncrementFunctor>::begin() const
	{/*
		if (Size == 0)
		{
			throw std::runtime_error("Unable to return a valid Iterator");
		}*/
		Iterator It(0, *this);
		return It;
	}

	template<typename T, typename IncrementFunctor>
	typename Vector<T, IncrementFunctor>::Iterator Vector<T, IncrementFunctor>::end() const
	{/*
		if (Size == 0)
		{
			throw std::runtime_error("Unable to return a valid Iterator");
		}*/
		Iterator It(Size, *this);
		return It;

	}

	template<typename T, typename IncrementFunctor>
	Vector<T, IncrementFunctor>::Iterator::Iterator():Index(0),Owner(nullptr)
	{

	}

	template<typename T, typename IncrementFunctor>
	Vector<T, IncrementFunctor>::Iterator::Iterator(const uint32_t& index, const Vector<T, IncrementFunctor>& owner) :Index(index), Owner(&owner)
	{

	}

	template<typename T, typename IncrementFunctor>
	T& Vector<T, IncrementFunctor>::Iterator::operator*()
	{
		if (Owner == nullptr)
		{
			throw std::runtime_error("Unable to deference Iterator");
		}
		return const_cast<T&>(Owner->operator[](Index));
	}

	template<typename T, typename IncrementFunctor>
	const T& Vector<T, IncrementFunctor>::Iterator::operator*() const
	{
		if (Owner == nullptr)
		{
			throw std::runtime_error("Unable to deference Iterator");
		}
		return Owner->operator[](Index);
	}

	template<typename T, typename IncrementFunctor>
	typename Vector<T, IncrementFunctor>::Iterator& Vector<T, IncrementFunctor>::Iterator::operator++()
	{
		if (Owner == nullptr)
		{
			throw std::runtime_error("Invalid Iterator");
		}
		++Index;
		return *this;
	}

	template<typename T, typename IncrementFunctor>
	typename Vector<T, IncrementFunctor>::Iterator Vector<T, IncrementFunctor>::Iterator::operator++(int)
	{
		if (Owner == nullptr)
		{
			throw std::runtime_error("Invalid Iterator");
		}
		Iterator temp = *this;
		++Index;
		return temp;
	}

	template<typename T, typename IncrementFunctor>
	bool Vector<T, IncrementFunctor>::Iterator::operator==(const Iterator& Rhs)
	{
		return ((Index == Rhs.Index) && (Owner == Rhs.Owner));
	}
	
	template<typename T, typename IncrementFunctor>
	bool Vector<T, IncrementFunctor>::Iterator::operator!=(const Iterator& Rhs)
	{
		return !(operator==(Rhs));
	}

	template<typename T, typename IncrementFunctor>
	typename Vector<T, IncrementFunctor>::Iterator& Vector<T, IncrementFunctor>::Iterator::operator=(const Iterator& rhs)
	{
		if (this != &rhs)
		{
			Index = rhs.Index;
			Owner = rhs.Owner;
		}

		return *this;
	}

	template<typename T, typename IncrementFunctor>
	typename Vector<T, IncrementFunctor>::Iterator Vector<T, IncrementFunctor>::Find(const T& Item) const
	{
		Iterator It = begin();
		for (; It != end(); ++It)
		{
			if (It.Owner->operator[](It.Index) == Item)
			{
				break;
			}
		}
		return It;
	}

	template<typename T>
	uint32_t BaseIncrementFunctor<T>::operator()(uint32_t size, uint32_t capacity) const
	{
		size;
		return capacity;
	}

	template<typename T, typename IncrementFunctor>
	void Vector<T, IncrementFunctor>::Resize(const uint32_t& size)
	{
		if (size > Capacity)
		{
			Reserve(size);
			for (uint32_t i = Size; i < size; ++i)
			{
				new(&Data[i])T();
			}
		}
		else
		{
			for (uint32_t i = Size - 1; i > size - 1; --i)
			{
				Data[i].~T();
			}
			Data = reinterpret_cast<T*>(realloc(Data, size * sizeof(T)));
			Capacity = Size;
		}
		Size = size;
	}
}