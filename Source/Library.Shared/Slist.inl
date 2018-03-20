#pragma once
#include "SList.h"
#include<iostream>
#include<exception>
#include<stdexcept>

namespace FieaGameEngine
{
	template<typename T>
	SList<T>::Node::Node(const T& Value, Node *NextNode = nullptr):Data(Value),Next(NextNode)
	{

	}

	template<typename T>
	SList<T>::SList() :FrontPointer(nullptr), BackPointer(nullptr), Size(0)
	{
		
	}
	
	template<typename T>
	SList<T>::SList(const SList<T> &OtherList) : FrontPointer(nullptr), BackPointer(nullptr), Size(0)
	{

		Node *Temp = OtherList.FrontPointer;

		while (Temp != nullptr)
		{
			this->PushBack(Temp->Data);
			Temp = Temp->Next;
		}	
	}

	template<typename T>
	SList<T>::SList(SList<T> &&OtherList) : FrontPointer(std::move(OtherList.FrontPointer)), BackPointer(std::move(OtherList.BackPointer)), Size(std::move(OtherList.Size))
	{
		OtherList.FrontPointer = nullptr;
		OtherList.BackPointer = nullptr;
		OtherList.Size = 0;
	}

	template<typename T>
	T& SList<T>::Front()
	{
		if (FrontPointer == nullptr)
		{
			throw std::runtime_error("List is Empty");
		}
		return FrontPointer->Data;
	}

	template<typename T>
	T& SList<T>::Back()
	{
		if (BackPointer == nullptr)
		{
			throw std::runtime_error("List is Empty");
		}
		return BackPointer->Data;
	}

	template<typename T>
	const T& SList<T>::Front() const
	{
		if (FrontPointer == nullptr)
		{
			throw std::runtime_error("List is Empty");
		}
		return (FrontPointer->Data);
	}

	template<typename T> 
	const T& SList<T>::Back() const
	{
		if (BackPointer == nullptr)
		{
			throw std::runtime_error("List is Empty");
		}
		return (BackPointer->Data);
	}


	template<typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& Data)
	{
		Node *Temp = new Node(Data);
		if (BackPointer == nullptr)
		{
			FrontPointer = Temp;
		}
		else
		{
			BackPointer->Next = Temp;
		}
		BackPointer = Temp;
		++Size;

		return Iterator(*this, BackPointer);
	}

	template<typename T>
	void SList<T>::PushFront(const T& Data)
	{
		FrontPointer = new Node(Data, FrontPointer);
		if (BackPointer == nullptr)
		{
			BackPointer = FrontPointer;
		}
		++Size;
	}

	template<typename T>
	void SList<T>::PopFront()
	{
		if (FrontPointer == nullptr)
		{
			throw std::runtime_error("Empty List");
		}
		Node *Temp = FrontPointer;
		FrontPointer = FrontPointer->Next;
		if (FrontPointer == nullptr)
		{
			BackPointer = nullptr;
		}
		delete Temp;
		--Size;
		
	}

	template<typename T>
	void SList<T>::PopBack()
	{
		if (Size > 1)
		{
			Node *Temp = FrontPointer;
			while (Temp->Next->Next != nullptr)
			{
				Temp = Temp->Next;
			}
			delete(Temp->Next);
			Temp->Next = nullptr;
			--Size;
			BackPointer = Temp;
		}
		else if (Size == 1)
		{
			delete FrontPointer;
			FrontPointer = nullptr;
			BackPointer = nullptr;
			Size = 0;
		}
		else
		{
			throw std::runtime_error("List is empty");
		}
	}

	template<typename T>
	void SList<T>::Clear()
	{
		Node *Temp = FrontPointer;

		while (FrontPointer != nullptr)
		{
			Temp = FrontPointer;
			FrontPointer = FrontPointer->Next;
			delete Temp;
		}
		Size = 0;
		BackPointer = nullptr;
	}

	template<typename T>
	int SList<T>::Length() const
	{
		return Size;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::begin()
	{
		/*if (FrontPointer == nullptr)
		{
			throw std::runtime_error("List is empty");
		}*/
		SList<T>::Iterator It(*this, FrontPointer);
		return It;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::end()
	{
		/*if (BackPointer == nullptr)
		{
			throw std::runtime_error("List is empty");
		}*/
		SList<T>::Iterator It(*this, nullptr/*BackPointer->Next*/);
		return It;
	}

	template<typename T>
	typename const SList<T>::Iterator SList<T>::begin() const
	{
		/*if (FrontPointer == nullptr)
		{
			throw std::runtime_error("List is empty");
		}*/
		const SList<T>::Iterator It(*this, FrontPointer);
		return It;
	}

	template<typename T>
	typename const SList<T>::Iterator SList<T>::end() const
	{
		/*if (BackPointer == nullptr)
		{
			throw std::runtime_error("List is empty");
		}*/
		const SList<T>::Iterator It(*this, nullptr/*BackPointer->Next*/);
		return It;
	}

	template<typename T>
	bool SList<T>::IsEmpty() const
	{
		return (Size == 0);
	}

	template<typename T>
	SList<T>& SList<T>::operator=(const SList<T> &Rhs)
	{
		if (this != &Rhs)
		{
			Clear();
			Node *Temp = Rhs.FrontPointer;

			while (Temp != nullptr)
			{
				PushBack(Temp->Data);
				Temp = Temp->Next;
			}
		}

		return *this;
	 }

	template<typename T>
	SList<T>& SList<T>::operator=(SList<T> &&Rhs)
	{
		if (this != &Rhs)
		{
			FrontPointer = std::move(Rhs.FrontPointer);
			BackPointer = std::move(Rhs.BackPointer);
			Size = Rhs.Size;
			Rhs.FrontPointer = nullptr;
			Rhs.BackPointer = nullptr;
			Rhs.Size = 0;
		}

		return *this;
	}

	template<typename T>
	bool SList<T>::Remove(const T& Item)
	{
		bool Result = false;
		if (FrontPointer->Data == Item)
		{
			Result = true;
			PopFront();
		}
		else if (BackPointer->Data == Item)
		{
			Result = true;
			PopBack();
		}
		else
		{
			for (Iterator It = begin(); It != end(); ++It)
			{
				if ((It.CurrentNode->Next != nullptr) && (It.CurrentNode->Next->Data == Item))
				{
					
					Node *Temp = It.CurrentNode->Next;
					It.CurrentNode->Next = It.CurrentNode->Next->Next;
					Result = true;
					delete Temp;
					--Size;
					break;
					
				}
			}
		}
		return Result;
	}

	template<typename T>
	void SList<T>::Remove(Iterator& It)
	{
		if (It.Owner == nullptr || It.CurrentNode == nullptr)
		{
			throw std::runtime_error("Invalid Iterator");
		}
		if (It.CurrentNode == FrontPointer)
		{
			PopFront();
		}
		else if (It.CurrentNode->Next != nullptr)
		{
			Node *Temp = It.CurrentNode->Next;
			if (Temp == BackPointer)
			{
				BackPointer = It.CurrentNode;
			}
			It.CurrentNode->Data = std::move(It.CurrentNode->Next->Data);
			It.CurrentNode->Next = std::move(It.CurrentNode->Next->Next);
			delete Temp;
			--Size;
		}
		else
		{
			PopBack();
		}
	}

	template<typename T>
	bool SList<T>::RemoveAll(const T& Item)
	{
		bool Result = false;
		while (Size > 0 && Front() == Item)
		{
			PopFront();
			Result = true;
		}
		while (Size > 0 && Back() == Item)
		{
			PopBack();
			Result = true;
		}
		if (Size > 0)
		{
			for (Iterator It = begin(); It != end();)
			{
				if ((It.CurrentNode->Next != nullptr) && (It.CurrentNode->Next->Data == Item))
				{
					Node *Temp = It.CurrentNode->Next;
					It.CurrentNode->Next = It.CurrentNode->Next->Next;
					delete Temp;
					Result = true;
					--Size;
				}
				else
				{
					++It;
				}
			}
		}
		return Result;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::Find(const T& Value) const
	{
		Iterator It = begin();
		for (; It != end(); ++It)
		{
			if ((*It) == Value)
			{
				break;
			}
		}
		return It;
	}

	template <typename T>
	void SList<T>::InsertAfter(typename SList<T>::Iterator& It, const T& Value)
	{
		if (It.Owner != this)
		{
			throw std::runtime_error("Invalid Iterator");
		}
		if (It.CurrentNode == BackPointer || It == end())
		{
			PushBack(Value);
		}
		else
		{
			Node *Temp = new Node(Value, It.CurrentNode->Next);
			It.CurrentNode->Next = Temp;
			++Size;
		}
	}

	template<typename T>
	SList<T>::~SList()
	{
		Clear();
	}

	template<typename T>
	SList<T>::Iterator::Iterator() :Owner(nullptr), CurrentNode(nullptr)
	{
		
	}

	template<typename T>
	SList<T>::Iterator::Iterator(const SList&  PointerToOwner, Node * NodeToPointTo) : Owner(const_cast<SList<T>*>(&PointerToOwner)), CurrentNode(NodeToPointTo)
	{

	}
	template<typename T>
	SList<T>::Iterator::Iterator(typename const SList<T>::Iterator& Rhs)
	{
		Owner = Rhs.Owner;
		CurrentNode = Rhs.CurrentNode;
	}

	template<typename T>
	SList<T>::Iterator::Iterator(typename SList<T>::Iterator&& Rhs):Owner(Rhs.Owner),CurrentNode(Rhs.CurrentNode)
	{
		Rhs.Owner = nullptr;
		Rhs.CurrentNode = nullptr;
	}

	template<typename T>
	T& SList<T>::Iterator::operator*()
	{
		if (CurrentNode == nullptr)
		{
			throw std::runtime_error("Dereferencing Null");
		}
		return CurrentNode->Data;
	}


	template<typename T>
	const T& SList<T>::Iterator::operator*() const
	{
		if (CurrentNode == nullptr)
		{
			throw std::runtime_error("Dereferencing Null");
		}
		return const_cast<const T&>(CurrentNode->Data);
	}

	template<typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator=(const Iterator& Rhs)
	{
		if (this != &Rhs)
		{
			Owner = Rhs.Owner;
			CurrentNode = Rhs.CurrentNode;
		}
		return *this;
	}

	template<typename T>
	bool SList<T>::Iterator::operator==(const Iterator & Rhs) const
	{
		
		return ((Owner == Rhs.Owner) && (CurrentNode == Rhs.CurrentNode));
	}
	
	template<typename T>
	bool SList<T>::Iterator::operator!=(const Iterator & Rhs) const
	{

		return !(operator==(Rhs));
	}

	template<typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (CurrentNode == nullptr)
		{
			throw std::runtime_error("Exceeded List Size");
		}
		CurrentNode = CurrentNode->Next;
		return *this;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		if (CurrentNode == nullptr)
		{
			throw std::runtime_error("Exceeded List Size");
		}
		Iterator temp = *this;
		operator++();
		return temp;
	}
}