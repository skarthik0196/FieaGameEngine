#include "AbstractFactory.h"

namespace FieaGameEngine
{
	template<typename T>
	void AbstractFactory<T>::AddFactory(const std::string& name, AbstractFactory& concreteFactory)
	{
		ConcreteFactories.Insert(std::make_pair(name, &concreteFactory));
	}

	template<typename T>
	void AbstractFactory<T>::RemoveFactory(AbstractFactory& concreteFactory)
	{
		ConcreteFactories.Remove(concreteFactory.ClassName());
	}

	template<typename T>
	typename HashMap<std::string, AbstractFactory<T>*>::Iterator AbstractFactory<T>::begin()
	{
		return ConcreteFactories.begin();
	}

	template<typename T>
	typename HashMap<std::string, AbstractFactory<T>*>::Iterator AbstractFactory<T>::end()
	{
		return ConcreteFactories.end();
	}

	template<typename T>
	AbstractFactory<T>* AbstractFactory<T>::Find(const std::string & name)
	{
		AbstractFactory<T> *result = nullptr;

		auto It = ConcreteFactories.Find(name);
		if (It != ConcreteFactories.end())
		{
			result = It->second;
		}
		return result;
	}

	template<typename T>
	T* AbstractFactory<T>::Create(const std::string& name)
	{
		AbstractFactory<T>* concreteFactory = Find(name);
		T* result = nullptr;

		if (concreteFactory != nullptr)
		{
			result = concreteFactory->Create();
		}
		return result;
	}

	template<typename T>
	void AbstractFactory<T>::ClearFactories()
	{
		for (auto& value : ConcreteFactories)
		{
			delete value.second;
		}
		ConcreteFactories.Clear();
	}

	template<typename T>
	HashMap<std::string, AbstractFactory<T>*> AbstractFactory<T>::ConcreteFactories;
}