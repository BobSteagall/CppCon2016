//==================================================================================================
//  File:
//      synthetic_pointer_interface.h
//
//  Summary:
//      Defines a very simple heap class for testing rhx_allocator.
//==================================================================================================
//
#ifndef SYNTHETIC_POINTER_INTERFACE_H_DEFINED
#define SYNTHETIC_POINTER_INTERFACE_H_DEFINED

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <iterator>

//--------------------------------------------------------------------------------------------------
//  Class:
//      synthetic_pointer_traits
//
//  Summary:
//      This class provides parameterized type aliases for using SFINAE with conversion
//      constructors, conversion operators, and comparison operators.
//--------------------------------------------------------------------------------------------------
struct synthetic_pointer_traits
{
    template<class From, class To>
    using implicitly_convertible =
        typename std::enable_if<std::is_convertible<From*, To*>::value, bool>::type;

    template<class From, class To>
    using explicit_conversion_required =
        typename std::enable_if<!std::is_convertible<From*, To*>::value, bool>::type;

    template<class T1, class T2>
    using implicitly_comparable =
        typename std::enable_if<std::is_convertible<T1*, T2 const*>::value ||
        std::is_convertible<T2*, T1 const*>::value, bool>::type;
};

#include "synthetic_typed_pointer_interface.h"
#include "synthetic_void_pointer_interface.h"
#include "synthetic_pointer_compare_ops.h"

#endif  //- SYNTHETIC_POINTER_INTERFACE_H_DEFINED
