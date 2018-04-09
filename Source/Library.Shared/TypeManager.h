#pragma once
#include "Datum.h"

namespace FieaGameEngine
{
	class Signature
	{
	public:
		std::string Name;
		Datum::DatumType Type;
		size_t Offset;
		uint32_t Size;

		Signature(const std::string& name, const Datum::DatumType& type, const size_t& offset, const uint32_t& size);
	};

	class TypeManager
	{
	public:
		TypeManager() = delete;
		TypeManager(const TypeManager& rhs) = delete;
		TypeManager(TypeManager&& rhs) = delete;
		~TypeManager() = delete;

		TypeManager& operator=(const TypeManager& rhs) = delete;
		TypeManager& operator=(TypeManager&& rhs) = delete;

		static void RegisterType(const uint64_t& runtimeID, Vector<Signature> signatures);
		static Vector<Signature>& GetSignatures(const uint64_t& runtimeID);
		static void RemoveType(const uint64_t& runtimeID);
		static void ClearAllTypes();

	private:
		static HashMap<uint64_t, Vector<Signature>> TypeSignatureMap;
	};
}

