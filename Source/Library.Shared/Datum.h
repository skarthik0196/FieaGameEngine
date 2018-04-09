#pragma once
#include<string>
#pragma warning (push)
#pragma warning (disable : 4201)
#include"glm\glm.hpp"
#include"glm\gtx\string_cast.hpp"
#pragma warning (pop)
#include"RTTI.h"
#include"HashMap.h"

//Use Scope& and test not null in Datum RTTI

namespace FieaGameEngine
{
	class Scope;
	/// <summary>
	/// The Datum class, marked final
	/// </summary>
	class Datum final
	{

	public:

		/// <summary>
		/// Enum representing the different Datum types
		/// </summary>
		enum class DatumType
		{
			None,
			Integer,
			Float,
			Vector4,
			Matrix4x4,
			String,
			RTTIPointer,
			Table,
			Max
		};

		/// <summary>
		/// Default constructor for Datum
		/// </summary>
		Datum();

		/// <summary>
		/// Scalar Overloads for the Constructor
		/// </summary>

		/// <summary>
		/// Scalar Typecast constructor for int32
		/// </summary>
		/// <param name="rhs">The Integer Data</param>
		Datum(const int32_t& rhs);

		/// <summary>
		/// Scalar typecast constructor for std::string
		/// </summary>
		/// <param name="rhs">The String data</param>
		Datum(const std::string& rhs);

		/// <summary>
		/// Scalar typecast constructor for floats
		/// </summary>
		/// <param name="rhs">The floating point data</param>
		Datum(const float& rhs);

		/// <summary>
		/// Scalar typecast constructor for glm::vec4
		/// </summary>
		/// <param name="rhs">The Vector 4 data in th glm vec4 container</param>
		Datum(const glm::vec4& rhs);

		/// <summary>
		/// Scalar typecast constructor for glm::mat4x4
		/// </summary>
		/// <param name="rhs">The 4x4 Matrix Data in the glm::mat4x4 container</param>
		Datum(const glm::mat4x4& rhs);

		/// <summary>
		/// Scalar typecast Constructor for RTTI*
		/// </summary>
		/// <param name="rhs">The RTTI* pointer</param>
		Datum(RTTI* const rhs);

		Datum(Scope* const rhs);	//TODO

		/// <summary>
		/// Copy Constructor for Datum that deep copies the Datum elements
		/// </summary>
		/// <param name="rhs">The Datum to be copied from</param>
		Datum(const Datum& rhs);

		/// <summary>
		/// Move Constructor for Datum
		/// </summary>
		/// <param name="rhs">The Datum whose Data is to be moved</param>
		Datum(Datum&& rhs);

		/// <summary>
		/// Destructor for Datum
		/// </summary>
		~Datum();

		/// <summary>
		/// Overloaded Assignment operator that deep copies Data
		/// </summary>
		/// <param name="rhs">The Datum to be copied from</param>
		/// <returns>The Datum after copying</returns>
		Datum& operator=(const Datum& rhs);

		/// <summary>
		/// Move Assignment operator that moves data
		/// </summary>
		/// <param name="rhs">The Datum whose data is to be moved</param>
		/// <returns>The Datum after moving the data</returns>
		Datum& operator=(Datum&& rhs);

		/// <summary>
		/// Scalar assignment operator overload for Integers
		/// </summary>
		/// <param name="rhs">The Integer data</param>
		/// <returns>Datum after assigning the data</returns>
		Datum& operator=(const int32_t& rhs);

		/// <summary>
		/// Scalar assignment operator overload for std::string
		/// </summary>
		/// <param name="rhs">The String Data</param>
		/// <returns>Datum after assigning the data</returns>
		Datum& operator=(const std::string& rhs);

		/// <summary>
		/// Scalar assignment operator overload for floats
		/// </summary>
		/// <param name="rhs">The float data</param>
		/// <returns>Datum after assigning the data</returns>
		Datum& operator=(const float& rhs);

		/// <summary>
		/// Scalar assignment operator overload for glm::vec4
		/// </summary>
		/// <param name="rhs">The Vector data in the glm::vec4 container</param>
		/// <returns>Datum after assigning the data</returns>
		Datum& operator=(const glm::vec4& rhs);

		/// <summary>
		/// Scalar assignment operator overload for glm::mat4x4
		/// </summary>
		/// <param name="rhs">The Mat4x4 data in the glm::mat4x4 cotainer</param>
		/// <returns>Datum after assigning the data</returns>
		Datum& operator=(const glm::mat4x4& rhs);

		/// <summary>
		/// Scalara assignment operator overload for RTTI*
		/// </summary>
		/// <param name="rhs">The RTTI pointer</param>
		/// <returns>Datum after assigning the data</returns>
		Datum& operator=(RTTI* const rhs);
		
		Datum& operator=(Scope* const rhs);	//TODO

		/// <summary>
		/// Overloaded Equality operator
		/// </summary>
		/// <param name="rhs">The datum to be compared with</param>
		/// <returns>A bool representing the state of equality</returns>
		bool operator==(const Datum& rhs) const;

		/// <summary>
		/// Overloaded Inequality operator
		/// </summary>
		/// <param name="rhs">The datum to be compared with</param>
		/// <returns>A bool representing the state of inequality</returns>
		bool operator!=(const Datum& rhs) const;

		/// <summary>
		/// Overloaded Scalar Equality operator for integers
		/// </summary>
		/// <param name="rhs">The integer to be compared too</param>
		/// <returns>A bool represnting the state of equality between the integer and the first element of the Datum</returns>
		bool operator==(const int32_t& rhs) const;

		/// <summary>
		///  Overloaded Scalar Equality operator for floats
		/// </summary>
		/// <param name="rhs">The float to be compared too</param>
		/// <returns>A bool represnting the state of equality between the float and the first element of the Datum</returns>
		bool operator==(const float& rhs) const;

		/// <summary>
		/// Overloaded Scalar Equality operator for strings
		/// </summary>
		/// <param name="rhs">The string to be compared too</param>
		/// <returns>A bool represnting the state of equality between the string and the first element of the Datum</returns>
		bool operator==(const std::string& rhs) const;

		/// <summary>
		///  Overloaded Scalar Equality operator for glm::vec4
		/// </summary>
		/// <param name="rhs">The glm::vec4 to be compared too</param>
		/// <returns>A bool represnting the state of equality between the vec4 and the first element of the Datum</returns>
		bool operator==(const glm::vec4& rhs) const;

		/// <summary>
		///  Overloaded Scalar Equality operator for glm::mat4x4
		/// </summary>
		/// <param name="rhs">The glm::mat4x4 to be compared too</param>
		/// <returns>A bool represnting the state of equality between the Mat4x4 and the first element of the Datum</returns>
		bool operator==(const glm::mat4x4& rhs) const;

		/// <summary>
		///  Overloaded Scalar Equality operator for RTTI*
		/// </summary>
		/// <param name="rhs">The RTTI* to be compared too</param>
		/// <returns>A bool represnting the state of equality between the RTTI* and the first element of the Datum</returns>
		bool operator==(RTTI* rhs) const;

		//bool operator==(Scope* rhs) const;	//TODO

		/// <summary>
		/// Overloaded Scalar inequality operator for Integers
		/// </summary>
		/// <param name="rhs">The Integer to be compared too</param>
		/// <returns>A bool represnting the state of equality between the integer and the first element of the Datum</returns>
		bool operator!=(const int32_t& rhs) const;

		/// <summary>
		/// Overloaded Scalar inequality operator for floats
		/// </summary>
		/// <param name="rhs">The float to be compared too</param>
		/// <returns></returns>
		bool operator!=(const float& rhs) const;

		/// <summary>
		/// Overloaded Scalar inequality operator for std::string
		/// </summary>
		/// <param name="rhs">The string to be compared too</param>
		/// <returns></returns>
		bool operator!=(const std::string& rhs) const;

		/// <summary>
		/// Overloaded Scalar inequality operator for glm::vec4
		/// </summary>
		/// <param name="rhs">The glm::vec4 to be compared too</param>
		/// <returns></returns>
		bool operator!=(const glm::vec4& rhs) const;

		/// <summary>
		/// Overloaded Scalar inequality operator for glm::mat4x4
		/// </summary>
		/// <param name="rhs">The glm::mat4x4 to be compared too</param>
		/// <returns></returns>
		bool operator!=(const glm::mat4x4& rhs) const;

		/// <summary>
		/// Overloaded Scalar inequality operator for RTTI*
		/// </summary>
		/// <param name="rhs">The RTTI* to be compared too</param>
		/// <returns></returns>
		bool operator!=(RTTI* rhs) const;

		//bool operator!=(Scope *rhs) const;		//TODO

		Scope& operator[](const uint32_t& index);		//TODO

		/// <summary>
		/// Get the type of the Datum
		/// </summary>
		/// <returns>Returns the Type of the datum in the form of the enum DatumType</returns>
		DatumType GetType() const;

		/// <summary>
		/// Sets the type of the Datum to the given the type
		/// </summary>
		/// <param name="newDatumType">The type the Datum must be set too</param>
		void SetType(const DatumType& newDatumType);

		/// <summary>
		/// Returns the total number of elements in the Datum
		/// </summary>
		/// <returns></returns>
		uint32_t Length() const;

		/// <summary>
		/// Resizes the Datum to the given Size, also sets Capacity=Size
		/// </summary>
		/// <param name="size">The new Size of the datum</param>
		void Resize(const uint32_t& size);

		/// <summary>
		/// Clears all data in the Datum provided the Datum is of internal storage. Does nothing if the Datum is external storage
		/// </summary>
		void Clear();

		/// <summary>
		/// Set External Storage for the Datum
		/// </summary>
		/// <param name="valueArray">Pointer to external integer data</param>
		/// <param name="size">The size of the external data</param>
		void SetStorage(int32_t* valueArray, const int32_t& size);

		/// <summary>
		/// Set External Storage for the Datum
		/// </summary>
		/// <param name="valueArray">Pointer to external float data</param>
		/// <param name="size">The size of the external data</param>
		void SetStorage(float* valueArray, const int32_t& size);

		/// <summary>
		/// Set External Storage for the Datum
		/// </summary>
		/// <param name="valueArray">Pointer to external string data</param>
		/// <param name="size">The size of the external data</param>
		void SetStorage(std::string* valueArray, const int32_t& size);


		/// <summary>
		/// Set External Storage for the Datum
		/// </summary>
		/// <param name="valueArray">Pointer to external glm::vec4 data</param>
		/// <param name="size">The size of the external data</param>
		void SetStorage(glm::vec4* valueArray, const int32_t& size);


		/// <summary>
		/// Set External Storage for the Datum
		/// </summary>
		/// <param name="valueArray">Pointer to external glm::mat4x4 data</param>
		/// <param name="size">The size of the external data</param>
		void SetStorage(glm::mat4x4* valueArray, const int32_t& size);


		/// <summary>
		/// Set External Storage for the Datum
		/// </summary>
		/// <param name="valueArray">Pointer to external RTTI** data</param>
		/// <param name="size">The size of the external data</param>
		void SetStorage(RTTI** valueArray, const int32_t& size);

		void SetStorage(void* externalStorage, const int32_t size);

		//void SetStorage(Scope** valueArray, const int32_t& size);	//TODO

		/// <summary>
		/// Set data with the given value at the given location. The location defaults to zero if it's not provided.
		/// Datum must exist in the Datum before it can be set
		/// </summary>
		/// <param name="value">Value of the data to set</param>
		/// <param name="index">The index at which data must be set with given value</param>
		void Set(const int32_t& value, const uint32_t& index = 0);

		/// <summary>
		/// Set data with the given value at the given location. The location defaults to zero if it's not provided.
		/// Datum must exist in the Datum before it can be set
		/// </summary>
		/// <param name="value">Value of the data to set</param>
		/// <param name="index">The index at which data must be set with given value</param>
		void Set(const float& value, const uint32_t& index = 0);

		/// <summary>
		/// Set data with the given value at the given location. The location defaults to zero if it's not provided.
		/// Datum must exist in the Datum before it can be set
		/// </summary>
		/// <param name="value">Value of the data to set</param>
		/// <param name="index">The index at which data must be set with given value</param>
		void Set(const std::string& value, const uint32_t& index = 0);

		/// <summary>
		/// Set data with the given value at the given location. The location defaults to zero if it's not provided.
		/// Datum must exist in the Datum before it can be set
		/// </summary>
		/// <param name="value">Value of the data to set</param>
		/// <param name="index">The index at which data must be set with given value</param>
		void Set(const glm::vec4& value, const uint32_t& index = 0);

		/// <summary>
		/// Set data with the given value at the given location. The location defaults to zero if it's not provided.
		/// Datum must exist in the Datum before it can be set
		/// </summary>
		/// <param name="value">Value of the data to set</param>
		/// <param name="index">The index at which data must be set with given value</param>
		void Set(const glm::mat4x4& value, const uint32_t& index = 0);

		/// <summary>
		/// Set data with the given value at the given location. The location defaults to zero if it's not provided.
		/// Datum must exist in the Datum before it can be set
		/// </summary>
		/// <param name="value">Value of the data to set</param>
		/// <param name="index">The index at which data must be set with given value</param>
		void Set(RTTI* value, const uint32_t& index = 0);

		void Set(Scope* value, const uint32_t& index = 0);		//TODO

		/// <summary>
		/// Pushback given value into the Datum
		/// </summary>
		/// <param name="value">The value to be pushed back</param>
		void PushBack(const int32_t& value);

		/// <summary>
		/// Pushback given value into the Datum
		/// </summary>
		/// <param name="value">The value to be pushed back</param>
		void PushBack(const float& value);

		/// <summary>
		/// Pushback given value into the Datum
		/// </summary>
		/// <param name="value">The value to be pushed back</param>
		void PushBack(const std::string& value);

		/// <summary>
		/// Pushback given value into the Datum
		/// </summary>
		/// <param name="value">The value to be pushed back</param>
		void PushBack(const glm::vec4& value);

		/// <summary>
		/// Pushback given value into the Datum
		/// </summary>
		/// <param name="value">The value to be pushed back</param>
		void PushBack(const glm::mat4x4& value);

		/// <summary>
		/// Pushback given value into the Datum
		/// </summary>
		/// <param name="value">The value to be pushed back</param>
		void PushBack(RTTI* value);

		void PushBack(Scope* value);	//TODO

		/// <summary>
		/// Remove last element from the Datum
		/// </summary>
		void PopBack();

		/// <summary>
		/// templated Get function to obtain data from the Datum. Get will fail if the type of the datum does not match the get that was used
		/// </summary>
		template<typename T>
		T& Get(const uint32_t& index = 0);

		/// <summary>
		/// Specialized Get for Integers returning integer data
		/// </summary>
		template<>
		int32_t& Get<int32_t>(const uint32_t& index);

		/// <summary>
		/// Specialized Get for Integers returning Float data
		/// </summary>
		template<>
		float& Get<float>(const uint32_t& index);

		/// <summary>
		/// Specialized Get for Integers returning String data
		/// </summary>
		template<>
		std::string& Get<std::string>(const uint32_t& index);

		/// <summary>
		/// Specialized Get for Integers returning glm::vec4 data
		/// </summary>
		template<>
		glm::vec4& Get<glm::vec4>(const uint32_t& index);

		/// <summary>
		/// Specialized Get for Integers returning glm:mat4x4 data
		/// </summary>
		template<>
		glm::mat4x4& Get<glm::mat4x4>(const uint32_t& index);

		/// <summary>
		/// Specialized Get for Integers returning RTTI* data
		/// </summary>
		template<>
		RTTI*& Get<RTTI*>(const uint32_t& index);

		template<>
		Scope*& Get<Scope*>(const uint32_t& index);

		/// <summary>
		/// Const versions of get functions below
		/// </summary>
		template<typename T>
		const T& Get(const uint32_t& index = 0) const;

		/// <summary>
		/// Specialized Get for Integers returning integer data
		/// </summary>
		template<>
		const int32_t& Get<int32_t>(const uint32_t& index) const;

		/// <summary>
		/// Specialized Get for Integers returning Float data
		/// </summary>
		template<>
		const float& Get<float>(const uint32_t& index) const;

		/// <summary>
		/// Specialized Get for Integers returning String data
		/// </summary>
		template<>
		const std::string& Get<std::string>(const uint32_t& index) const;

		/// <summary>
		/// Specialized Get for Integers returning glm::vec4 data
		/// </summary>
		template<>
		const glm::vec4& Get<glm::vec4>(const uint32_t& index) const;

		/// <summary>
		/// Specialized Get for Integers returning glm:mat4x4 data
		/// </summary>
		template<>
		const glm::mat4x4& Get<glm::mat4x4>(const uint32_t& index) const;

		/// <summary>
		/// Specialized Get for Integers returning RTTI* data
		/// </summary>
		template<>
		RTTI* const & Get<RTTI*>(const uint32_t& index) const;

		template<>
		Scope* const & Get<Scope*>(const uint32_t& index) const;


		/// <summary>
		/// Set data from the given string, can only set data that is of the current type
		/// </summary>
		/// <param name="data">The data in std::string format</param>
		/// <param name="index">The index at which data needs to be set</param>
		void SetFromString(const std::string& data,const uint32_t& index = 0);

		/// <summary>
		/// Convert the data at the given index to a string format
		/// </summary>
		/// <param name="index">The index of the data that needs to be converted into a string</param>
		/// <returns>The data in string format</returns>
		std::string ToString(const uint32_t& index = 0);

		/// <summary>
		/// Reserves capacity in the Datum, it is to be noted that Reserve can only increase capacity
		/// </summary>
		/// <param name="capacity">The new capacity</param>
		void Reserve(uint32_t capacity);

		/// <summary>
		/// Find returns the index of the element if found, returns Size otherwise
		/// </summary>
		/// <param name="data">The data to be found</param>
		/// <returns>The index containing the element, size otherwise</returns>
		uint32_t Find(const int32_t& data);

		/// <summary>
		/// Find returns the index of the element if found, returns Size otherwise
		/// </summary>
		/// <param name="data">The data to be found</param>
		/// <returns>The index containing the element, size otherwise</returns>
		uint32_t Find(const float& data);

		/// <summary>
		/// Find returns the index of the element if found, returns Size otherwise
		/// </summary>
		/// <param name="data">The data to be found</param>
		/// <returns>The index containing the element, size otherwise</returns>
		uint32_t Find(const std::string& data);

		/// <summary>
		/// Find returns the index of the element if found, returns Size otherwise
		/// </summary>
		/// <param name="data">The data to be found</param>
		/// <returns>The index containing the element, size otherwise</returns>
		uint32_t Find(const glm::vec4& data);

		/// <summary>
		/// Find returns the index of the element if found, returns Size otherwise
		/// </summary>
		/// <param name="data">The data to be found</param>
		/// <returns>The index containing the element, size otherwise</returns>
		uint32_t Find(const glm::mat4x4& data);

		/// <summary>
		/// Find returns the index of the element if found, returns Size otherwise
		/// </summary>
		/// <param name="data">The data to be found</param>
		/// <returns>The index containing the element, size otherwise</returns>
		uint32_t Find(const RTTI* data);

		//uint32_t Find(const Scope* data);		//TODO

		/// <summary>
		/// Removes the given data from the Datum if it exists, does nothing otherwise
		/// </summary>
		/// <param name="data">The Data to be removed if found</param>
		/// <returns>A bool determining whether the remove succeeded</returns>
		bool Remove(const int32_t& data);

		/// <summary>
		/// Removes the given data from the Datum if it exists, does nothing otherwise
		/// </summary>
		/// <param name="data">The Data to be removed if found</param>
		/// <returns>A bool determining whether the remove succeeded</returns>
		bool Remove(const float& data);

		/// <summary>
		/// Removes the given data from the Datum if it exists, does nothing otherwise
		/// </summary>
		/// <param name="data">The Data to be removed if found</param>
		/// <returns>A bool determining whether the remove succeeded</returns>
		bool Remove(const std::string& data);

		/// <summary>
		/// Removes the given data from the Datum if it exists, does nothing otherwise
		/// </summary>
		/// <param name="data">The Data to be removed if found</param>
		/// <returns>A bool determining whether the remove succeeded</returns>
		bool Remove(const glm::vec4& data);

		/// <summary>
		/// Removes the given data from the Datum if it exists, does nothing otherwise
		/// </summary>
		/// <param name="data">The Data to be removed if found</param>
		/// <returns>A bool determining whether the remove succeeded</returns>
		bool Remove(const glm::mat4x4& data);

		/// <summary>
		/// Removes the given data from the Datum if it exists, does nothing otherwise
		/// </summary>
		/// <param name="data">The Data to be removed if found</param>
		/// <returns>A bool determining whether the remove succeeded</returns>
		bool Remove(const RTTI* data);

		//bool Remove(const Scope* data); //TODO

		/// <summary>
		/// Removes data from the given index as long as the index given is valid
		/// </summary>
		/// <param name="index">The index at which data is supposed to be removed</param>
		/// <returns>A bool determining whetehr the remove succeeded</returns>
		bool RemoveWithIndex(const uint32_t& index);

		bool IsExternal();
							   

	private:
		std::string IntToString(const uint32_t& index);
		std::string FloatToString(const uint32_t& index);
		std::string VecToString(const uint32_t& index);
		std::string MatToString(const uint32_t& index);
		std::string StringWrapper(const uint32_t& index);
		std::string InvalidType(const uint32_t& index);
		std::string RTTItoString(const uint32_t& index);

		void IntFromString(const std::string& data, const uint32_t& index);
		void FloatFromString(const std::string& data, const uint32_t& index);
		void Vec4FromString(const std::string& data, const uint32_t& index);
		void Mat4x4FromString(const std::string& data, const uint32_t& index);
		void InvalidConversion(const std::string& data, const uint32_t& index);

		void RemoveIntWithIndex(const uint32_t& index);
		void RemoveFloatWithIndex(const uint32_t& index);
		void RemoveStringWithIndex(const uint32_t& index);
		void RemoveVectorWithIndex(const uint32_t& index);
		void RemoveMatrixWithIndex(const uint32_t& index);
		void RemoveRTTIWithIndex(const uint32_t& index);
		void RemoveTableWithIndex(const uint32_t& index);
		

		union DatumValues
		{
			int32_t		*IntValues;
			float		*FloatValues;
			std::string *StringValues;
			glm::vec4	*Vec4Values;
			glm::mat4x4 *Mat4x4Values;
			RTTI		**RTTIPointerArray;
			Scope		**ScopePointerArray;
			void		*GeneralPurposePointer;

			DatumValues();
		};
		DatumValues Data;
		DatumType Type;
		uint32_t Size;
		uint32_t Capacity;
		bool IsExternalStorage;

		static size_t TypeSizeMap[];
	};


	template<>
	int32_t& Datum::Get<int32_t>(const uint32_t& index)
	{
		if (index > Size || Type != DatumType::Integer)
		{
			throw std::runtime_error("Invalid Index");
		}
		return Data.IntValues[index];
	}

	template<>
	glm::vec4& Datum::Get<glm::vec4>(const uint32_t& index)
	{
		if (index > Size || Type != DatumType::Vector4)
		{
			throw std::runtime_error("Invalid Index");
		}
		return Data.Vec4Values[index];
	}

	template<>
	float& Datum::Get<float>(const uint32_t& index)
	{
		if (index > Size || Type != DatumType::Float)
		{
			throw std::runtime_error("Invalid Index");
		}
		return Data.FloatValues[index];
	}

	template<>
	glm::mat4x4& Datum::Get<glm::mat4x4>(const uint32_t& index)
	{
		if (index > Size || Type != DatumType::Matrix4x4)
		{
			throw std::runtime_error("Invalid Index");
		}
		return Data.Mat4x4Values[index];
	}

	template<>
	std::string& Datum::Get<std::string>(const uint32_t& index)
	{
		if (index > Size || Type != DatumType::String)
		{
			throw std::runtime_error("Invalid Index");
		}
		return Data.StringValues[index];
	}

	template<>
	RTTI*& Datum::Get<RTTI*>(const uint32_t& index)
	{
		if (index > Size || Type != DatumType::RTTIPointer)
		{
			throw std::runtime_error("Invalid Index");
		}
		return Data.RTTIPointerArray[index];
	}

	template<>
	Scope*& Datum::Get<Scope*>(const uint32_t& index)
	{
		if (index > Size || Type != DatumType::Table)
		{
			throw std::runtime_error("Invalid Index");
		}
		return Data.ScopePointerArray[index];
	}

	template<>
	const int32_t& Datum::Get<int32_t>(const uint32_t& index) const
	{
		if (index > Size || Type != DatumType::Integer)
		{
			throw std::runtime_error("Invalid Index");
		}
		return Data.IntValues[index];
	}

	template<>
	const glm::vec4& Datum::Get<glm::vec4>(const uint32_t& index) const
	{
		if (index > Size || Type != DatumType::Vector4)
		{
			throw std::runtime_error("Invalid Index");
		}
		return Data.Vec4Values[index];
	}

	template<>
	const float& Datum::Get<float>(const uint32_t& index) const
	{
		if (index > Size || Type != DatumType::Float)
		{
			throw std::runtime_error("Invalid Index");
		}
		return Data.FloatValues[index];
	}

	template<>
	const glm::mat4x4& Datum::Get<glm::mat4x4>(const uint32_t& index) const
	{
		if (index > Size || Type != DatumType::Matrix4x4)
		{
			throw std::runtime_error("Invalid Index");
		}
		return Data.Mat4x4Values[index];
	}

	template<>
	const std::string& Datum::Get<std::string>(const uint32_t& index) const
	{
		if (index > Size || Type != DatumType::String)
		{
			throw std::runtime_error("Invalid Index");
		}
		return Data.StringValues[index];
	}

	template<>
	RTTI* const & Datum::Get<RTTI*>(const uint32_t& index) const
	{
		if (index > Size || Type != DatumType::RTTIPointer)
		{
			throw std::runtime_error("Invalid Index");
		}
		return Data.RTTIPointerArray[index];
	}

	template<>
	Scope* const & Datum::Get<Scope*>(const uint32_t& index) const
	{
		if (index > Size || Type != DatumType::RTTIPointer)
		{
			throw std::runtime_error("Invalid Index");
		}
		return Data.ScopePointerArray[index];
	}
}

