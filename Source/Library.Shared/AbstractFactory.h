#pragma once
#include<string>
#include"HashMap.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Templated Abstract Factory class
	/// </summary>
	template<typename T>
	class AbstractFactory
	{
	public:
		/// <summary>
		/// Defaulted Constructor
		/// </summary>
		AbstractFactory() = default;

		/// <summary>
		/// Defaulted Copy Constructor
		/// </summary>
		/// <param name="rhs"></param>
		AbstractFactory(const AbstractFactory& rhs) = default;

		/// <summary>
		/// Defaulted Move Constructor
		/// </summary>
		/// <param name="rhs"></param>
		AbstractFactory(AbstractFactory&& rhs) = default;

		/// <summary>
		/// Defaulted Virtual Destructor
		/// </summary>
		virtual ~AbstractFactory() = default;

		/// <summary>
		/// Defaulted Copy Assignment Operator
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		AbstractFactory& operator=(const AbstractFactory& rhs) = default;

		/// <summary>
		/// Defaulted Move Assignment Operator
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		AbstractFactory& operator=(AbstractFactory&& rhs) = default;

		/// <summary>
		/// Static Function to add a given Concrete Factory into the FactoryList
		/// </summary>
		/// <param name="name">Name of the Concrete Factory</param>
		/// <param name="concreteFactory">Reference to a Concrete Factory</param>
		static void AddFactory(const std::string& name, AbstractFactory& concreteFactory);

		/// <summary>
		/// Static function to remove a Concrete Factory from the Factory List
		/// </summary>
		/// <param name="concreteFactory">A reference to a Concrete Factory</param>
		static void RemoveFactory(AbstractFactory& concreteFactory);

		/// <summary>
		/// Returns an Iterator to the beginning of the Factory List
		/// </summary>
		/// <returns>A HashMap Iterator</returns>
		static typename HashMap<std::string, AbstractFactory<T>*>::Iterator begin();

		/// <summary>
		/// Returns an Iterator the the end of the Factory List
		/// </summary>
		/// <returns>A Hashmap Iterator</returns>
		static typename HashMap<std::string, AbstractFactory<T>*>::Iterator end();

		/// <summary>
		/// Finds a given AbstractFactory in the factory list and returns a pointer
		/// </summary>
		/// <param name="name">The name of the associated Factory</param>
		/// <returns>An Abstract Factory pointer</returns>
		static AbstractFactory* Find(const std::string& name);

		/// <summary>
		/// Static function to create an Object of the type named in the string
		/// </summary>
		/// <param name="name">String representing the name of the object to be created</param>
		/// <returns>A T* pointer to the object to be created</returns>
		static T* Create(const std::string& name);

		/// <summary>
		/// Clears all Factories in the list, Note that this must be called to prevent Memory leaks if the factory is no longer required
		/// </summary>
		static void ClearFactories();

		/// <summary>
		/// Virtual Pure Function to create an object of said type
		/// </summary>
		/// <returns>A T* pointer representing the object to be created</returns>
		virtual T* Create() = 0;

		/// <summary>
		/// Virtual Pure Function that returns the name of the object that can be created by the factory
		/// </summary>
		/// <returns>A string containing the name of the class</returns>
		virtual const std::string& ClassName() = 0;

	private:
		static HashMap<std::string, AbstractFactory<T>*> ConcreteFactories;
	};
}

#include"AbstractFactory.inl"
