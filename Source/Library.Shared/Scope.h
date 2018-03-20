#pragma once
#include "RTTI.h"
#include "HashMap.h"
#include "Vector.h"
#include "Datum.h"
#define OUT

namespace FieaGameEngine
{
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI)

	public:
		/// <summary>
		/// Explicit constructor to iniitalize a scope with required capacity. The capacity is defaulted to 5 
		/// </summary>
		explicit Scope(const uint32_t& capacity = 5U);

		/// <summary>
		/// Copy constructor for scope
		/// </summary>
		/// <param name="rhs">Const reference to the scope to be copied from</param>
		Scope(const Scope& rhs);

		/// <summary>
		/// Move Constructor for Scope
		/// </summary>
		/// <param name="rhs">Reference to the scope after moving</param>
		Scope(Scope&& rhs);

		/// <summary>
		/// Virtual destructor for Scope for future inheritance
		/// </summary>
		virtual ~Scope();

		/// <summary>
		/// Overloaded operator=
		/// </summary>
		/// <param name="rhs">Const refrence to Scope</param>
		/// <returns></returns>
		Scope& operator=(const Scope& rhs);

		/// <summary>
		/// Move Assignment Operator for Scope
		/// </summary>
		/// <param name="rhs">The scope to be moved</param>
		/// <returns>Scope reference after moving</returns>
		Scope& operator=(Scope&& rhs);

		/// <summary>
		/// Overloaded operator ==
		/// </summary>
		/// <param name="rhs">Const reference to a scope</param>
		/// <returns>A bool representing the state of equivalence</returns>
		bool operator==(const Scope& rhs) const;

		/// <summary>
		/// Overloaded operator !=
		/// </summary>
		/// <param name="rhs">Const reference to an rhs</param>
		/// <returns>A bool representing the state of equivalence</returns>
		bool operator!=(const Scope& rhs) const;

		/// <summary>
		/// The append function adds a new string Datum pair to the Scope if the datum doesn't exist else returns a reference to the existing one
		/// </summary>
		/// <param name="stringKey">The string key</param>
		/// <returns>A reference to the Datum that was created or already existed</returns>
		Datum& Append(const std::string& stringKey);

		/// <summary>
		/// Creates a new Scope and appends it to the current scope a.k.a nesting scopes
		/// </summary>
		/// <param name="stringKey">The key under which the scope must be appended</param>
		/// <returns>A Scope reference</returns>
		Scope& AppendScope(const std::string& stringKey);

		/// <summary>
		/// Overloaded operator [] that takes a string key, functionally similar to Append
		/// </summary>
		/// <param name="stringKey">A string key</param>
		/// <returns>A datum reference to the created or existing Datum</returns>
		Datum& operator[](const std::string& stringKey);

		/// <summary>
		/// Overloaded operator[] that takes an unsigned integer
		/// </summary>
		/// <param name="index">A uint32_t representing an index</param>
		/// <returns>Returns the index'th Datum in order of insertion</returns>
		Datum& operator[](const uint32_t& index);

		/// <summary>
		/// The Adopt function adopts the given Scope under the given string to the current Scope
		/// </summary>
		/// <param name="child">The Scope to be adopted</param>
		/// <param name="name">The string key</param>
		/// <returns>A Datum reference of the Datum the Scope was inserted in</returns>
		Datum& Adopt(Scope& child, const std::string& name);

		/// <summary>
		/// Returns a pointer to the parent scope
		/// </summary>
		/// <returns>Returns a pointer to the parent scope</returns>
		Scope* GetParent() const;

		/// <summary>
		/// Find looks for the given key in the Scope and returns the datum it is bound to if found else returns nullptr
		/// </summary>
		/// <param name="stringKey">The string key to search for</param>
		/// <returns></returns>
		Datum* Find(const std::string& stringKey) const;

		/// <summary>
		/// Search looks recurisved up the hierarchy of scopes for the presence of the given key
		/// </summary>
		/// <param name="stringKey">The key to search for</param>
		/// <param name="foundScope">The immediate scope under which the key is found, Output parameter and must not be nullptr</param>
		/// <returns></returns>
		Datum* Search(const std::string& stringKey, OUT Scope** foundScope = nullptr);

		/// <summary>
		/// Finds the key of the given Scope
		/// </summary>
		/// <param name="scopeAddress">Pointer to the scope whose string key must be found</param>
		/// <returns>A string containing the name of the key of the datum</returns>
		std::string FindName(const Scope& scopeAddress) const;


		Datum* FindContainedScope(const Scope& scopeAddress, OUT uint32_t& foundIndex, OUT std::string& name) const;

		/// <summary>
		/// Returns the class name as a string
		/// </summary>
		/// <returns>A string containing the class name</returns>
		std::string ToString() const;

		/// <summary>
		/// Overloaded Equals function from RTTI, identical in functionality to overloaded ==
		/// </summary>
		/// <param name="rhs">The RTTI pointer</param>
		/// <returns>A bool representing the state of equivalence</returns>
		bool Equals(const RTTI* rhs) const;

		/// <summary>
		/// Clear clears all data that is in use by the Scope, destroys all child scopes
		/// </summary>
		void Clear();
		
		/// <summary>
		/// Orphans the given child from the Scope
		/// </summary>
		/// <param name="child">The adress of the Child to be orphaned</param>
		void Orphan(Scope *child);

	protected:
		const Vector<std::pair<std::string, Datum>*>& GetOrderedVector() const;

	private:

		HashMap<std::string, Datum> Table;
		Vector<std::pair<std::string, Datum>*> OrderedTable;
		Scope* ParentScope{ nullptr };
	};

}
