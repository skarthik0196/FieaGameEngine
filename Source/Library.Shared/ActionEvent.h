#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Action Event deriving from Action
	/// </summary>
	class ActionEvent : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action)
	public:

		/// <summary>
		/// Constructor
		/// </summary>
		ActionEvent();

		/// <summary>
		/// Parameterized Constructor that takes in a string
		/// </summary>
		/// <param name="name"></param>
		ActionEvent(const std::string& name);

		/// <summary>
		/// Defaulted Copy and Move semantics
		/// </summary>
		/// <param name="rhs"></param>
		ActionEvent(const ActionEvent& rhs) = default;
		ActionEvent(ActionEvent&& rhs) = default;

		ActionEvent& operator=(const ActionEvent& rhs) = default;
		ActionEvent& operator=(ActionEvent&& rhs) = default;

		/// <summary>
		///  Defaulted Destructor
		/// </summary>
		virtual ~ActionEvent() = default;

		/// <summary>
		/// Updated method that takes in worldstate
		/// </summary>
		/// <param name="worldState">Reference to WorldState</param>
		void Update(WorldState& worldState);

		/// <summary>
		/// Get Signatures
		/// </summary>
		/// <returns>Vector of Signature</returns>
		static Vector<Signature> GetSignature();

		/// <summary>
		/// Set SubType
		/// </summary>
		/// <param name="subType">String containing the subtype</param>
		void SetSubType(const std::string& subType);

		/// <summary>
		/// Get Subtype
		/// </summary>
		/// <returns>String</returns>
		const std::string& GetSubType();

		/// <summary>
		/// Set Delay
		/// </summary>
		/// <param name="delay">chrono::milliseconds/duration</param>
		void SetDelay(const std::chrono::milliseconds& delay);

		/// <summary>
		/// Get Delay
		/// </summary>
		/// <returns>chrono::milliseconds</returns>
		const std::chrono::milliseconds GetDelay();

	private:
		uint32_t Delay;
		std::string SubType;
	};
}