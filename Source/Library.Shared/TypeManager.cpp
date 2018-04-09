#include "pch.h"
#include "TypeManager.h"


namespace FieaGameEngine
{
	Signature::Signature(const std::string& name, const Datum::DatumType& type, const size_t& offset, const uint32_t& size)
		: Name(name), Type(type), Offset(offset), Size(size)
	{

	}

	void TypeManager::RegisterType(const uint64_t & runtimeID, Vector<Signature> signatures)
	{
		TypeSignatureMap.Insert(std::make_pair(runtimeID, signatures));
	}

	Vector<Signature>& TypeManager::GetSignatures(const uint64_t& runtimeID)
	{
		return TypeSignatureMap.At(runtimeID);
	}

	void TypeManager::RemoveType(const uint64_t & runtimeID)
	{
		TypeSignatureMap.Remove(runtimeID);
	}

	void TypeManager::ClearAllTypes()
	{
		TypeSignatureMap.Clear();
	}

	HashMap<uint64_t, Vector<Signature>> TypeManager::TypeSignatureMap;
}