#pragma once
#include "ActionList.h"
#include "EventSubscriber.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Abstract Reaction Class
	/// </summary>
	class Reaction  : public ActionList, public EventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList)

	public:
		/// <summary>
		/// Defaulted Constructors and Assignment operators
		/// </summary>
		Reaction() = default;
		Reaction(const Reaction& rhs) = default;
		Reaction(Reaction&& rhs) = default;

		Reaction& operator=(const Reaction& rhs) = default;
		Reaction& operator=(Reaction&& rhs) = default;

		/// <summary>
		/// Defaulted Destructor
		/// </summary>
		virtual ~Reaction() = default;
		
		/// <summary>
		/// Get Signatures
		/// </summary>
		/// <returns>Vector of Signatures</returns>
		static Vector<Signature> GetSignature();

	protected:
		explicit Reaction(const uint64_t& runtimeTypeID, const std::string& name);
		explicit Reaction(const uint64_t& runtimeTypeID);
	};
}

