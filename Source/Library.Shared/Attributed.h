#pragma once
#include"Scope.h"
#include "TypeManager.h"

namespace FieaGameEngine
{
	/// <summary>
	/// The Attributed Class that wraps Scope with the context of Prescribed and Auxilary Attributes
	/// </summary>
	class Attributed : public Scope
	{

		RTTI_DECLARATIONS(Attributed, Scope)
		
	protected:
		/// <summary>
		/// Explicit Protected Constructor to be used by the classes deriving from Attributed to set the RuntimTypeID
		/// </summary>
		explicit Attributed(const uint64_t& runtimeTypeID);

		/// <summary>
		/// Adds an Internal Prescribed Attribute to the underlying Scope
		/// </summary>
		/// <param name="name">The key with which the Attributed is to be added</param>
		/// <param name="defaultValue">Default Value of the variable</param>
		/// <param name="size">The Size of the Attribute</param>
		/// <returns>A reference to the datum that was just created in the scope</returns>
		Datum& AddInternalAttribute(const std::string& name, const int32_t& defaultValue, const uint32_t& size);

		/// <summary>
		/// Adds an Internal Prescribed Attribute to the underlying Scope
		/// </summary>
		/// <param name="name">The key with which the Attributed is to be added</param>
		/// <param name="defaultValue">Default Value of the variable</param>
		/// <param name="size">The Size of the Attribute</param>
		/// <returns>A reference to the datum that was just created in the scope</returns>
		Datum& AddInternalAttribute(const std::string& name, const float& defaultValue, const uint32_t& size);

		/// <summary>
		/// Adds an Internal Prescribed Attribute to the underlying Scope
		/// </summary>
		/// <param name="name">The key with which the Attributed is to be added</param>
		/// <param name="defaultValue">Default Value of the variable</param>
		/// <param name="size">The Size of the Attribute</param>
		/// <returns>A reference to the datum that was just created in the scope</returns>
		Datum& AddInternalAttribute(const std::string& name, const std::string& defaultValue, const uint32_t& size);
		
		/// <summary>
		/// Adds an Internal Prescribed Attribute to the underlying Scope
		/// </summary>
		/// <param name="name">The key with which the Attributed is to be added</param>
		/// <param name="defaultValue">Default Value of the variable</param>
		/// <param name="size">The Size of the Attribute</param>
		/// <returns>A reference to the datum that was just created in the scope</returns>
		Datum& AddInternalAttribute(const std::string& name, const glm::vec4& defaultValue, const uint32_t& size);
		
		/// <summary>
		/// Adds an Internal Prescribed Attribute to the underlying Scope
		/// </summary>
		/// <param name="name">The key with which the Attributed is to be added</param>
		/// <param name="defaultValue">Default Value of the variable</param>
		/// <param name="size">The Size of the Attribute</param>
		/// <returns>A reference to the datum that was just created in the scope</returns>
		Datum& AddInternalAttribute(const std::string& name, const glm::mat4x4& defaultValue, const uint32_t& size);

		/// <summary>
		/// Adds an Internal Prescribed Attribute to the underlying Scope
		/// </summary>
		/// <param name="name">The key with which the Attributed is to be added</param>
		/// <param name="defaultValue">Default Value of the variable</param>
		/// <param name="size">The Size of the Attribute</param>
		/// <returns>A reference to the datum that was just created in the scope</returns>
		Datum& AddInternalAttribute(const std::string& name, RTTI* defaultValue, const uint32_t& size);

		/// <summary>
		/// Adds a nested scope into the underlying scope
		/// </summary>
		/// <param name="name">The key with which the scope is to be added</param>
		/// <returns>A reference to the scope that was just added</returns>
		Datum& AddNestedScope(const std::string& name);
		
		/// <summary>
		/// Adds the given scope to the underlying scope
		/// </summary>
		/// <param name="name">Key under which the scope is to be added</param>
		/// <param name="newScope">The new scope to be added</param>
		/// <returns>A datum reference</returns>
		Datum& AddNestedScope(const std::string& name, Scope& newScope);

		/// <summary>
		/// Creates and Adds a nested scope to the underlying scope
		/// </summary>
		/// <param name="name">Key under which the scope is to be added</param>
		/// <returns>A datum reference to  the new datum</returns>
		Scope& CreateNestedScope(const std::string& name);

		/// <summary>
		/// Adds an external prescribed attribute to the underlying scope
		/// </summary>
		/// <param name="name">The key under which the attribute is to be added</param>
		/// <param name="storageLocation">External Storage Address</param>
		/// <param name="defaultValue">The default value with which the data much be set</param>
		/// <param name="size">The size of the external storage</param>
		/// <returns>A reference to the datum that was just created</returns>
		Datum& AddExternalAttribute(const std::string& name, int32_t* storageLocation, const uint32_t& size);

		/// <summary>
		/// Adds an external prescribed attribute to the underlying scope
		/// </summary>
		/// <param name="name">The key under which the attribute is to be added</param>
		/// <param name="storageLocation">External Storage Address</param>
		/// <param name="defaultValue">The default value with which the data much be set</param>
		/// <param name="size">The size of the external storage</param>
		/// <returns>A reference to the datum that was just created</returns>
		Datum& AddExternalAttribute(const std::string& name, float* storageLocation, const uint32_t& size);

		/// <summary>
		/// Adds an external prescribed attribute to the underlying scope
		/// </summary>
		/// <param name="name">The key under which the attribute is to be added</param>
		/// <param name="storageLocation">External Storage Address</param>
		/// <param name="defaultValue">The default value with which the data much be set</param>
		/// <param name="size">The size of the external storage</param>
		/// <returns>A reference to the datum that was just created</returns>
		Datum& AddExternalAttribute(const std::string& name, std::string* storageLocation, const uint32_t& size);

		/// <summary>
		/// Adds an external prescribed attribute to the underlying scope
		/// </summary>
		/// <param name="name">The key under which the attribute is to be added</param>
		/// <param name="storageLocation">External Storage Address</param>
		/// <param name="defaultValue">The default value with which the data much be set</param>
		/// <param name="size">The size of the external storage</param>
		/// <returns>A reference to the datum that was just created</returns>
		Datum& AddExternalAttribute(const std::string& name, glm::vec4* storageLocation, const uint32_t& size);

		/// <summary>
		/// Adds an external prescribed attribute to the underlying scope
		/// </summary>
		/// <param name="name">The key under which the attribute is to be added</param>
		/// <param name="storageLocation">External Storage Address</param>
		/// <param name="defaultValue">The default value with which the data much be set</param>
		/// <param name="size">The size of the external storage</param>
		/// <returns>A reference to the datum that was just created</returns>
		Datum& AddExternalAttribute(const std::string& name, glm::mat4x4* storageLocation, const uint32_t& size);

		/// <summary>
		/// Adds an external prescribed attribute to the underlying scope
		/// </summary>
		/// <param name="name">The key under which the attribute is to be added</param>
		/// <param name="storageLocation">External Storage Address</param>
		/// <param name="defaultValue">The default value with which the data much be set</param>
		/// <param name="size">The size of the external storage</param>
		/// <returns>A reference to the datum that was just created</returns>
		Datum& AddExternalAttribute(const std::string& name, RTTI** storageLocation, const uint32_t& size);

		/// <summary>
		/// Update the storage of the attribute stored under the given key
		/// </summary>
		/// <param name="name">The key</param>
		/// <param name="storageLocation">The new storage location</param>
		/// <param name="size">The size of the storage</param>
		void UpdateStorage(const std::string& name, int32_t* storageLocation, const uint32_t& size);

		/// <summary>
		/// Update the storage of the attribute stored under the given key
		/// </summary>
		/// <param name="name">The key</param>
		/// <param name="storageLocation">The new storage location</param>
		/// <param name="size">The size of the storage</param>
		void UpdateStorage(const std::string& name, float* storageLocation, const uint32_t& size);

		/// <summary>
		/// Update the storage of the attribute stored under the given key
		/// </summary>
		/// <param name="name">The key</param>
		/// <param name="storageLocation">The new storage location</param>
		/// <param name="size">The size of the storage</param>
		void UpdateStorage(const std::string& name, std::string* storageLocation, const uint32_t& size);

		/// <summary>
		/// Update the storage of the attribute stored under the given key
		/// </summary>
		/// <param name="name">The key</param>
		/// <param name="storageLocation">The new storage location</param>
		/// <param name="size">The size of the storage</param>
		void UpdateStorage(const std::string& name, glm::vec4* storageLocation, const uint32_t& size);

		/// <summary>
		/// Update the storage of the attribute stored under the given key
		/// </summary>
		/// <param name="name">The key</param>
		/// <param name="storageLocation">The new storage location</param>
		/// <param name="size">The size of the storage</param>
		void UpdateStorage(const std::string& name, glm::mat4x4* storageLocation, const uint32_t& size);

		/// <summary>
		/// Update the storage of the attribute stored under the given key
		/// </summary>
		/// <param name="name">The key</param>
		/// <param name="storageLocation">The new storage location</param>
		/// <param name="size">The size of the storage</param>
		void UpdateStorage(const std::string& name, RTTI** storageLocation, const uint32_t& size);

	public:
		/// <summary>
		/// Default constructor that initializes the RuntimTypeID to Attributed's default
		/// </summary>
		Attributed();

		/// <summary>
		/// Virtual Destructor
		/// </summary>
		virtual ~Attributed();

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Reference to the Attributed that must be copied from</param>
		Attributed(const Attributed& rhs);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">Reference to the Attributed that must be moved</param>
		Attributed(Attributed&& rhs);

		/// <summary>
		/// Checks if an attribute under the given key exists
		/// </summary>
		/// <param name="name">The key</param>
		/// <returns>A boolean denoting whether the attribute exists or not</returns>
		bool IsAttribute(const std::string& name) const;

		/// <summary>
		/// Checks if a prescribed attribute under the given name exists
		/// </summary>
		/// <param name="name">The key</param>
		/// <returns>A boolean denoting if the given prescribed attribute exists</returns>
		bool IsPrescribedAttribute(const std::string& name) const;

		/// <summary>
		/// Checks if an auxillary attribute under the given name exists
		/// </summary>
		/// <param name="name">They key</param>
		/// <returns>A boolean denoting if the given auxillary attribute exists</returns>
		bool IsAuxillaryAttribute(const std::string& name) const;

		/// <summary>
		/// Appens an auxillary attribute to the underlying scope
		/// </summary>
		/// <param name="name">The key under which the auxillary attribute must be appended</param>
		/// <returns>A reference to the Datum that was created</returns>
		Datum& AppendAuxillaryAttribute(const std::string& name);

		/// <summary>
		/// Get All prescribed attributes in the underlying scope
		/// </summary>
		/// <returns>A vector contatining all prescribed attributes</returns>
		Vector<Signature/*std::string*/> GetPrescribedAttributes() const;

		/// <summary>
		/// Get all Auxillary attributes in the underlying scope
		/// </summary>
		/// <returns>A Vector containing all auxiullary attributes</returns>
		Vector<std::string> GetAuxillaryAttributes() const;
		
		/// <summary>
		/// Get all Attributes in the underlying scope
		/// </summary>
		/// <returns>A Vector containing all attributes</returns>
		Vector<std::string> GetAttributes() const;

		/// <summary>
		/// Overloaded Copy assignment operator
		/// </summary>
		/// <param name="rhs">Reference to the attribute to be copied from</param>
		/// <returns>The attributed object after copying</returns>
		Attributed& operator=(const Attributed& rhs);

		/// <summary>
		/// Overloaded Move assignment operator
		/// </summary>
		/// <param name="rhs">Reference to the attribute to be move</param>
		/// <returns>The attributed object after the move</returns>
		Attributed& operator=(Attributed&& rhs);

		void Populate(const uint64_t& runtimeID);

	private:
		void AddPrescribedKey(const std::string& name);

		static HashMap<uint64_t, Attributed*> FirstEntry;
		static HashMap<uint64_t, Vector<std::string>> PrescribedAttributes;
	};
}