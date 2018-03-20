#include "pch.h"
#include "AttributedFoo.h"

using namespace FieaGameEngine;

RTTI_DEFINITIONS(AttributedFoo);

AttributedFoo::AttributedFoo() :Attributed(TypeIdClass())
{
	InitializeSignatures();
}


AttributedFoo::~AttributedFoo()
{

}


AttributedFoo::AttributedFoo(const AttributedFoo& rhs):Attributed(rhs),ExternalInteger(rhs.ExternalInteger), ExternalFloat(rhs.ExternalFloat),
ExternalString(rhs.ExternalString), ExternalVector(rhs.ExternalVector), ExternalMatrix(rhs.ExternalMatrix), ExternalRTTI(rhs.ExternalRTTI)
{
	std::copy(std::begin(rhs.ExternalIntArray), std::end(rhs.ExternalIntArray), stdext::checked_array_iterator<int32_t*>(ExternalIntArray, 5));
	std::copy(std::begin(rhs.ExternalFloatArray), std::end(rhs.ExternalFloatArray), stdext::checked_array_iterator<float*>(ExternalFloatArray, 5));
	std::copy(std::begin(rhs.ExternalStringArray), std::end(rhs.ExternalStringArray), stdext::checked_array_iterator<std::string*>(ExternalStringArray, 5));
	std::copy(std::begin(rhs.ExternalVectorArray), std::end(rhs.ExternalVectorArray), stdext::checked_array_iterator<glm::vec4*>(ExternalVectorArray, 5));
	std::copy(std::begin(rhs.ExternalMatrixArray), std::end(rhs.ExternalMatrixArray), stdext::checked_array_iterator<glm::mat4x4*>(ExternalMatrixArray, 5));

	UpdateExternalStorage();
}

AttributedFoo& AttributedFoo::operator=(const AttributedFoo& rhs)
{
	if (this != &rhs)
	{
		Attributed::operator=(rhs);

		ExternalInteger = rhs.ExternalInteger;
		ExternalFloat = rhs.ExternalFloat;
		ExternalString = rhs.ExternalString;
		ExternalVector = rhs.ExternalVector;
		ExternalMatrix = rhs.ExternalMatrix;
		ExternalRTTI = rhs.ExternalRTTI;

		std::copy(std::begin(rhs.ExternalIntArray), std::end(rhs.ExternalIntArray), stdext::checked_array_iterator<int32_t*>(ExternalIntArray,5));
		std::copy(std::begin(rhs.ExternalFloatArray), std::end(rhs.ExternalFloatArray), stdext::checked_array_iterator<float*>(ExternalFloatArray, 5));
		std::copy(std::begin(rhs.ExternalStringArray), std::end(rhs.ExternalStringArray), stdext::checked_array_iterator<std::string*>(ExternalStringArray, 5));
		std::copy(std::begin(rhs.ExternalVectorArray), std::end(rhs.ExternalVectorArray), stdext::checked_array_iterator<glm::vec4*>(ExternalVectorArray, 5));
		std::copy(std::begin(rhs.ExternalMatrixArray), std::end(rhs.ExternalMatrixArray), stdext::checked_array_iterator<glm::mat4x4*>(ExternalMatrixArray , 5));

		UpdateExternalStorage();
	}

	return *this;
}

AttributedFoo::AttributedFoo(AttributedFoo&& rhs): Attributed(std::move(rhs)), ExternalInteger(std::move(rhs.ExternalInteger)), ExternalFloat(std::move(rhs.ExternalFloat)),
ExternalString(std::move(rhs.ExternalString)), ExternalVector(std::move(rhs.ExternalVector)), ExternalMatrix(std::move(rhs.ExternalMatrix)), ExternalRTTI(std::move(rhs.ExternalRTTI))
{

	std::move(std::begin(rhs.ExternalIntArray), std::end(rhs.ExternalIntArray), stdext::checked_array_iterator<int32_t*>(ExternalIntArray, 5));
	std::move(std::begin(rhs.ExternalFloatArray), std::end(rhs.ExternalFloatArray), stdext::checked_array_iterator<float*>(ExternalFloatArray, 5));
	std::move(std::begin(rhs.ExternalStringArray), std::end(rhs.ExternalStringArray), stdext::checked_array_iterator<std::string*>(ExternalStringArray, 5));
	std::move(std::begin(rhs.ExternalVectorArray), std::end(rhs.ExternalVectorArray), stdext::checked_array_iterator<glm::vec4*>(ExternalVectorArray, 5));
	std::move(std::begin(rhs.ExternalMatrixArray), std::end(rhs.ExternalMatrixArray), stdext::checked_array_iterator<glm::mat4x4*>(ExternalMatrixArray, 5));

	UpdateExternalStorage();
}

AttributedFoo& AttributedFoo::operator=(AttributedFoo&& rhs)
{
	if (this != &rhs)
	{
		Attributed::operator=(std::move(rhs));

		ExternalInteger = std::move(rhs.ExternalInteger);
		ExternalFloat = std::move(rhs.ExternalFloat);
		ExternalString = std::move(rhs.ExternalString);
		ExternalVector = std::move(rhs.ExternalVector);
		ExternalMatrix = std::move(rhs.ExternalMatrix);
		ExternalRTTI = std::move(rhs.ExternalRTTI);

		std::move(std::begin(rhs.ExternalIntArray), std::end(rhs.ExternalIntArray), stdext::checked_array_iterator<int32_t*>(ExternalIntArray, 5));
		std::move(std::begin(rhs.ExternalFloatArray), std::end(rhs.ExternalFloatArray), stdext::checked_array_iterator<float*>(ExternalFloatArray, 5));
		std::move(std::begin(rhs.ExternalStringArray), std::end(rhs.ExternalStringArray), stdext::checked_array_iterator<std::string*>(ExternalStringArray, 5));
		std::move(std::begin(rhs.ExternalVectorArray), std::end(rhs.ExternalVectorArray), stdext::checked_array_iterator<glm::vec4*>(ExternalVectorArray, 5));
		std::move(std::begin(rhs.ExternalMatrixArray), std::end(rhs.ExternalMatrixArray), stdext::checked_array_iterator<glm::mat4x4*>(ExternalMatrixArray, 5));

		UpdateExternalStorage();
	}

	return *this;
}

void AttributedFoo::InitializeSignatures()
{
	AddInternalAttribute("Health", 100, 1);
	AddInternalAttribute("FireRate", 0.5f, 1);
	AddInternalAttribute("Powers", "N/A", 4);
	AddInternalAttribute("Position", glm::vec4(), 1);
	AddInternalAttribute("Transformation Matrix", glm::mat4x4(1.0f), 1);
	AddInternalAttribute("PointerToSelf", this, 1);

	CreateNestedScope("NestedScope").Append("ContrivedValue") = 100;

	ContrivedScope = new Scope();
	AddNestedScope("NestedContrivedScope", *ContrivedScope);
	AddNestedScope("Just Why?");

	AddExternalAttribute("E1", &ExternalInteger, 1);
	AddExternalAttribute("E2", &ExternalFloat, 1);
	AddExternalAttribute("E3", &ExternalString, 1);
	AddExternalAttribute("E4", &ExternalVector, 1);
	AddExternalAttribute("E5", &ExternalMatrix, 1);
	AddExternalAttribute("E6", &ExternalRTTI, 1);

	AddExternalAttribute("Array1", std::begin(ExternalIntArray), 5);
	AddExternalAttribute("Array2", std::begin(ExternalFloatArray), 5);
	AddExternalAttribute("Array3", std::begin(ExternalStringArray), 5);
	AddExternalAttribute("Array4", std::begin(ExternalVectorArray), 5);
	AddExternalAttribute("Array5", std::begin(ExternalMatrixArray), 5);

}

void AttributedFoo::ContrivedValueChangingFunction()
{
	ExternalInteger = 5;
	ExternalFloat = 5.6f;
	ExternalRTTI = this;
	ExternalVector = glm::vec4(3.6f, 6.47f, 6.7f, 1.56f);
	ExternalMatrix = glm::mat4x4(7.0f);
	ExternalString = std::string("Why? Why?");

	int32_t tempArray[5]{ 5,7,3,8,1 };
	std::copy(std::begin(tempArray), std::end(tempArray), stdext::checked_array_iterator<int32_t*>(ExternalIntArray, 5));
}

bool AttributedFoo::operator==(const AttributedFoo& rhs)
{
	bool Result = false;

	Result = (ExternalInteger == rhs.ExternalInteger && ExternalFloat == rhs.ExternalFloat && ExternalString == rhs.ExternalString && ExternalVector == rhs.ExternalVector
			  && ExternalMatrix == rhs.ExternalMatrix && ExternalRTTI == rhs.ExternalRTTI && (std::equal(std::begin(ExternalIntArray), std::end(ExternalIntArray), stdext::checked_array_iterator<int32_t*>(ExternalIntArray, 5))));

	return Result;
}

void AttributedFoo::UpdateExternalStorage()
{
	UpdateStorage("E1", &ExternalInteger, 1);
	UpdateStorage("E2", &ExternalFloat, 1);
	UpdateStorage("E3", &ExternalString, 1);
	UpdateStorage("E4", &ExternalVector, 1);
	UpdateStorage("E5", &ExternalMatrix, 1);
	UpdateStorage("E6", &ExternalRTTI, 1);
	UpdateStorage("Array1", std::begin(ExternalIntArray), 5);
	UpdateStorage("Array2", std::begin(ExternalFloatArray), 5);
	UpdateStorage("Array3", std::begin(ExternalStringArray), 5);
	UpdateStorage("Array4", std::begin(ExternalVectorArray), 5);
	UpdateStorage("Array5", std::begin(ExternalMatrixArray), 5);
}