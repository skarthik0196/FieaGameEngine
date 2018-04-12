#pragma once
#include "Reaction.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Reaction Attributed class that derives from Reaction
	/// </summary>
	class ReactionAttributed : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction)

	public:

		/// <summary>
		/// Constructor
		/// </summary>
		ReactionAttributed();

		/// <summary>
		/// Parameterized Constructor taking in a string 
		/// </summary>
		/// <param name="name">String representing name</param>
		ReactionAttributed(const std::string& name);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Reference to another ReactionAttributed</param>
		ReactionAttributed(const ReactionAttributed& rhs);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Reference to another ReactionAttributed</param>
		ReactionAttributed(ReactionAttributed&& rhs);

		/// <summary>
		/// Virtual Destructor
		/// </summary>
		virtual ~ReactionAttributed();

		/// <summary>
		/// Defaulted Copy and Move Assignment operator
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		ReactionAttributed& operator=(const ReactionAttributed& rhs) = default;
		ReactionAttributed& operator=(ReactionAttributed&& rhs)  = default;

		/// <summary>
		/// Get Signatures
		/// </summary>
		/// <returns>A Vector of Signatures</returns>
		static Vector<Signature> GetSignature();

		/// <summary>
		/// Set SubType
		/// </summary>
		/// <param name="subType">String representing the subtype</param>
		void SetSubType(const std::string& subType);

		/// <summary>
		/// Get Subtype
		/// </summary>
		/// <returns>A string</returns>
		const std::string& GetSubType();

		/// <summary>
		/// Notfiy function to be notified of events
		/// </summary>
		/// <param name="event"></param>
		void Notify(EventPublisher* event);

	private:
		std::string SubType;
	};
}