#pragma once
#include "pch.h"
#include<sstream>
#include "Datum.h"

// Ideas
// Find and Remove could be refactored to use templates and function pointers
// Templated Unit Tests


namespace FieaGameEngine
{


	Datum::Datum(): Size(0), Capacity(0), IsExternalStorage(false), Type(DatumType::None)
	{

	}

	Datum::DatumValues::DatumValues(): GeneralPurposePointer(nullptr)
	{

	}

#pragma region ScalarConstructorOverloads

	Datum::Datum(const int32_t& rhs):Size(0), Capacity(0), IsExternalStorage(false), Type(DatumType::Integer)
	{
		PushBack(rhs);
	}

	Datum::Datum(const float& rhs) : Size(0), Capacity(0), IsExternalStorage(false), Type(DatumType::Float)
	{
		PushBack(rhs);
	}

	Datum::Datum(const std::string& rhs) : Size(0), Capacity(0), IsExternalStorage(false), Type(DatumType::String)
	{
		PushBack(rhs);
	}

	Datum::Datum(const glm::vec4& rhs) : Size(0), Capacity(0), IsExternalStorage(false), Type(DatumType::Vector4)
	{
		PushBack(rhs);
	}

	Datum::Datum(const glm::mat4x4& rhs) : Size(0), Capacity(0), IsExternalStorage(false), Type(DatumType::Matrix4x4)
	{
		PushBack(rhs);
	}

	Datum::Datum(RTTI *rhs) : Size(0), Capacity(0), IsExternalStorage(false), Type(DatumType::RTTIPointer)
	{
		PushBack(rhs);
	}

	Datum::Datum(Scope *rhs) : Size(0), Capacity(0), IsExternalStorage(false), Type(DatumType::Table)
	{
		PushBack(rhs);
	}

#pragma endregion ScalarConstructorOverloads

	Datum::~Datum()
	{
		if (IsExternalStorage == false)
		{
			Clear();
			free(Data.GeneralPurposePointer);
		}
	}

	Datum::Datum(const Datum& rhs):Datum()
	{
		Type = rhs.Type;

		if (rhs.IsExternalStorage == true)
		{
			Data.GeneralPurposePointer = rhs.Data.GeneralPurposePointer;
			Capacity = rhs.Capacity;
		}
		else
		{
			Reserve(rhs.Capacity);
			if (rhs.Type == DatumType::String)
			{
				for (uint32_t i = 0; i < rhs.Size; i++)
				{
					PushBack(rhs.Data.StringValues[i]);
				}
			}
			else
			{
				size_t typeSize = TypeSizeMap[static_cast<int32_t>(Type)];
				memcpy(Data.GeneralPurposePointer, rhs.Data.GeneralPurposePointer, typeSize*rhs.Size);
			}
		}
		Size = rhs.Size;
		IsExternalStorage = rhs.IsExternalStorage;
	}

	Datum::Datum(Datum&& rhs) :Type(rhs.Type), IsExternalStorage(rhs.IsExternalStorage), Size(rhs.Size), Capacity(rhs.Capacity), Data(rhs.Data)
	{
		rhs.Data.GeneralPurposePointer = nullptr;
		rhs.Size = 0;
		rhs.Capacity = 0;
		rhs.IsExternalStorage = false;
		rhs.Type = DatumType::None;
	}

	Datum& Datum::operator=(Datum&& rhs)
	{
		if (this != &rhs)
		{
			Type = rhs.Type;
			Size = rhs.Size;
			Capacity = rhs.Capacity;
			IsExternalStorage = rhs.IsExternalStorage;
			Data.GeneralPurposePointer = rhs.Data.GeneralPurposePointer;

			rhs.Data.GeneralPurposePointer = nullptr;
			rhs.Size = 0;
			rhs.Capacity = 0;
			rhs.IsExternalStorage = false;
			rhs.Type = DatumType::None;
		}
		return *this;
	}

	Datum& Datum::operator=(const Datum& rhs)
	{
		if (this != &rhs)
		{		
			if (IsExternalStorage == false)
			{
				Clear();
				free(Data.GeneralPurposePointer);
			}

			Data.GeneralPurposePointer = nullptr;
			Capacity = 0;
			Size = 0;
			Type = rhs.Type;
			IsExternalStorage = rhs.IsExternalStorage;

			if (rhs.IsExternalStorage == true)
			{
				Data.GeneralPurposePointer = rhs.Data.GeneralPurposePointer;
				Capacity = rhs.Capacity;
			}
			else
			{
				Reserve(rhs.Capacity);
				if (rhs.Type == DatumType::String)
				{
					for (uint32_t i = 0; i < rhs.Size; i++)
					{
						PushBack(rhs.Data.StringValues[i]);
					}
				}
				else
				{
					size_t typeSize = TypeSizeMap[static_cast<int32_t>(Type)];
					memcpy(Data.GeneralPurposePointer, rhs.Data.GeneralPurposePointer, typeSize*rhs.Size);
				}
			}
			Size = rhs.Size;		
		}
		return *this;
	}

	bool Datum::operator==(const Datum& rhs) const
	{
		bool Result = true;
		if (this == &rhs)
		{
			Result = true;
		}
		else
		{
			if (Size == rhs.Size && Type == rhs.Type)
			{
				if (Type == DatumType::String)
				{
					for (uint32_t i = 0; i < Size; i++)
					{
						if (Data.StringValues[i] != rhs.Data.StringValues[i])
						{
							Result = false;
							break;
						}
					}
				}
				else if (Type == DatumType::RTTIPointer || Type==DatumType::Table)
				{
					for (uint32_t i = 0; i < Size; i++)
					{
						if ((Data.RTTIPointerArray[i] == nullptr || rhs.Data.RTTIPointerArray[i] == nullptr) || (!(Data.RTTIPointerArray[i]->Equals(rhs.Data.RTTIPointerArray[i]))))
						{
							Result = false;
							break;
						}
					}
				}
				else
				{
					if (!std::memcmp(Data.GeneralPurposePointer, rhs.Data.GeneralPurposePointer, Size*TypeSizeMap[static_cast<int32_t>(Type)]))
					{
						Result = true;
					}
					else
					{
						Result = false;
					}
				}
			}
			else
			{
				Result = false;
			}
		}
		return Result;
	}

	bool Datum::operator!=(const Datum& rhs) const
	{
		return !(operator==(rhs));
	}

#pragma region ScalarEqualityOperatorOverloads
	bool Datum::operator==(const int32_t& rhs) const
	{
		bool Result = false;
		if (Size != 1)
		{
			Result = false;
		}
		else
		{
			Result = (Data.IntValues[0] == rhs);
		}
		return Result;
	}

	bool Datum::operator==(const float& rhs) const
	{
		bool Result = false;
		if (Size != 1)
		{
			Result = false;
		}
		else
		{
			Result = (Data.FloatValues[0] == rhs);
		}
		return Result;
	}

	bool Datum::operator==(const std::string& rhs) const
	{
		bool Result = false;
		if (Size != 1)
		{
			Result = false;
		}
		else
		{
			Result = (Data.StringValues[0] == rhs);
		}
		return Result;
	}

	bool Datum::operator==(const glm::vec4& rhs) const
	{
		bool Result = false;
		if (Size != 1)
		{
			Result = false;
		}
		else
		{
			Result = (Data.Vec4Values[0] == rhs);
		}
		return Result;
	}

	bool Datum::operator==(const glm::mat4x4& rhs) const
	{
		bool Result = false;
		if (Size != 1)
		{
			Result = false;
		}
		else
		{
			Result = (Data.Mat4x4Values[0] == rhs);
		}
		return Result;
	}

	bool Datum::operator==(RTTI* rhs) const
	{
		if (rhs == nullptr)
		{
			throw std::runtime_error("Rhs is nullptr");
		}
		bool Result = false;
		if (Size != 1)
		{
			Result = false;
		}
		else
		{
			Result = (Data.RTTIPointerArray[0]->Equals(rhs));
		}
		return Result;
	}

	//bool Datum::operator==(Scope* rhs) const
	//{
	//	bool Result = false;
	//	if (Size != 1)
	//	{
	//		Result = false;
	//	}
	//	else
	//	{
	//		Result = (Data.ScopePointerArray[0]->Equals(rhs));
	//	}
	//	return Result;
	//}

#pragma endregion ScalarEqualityOperatorOverloads

#pragma region ScalarInequalityOperatorOverloads
	bool Datum::operator!=(const int32_t& rhs) const
	{
		return (!(operator==(rhs)));
	}

	bool Datum::operator!=(const float& rhs) const
	{
		return (!(operator==(rhs)));
	}

	bool Datum::operator!=(const std::string& rhs) const
	{
		return (!(operator==(rhs)));
	}

	bool Datum::operator!=(const glm::vec4& rhs) const
	{
		return (!(operator==(rhs)));
	}

	bool Datum::operator!=(const glm::mat4x4& rhs) const
	{
		return (!(operator==(rhs)));
	}

	bool Datum::operator!=(RTTI* rhs) const
	{
		return (!(operator==(rhs)));
	}

#pragma endregion ScalarInequalityOperatorOverloads

#pragma region Scalar Operator=
	Datum& Datum::operator=(const int32_t& rhs)
	{
		if (Type != DatumType::None && Type!=DatumType::Integer)
		{
			throw std::runtime_error("Type Mismatch");
		}
		if (Size >= 1)
		{
			Set(rhs);
		}
		else
		{
			PushBack(rhs);
		}
		return *this;
	}

	Datum& Datum::operator=(const float& rhs)
	{
		if (Type != DatumType::None && Type != DatumType::Float)
		{
			throw std::runtime_error("Type Mismatch");
		}
		if (Size >= 1)
		{
			Set(rhs);
		}
		else
		{
			PushBack(rhs);
		}
		return *this;
	}

	Datum& Datum::operator=(const std::string& rhs)
	{
		if (Type != DatumType::None && Type != DatumType::String)
		{
			throw std::runtime_error("Type Mismatch");
		}
		if (Size >= 1)
		{
			Set(rhs);
		}
		else
		{
			PushBack(rhs);
		}
		return *this;
	}

	Datum& Datum::operator=(const glm::vec4& rhs)
	{
		if (Type != DatumType::None && Type != DatumType::Vector4)
		{
			throw std::runtime_error("Type Mismatch");
		}
		if (Size >= 1)
		{
			Set(rhs);
		}
		else
		{
			PushBack(rhs);
		}
		return *this;
	}

	Datum& Datum::operator=(const glm::mat4x4& rhs)
	{
		if (Type != DatumType::None && Type != DatumType::Matrix4x4)
		{
			throw std::runtime_error("Type Mismatch");
		}
		if (Size >= 1)
		{
			Set(rhs);
		}
		else
		{
			PushBack(rhs);
		}
		return *this;
	}

	Datum& Datum::operator=(RTTI* rhs)
	{
		if (Type != DatumType::None && Type != DatumType::RTTIPointer)
		{
			throw std::runtime_error("Type Mismatch");
		}
		if (Size >= 1)
		{
			Set(rhs);
		}
		else
		{
			PushBack(rhs);
		}
		return *this;
	}

	Datum& Datum::operator=(Scope* rhs)
	{
		if (Type != DatumType::None && Type != DatumType::Table)
		{
			throw std::runtime_error("Type Mismatch");
		}
		if (Size >= 1)
		{
			Set(rhs);
		}
		else
		{
			PushBack(rhs);
		}
		return *this;
	}

#pragma endregion Scalar Operator=

	Scope& Datum::operator[](const uint32_t& index)
	{
		return *Get<Scope*>(index);
	}

	Datum::DatumType Datum::GetType() const
	{
		return Type;
	}

	void Datum::SetType(const DatumType & newDatumType)
	{
		if (Type == DatumType::None && newDatumType!=DatumType::Max)
		{
			Type = newDatumType;
		}
		else
		{
			if (Type != newDatumType)
			{
				throw std::runtime_error("Data Type already set");
			}
		}
	}

	uint32_t Datum::Length() const
	{
		return Size;
	}

	void Datum::Clear()
	{
		if (IsExternalStorage == false)
		{
			if (Type == DatumType::String)
			{
				using namespace std;
				for (uint32_t i = 0; i < Size; i++)
				{
					Data.StringValues[i].~string();
				}
			}
			Size = 0;
		}
	}

	void Datum::Reserve(uint32_t capacity)
	{
		size_t typeSize = TypeSizeMap[static_cast<int32_t>(Type)];
		if(capacity > Capacity)
		{
			Data.GeneralPurposePointer = std::realloc(Data.GeneralPurposePointer, capacity * typeSize);
			Capacity = capacity;
		}
	}

#pragma region ToStrings
	std::string Datum::ToString(const uint32_t& index)
	{
		if (index >= Size)
		{
			throw std::runtime_error("Invalid Index");
		}

		std::string(Datum::*ToStringFunctionPointer[9])(const uint32_t&){ &Datum::InvalidType, &Datum::IntToString, &Datum::FloatToString, &Datum::VecToString, &Datum::MatToString, &Datum::StringWrapper, &Datum::RTTItoString, &Datum::RTTItoString, &Datum::InvalidType };

		return (this->*ToStringFunctionPointer[static_cast<int32_t>(Type)])(index);
	}

	std::string Datum::IntToString(const uint32_t& index)
	{
		return (std::to_string(Data.IntValues[index]));
	}

	std::string Datum::FloatToString(const uint32_t& index)
	{
		return std::to_string(Data.FloatValues[index]);
	}

	std::string Datum::VecToString(const uint32_t& index)
	{
		return glm::to_string(Data.Vec4Values[index]);
	}

	std::string Datum::MatToString(const uint32_t& index)
	{
		return glm::to_string(Data.Mat4x4Values[index]);
	}

	std::string Datum::StringWrapper(const uint32_t& index)
	{
		return Data.StringValues[index];
	}

	std::string Datum::InvalidType(const uint32_t& index)
	{
		index;
		throw std::runtime_error("Unable to convert to String");
	}

	std::string Datum::RTTItoString(const uint32_t& index)
	{
		return Data.RTTIPointerArray[index]->ToString();
	}
#pragma endregion ToStrings

#pragma region SetFromStrings

	void Datum::SetFromString(const std::string& data, const uint32_t& index)
	{
		if (index >= Size)
		{
			throw std::runtime_error("Invalid Size");
		}
		void (Datum::*SetFromStringFunctionPointer[9])(const std::string&, const uint32_t&) { &Datum::InvalidConversion, &Datum::IntFromString, &Datum::FloatFromString, &Datum::Vec4FromString, &Datum::Mat4x4FromString, &Datum::Set, &Datum::InvalidConversion, &Datum::InvalidConversion, &Datum::InvalidConversion};

		(this->*SetFromStringFunctionPointer[static_cast<int32_t>(Type)])(data, index);
	}

	void Datum::IntFromString(const std::string& data, const uint32_t& index)
	{
		Set(std::stoi(data), index);
	}

	void Datum::FloatFromString(const std::string& data, const uint32_t& index)
	{
		Set(std::stof(data), index);
	}

	void Datum::Vec4FromString(const std::string& data, const uint32_t& index)
	{
		glm::vec4 temp;
		sscanf_s(data.c_str(), "vec4(%f , %f , %f , %f )", &temp.x, &temp.y, &temp.z, &temp.w);
		Set(temp, index);
	}

	void Datum::Mat4x4FromString(const std::string& data, const uint32_t& index)
	{
		glm::mat4x4 temp;
		float tempFloat;

		std::stringstream StringStream;
		StringStream.str(data.substr(8));

		int i = 0, j = 0;
		while (StringStream >> tempFloat)
		{
			temp[i][j] = tempFloat;
			++i;
			if (i >= 4)
			{
				i = 0;
				++j;
				StringStream.ignore(5, '(');
			}
			else
			{
				StringStream.ignore(3, ',');
			}
		}
		Set(temp, index);
	}

	void Datum::InvalidConversion(const std::string& data, const uint32_t& index)
	{
		UNREFERENCED_PARAMETER(data);
		UNREFERENCED_PARAMETER(index);
		throw std::runtime_error("Invalid Conversion");
	}

#pragma endregion SetFromStrings

#pragma region Sets
	void Datum::Set(const int32_t& value, const uint32_t& index)
	{
		if (((Type != DatumType::None) && (Type != DatumType::Integer)) || (index >= Size))
		{
			throw std::runtime_error("Type mismatch or Setting an invalid Location");
		}
		Data.IntValues[index] = value;
	}

	void Datum::Set(const float& value, const uint32_t& index)
	{
		if (((Type != DatumType::None) && (Type != DatumType::Float)) || (index >= Size))
		{
			throw std::runtime_error("Type mismatch or Setting an invalid Location");
		}
		Data.FloatValues[index] = value;
	}

	void Datum::Set(const std::string& value, const uint32_t& index)
	{
		if (((Type != DatumType::None) && (Type != DatumType::String)) || (index >= Size))
		{
			throw std::runtime_error("Type mismatch or Setting an invalid Location");
		}
		Data.StringValues[index] = value;
	}

	void Datum::Set(const glm::vec4& value, const uint32_t& index)
	{
		if (((Type != DatumType::None) && (Type != DatumType::Vector4)) || (index >= Size))
		{
			throw std::runtime_error("Type mismatch or Setting an invalid Location");
		}
		Data.Vec4Values[index] = value;
	}

	void Datum::Set(RTTI* value, const uint32_t& index)
	{
		if (((Type != DatumType::None) && (Type != DatumType::RTTIPointer)) || (index >= Size))
		{
			throw std::runtime_error("Type mismatch or Setting an invalid Location");
		}
		Data.RTTIPointerArray[index] = value;
	}

	void Datum::Set(const glm::mat4x4& value, const uint32_t& index)
	{
		if (((Type != DatumType::None) && (Type != DatumType::Matrix4x4)) || (index >= Size))
		{
			throw std::runtime_error("Type mismatch or Setting an invalid Location");
		}
		Data.Mat4x4Values[index] = value;
	}

	void Datum::Set(Scope* value, const uint32_t& index)
	{
		if (((Type != DatumType::None) && (Type != DatumType::Table)) || (index >= Size))
		{
			throw std::runtime_error("Type mismatch or Setting an invalid Location");
		}
		Data.ScopePointerArray[index] = value;
	}

#pragma endregion Sets

#pragma region PushBacks
	void Datum::PushBack(const int32_t& value)
	{
		if ((Type != DatumType::None && Type != DatumType::Integer) || (IsExternalStorage==true))
		{
			throw std::runtime_error("Invalid Type/ External Storage specified");
		}
		if (Capacity == 0)
		{
			SetType(DatumType::Integer);
			Reserve(1);
		}
		else if (Size == Capacity)
		{
			Reserve(Capacity * 2);
		}
		new(&Data.IntValues[Size])int(value);
		Size++;
	}

	void Datum::PushBack(const float& value)
	{
		if ((Type != DatumType::None && Type != DatumType::Float) || (IsExternalStorage == true))
		{
			throw std::runtime_error("Invalid Type/ External Storage specified");
		}
		if (Capacity == 0 && IsExternalStorage == false)
		{
			SetType(DatumType::Float);
			Reserve(1);
		}
		if (Size == Capacity)
		{
			Reserve(Capacity * 2);
		}
		new(&Data.FloatValues[Size])float(value);
		Size++;
	}

	void Datum::PushBack(const std::string& value)
	{
		if ((Type != DatumType::None && Type != DatumType::String) || (IsExternalStorage == true))
		{
			throw std::runtime_error("Invalid Type/ External Storage specified");
		}
		if (Capacity == 0 && IsExternalStorage == false)
		{
			SetType(DatumType::String);
			Reserve(1);
		}
		if (Size == Capacity)
		{
			Reserve(Capacity * 2);
		}
		new(&Data.StringValues[Size])std::string(value);
		Size++;
	}

	void Datum::PushBack(const glm::vec4& value)
	{
		if ((Type != DatumType::None && Type != DatumType::Vector4) || (IsExternalStorage == true))
		{
			throw std::runtime_error("Invalid Type/ External Storage specified");
		}
		if (Capacity == 0 && IsExternalStorage == false)
		{
			SetType(DatumType::Vector4);
			Reserve(1);
		}
		if (Size == Capacity)
		{
			Reserve(Capacity * 2);
		}
		new(&Data.Vec4Values[Size])glm::vec4(value);
		Size++;
	}

	void Datum::PushBack(const glm::mat4x4& value)
	{
		if ((Type != DatumType::None && Type != DatumType::Matrix4x4) || (IsExternalStorage == true))
		{
			throw std::runtime_error("Invalid Type/ External Storage specified");
		}
		if (Capacity == 0 && IsExternalStorage == false)
		{
			SetType(DatumType::Matrix4x4);
			Reserve(1);
		}
		if (Size == Capacity)
		{
			Reserve(Capacity * 2);
		}
		new(&Data.Mat4x4Values[Size])glm::mat4x4(value);
		Size++;
	}

	void Datum::PushBack(RTTI* value)
	{
		if ((Type != DatumType::None && Type != DatumType::RTTIPointer) || (IsExternalStorage == true))
		{
			throw std::runtime_error("Invalid Type/ External Storage specified");
		}
		if (Capacity == 0 && IsExternalStorage == false)
		{
			SetType(DatumType::RTTIPointer);
			Reserve(1);
		}
		if (Size == Capacity)
		{
			Reserve(Capacity * 2);
		}
		Data.RTTIPointerArray[Size] = value;
		Size++;
	}

	void Datum::PushBack(Scope* value)
	{
		if ((Type != DatumType::None && Type != DatumType::Table) || (IsExternalStorage == true))
		{
			throw std::runtime_error("Invalid Type/ External Storage specified");
		}
		if (Capacity == 0 && IsExternalStorage == false)
		{
			SetType(DatumType::Table);
			Reserve(1);
		}
		if (Size == Capacity)
		{
			Reserve(Capacity * 2);
		}
		Data.ScopePointerArray[Size] = value;
		Size++;
	}

#pragma endregion PushBacks

	void Datum::PopBack()
	{
		if (Size == 0 || IsExternalStorage == true)
		{
			throw std::runtime_error("Invalid Call to PopBack, Datum is either empty or has external storage");
		}
		if (Type == DatumType::String)
		{
			using namespace std;
			Data.StringValues[Size - 1].~string();
		}
		Size--;
	}

#pragma region SetStorage
	void Datum::SetStorage(int32_t* valueArray, const int32_t& size)
	{
		if (size == 0)
		{
			throw std::runtime_error("Invalid Size");
		}

		if ((IsExternalStorage == false) && (Capacity > 0))
		{
			Clear();
			free(Data.GeneralPurposePointer);
		}
		Type = DatumType::Integer;
		IsExternalStorage = true;
		Data.IntValues = valueArray;
		Capacity = size;
		Size = size;
	}

	void Datum::SetStorage(float* valueArray, const int32_t& size)
	{
		if (size == 0)
		{
			throw std::runtime_error("Invalid Size");
		}

		if ((IsExternalStorage == false) && (Capacity > 0))
		{
			Clear();
			free(Data.GeneralPurposePointer);
		}
		Type = DatumType::Float;
		IsExternalStorage = true;
		Data.FloatValues = valueArray;
		Capacity = size;
		Size = size;
	}

	void Datum::SetStorage(std::string* valueArray, const int32_t& size)
	{
		if (size == 0)
		{
			throw std::runtime_error("Invalid Size");
		}

		if ((IsExternalStorage == false) && (Capacity > 0))
		{
			Clear();
			free(Data.GeneralPurposePointer);
		}
		Type = DatumType::String;
		IsExternalStorage = true;
		Data.StringValues = valueArray;
		Capacity = size;
		Size = size;
	}

	void Datum::SetStorage(glm::vec4* valueArray, const int32_t& size)
	{
		if (size == 0)
		{
			throw std::runtime_error("Invalid Size");
		}

		if ((IsExternalStorage == false) && (Capacity > 0))
		{
			Clear();
			free(Data.GeneralPurposePointer);
		}
		Type = DatumType::Vector4;
		IsExternalStorage = true;
		Data.Vec4Values = valueArray;
		Capacity = size;
		Size = size;
	}

	void Datum::SetStorage(glm::mat4x4* valueArray, const int32_t& size)
	{
		if (size == 0)
		{
			throw std::runtime_error("Invalid Size");
		}

		if ((IsExternalStorage == false) && (Capacity > 0))
		{
			Clear();
			free(Data.GeneralPurposePointer);
		}
		Type = DatumType::Matrix4x4;
		IsExternalStorage = true;
		Data.Mat4x4Values = valueArray;
		Capacity = size;
		Size = size;
	}

	void Datum::SetStorage(RTTI** valueArray, const int32_t& size)
	{
		if (size == 0)
		{
			throw std::runtime_error("Invalid Size");
		}

		if ((IsExternalStorage == false) && (Capacity > 0))
		{
			Clear();
			free(Data.GeneralPurposePointer);
		}
		Type = DatumType::RTTIPointer;
		IsExternalStorage = true;
		Data.RTTIPointerArray = valueArray;
		Capacity = size;
		Size = size;
	}

	/*void Datum::SetStorage(Scope** valueArray, const int32_t& size)
	{
		if (size == 0)
		{
			throw std::runtime_error("Invalid Size");
		}

		if ((IsExternalStorage == false) && (Capacity > 0))
		{
			Clear();
			free(Data.GeneralPurposePointer);
		}
		Type = DatumType::Table;
		IsExternalStorage = true;
		Data.ScopePointerArray = valueArray;
		Capacity = size;
		Size = size;
	}*/

#pragma endregion SetStorage

	void Datum::Resize(const uint32_t& size)
	{
		if (Type == DatumType::None)
		{
			throw std::runtime_error("No Type Initialized or External Storage");
		}
		if (!IsExternalStorage)
		{
			if (Capacity < size)
			{
				Reserve(size);
				if (Type == DatumType::String)
				{
					for (uint32_t i = Size; i < size; ++i)
					{
						new(&Data.StringValues[i])std::string();
					}
				}
				Size = size;
			}
			else
			{
				if (Type == DatumType::String)
				{
					for (uint32_t i = Size - 1; i > size - 1; --i)
					{
						using namespace std;
						Data.StringValues[i].~string();
					}
				}
				Size = size;
				Data.GeneralPurposePointer = std::realloc(Data.GeneralPurposePointer, Size*TypeSizeMap[static_cast<int32_t>(Type)]);
				Capacity = Size;
			}
		}
	}

#pragma region Finds
	uint32_t Datum::Find(const int32_t& value)
	{
		if (Type != DatumType::Integer)
		{
			throw std::runtime_error("Invalid Find");
		}
		uint32_t i = 0;
		for (; i < Size; i++)
		{
			if (Data.IntValues[i] == value)
			{
				break;
			}
		}
		return i;
	}

	uint32_t Datum::Find(const float& value)
	{
		if (Type != DatumType::Float)
		{
			throw std::runtime_error("Invalid Find");
		}
		uint32_t i = 0;
		for (; i < Size; i++)
		{
			if (Data.FloatValues[i] == value)
			{
				break;
			}
		}
		return i;
	}

	uint32_t Datum::Find(const RTTI* value)
	{
		if (Type != DatumType::RTTIPointer && Type!= DatumType::Table)
		{
			throw std::runtime_error("Invalid Find");
		}
		uint32_t i = 0;
		for (; i < Size; i++)
		{
			if (Data.RTTIPointerArray[i]->Equals(value))
			{
				break;
			}
		}
		return i;
	}

	uint32_t Datum::Find(const std::string& value)
	{
		if (Type != DatumType::String)
		{
			throw std::runtime_error("Invalid Find");
		}
		uint32_t i = 0;
		for (; i < Size; i++)
		{
			if (Data.StringValues[i] == value)
			{
				break;
			}
		}
		return i;
	}

	uint32_t Datum::Find(const glm::vec4& value)
	{
		if (Type != DatumType::Vector4)
		{
			throw std::runtime_error("Invalid Find");
		}
		uint32_t i = 0;
		for (; i < Size; i++)
		{
			if (Data.Vec4Values[i] == value)
			{
				break;
			}
		}
		return i;
	}

	uint32_t Datum::Find(const glm::mat4x4& value)
	{
		if (Type != DatumType::Matrix4x4)
		{
			throw std::runtime_error("Invalid Find");
		}
		uint32_t i = 0;
		for (; i < Size; ++i)
		{
			if (Data.Mat4x4Values[i] == value)
			{
				break;
			}
		}
		return i;
	}
#pragma endregion Finds

#pragma region Removes

	bool Datum::Remove(const int32_t& data)
	{
		if (IsExternalStorage == true)
		{
			throw std::runtime_error("Cannot remove data from External Storage");
		}

		bool result = false;
		uint32_t index;
		index = Find(data);

		if (index < Size)
		{
			result = true;
			std::memmove(Data.IntValues + index, Data.IntValues + index + 1, (Size - index - 1) * TypeSizeMap[static_cast<int32_t>(Type)]);
			--Size;
		}

		return result;
	}

	bool Datum::Remove(const float& data)
	{
		if (IsExternalStorage == true)
		{
			throw std::runtime_error("Cannot remove data from External Storage");
		}

		bool result = false;
		uint32_t index;
		index = Find(data);

		if (index < Size)
		{
			result = true;
			std::memmove(Data.FloatValues + index, Data.FloatValues + index + 1, (Size - index - 1) * TypeSizeMap[static_cast<int32_t>(Type)]);
			--Size;
		}

		return result;
	}

	bool Datum::Remove(const std::string& data)
	{
		if (IsExternalStorage == true)
		{
			throw std::runtime_error("Cannot remove data from External Storage");
		}

		bool result = false;
		uint32_t index;
		index = Find(data);

		if (index < Size)
		{
			using namespace std;
			Data.StringValues[index].~string();
			result = true;
			std::memmove(Data.StringValues + index, Data.StringValues + index + 1, (Size - index - 1) * TypeSizeMap[static_cast<int32_t>(Type)]);
			--Size;
		}

		return result;
	}

	bool Datum::Remove(const glm::vec4& data)
	{
		if (IsExternalStorage == true)
		{
			throw std::runtime_error("Cannot remove data from External Storage");
		}

		bool result = false;
		uint32_t index;
		index = Find(data);

		if (index < Size)
		{
			result = true;
			std::memmove(Data.Vec4Values + index, Data.Vec4Values + index + 1, (Size - index - 1) * TypeSizeMap[static_cast<int32_t>(Type)]);
			--Size;
		}

		return result;
	}

	bool Datum::Remove(const glm::mat4x4& data)
	{
		if (IsExternalStorage == true)
		{
			throw std::runtime_error("Cannot remove data from External Storage");
		}

		bool result = false;
		uint32_t index;
		index = Find(data);

		if (index < Size)
		{
			result = true;
			std::memmove(Data.Mat4x4Values + index, Data.Mat4x4Values + index + 1, (Size - index - 1) * TypeSizeMap[static_cast<int32_t>(Type)]);
			--Size;
		}

		return result;
	}

	bool Datum::Remove(const RTTI* data)
	{
		if (IsExternalStorage == true)
		{
			throw std::runtime_error("Cannot remove data from External Storage");
		}

		bool result = false;
		uint32_t index;
		index = Find(data);

		if (index < Size)
		{
			result = true;
			std::memmove(Data.RTTIPointerArray + index, Data.RTTIPointerArray + index + 1, (Size - index - 1) * TypeSizeMap[static_cast<int32_t>(Type)]);
			--Size;
		}

		return result;
	}

	bool Datum::RemoveWithIndex(const uint32_t& index)
	{
		bool result = false;
		static void (Datum::*RemoveWithIndexFunctionPointer[9])(const uint32_t&) { nullptr, &Datum::RemoveIntWithIndex, &Datum::RemoveFloatWithIndex, &Datum::RemoveVectorWithIndex, &Datum::RemoveMatrixWithIndex, &Datum::RemoveStringWithIndex, &Datum::RemoveRTTIWithIndex, &Datum::RemoveTableWithIndex, nullptr };
		if (index < Size)
		{
			result = true;
			(this->*RemoveWithIndexFunctionPointer[static_cast<int32_t>(Type)])(index);
			--Size;
		}
		return result;
	}

	void Datum::RemoveIntWithIndex(const uint32_t& index)
	{
		std::memmove(Data.IntValues + index, Data.IntValues + index + 1, (Size - index - 1) * TypeSizeMap[static_cast<int32_t>(Type)]);
	}

	void Datum::RemoveFloatWithIndex(const uint32_t& index)
	{
		std::memmove(Data.FloatValues + index, Data.FloatValues + index + 1, (Size - index - 1) * TypeSizeMap[static_cast<int32_t>(Type)]);
	}

	void Datum::RemoveStringWithIndex(const uint32_t& index)
	{
		using namespace std;
		Data.StringValues[index].~string();
		std::memmove(Data.StringValues + index, Data.StringValues + index + 1, (Size - index - 1) * TypeSizeMap[static_cast<int32_t>(Type)]);
	}

	void Datum::RemoveVectorWithIndex(const uint32_t& index)
	{
		std::memmove(Data.Vec4Values + index, Data.Vec4Values + index + 1, (Size - index - 1) * TypeSizeMap[static_cast<int32_t>(Type)]);
	}

	void Datum::RemoveMatrixWithIndex(const uint32_t& index)
	{
		std::memmove(Data.Mat4x4Values + index, Data.Mat4x4Values + index + 1, (Size - index - 1) * TypeSizeMap[static_cast<int32_t>(Type)]);
	}

	void Datum::RemoveRTTIWithIndex(const uint32_t& index)
	{
		std::memmove(Data.RTTIPointerArray + index, Data.RTTIPointerArray + index + 1, (Size - index - 1) * TypeSizeMap[static_cast<int32_t>(Type)]);
	}

	void Datum::RemoveTableWithIndex(const uint32_t& index)
	{
		std::memmove(Data.ScopePointerArray + index, Data.ScopePointerArray + index + 1, (Size - index - 1) * TypeSizeMap[static_cast<int32_t>(Type)]);
	}
#pragma endregion EndRemoves

	bool Datum::IsExternal()
	{
		return IsExternalStorage;
	}

	size_t Datum::TypeSizeMap[9] = { 0, sizeof(int32_t), sizeof(float), sizeof(glm::vec4), sizeof(glm::mat4x4), sizeof(std::string), sizeof(RTTI*), sizeof(Scope*), 0 };
}
