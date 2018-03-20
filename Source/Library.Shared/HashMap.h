#pragma once
#include<memory>
#include<functional>
#include<utility>
#include"Vector.h"
#include"SList.h"

//Functions that have output parameters have their output parameters marked with OUT
#define OUT


namespace FieaGameEngine
{
	/// <summary>
	/// Additive Hash function that adds each byte of the data and multiplies by a prime number
	/// </summary>
	uint32_t AdditiveHash(const char *data, uint32_t size);

	/// <summary>
	/// Default Hash Functor class
	/// </summary>
	template<typename T>
	class DefaultHash
	{
	public:
		DefaultHash() = default;
		~DefaultHash() = default;
		DefaultHash(const DefaultHash&) = default;	

		/// <summary>
		/// Functor that returns the computed Hash value from the additive hash function
		/// </summary>
		/// <param name="key">Const refrence to Key</param>
		/// <returns>Returns the computed hash value</returns>
		uint32_t operator()(const T& key) const;

	};

	/// <summary>
	/// Specialized char* Functor class
	/// </summary>
	template<>
	class DefaultHash<char*>
	{
	public:
		/// <summary>
		/// Functor that returns the computed Hash value from the additive hash function
		/// </summary>
		/// <param name="key">char* key</param>
		/// <returns>Computed hash value</returns>
		uint32_t operator()(const char* key) const;
	};

	/// <summary>
	/// Default Comparator Class
	/// </summary>
	template<typename T>
	class DefaultComparator
	{
	public:
		DefaultComparator() = default;
		~DefaultComparator() = default;
		DefaultComparator(const DefaultComparator&) = default;

		/// <summary>
		/// Default equality functor
		/// </summary>
		/// <param name="data1">Reference to the first data</param>
		/// <param name="data2">Reference to the second data</param>
		/// <returns>A bool representing if the two data items were equal</returns>
		bool operator()(const T& data1, const T& data2) const;
	};

	/// <summary>
	/// Specialized Comparator class for char*
	/// </summary>
	template<>
	class DefaultComparator<char*>
	{
	public:
		/// <summary>
		/// Specialized functor for char* comparisons
		/// </summary>
		/// <param name="data1">First char*</param>
		/// <param name="data2">Second char*</param>
		/// <returns>A bool representing if the two char* were equal</returns>
		bool operator()(char* data1, char* data2) const;
	};

	/// <summary>
	/// Templated Hashmap class with defaulted Hashfunctor and ComparatorFunctor arguments
	/// The user can supply their own HashFunctor and ComparatorFunctor for the HashMap and are recommended to do so if they use their own classes as keys
	/// </summary>
	template<typename TKey, typename TData, typename HashFunctor = DefaultHash<TKey>, typename ComparatorFunctor = DefaultComparator<TKey>>
	class HashMap
	{
	private:
		typedef std::pair<TKey, TData> PairType;
		typedef SList<PairType> HashElement;

	public:
		/// <summary>
		/// The HashMap iterator class
		/// </summary>
		class Iterator
		{
			friend class HashMap;
		public:
			/// <summary>
			/// Default constructor for Iterator
			/// </summary>
			Iterator();
			/// <summary>
			/// Copy Constructor for Iterator that's been defaulted
			/// </summary>
			/// <param name="otherIterator">const reference to the other Iterator</param>
			Iterator(const Iterator& otherIterator) = default;

			/// <summary>
			/// Destructor for the Iterator
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// Overloaded Operator*
			/// </summary>
			/// <returns>Returns the Key,Value pair that the Iterator is pointing too</returns>
			PairType& operator*();
			
			/// <summary>
			/// Overloaded Operator->
			/// </summary>
			/// <returns>Returns a point to the Key,Value pair that the Iterator is pointing to</returns>
			PairType* operator->();

			/// <summary>
			/// Overloaded operator==
			/// </summary>
			/// <param name="otherIterator">A const reference to the Other Iterator</param>
			/// <returns>A bool representing if the two Iterators are equal or not</returns>
			bool operator==(const Iterator& otherIterator);

			/// <summary>
			/// Overloaded operator!=
			/// </summary>
			/// <param name="otherIterator">A const reference to the Other Iterator</param>
			/// <returns>A bool representing if the two Iterators are unequal</returns>
			bool operator!=(const Iterator& otherIterator);

			/// <summary>
			/// Overloaded operator++ prefix
			/// </summary>
			/// <returns>Incremented Iterator</returns>
			Iterator& operator++();

			/// <summary>
			/// Overloaded Iterator postfix
			/// </summary>
			/// <param name="">N/A</param>
			/// <returns>Iterators before increment</returns>
			Iterator operator++(int);
		private:
			Iterator(const HashMap& owner, uint32_t index, typename const HashElement::Iterator& ListIterator);

			const HashMap* Owner;
			uint32_t BucketIndex;
			typename HashElement::Iterator SListIterator;
		};

		/// <summary>
		/// Explicit Constructor that takes the HashMap buckets size as a default argument, has a default bucket size of 10
		/// </summary>
		/// <param name="bucketSize">Total number of buckets in the HashMap</param>
		explicit HashMap(uint32_t bucketSize = 10);

		HashMap(const HashMap& rhs) = default;

		HashMap(HashMap&& rhs);
		HashMap& operator=(HashMap&& rhs);

		/// <summary>
		/// Constructor with an Initializer List as an Input paramter
		/// </summary>
		/// <param name="list">Initializer list of pair types<TKey,TData></param>
		HashMap(std::initializer_list<PairType> list);

		/// <summary>
		/// HashMap Destructor
		/// </summary>
		~HashMap();

		/// <summary>
		/// The Clear function that removes all elements in the HashMap
		/// </summary>
		void Clear();

		/// <summary>
		/// Insert adds a Key,Value pair into the HashMap, If the key already exists then Insert will not overwrite the value present in the Map
		/// </summary>
		/// <param name="data">The Key,Value pair</param>
		/// <returns>An Iterator pointing the the Key Value pair that was inserted into the HashMap or the Key, Value pair that already existed in the HashMap</returns>
		Iterator Insert(const PairType& data);

		/// <summary>
		/// Insert adds a Key,Value pair into the HashMap, If the key already exists then Insert will not overwrite the value present in the Map
		/// </summary>
		/// <param name="data">The Key,Value pair</param>
		/// <param name="isInserted">An Output bool parameter that will be set if the value was inserted successfully</param>
		/// <returns>An Iterator pointing the the Key Value pair that was inserted into the HashMap or the Key, Value pair that already existed in the HashMap </returns>
		Iterator Insert(const PairType& data, OUT bool& isInserted);

		/// <summary>
		/// Checks if the given key is already present in the HashMap and returns an Iterator to it if present, else returns end
		/// </summary>
		/// <param name="key">A const reference to the key that needs to be searched for</param>
		/// <returns>An Iterator pointing to the KeyValue pair if the key was present, end otherwise</returns>
		Iterator Find(const TKey& key) const;

		/// <summary>
		/// Overloaded [] operator, will insert a new Key into the HashMap if the key provided does not already exist
		/// </summary>
		/// <param name="key">Const reference to the Key</param>
		/// <returns>The Value located at the Key</returns>
		TData& operator[](const TKey& key);

		/// <summary>
		/// The number of elements present in the HashMap
		/// </summary>
		/// <returns>An unsigned integer representing the number of elements present in the HashMap</returns>
		uint32_t Length() const;

		/// <summary>
		/// Removes the key value pair associate with the given the key in the HashMap
		/// </summary>
		/// <param name="key">A const reference to the key</param>
		/// <returns>A boolean represnting whether the key was removed or not</returns>
		bool Remove(const TKey& key);

		/// <summary>
		/// Returns whether the given key is present in the HashMap
		/// </summary>
		/// <param name="key">A const reference to the key</param>
		/// <returns>A boolean representing whether the key exists in the HashMap</returns>
		bool ContainsKey(const TKey& key) const;

		/// <summary>
		/// Returns whether the given key is present in the HashMap
		/// </summary>
		/// <param name="key">A const reference to the key</param>
		/// <param name="data">An output parameter that returns the data present at the key location if it was found</param>
		/// <returns>A boolean representing whether the key exists in the HashMap</returns>
		bool ContainsKey(const TKey& key, OUT TData& data);

		/// <summary>
		/// Const version of Contains Key with an output parameter
		/// </summary>
		/// <param name="key">A const reference to the key</param>
		/// <param name="data">An output parameter that returns the data present at the key location if it was found</param>
		/// <returns>A boolean representing whether the key exists in the HashMap</returns>
		bool ContainsKey(const TKey& key, OUT TData& data) const;

		/// <summary>
		/// Returns a reference to the data located at the given Key location, will throw an exception if the key does not exist
		/// </summary>
		/// <param name="key">Const reference to the key</param>
		/// <returns>A reference to the value located at the key location</returns>
		TData& At(const TKey& key);

		/// <summary>
		/// Returns a reference to the data located at the given Key location, will throw an exception if the key does not exist
		/// </summary>
		/// <param name="key">Const reference to the key</param>
		/// <returns>A reference to the value located at the key location</returns>
		const TData& At(const TKey& key) const;

		/// <summary>
		/// Returns an Iterator the first element of the HashMap
		/// </summary>
		/// <returns>Returns an Iterator the first element of the HashMap</returns>
		Iterator begin() const;

		/// <summary>
		/// Returns an Iterator pointing to the end of the HashMap
		/// </summary>
		/// <returns>Returns an Iterator pointing to the end of the HashMap</returns>
		Iterator end() const;


	private:
		Iterator Find(const TKey& key, uint32_t& bucketIndex) const;

		Vector<HashElement> Buckets;
		HashFunctor HashFunction;
		ComparatorFunctor ComparatorFunction;
		uint32_t Size;
	};
}
#include"Hashmap.inl"