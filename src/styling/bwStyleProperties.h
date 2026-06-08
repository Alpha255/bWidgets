#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace bWidgets
{
	class bwColor;

	/**
	 * \class bwStyleProperty
	 * \brief Simple class for managing properties that can be manipulated through
	 *        stylesheets (CSS).
	 *
	 * Idea of the bwStyleProperty is to add a string identifier to a variable
	 * (basic RTTI) so that stylesheet rules can be mapped to this concrete
	 * variable. Use the bwStyleProperties class to manage (add, lookup and
	 * iterate) a list of properties.
	 *
	 * It is possible to reference an existing variable with a bwStyleProperty,
	 * meaning the property is __not__ owner of it. To do so call the
	 * bwStyleProperties::addFoo() function with the reference argument set. Such
	 * usage looks like this:
	 * \code
	 * bwStyleProperties properties;
	 * int32_t some_int = 0;
	 *
	 * bwStyleProperty& property = properties.addInteger("int_name", some_int);
	 * property.setValue(42);
	 * assert(some_int == 42);
	 * \endcode
	 */

	class bwStyleProperty
	{
		template<typename> friend class bwStylePropertyInternal;

	public:
		enum class Type
		{
			BOOL,
			INTEGER,
			FLOAT,
			COLOR,
		};

		void setValue(bool);
		void setValue(int32_t);
		void setValue(float);
		void setValue(const bwColor&);
		void setValue(const bwStyleProperty&);
		void setValueToDefault();

		void setDefaultValue(bool);
		void setDefaultValue(int32_t);
		void setDefaultValue(float);
		void setDefaultValue(const bwColor&);

		std::string_view getIdentifier() const;
		Type getType() const;
		std::string getValueString() const;

	private:
		bwStyleProperty(std::string_view identifier, enum Type type);

		const std::string identifier;
		enum Type type;
	};

	/**
	 * \class bwStyleProperties
	 * \brief Manage a list of properties (bwStyleProperty instances).
	 *
	 * This class can be used to do the following:
	 * * Add/register new properties (addFoo() functions).
	 * * Lookup a property from its identifier (lookup() function).
	 * * Get iterators to iterate over all properties.
	 */
	class bwStyleProperties
	{
	public:
		// Store properties as pointer, they are actually created as bwStylePropertyInternal instances.
		using PropertyList = std::vector<std::unique_ptr<bwStyleProperty>>;
		using iterator = PropertyList::iterator;
		using const_iterator = PropertyList::const_iterator;

		bwStyleProperty& addBool(const std::string_view& name, bool& reference);
		bwStyleProperty& addBool(const std::string_view& name);
		bwStyleProperty& addInteger(const std::string_view& name, int32_t& reference);
		bwStyleProperty& addInteger(const std::string_view& name);
		bwStyleProperty& addFloat(const std::string_view& name, float& reference);
		bwStyleProperty& addFloat(const std::string_view& name);
		bwStyleProperty& addColor(const std::string_view& name, class bwColor& reference);
		bwStyleProperty& addColor(const std::string_view& name);
		bwStyleProperty& addProperty(const std::string_view& name, const bwStyleProperty::Type prop_type);

		const bwStyleProperty* lookup(const std::string_view& name) const;

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;

	private:
		PropertyList properties{};
	};

}  // namespace bWidgets
