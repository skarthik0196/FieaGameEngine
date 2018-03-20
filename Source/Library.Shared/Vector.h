#pragma once
#include<memory>
#include<functional>
#include<stdlib.h>

//Implement Resize
namespace FieaGameEngine
{
	/// <summary>
	/// Base Functor class for the default increment strategy
	/// </summary>
	template<typename T>
	class BaseIncrementFunctor
	{
	public:
		BaseIncrementFunctor() = default;
		BaseIncrementFunctor(const BaseIncrementFunctor&) = default;
		
		/// <summary>
		/// operator() overload (functor)
		/// The functions returns an unsigned integers that determines how much more capcity needs to be added to the vector
		/// For example, if the current vector capacity was 2 and the Functor returned 2, the new vector capacity would be 4
		/// </summary>
		/// <param name="size">Current Size of the Vector</param>
		/// <param name="capacity">Current Capacity of the Vector</param>
		/// <returns>An Unsigned integer dictating how much more capacity needs to be added to the vector</returns>
		uint32_t operator()(uint32_t size, uint32_t capacity) const;

		~BaseIncrementFunctor() = default;
	};


	/// <summary>
	/// Templated Vector Class that represents a Vector container
	/// The user can specify their own increment strategy as an additional template parameter which is used as a functor
	/// The functor in question must return an unsigned integer
	/// </summary>
	template<typename T, typename IncrementFunctor = BaseIncrementFunctor<int>>
	class Vector
	{
	public:
		/// <summary>
		/// Public Iterator class used to iterate through the elements of the array
		/// </summary>
		class Iterator
		{
			friend class Vector;
		public:
			/// <summary>
			/// Default constructor of Iterator
			/// </summary>
			Iterator();

			/// <summary>
			/// Copy constructor of Iterator
			/// </summary>
			/// <param name="otherIterator">Const reference to the Iterator to be copied from</param>
			Iterator(const Iterator& otherIterator) = default;

			/// <summary>
			/// Overloaded * operator to dereference the Iterator
			/// </summary>
			/// <returns>A T&</returns>
			T& operator*();

			/// <summary>
			/// Const version of operator *
			/// </summary>
			/// <returns>A const T&</returns>
			const T& operator*() const;

			/// <summary>
			/// Overloaded prefix operator ++
			/// </summary>
			/// <returns>A Iterator& of the incremented operator</returns>
			Iterator& operator++();

			/// <summary>
			/// Overloaded postfix operator ++
			/// </summary>
			/// <param name="">N/A</param>
			/// <returns>The pre incremented state of the iterator</returns>
			Iterator operator++(int);

			/// <summary>
			/// Overloaded Equality operator
			/// </summary>
			/// <param name="Rhs">Const reference to the other Iterator</param>
			/// <returns>A bool signifying the result of the equality check</returns>
			bool operator==(const Iterator& Rhs );

			/// <summary>
			/// Overloaded Inequality operator
			/// </summary>
			/// <param name="Rhs">Const reference to the other Iterator</param>
			/// <returns>A bool signifying the result of the inequality check</returns>
			bool operator!=(const Iterator& Rhs);

			/// <summary>
			/// Overloaded Assignement oeprator
			/// </summary>
			/// <param name="Rhs">Copies the data from the given Iterator</param>
			/// <returns>The Iterator after copying the values</returns>
			Iterator& operator=(const Iterator& Rhs);
		private:
			/// <summary>
			/// Private  parameterized constructor
			/// </summary>
			/// <param name="index">Index to be initialized with</param>
			/// <param name="owner">Const reference to the owner of the Vector that the Iterator is referring to</param>
			Iterator( const uint32_t& index, const Vector<T, IncrementFunctor>& owner);

			const Vector<T, IncrementFunctor> *Owner;
			uint32_t Index;
		};
		/// <summary>
		/// Default constructor for the Vector with an optional parameter for the capacity
		/// </summary>
		/// <param name="capacity">Optional argument determining the intial capacity of the Vector</param>
		explicit Vector(const uint32_t& capacity = 1);

		/// <summary>
		/// Destructor
		/// </summary>
		~Vector();

		/// <summary>
		/// Copy Constructor for the Vector
		/// </summary>
		/// <param name="otherVector"></param>
		Vector(const Vector& otherVector);

		Vector(Vector&& otherVector);

		/// <summary>
		/// Overloaded Operator assignment to copy data from another Vector
		/// </summary>
		/// <param name="otherVector">Const reference to the other vector</param>
		/// <returns>A reference to the vector after copying</returns>
		Vector<T, IncrementFunctor>& operator=(const Vector<T, IncrementFunctor>& otherVector);

		Vector<T, IncrementFunctor>& operator=(Vector<T, IncrementFunctor>&& otherVector);

		/// <summary>
		/// Overloaded operator []
		/// </summary>
		/// <param name="index">The index</param>
		/// <returns>A T& to the data stored at that index</returns>
		T& operator[](const uint32_t& index);

		/// <summary>
		/// Const version of operator []
		/// </summary>
		/// <param name="index">Index</param>
		/// <returns>A const T&</returns>
		const T& operator[](const uint32_t& index) const;

		/// <summary>
		/// Front returns the first element in the Vector
		/// </summary>
		/// <returns>A T& to the first element in the Vector</returns>
		T& Front();

		/// <summary>
		/// Const version of Front()
		/// </summary>
		/// <returns>A const T& reference to the first element in the vector</returns>
		const T& Front() const;

		/// <summary>
		/// Back returns the last element in the Vector
		/// </summary>
		/// <returns>A T& to the last element in the Vector</returns>
		T& Back();

		/// <summary>
		/// const version of Back()
		/// </summary>
		/// <returns>A const T& to the last element in the Vector</returns>
		const T& Back() const;

		/// <summary>
		/// The functional equivalent of [] that returns the value stored at the given index
		/// </summary>
		/// <param name="index">Index</param>
		/// <returns>A T& to the value stored at the given index</returns>
		T& At(const uint32_t& index);

		/// <summary>
		/// Const version of At()
		/// </summary>
		/// <param name="index">Index</param>
		/// <returns>A const T& to the value stored at the given index</returns>
		const T& At(const uint32_t& index) const;

		/// <summary>
		/// Checks if the Vector is empty
		/// </summary>
		/// <returns>A Bool determining if the Vector is empty or not</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Clears all Data from the Vector, Sets the Size to 0 but does not affect capacity
		/// </summary>
		void Clear();

		/// <summary>
		/// Reserves more space for the vector equal to the amount of capcity provided
		/// </summary>
		/// <param name="capacity">The new capacity amount</param>
		void Reserve(const uint32_t& capacity);

		/// <summary>
		/// Shrinks the vector to make the Capacity equal to Size
		/// </summary>
		void Shrink();

		void Resize(const uint32_t& size);

		/// <summary>
		/// Removes the element from the specified index of the Vector
		/// </summary>
		/// <param name="index">The index of the element to be removed</param>
		void Remove(const uint32_t& index);

		/// <summary>
		/// Overload for remove which removes a range of values (inclusive)
		/// </summary>
		/// <param name="StartPoint">Start Iterator for Remove</param>
		/// <param name="endPoint">End Iterator for Remove</param>
		void Remove(const Iterator& startPoint, const Iterator& endPoint);

		/// <summary>
		/// Pushback adds an element into the Vector
		/// </summary>
		/// <param name="data">A const T& to the data to be added into the Vector</param>
		void PushBack(const T& data);

		/// <summary>
		/// Popback removes the last element from the Vector 
		/// </summary>
		void PopBack();

		/// <summary>
		/// Returns the Size of the vector
		/// </summary>
		/// <returns>An unsigned integer specifying the current size of the Vector</returns>
		uint32_t Length() const;

		/// <summary>
		/// Returns the current Capacity of the vector
		/// </summary>
		/// <returns>An unsigned integer specifying the current capacity of the vector</returns>
		uint32_t CurrentCapacity() const;

		/// <summary>
		/// Returns an iterator pointing to the first element in the Vector
		/// </summary>
		/// <returns>An Iterator pointing to the first element in the Vector</returns>
		Iterator begin() const;

		/// <summary>
		/// Returns an Iterator pointing beyond the last element of the array
		/// </summary>
		/// <returns>Returns an Iterator pointing beyond the last element of the array</returns>
		Iterator end() const;

		/// <summary>
		/// Returns an Iterator pointing to the element if it was found, else returns end
		/// </summary>
		/// <param name="Item"></param>
		/// <returns>Returns an Iterator pointing to the element if it was found, else returns end</returns>
		Iterator Find(const T& item) const;

	private:
		uint32_t Size;
		uint32_t Capacity;
		T* Data;
		IncrementFunctor IncrementFunction;
	};
}
#include"Vector.inl"
