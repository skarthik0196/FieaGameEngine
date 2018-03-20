#include "pch.h"
#include "Attributed.h"


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Attributed)

	Attributed::Attributed()
	{
		AddInternalAttribute("this", this, 1);
	}

	Attributed::Attributed(const uint64_t& runtimeTypeID)
	{
		Datum &datum = Append("this");
		datum.PushBack(static_cast<RTTI*>(this));
		Attributed* FirstEntryPointer = (*FirstEntry.Insert(std::make_pair(runtimeTypeID, this))).second;
		if (FirstEntryPointer == this)
		{
			PrescribedAttributes[runtimeTypeID].PushBack("this");
		}
	}


	Attributed::~Attributed()
	{

	}

	Attributed::Attributed(const Attributed& rhs) : Scope(rhs)
	{
		(*this)["this"] = static_cast<RTTI*>(this);
	}

	Attributed::Attributed(Attributed&& rhs) : Scope(std::move(rhs))
	{
		(*this)["this"] = static_cast<RTTI*>(this);
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		if (this != &rhs)
		{
			Scope::operator=(rhs);
			(*this)["this"] = static_cast<RTTI*>(this);
		}

		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& rhs)
	{
		if (this != &rhs)
		{
			Scope::operator=(std::move(rhs));
			(*this)["this"] = static_cast<RTTI*>(this);
		}

		return *this;
	}

	Datum& Attributed::AppendAuxillaryAttribute(const std::string& name)
	{
		if (PrescribedAttributes[TypeIdInstance()].Find(name) != PrescribedAttributes[TypeIdInstance()].end())
		{
			throw std::runtime_error("Invalid String");
		}

		return (Append(name));
	}

	bool Attributed::IsAttribute(const std::string& name) const
	{
		return (Find(name) != nullptr);
	}

	bool Attributed::IsPrescribedAttribute(const std::string& name) const
	{
		bool Result = false;
		HashMap<uint64_t, Vector<std::string>>::Iterator It = PrescribedAttributes.Find(TypeIdInstance());
		if (It != PrescribedAttributes.end())
		{
			Vector<std::string>::Iterator VIt = (*It).second.Find(name);
			if (VIt != (*It).second.end())
			{
				Result = true;
			}
		}
		return Result;
	}

	bool Attributed::IsAuxillaryAttribute(const std::string& name) const
	{
		return ((IsPrescribedAttribute(name) == false) && (IsAttribute(name) == true));
	}

#pragma region AddInternalAttributes
	Datum& Attributed::AddInternalAttribute(const std::string& name, const int32_t& defaultValue, const uint32_t& size)
	{
		Datum &datum = Append(name);
		for (uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(defaultValue);
		}

		AddPrescribedKey(name);

		return datum;
	}

	Datum& Attributed::AddInternalAttribute(const std::string& name, const float& defaultValue, const uint32_t& size)
	{
		Datum &datum = Append(name);
		for (uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(defaultValue);
		}

		AddPrescribedKey(name);

		return datum;
	}

	Datum& Attributed::AddInternalAttribute(const std::string& name, const std::string& defaultValue, const uint32_t& size)
	{
		Datum &datum = Append(name);
		for (uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(defaultValue);
		}

		AddPrescribedKey(name);

		return datum;
	}

	Datum& Attributed::AddInternalAttribute(const std::string& name, const glm::vec4& defaultValue, const uint32_t& size)
	{
		Datum &datum = Append(name);
		for (uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(defaultValue);
		}

		AddPrescribedKey(name);

		return datum;
	}

	Datum& Attributed::AddInternalAttribute(const std::string& name, const glm::mat4x4& defaultValue, const uint32_t& size)
	{
		Datum &datum = Append(name);
		for (uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(defaultValue);
		}

		AddPrescribedKey(name);

		return datum;
	}

	Datum& Attributed::AddInternalAttribute(const std::string& name, RTTI* defaultValue, const uint32_t& size)
	{
		Datum &datum = Append(name);
		for (uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(defaultValue);
		}

		AddPrescribedKey(name);

		return datum;
	}

	Scope& Attributed::CreateNestedScope(const std::string& name)
	{
		Scope& scope = AppendScope(name);

		AddPrescribedKey(name);

		return scope;
	}

	Datum& Attributed::AddNestedScope(const std::string& name)
	{
		Datum& scope = Append(name);
		scope.SetType(Datum::DatumType::Table);

		AddPrescribedKey(name);

		return scope;
	}

	Datum& Attributed::AddNestedScope(const std::string& name, Scope& newScope)
	{
		Datum& scope = Append(name);
		scope.SetType(Datum::DatumType::Table);
		scope.PushBack(&newScope);

		AddPrescribedKey(name);

		return scope;
	}

#pragma endregion AddInternalAttributes

#pragma region AddExternalAttributes

	Datum& Attributed::AddExternalAttribute(const std::string& name, int32_t* storageLocation, const uint32_t& size)
	{
		Datum &datum = Append(name);
		datum.SetStorage(storageLocation, size);
		AddPrescribedKey(name);

		return datum;
	}

	Datum& Attributed::AddExternalAttribute(const std::string& name, float* storageLocation, const uint32_t& size)
	{
		Datum &datum = Append(name);
		datum.SetStorage(storageLocation, size);
		AddPrescribedKey(name);

		return datum;
	}

	Datum& Attributed::AddExternalAttribute(const std::string& name, std::string* storageLocation, const uint32_t& size)
	{
		Datum &datum = Append(name);
		datum.SetStorage(storageLocation, size);
		AddPrescribedKey(name);

		return datum;
	}

	Datum& Attributed::AddExternalAttribute(const std::string& name, glm::vec4* storageLocation, const uint32_t& size)
	{
		Datum &datum = Append(name);
		datum.SetStorage(storageLocation, size);
		AddPrescribedKey(name);

		return datum;
	}

	Datum& Attributed::AddExternalAttribute(const std::string& name, glm::mat4x4* storageLocation, const uint32_t& size)
	{
		Datum &datum = Append(name);
		datum.SetStorage(storageLocation, size);
		AddPrescribedKey(name);

		return datum;
	}

	Datum& Attributed::AddExternalAttribute(const std::string& name, RTTI** storageLocation, const uint32_t& size)
	{
		Datum &datum = Append(name);
		datum.SetStorage(storageLocation, size);
		AddPrescribedKey(name);

		return datum;
	}

#pragma endregion AddExternalAttributes

#pragma region UpdateStorage
	void Attributed::UpdateStorage(const std::string& name, int32_t* storageLocation, const uint32_t& size)
	{
		(*this)[name].SetStorage(storageLocation, size);
	}

	void Attributed::UpdateStorage(const std::string& name, float* storageLocation, const uint32_t& size)
	{
		(*this)[name].SetStorage(storageLocation, size);
	}

	void Attributed::UpdateStorage(const std::string& name, std::string* storageLocation, const uint32_t& size)
	{
		(*this)[name].SetStorage(storageLocation, size);
	}

	void Attributed::UpdateStorage(const std::string& name, glm::vec4* storageLocation, const uint32_t& size)
	{
		(*this)[name].SetStorage(storageLocation, size);
	}

	void Attributed::UpdateStorage(const std::string& name, glm::mat4x4* storageLocation, const uint32_t& size)
	{
		(*this)[name].SetStorage(storageLocation, size);
	}

	void Attributed::UpdateStorage(const std::string& name, RTTI** storageLocation, const uint32_t& size)
	{
		(*this)[name].SetStorage(storageLocation, size);
	}
#pragma endregion UpdateStorage

	void Attributed::AddPrescribedKey(const std::string& name)
	{
		bool isFirstEntry;
		Attributed* firstEntryPointer = (*(FirstEntry.Insert(std::pair<uint64_t, Attributed*>(TypeIdInstance(), this), isFirstEntry))).second;
		if ((isFirstEntry == true) || (firstEntryPointer == this))
		{
			if (PrescribedAttributes[TypeIdInstance()].Find(name) == PrescribedAttributes[TypeIdInstance()].end())
			{
				PrescribedAttributes[TypeIdInstance()].PushBack(name);
			}
		}
	}

	Vector<std::string> Attributed::GetAttributes() const
	{
		const Vector<std::pair<std::string, Datum>*>& orderedVector = GetOrderedVector();
		Vector<std::string> temp;

		for (auto& value : orderedVector)
		{
			temp.PushBack(value->first);
		}
		return temp;
	}

	//Do Hashmap Finds
	Vector<std::string> Attributed::GetPrescribedAttributes() const
	{
		const Vector<std::pair<std::string, Datum>*>& orderedVector = GetOrderedVector();
		Vector<std::string> temp;

		Vector<std::string>& prescribedKeyList = PrescribedAttributes[TypeIdInstance()];
		for (auto& value : orderedVector)
		{
			if (prescribedKeyList.Find(value->first) != prescribedKeyList.end())
			{
				temp.PushBack(value->first);
			}
		}
		return temp;
	}

	// Prescribed Attributes +1
	// 
	Vector<std::string> Attributed::GetAuxillaryAttributes() const
	{
		const Vector<std::pair<std::string, Datum>*>& orderedVector = GetOrderedVector();
		Vector<std::string> temp;

		Vector<std::string>& prescribedKeyList = PrescribedAttributes[TypeIdInstance()];
		for (auto& value : orderedVector)
		{
			if (prescribedKeyList.Find(value->first) == prescribedKeyList.end())
			{
				temp.PushBack(value->first);
			}
		}

		return temp;
	}

	HashMap<uint64_t, Vector<std::string>> Attributed::PrescribedAttributes;
	HashMap<uint64_t, Attributed*> Attributed::FirstEntry;
}
