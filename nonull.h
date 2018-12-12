#ifndef DRAGAZO_NONULL
#define DRAGAZO_NONULL

#include <iostream>
#include <memory>
#include <exception>
#include <stdexcept>
#include <type_traits>

// defines a wrapper for T that is not allowed to be null.
// any attempt to set it to null will result in an exception of type std::invalid_argument.
// ----------------------------------------------------------
// the following concern requirements for T:
//  - must be comparable to and constructable/assignable from nullptr.
//  -  copy construction/assignment (if applicable) must not result in a null value for either object.
//  - move construction/assignment (if applicable) may result in a null value for the moved-from object.
//  - must have operators * and -> (which are const).
//  - may optionally have [] or () operators.
// the following concern the guarantees of this wrapper:
//  - at all times, when in a defined state, will contain a non-null value of type T.
//  - copying a nonull value is well-defined (if T permits it).
//  - moving from a nonull value places it in an undefined, but valid, state based on T (for raw pointers, no change).
template<typename T>
class nonull
{
private: // -- data -- //

	T value; // the actual wrapped value

public: // -- standard C++ intrinsics -- //

	nonull() = delete; // can't default construct nonull
	~nonull() = default;

	nonull(const nonull&) = default;
	nonull(nonull&&) = default;

	nonull &operator=(const nonull&) = default;
	nonull &operator=(nonull&&) = default;

public: // -- value ctor/asgn -- //

	nonull(std::nullptr_t) = delete;
	nonull &operator=(std::nullptr_t) = delete;

	template<typename U>
	nonull(U &&new_value) : value(std::forward<U>(new_value))
	{
		if (value == nullptr) throw std::invalid_argument("nonull wrapper was constructed null");
	}
	
	template<typename U>
	nonull &operator=(U &&new_value)
	{
		value = std::forward<U>(new_value);
		if (value == nullptr) throw std::invalid_argument("nonull wrapper was assigned null");
		return *this;
	}

public: // -- value access -- //

	template<typename U = T, typename std::enable_if<std::is_same<T, U>::value && std::is_convertible<const T&, T>::value, int>::type = 0>
	operator T() const& { return value; }
	template<typename U = T, typename std::enable_if<std::is_same<T, U>::value && std::is_convertible<T&&, T>::value, int>::type = 0>
	operator T() && { return std::move(value); }

	typename std::conditional<std::is_pointer<T>::value, T, const T&>::type
		operator->() const { return value; }

	decltype(auto) operator*() const { return *value; }

	template<typename U>
	decltype(auto) operator[](U &&arg) const { return value[std::forward<U>(arg)]; }

	template<typename ...Args>
	decltype(auto) operator()(Args &&...args) const { return value(std::forward<Args>(args)...); }
};

// defines a nonull wrapper for a raw pointer
template<typename T>
using nonull_ptr = nonull<T*>;

// defines a nonull wrapper for a std::unique_ptr
template<typename T, typename Deleter = std::default_delete<T>>
using nonull_unique_ptr = nonull<std::unique_ptr<T, Deleter>>;

// defines a nonull wrapper for a std::shared_ptr
template<typename T>
using nonull_shared_ptr = nonull<std::shared_ptr<T>>;

#endif
