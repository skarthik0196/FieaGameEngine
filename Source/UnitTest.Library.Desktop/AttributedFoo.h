#pragma once
#include"Attributed.h"
#include"TypeManager.h"
#include"Vector.h"

class AttributedFoo : public FieaGameEngine::Attributed
{
	RTTI_DECLARATIONS(AttributedFoo, Attributed)

public:
	AttributedFoo();
	~AttributedFoo();

	AttributedFoo(const AttributedFoo& rhs);
	AttributedFoo(AttributedFoo&& rhs);

	AttributedFoo& operator=(const AttributedFoo& rhs);
	AttributedFoo& operator=(AttributedFoo&& rhs);

	bool operator==(const AttributedFoo& rhs);

	void ContrivedValueChangingFunction();

	void UpdateExternalStorage();

	const int32_t& GetE1() const;

	static FieaGameEngine::Vector<FieaGameEngine::Signature> GetSignature();

private:
	void InitializeSignatures();

	int32_t ExternalInteger {0};
	float ExternalFloat {0.0f};
	std::string ExternalString {std::string("")};
	glm::vec4 ExternalVector {glm::vec4()};
	glm::mat4x4 ExternalMatrix {glm::mat4x4(1.0f)};
	RTTI* ExternalRTTI {nullptr};

	int32_t ExternalIntArray[5];
	float ExternalFloatArray[5];
	std::string ExternalStringArray[5];
	glm::vec4 ExternalVectorArray[5];
	glm::mat4x4 ExternalMatrixArray[5];

	Scope* ContrivedScope { nullptr };
};

