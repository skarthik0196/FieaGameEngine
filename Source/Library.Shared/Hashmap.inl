#pragma once
#include"HashMap.h"
#include <cstring>

namespace FieaGameEngine
{
	template<typename T>
	uint32_t DefaultHash<T>::operator()(const T& key) const
	{
		const char *data = reinterpret_cast<const char*>(&(key));
		uint32_t size = static_cast<uint32_t>(sizeof(T));
		return (AdditiveHash(data, size));
	}

	template<>
	inline uint32_t DefaultHash<std::string>::operator()(const std::string& key) const
	{
		return (static_cast<uint32_t>(AdditiveHash(key.c_str(), static_cast<uint32_t>(key.length()))));
	}

	inline uint32_t DefaultHash<char*>::operator()(const char* key) const
	{
		return (static_cast<uint32_t>((AdditiveHash(key, static_cast<uint32_t>(strlen(key))))));
	}

	inline uint32_t AdditiveHash(const char *data, uint32_t size)
	{
		static const uint32_t PrimeNo = 17;
		uint32_t Result = 0;

		for (uint32_t i = 0; i < size; ++i)
		{
			Result = Result + (PrimeNo * data[i]);
		}

		return Result;
	}

	template <typename T>
	bool DefaultComparator<T>::operator()(const T& data1, const T& data2) const
	{
		return (data1 == data2);
	}

	inline bool DefaultComparator<char*>::operator()(char* data1, char* data2) const
	{
		bool result;
		if (!strcmp(data1, data2))
		{
			result = true;
		}
		else
		{
			result = false;
		}

		return result;
	}
	
	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::HashMap(uint32_t bucketSize) :Buckets(bucketSize), Size(0)
	{
		if (bucketSize <= 0)
		{
			throw std::runtime_error("Invalid Size");
		}
		for (uint32_t i = 0; i < Buckets.CurrentCapacity(); ++i)
		{
			Buckets.PushBack(HashElement());
		}
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::HashMap(std::initializer_list<typename PairType> list) :Buckets(static_cast<uint32_t>(list.size())), Size(0)
	{
		for (uint32_t i = 0; i < Buckets.CurrentCapacity(); ++i)
		{
			Buckets.PushBack(HashElement());
		}
		for (auto &value : list)
		{
			Insert(value);
		}
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::HashMap(HashMap&& rhs) :Buckets(std::move(rhs.Buckets)),HashFunction(std::move(rhs.HashFunction)), ComparatorFunction(std::move(rhs.ComparatorFunction)), Size(std::move(rhs.Size))
	{
		rhs.Size = 0;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::~HashMap()
	{

	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	HashMap<TKey, TData, HashFunctor, ComparatorFunctor>& HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::operator=(HashMap&& rhs)
	{
		Buckets = std::move(rhs.Buckets);
		HashFunction = std::move(rhs.HashFunction);
		ComparatorFunction = std::move(rhs.ComparatorFunction);
		Size = rhs.Size;
		rhs.Size = 0;
		
		return *this;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	void HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Clear()
	{
		Vector<HashElement>::Iterator It = Buckets.begin();
		for (auto &lists : Buckets)
		{
			lists.Clear();
		}
		Size = 0;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::Iterator() :Owner(nullptr), BucketIndex(0), SListIterator(HashElement::Iterator())
	{

	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::Iterator(const HashMap& owner, uint32_t index, typename const HashElement::Iterator& ListIterator) : Owner(&owner), BucketIndex(index), SListIterator(ListIterator)
	{

	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Insert(const PairType& data)
	{
		bool IsInserted;
		return (Insert(data, IsInserted));
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Insert(const PairType& data, OUT bool& isInserted)
	{
		HashElement::Iterator listIt;
		uint32_t bucketIndex;
		Iterator It = Find(data.first , bucketIndex);

		if (It.BucketIndex >= Buckets.Length())
		{
			
			isInserted = true;
			listIt = Buckets[bucketIndex].PushBack(data);
			It.SListIterator = listIt;
			It.BucketIndex = bucketIndex;
			++Size;
		}
		else
		{
			isInserted = false;
		}

		return It;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Find(const TKey& key) const
	{
		uint32_t bucketIndex;
		
		return (Find(key, bucketIndex));
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Find(const TKey& key, uint32_t& hashIndex) const
	{
		uint32_t bucketIndex = (HashFunction(key)) % Buckets.Length();
		hashIndex = bucketIndex;
		HashElement::Iterator listIt;

		if (Buckets[bucketIndex].IsEmpty() == false)
		{
			listIt = Buckets[bucketIndex].begin();
			for (; listIt != Buckets[bucketIndex].end(); ++listIt)
			{
				if (ComparatorFunction((*listIt).first, key))
				{
					break;
				}
			}
			if (listIt == Buckets[bucketIndex].end())
			{
				listIt = Buckets[Buckets.Length() - 1].end();
				bucketIndex = Buckets.Length();
			}
		}
		else
		{
			listIt = Buckets[Buckets.Length() - 1].end();
			bucketIndex = Buckets.Length();
		}

		Iterator It(*this, bucketIndex, listIt);
		return It;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename TData& HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::operator[](const TKey& key)
	{
		return ((*(Insert(std::make_pair(key, TData())))).second);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	uint32_t HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Length() const
	{
		return Size;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	bool HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Remove(const TKey & key)
	{
		bool Result = false;
		Iterator It = Find(key);
		if (It.BucketIndex < Buckets.Length())
		{
			Buckets[It.BucketIndex].Remove(It.SListIterator);
			Result = true;
			--Size;
		}
		return Result;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	bool HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::ContainsKey(const TKey & key) const
	{
		TData data;
		return(ContainsKey(key,data));
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	bool HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::ContainsKey(const TKey & key, OUT TData& data)
	{
		bool result = false;
		Iterator It = Find(key);
		if (It.BucketIndex < Buckets.Length())
		{
			data = (*It).second;
			result = true;
		}
		else
		{
			result = false;
		}
		return result;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	bool HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::ContainsKey(const TKey & key, OUT TData& data) const
	{
		bool result = false;
		Iterator It = Find(key);
		if (It.BucketIndex < Buckets.Length())
		{
			data = (*It).second;
			result = true;
		}
		else
		{
			result = false;
		}
		return result;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	TData& HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::At(const TKey & key)
	{
		Iterator It = Find(key);
		if (It.BucketIndex >= Buckets.Length())
		{
			throw std::runtime_error("Key does not exist");
		}
		return ((*(It.SListIterator)).second);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	const TData& HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::At(const TKey & key) const
	{
		Iterator It = Find(key);
		if (It.BucketIndex >= Buckets.Length())
		{
			throw std::runtime_error("Key does not exist");
		}
		return ((*(It.SListIterator)).second);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::begin() const
	{
		
		Iterator It(*this, 0, Buckets[0].begin());
		while ((It.BucketIndex < Buckets.Length()) && (Buckets[It.BucketIndex].IsEmpty() == true))
		{
			It.BucketIndex++;
		}
		if (It.BucketIndex < Buckets.Length())
		{
			It.SListIterator = Buckets[It.BucketIndex].begin();
		}
		else
		{
			It.SListIterator = Buckets[Buckets.Length() - 1].begin();
		}

		return It;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::end() const
	{
		return Iterator(*this, Buckets.Length(), Buckets[Buckets.Length() - 1].end());
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::PairType& HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::operator*()
	{
		if (Owner == nullptr || (BucketIndex >= Owner->Buckets.Length()))
		{
			throw std::runtime_error("Invalid Iterator");
		}
		return *(SListIterator);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::PairType* HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::operator->()
	{
		if (Owner == nullptr || (BucketIndex >= Owner->Buckets.Length()))
		{
			throw std::runtime_error("Invalid Iterator");
		}
		return &(*(SListIterator));
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	bool HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::operator==(const Iterator& otherIterator)
	{
		return((Owner == otherIterator.Owner) && (BucketIndex == otherIterator.BucketIndex) && (SListIterator == otherIterator.SListIterator));
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	bool HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::operator!=(const Iterator& otherIterator)
	{
		return(!(operator==(otherIterator)));
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator& HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::operator++()
	{
		if ((Owner == nullptr) ||( BucketIndex >= Owner->Buckets.Length()))
		{
			throw std::runtime_error("Invalid Iterator");
		}

		SListIterator++;
		if (SListIterator == Owner->Buckets[BucketIndex].end())
		{
			if (BucketIndex < (Owner->Buckets.Length()-1))
			{
				BucketIndex++;
				while ((BucketIndex < (Owner->Buckets.Length() - 1)) && (Owner->Buckets[BucketIndex].IsEmpty() == true))
				{
					BucketIndex++;
				}
				SListIterator = Owner->Buckets[BucketIndex].begin();
			}
		}	
		if (SListIterator == ((Owner->Buckets[(Owner->Buckets.Length() - 1)]).end()))
		{
			BucketIndex = Owner->Buckets.Length();
		}
		return *this;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename ComparatorFunctor>
	typename HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator HashMap<TKey, TData, HashFunctor, ComparatorFunctor>::Iterator::operator++(int)
	{
		if (Owner == nullptr)
		{
			throw std::runtime_error("Invalid Iterator");
		}
		Iterator temp = *this;
		operator++();
		return temp;
	}
}