#pragma once
#include "Attributed.h"

namespace FieaGameEngine
{
	class World;

	/// <summary>
	/// Event Message Attributed that contains a common payload
	/// </summary>
	class EventMessageAttributed : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed)
	public:
		/// <summary>
		/// Constructor
		/// </summary>
		EventMessageAttributed();

		/// <summary>
		/// Defaulted Copy and Move semantics and
		/// </summary>
		EventMessageAttributed(const EventMessageAttributed& rhs) = default;
		EventMessageAttributed(EventMessageAttributed&& rhs) = default;

		EventMessageAttributed& operator=(const EventMessageAttributed& rhs) = default;
		EventMessageAttributed& operator=(EventMessageAttributed&& rhs) = default;

		/// <summary>
		/// Defaulted Destructor
		/// </summary>
		virtual ~EventMessageAttributed() = default;

		/// <summary>
		/// Set Subtype
		/// </summary>
		/// <param name="subType">String</param>
		void SetSubType(const std::string& subType);

		/// <summary>
		/// Set World
		/// </summary>
		/// <param name="world">Reference to World</param>
		void SetWorld(World& world);

		/// <summary>
		/// Get Subtype
		/// </summary>
		/// <returns>String reference</returns>
		const std::string& GetSubType() const;

		/// <summary>
		/// Get World
		/// </summary>
		/// <returns>World Pointer</returns>
		World* GetWorld() const;

		/// <summary>
		/// Get Signatures
		/// </summary>
		/// <returns>Vector of Signatures</returns>
		static Vector<Signature> GetSignature();

	private:
		std::string SubType;
		World* WorldPointer;
	};
}

