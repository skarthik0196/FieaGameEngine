#pragma once
#include "AbstractFactory.h"

/// <summary>
/// The following Macro creates a Concrete Factory of the passed in type
/// </summary>
#define CreateConcreteFactory(ConcreteType, AbstractType)\
new ConcreteFactory<ConcreteType, AbstractType>(#ConcreteType);

namespace FieaGameEngine
{
	/// <summary>
	/// Templated Concrete Factory Class
	/// </summary>
	template<typename T, typename U>
	class ConcreteFactory : public AbstractFactory<U>
	{
	public:
		/// <summary>
		/// Explicit constructor that takes the name of the object to be created as a parameter
		/// </summary>
		/// <param name="name"></param>
		explicit ConcreteFactory(const std::string& name) : Name(name)
		{
			AbstractFactory<U>::AddFactory(name, *this);
		}

		/// <summary>
		/// Defaulted Destructor
		/// </summary>
		~ConcreteFactory() = default;

		/// <summary>
		/// Deleted Copy Constructor and copy assignment
		/// </summary>
		/// <param name="rhs"></param>
		ConcreteFactory(const ConcreteFactory& rhs) = delete;
		ConcreteFactory& operator=(const ConcreteFactory& rhs) = delete;

		/// <summary>
		/// Defaulted Move Constructor and Move Assignment
		/// </summary>
		/// <param name="rhs"></param>
		ConcreteFactory(ConcreteFactory&& rhs) = default;
		ConcreteFactory& operator=(ConcreteFactory&& rhs) = default;


		/// <summary>
		/// Virtual Overloaded Create Function that creates the object of specified type
		/// </summary>
		/// <returns>A pointer to the object of the given type</returns>
		U* Create()
		{
			return new T();
		}

		/// <summary>
		/// Virtual overloaded Classname function
		/// </summary>
		/// <returns>String representing the class name</returns>
		const std::string& ClassName()
		{
			return Name;
		}

	private:
		std::string Name;
	};
}
