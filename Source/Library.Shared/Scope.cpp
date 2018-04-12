#pragma once
#include"pch.h"
#include "Scope.h"

//Reserve Before copying assignment - Done
//Changed FindName to use a reference instead - Done
//Set output parameter to nullptr in Search - Done
//use string.empty() - Done
//Check for null while doing As in Equals method. -Done

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Scope)

	Scope::Scope(const uint32_t& capacity) :OrderedTable(capacity), Table(capacity)
	{

	}

	Scope::~Scope()
	{
		Clear();
		if (ParentScope != nullptr)
		{
			ParentScope->Orphan(this);
		}
	}

	Scope::Scope(const Scope& rhs)
	{
		operator=(rhs);
	}

	Scope::Scope(Scope&& rhs)
	{
		if (rhs.ParentScope != nullptr)
		{
			std::string key;
			uint32_t index;
			Datum& datum = *((rhs.ParentScope)->FindContainedScope(rhs, index, key));
			datum.Set(this, index);
		}

		ParentScope = rhs.ParentScope;
		Table = std::move(rhs.Table);
		OrderedTable = std::move(rhs.OrderedTable);

		for (auto& value : OrderedTable)
		{
			if (value->second.GetType() == Datum::DatumType::Table)
			{
				Datum &datum = value->second;
				for (uint32_t i = 0; i < datum.Length(); ++i)
				{
					datum.Get<Scope*>(i)->ParentScope = this;
				}
			}
		}

		rhs.ParentScope = nullptr;
	}

	Scope& Scope::operator=(const Scope& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			ParentScope = nullptr;
			OrderedTable.Reserve(rhs.OrderedTable.Length());
			for (auto& value : rhs.OrderedTable)
			{
				if (value->second.GetType() == Datum::DatumType::Table)
				{
					for (uint32_t i = 0; i < value->second.Length(); ++i)
					{
						Scope *temp = value->second.Get<Scope*>(i)->Clone();
						*temp = *(value->second.Get<Scope*>(i));
						Adopt(*temp, value->first);
					}
				}
				else
				{
					Append(value->first) = value->second;
				}
			}

		}
		return *this;
	}

	Scope& Scope::operator=(Scope&& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			if (ParentScope != nullptr)
			{
				ParentScope->Orphan(this);
			}

			if (rhs.ParentScope != nullptr)
			{
				std::string key;
				uint32_t index;
				Datum& datum = *((rhs.ParentScope)->FindContainedScope(rhs, index, key));
				datum.Set(this, index);
			}

			ParentScope = std::move(rhs.ParentScope);
			Table = std::move(rhs.Table);
			OrderedTable = std::move(rhs.OrderedTable);

			for (auto& value : OrderedTable)
			{
				if (value->second.GetType() == Datum::DatumType::Table)
				{
					Datum &datum = value->second;
					for (uint32_t i = 0; i < datum.Length(); ++i)
					{
						datum.Get<Scope*>(i)->ParentScope = this;
					}
				}
			}
		}
		return *this;
	}

	Datum& Scope::Append(const std::string& stringKey)
	{
		if (stringKey.empty() == true)
		{
			throw std::runtime_error("Empty String not allowed");
		}
		bool isInserted;
		HashMap<std::string, Datum>::Iterator It = Table.Insert(std::pair<std::string, Datum>(stringKey, std::move(Datum())), isInserted);
		if (isInserted == true)
		{
			OrderedTable.PushBack(&(*It));
		}
		return It->second;
	}

	Scope& Scope::AppendScope(const std::string& stringKey)
	{
		Datum &datum = Append(stringKey);
		if ((datum.GetType() != Datum::DatumType::None) && (datum.GetType() != Datum::DatumType::Table))
		{
			throw std::runtime_error(" Datum of different type already exists at specified key ");
		}
		Scope *temp = new Scope();
		temp->ParentScope = this;
		datum.PushBack(temp);
		return *temp;

	}

	Datum& Scope::operator[](const uint32_t& index)
	{
		return OrderedTable[index]->second;
	}

	Datum& Scope::operator[](const std::string& stringkey)
	{
		return(Append(stringkey));
	}

	bool Scope::operator==(const Scope& rhs) const
	{
		bool Result = true;
		bool isFound = false;

		if (OrderedTable.Length() == rhs.OrderedTable.Length())
		{
			for (auto& value : OrderedTable)
			{
				isFound = false;
				for (auto& rhsValue : rhs.OrderedTable)
				{
					if ((*rhsValue == *value))
					{
						isFound = true;
						break;
					}
				}
				if (isFound == false)
				{
					Result = false;
					break;
				}
			}
		}
		else
		{
			Result = false;
		}

		return Result;
	}

	bool Scope::operator!=(const Scope& rhs) const
	{
		return !(operator==(rhs));
	}

	void Scope::Clear()
	{
		for (auto& value : OrderedTable)
		{
			if (value->second.GetType() == Datum::DatumType::Table)
			{
				for (uint32_t i = 0; i < value->second.Length(); ++i)
				{
					value->second.Get<Scope*>(i)->ParentScope = nullptr;
					delete value->second.Get<Scope*>(i);
				}
			}
		}
		Table.Clear();
		OrderedTable.Clear();
	}

	Scope* Scope::GetParent() const
	{
		return ParentScope;
	}

	std::string Scope::FindName(const Scope& scopeAddress) const
	{
		uint32_t index;
		std::string name = "";

		Datum *d = FindContainedScope(scopeAddress, index, name);
		UNREFERENCED_PARAMETER(d);

		return name;
	}

	Datum* Scope::FindContainedScope(const Scope& scopeAddress, OUT uint32_t& foundIndex, OUT std::string& name) const
	{
		Datum *result = nullptr;
		std::string key = "";
		uint32_t index = 0;

		for (auto& value : OrderedTable)
		{
			if (value->second.GetType() == Datum::DatumType::Table)
			{
				Datum &datum = value->second;
				for (index = 0; index < datum.Length(); ++index)
				{
					if (datum.Get<Scope*>(index) == &scopeAddress)
					{
						result = &datum;
						key = value->first;
						break;
					}
				}
				if (key != "")
				{
					break;
				}
			}
		}

		name = key;
		foundIndex = index;

		return result;
	}

	Datum* Scope::Find(const std::string& stringKey) const
	{
		HashMap<std::string, Datum>::Iterator It = Table.Find(stringKey);
		Datum *temp = nullptr;

		if (It != Table.end())
		{
			temp = &(It->second);
		}
		return temp;
	}

	Datum* Scope::Search(const std::string& stringKey, OUT Scope** foundScope)
	{
		Datum *temp = Find(stringKey);
		if (temp == nullptr)
		{
			if (ParentScope != nullptr)
			{
				temp = ParentScope->Search(stringKey, foundScope);
			}
			else
			{
				*foundScope = nullptr;

			}
		}
		else
		{
			if (foundScope != nullptr)
			{
				*foundScope = this;
			}
		}
		return temp;
	}

	Datum& Scope::Adopt(Scope& child, const std::string& name)
	{
		if (this == &child)
		{
			throw std::runtime_error("Trying to adopt self");
		}

		Datum& datum = Append(name);
		if (datum.GetType() == Datum::DatumType::None || datum.GetType() == Datum::DatumType::Table)
		{
			if (child.ParentScope != nullptr)
			{
				child.ParentScope->Orphan(&child);
			}
			datum.PushBack(&child);
			child.ParentScope = this;
		}
		return datum;
	}

	void Scope::Orphan(Scope *child)
	{
		if (child == nullptr || child == this)
		{
			throw std::runtime_error("Trying to orphan self");
		}

		Scope *temp = nullptr;
		std::string key;
		uint32_t i = 0;

		for (; i < OrderedTable.Length(); ++i)
		{
			if (OrderedTable[i]->second.GetType() == Datum::DatumType::Table)
			{
				Datum& datum = OrderedTable[i]->second;
				for (uint32_t j = 0; j < datum.Length(); ++j)
				{
					if (datum.Get<Scope*>(j) == child)
					{
						temp = datum.Get<Scope*>(j);
						datum.RemoveWithIndex(j);
						break;
					}
				}

				if (temp != nullptr)
				{
					break;
				}
			}
		}
	}

	Scope* Scope::Clone() const
	{
		return (new Scope());
	}

	std::string Scope::ToString() const
	{
		return std::string("Scope");
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		bool Result = false;

		if (rhs == nullptr)
		{
			throw std::runtime_error("Rhs is nullptr");
		}

		Scope* temp = rhs->As<Scope>();

		if (temp != nullptr)
		{
			Result = operator==(*temp);
		}

		return Result;
	}

	const Vector<std::pair<std::string, Datum>*>& Scope::GetOrderedVector() const
	{
		return OrderedTable;
	}
}