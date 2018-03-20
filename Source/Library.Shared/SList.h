#pragma once

namespace FieaGameEngine
{
	/// <summary>
	/// The Class Slist is a templated class that represents a Linked List Data Structure
	/// SList contains an internal class called Node which forms the basis of the linked list
	/// </summary>
	template <typename T>
	class SList
	{
	public:
		/// <summary>
		///  The internal node class that makes up the linked list
		///  class is not exposed to the user and is hidden inside the SList
		/// </summary>
		class Node
		{
			friend class SList;
		public:
			/// <summary>
			/// Parameterized Constructor for node to initialize the data members with values
			/// </summary>
			/// <param name="Value">Const reference to T value</param>
			/// <param name="NextNode">Pointer to the next node in the linked list, has a default value of nullptr</param>
			Node(const T& Value, Node* NextNode = nullptr);

			/// <summary>
			/// Destructor for Node
			/// </summary>
			~Node() = default;
		private:
			T Data;					//!< Contains the actual data member of the templated type
			Node *Next;				//!< Stores a pointer to the next node in the list
		};
	public:
		/// <summary>
		/// The Iterator Class for SList used to iterate over the elements of the list
		/// </summary>
		class Iterator
		{
			friend class SList;		
		public:
			/// <summary>
			/// Default constructor
			/// </summary>
			Iterator();

			/// <summary>
			/// Copy Constructor for the Iterator
			/// </summary>
			/// <param name="Rhs">Const reference to the other Iterator</param>
			Iterator(const Iterator &Rhs);

			/// <summary>
			/// Move Constructor for the Iterator
			/// </summary>
			/// <param name="Rhs">The Iterator who's data needs to be moved</param>
			Iterator(Iterator &&Rhs);

			/// <summary>
			/// Overloaded * operator used to obtain the data of the list element the iterator is pointing to
			/// </summary>
			/// <returns>Returns a reference to the data</returns>
			T& operator*();

			/// <summary>
			/// Const version of the * overload
			/// </summary>
			/// <returns>Returns a const reference to the data</returns>
			const T& operator*() const;

			/// <summary>
			/// Overloaded assignment operator
			/// </summary>
			/// <param name="Rhs">const reference to the other Iterator</param>
			/// <returns>Returns a reference to the iterator after copyign data</returns>
			Iterator& operator=(const Iterator &Rhs);

			/// <summary>
			/// Overloaded == operator for checking equality between operators
			/// </summary>
			/// <param name="Rhs">Const refernce to the other iterator</param>
			/// <returns>A boolean representing whether the two iterators were equal</returns>
			bool operator==(const Iterator &Rhs) const;

			/// <summary>
			/// Overloaded != operator for checking inequality between operators
			/// </summary>
			/// <param name="Rhs">Const reference to the other iterator</param>
			/// <returns>A boolean represnting whether the two iterators are inequal</returns>
			bool operator!=(const Iterator &Rhs) const;

			/// <summary>
			/// Overloaded Pre-increment operator
			/// </summary>
			/// <returns>Incremented iterator</returns>
			Iterator& operator++();

			/// <summary>
			/// Overloaded Post-increment operator
			/// </summary>
			/// <param name="">N/A</param>
			/// <returns>Returns the temporary iterator before it was incremented</returns>
			Iterator operator++(int);
			
		private:
			Iterator(const SList& PointerToOwner, Node *NodeToPointTo);
			Node *CurrentNode;
			SList *Owner;
		};
		/// <summary>
		/// Default Constructor for SList that initializes FrontPointer and BackPointer to nullptr and also Size to 0
		/// </summary>
		SList();

		/// <summary>
		/// Destructor for the SList test
		/// </summary>
		~SList();

		/// <summary>
		/// Copy Constructor for SList with Deep Copy semantics
		/// </summary>
		/// <param name="OtherList">Const reference to the other list</param>
		SList(const SList<T> &OtherList);

		SList(SList<T>&& OtherList);

		/// <summary>
		///  Front returns a T& reference to the Data present in the node pointed to by the FrontPointer
		/// </summary>
		/// <returns>Returns a reference to the data pointer to by Front Pointer</returns>
		T& Front();

		/// <summary>
		///	Back returns a T& reference to the Data present in the node pointed to by the BackPointer 
		/// </summary>
		/// <returns>Returns a reference to the data pointed to by Back Pointer</returns>
		T& Back();

		/// <summary>
		/// Const version of Front()
		/// </summary>
		/// <returns>Returns a Const reference to the data pointed to by Front Pointer</returns>
		const T& Front() const;

		/// <summary>
		/// Const version of Back()
		/// </summary>
		/// <returns>Returns a Const reference to the data pointed to by Back Pointer</returns>
		const T& Back() const;

		/// All Push and Pop operations adjust the linked list accordingly after performing their respective operations

		/// <summary>
		/// PushBack inserts a noce to the end of the linked list
		/// </summary>
		/// <param name="Data">Const reference to the data to be inserted</param>
		Iterator PushBack(const T& Data);

		/// <summary>
		/// PushFront inserts a node to the beginning of the linked list
		/// </summary>
		/// <param name="Data">Const reference to the data to be inserted</param>
		void PushFront(const T& Data);

		/// <summary>
		/// PopFront deletes the node present in the beginning of the linked list
		/// </summary>
		void PopFront();

		/// <summary>
		/// PopBack deletes the node at the end of the list
		/// </summary>
		void PopBack();

		/// <summary>
		/// Clear deletes all the nodes in the linked list and reset the FrontPointer and BackPointer to nullptr and also resets Size to 0
		/// </summary>
		void Clear();

		/// <summary>
		/// Returns a boolean that signifies whether the list is empty or not
		/// </summary>
		/// <returns>A Boolean designating whether the list is empty or not</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Length returns the size of the linked lsit
		/// </summary>
		/// <returns>Returns the length of the linked list</returns>
		int Length() const;

		/// <summary>
		/// Searches the list for the first instance of the given item and removes it if present.
		/// </summary>
		/// <param name="Item">Const reference to the value of the element that needs to be deleted</param>
		/// <returns>Returns a boolean represnting whether the element was found and deleted</returns>
		bool Remove(const T& Item);

		void Remove(Iterator& It);

		/// <summary>
		/// Searches the list for all instances of the given item and removes them if present
		/// </summary>
		/// <param name="Item">Const reference to the value of the element that needs to be deleted</param>
		/// <returns>A boolean representing whether the elements were found and deleted, If at least one instance was deleted, this will return true</returns>
		bool RemoveAll(const T& Item);

		/// <summary>
		/// Returns an iterator pointing to the beginning of the list
		/// </summary>
		/// <returns>Returns an iterator pointing to the beginning of the list</returns>
		Iterator begin();

		/// <summary>
		/// Returns an iterator pointing beyond the end of the list
		/// </summary>
		/// <returns>Returns an iterator pointing beyond the end of the list</returns>
		Iterator end();

		/// <summary>
		/// Searches the list to find whether the given element was present and returns an iterator to the element if it was present
		/// </summary>
		/// <param name="Value">Value of the element to be found</param>
		/// <returns>Iterator pointing to the element if present, nullptr otherwise</returns>
		Iterator Find(const T& Value) const;

		/// <summary>
		/// Inserts an element after a given iterator in the list
		/// </summary>
		/// <param name="It">The iterator after which the node needs to be inserted</param>
		/// <param name="Value">The value to be inserted</param>
		void InsertAfter(Iterator& It, const T& Value);
		
		/// <summary>
		/// Const version of begin
		/// </summary>
		/// <returns>Const Iterator to the beginning of the list</returns>
		const Iterator begin() const;

		/// <summary>
		/// Const version of end
		/// </summary>
		/// <returns>Const Iterator to the end of the list</returns>
		const Iterator end() const;

		/// <summary>
		/// Overloaded Operator= with deep copy semantics, also checks for self assignment.
		/// </summary>
		/// <param name="Rhs">Const SList reference to the other list</param>
		/// <returns>SList refernce</returns>
		SList<T>& operator=(const SList<T> &Rhs);

		SList<T>& operator=(SList<T> &&Rhs);

	private:

		Node *FrontPointer; /// Pointer to the beginning of the linked list
		Node *BackPointer;	/// Pointer to the end of the linked list
		unsigned int Size;			/// Size/Length of the linked list

	};
}

#include "Slist.inl"