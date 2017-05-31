//==================================================================================================
//  File:
//      synthetic_void_pointer_interface.h
//
//  Summary:
//      Partial specialization synthetic_pointer<void, AM>.
//==================================================================================================
//
#ifndef SYNTHETIC_VOID_POINTER_INTERFACE_H_DEFINED
#define SYNTHETIC_VOID_POINTER_INTERFACE_H_DEFINED

#include <cstddef>
#include <cstdint>
#include <type_traits>

//--------------------------------------------------------------------------------------------------
//  Class:
//      synthetic_pointer<void, AM>
//
//  Summary:
//      Partial specialization of synthetic_pointer<T, AM>, removing arithmetic, indexing,
//      and dereferencing operations.
//--------------------------------------------------------------------------------------------------
//
template<class AM>
class synthetic_pointer<void, AM>
{
  public:
    //- Re-binder alias required for C++11 support.
    //
    template<class U>
    using rebind = synthetic_pointer<U, AM>;

    //- Other aliases required by allocator_traits<T>, pointer_traits<T>, and the containers.
    //
    using difference_type   = typename AM::difference_type;
    using pointer           = synthetic_pointer;

  public:
    //- Special member functions - make intentions explicit.
    //
    ~synthetic_pointer() noexcept = default;

    synthetic_pointer() noexcept = default;
    synthetic_pointer(synthetic_pointer&&) noexcept = default;
    synthetic_pointer(synthetic_pointer const&) noexcept = default;

    synthetic_pointer&  operator =(synthetic_pointer&&) noexcept = default;
    synthetic_pointer&  operator =(synthetic_pointer const&) noexcept = default;

    //- User-defined construction.  Allow only implicit conversion at compile time.
    //
    synthetic_pointer(AM am);
    synthetic_pointer(std::nullptr_t);
    template<class U, synthetic_pointer_traits::implicitly_convertible<U, void> = true>
    synthetic_pointer(U* p);
    template<class U, synthetic_pointer_traits::implicitly_convertible<U, void> = true>
    synthetic_pointer(synthetic_pointer<U, AM> const& p);

    //- User-defined assignment.
    //
    synthetic_pointer&   operator =(std::nullptr_t);
    template<class U, synthetic_pointer_traits::implicitly_convertible<U, void> = true>
    synthetic_pointer&   operator =(U* p);
    template<class U, synthetic_pointer_traits::implicitly_convertible<U, void> = true>
    synthetic_pointer&   operator =(synthetic_pointer<U, AM> const& p);

    //- User-defined conversion.
    //
    explicit    operator bool() const;
    template<class U, synthetic_pointer_traits::implicitly_convertible<void, U> = true>
                operator U* () const;
    template<class U, synthetic_pointer_traits::explicit_conversion_required<void, U> = true>
    explicit    operator U* () const;
    template<class U, synthetic_pointer_traits::explicit_conversion_required<void, U> = true>
    explicit    operator synthetic_pointer<U, AM>() const;

    //- Helper functions used to implement the comparison operators.
    //
    bool    equals(std::nullptr_t) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<void, U> = true>
    bool    equals(U const* p) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<void, U> = true>
    bool    equals(synthetic_pointer<U, AM> const& p) const;

    bool    greater_than(std::nullptr_t) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<void, U> = true>
    bool    greater_than(U const* p) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<void, U> = true>
    bool    greater_than(synthetic_pointer<U, AM> const& p) const;

    bool    less_than(std::nullptr_t) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<void, U> = true>
    bool    less_than(U const* p) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<void, U> = true>
    bool    less_than(synthetic_pointer<U, AM> const& p) const;

  private:
    template<class OT, class OAM> friend class synthetic_pointer;   //- For conversion ctor

    AM      m_addrmodel;
};

//--------------------------------------------------------------------------------------------------
//  Facility:   synthetic_pointer<void,AM>
//--------------------------------------------------------------------------------------------------
//
template<class AM> inline
synthetic_pointer<void, AM>::synthetic_pointer(AM am)
:   m_addrmodel{am}
{}

template<class AM> inline
synthetic_pointer<void, AM>::synthetic_pointer(std::nullptr_t)
:   m_addrmodel(nullptr)
{}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_convertible<U, void>> inline
synthetic_pointer<void, AM>::synthetic_pointer(U* p)
{
    m_addrmodel.assign_from(p);
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_convertible<U, void>> inline
synthetic_pointer<void, AM>::synthetic_pointer(synthetic_pointer<U, AM> const& src)
:   m_addrmodel(src.m_addrmodel)
{}

//------
//
template<class AM> inline
synthetic_pointer<void, AM>&
synthetic_pointer<void, AM>::operator =(std::nullptr_t)
{
    m_addrmodel = nullptr;
    return *this;
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_convertible<U, void>> inline
synthetic_pointer<void, AM>&
synthetic_pointer<void, AM>::operator =(U* p)
{
    m_addrmodel.assign_from(p);
    return *this;
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_convertible<U, void>> inline
synthetic_pointer<void, AM>&
synthetic_pointer<void, AM>::operator =(synthetic_pointer<U, AM> const& rhs)
{
    m_addrmodel = rhs.m_addrmodel;
    return *this;
}

//------
//
template<class AM> inline
synthetic_pointer<void, AM>::operator bool() const
{
    return !m_addrmodel.equals(nullptr);
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_convertible<void, U>> inline
synthetic_pointer<void, AM>::operator U* () const
{
    return static_cast<U*>(m_addrmodel.address());
}

template<class AM>
template<class U, synthetic_pointer_traits::explicit_conversion_required<void, U>> inline
synthetic_pointer<void, AM>::operator U* () const
{
    return static_cast<U*>(m_addrmodel.address());
}

template<class AM>
template<class U, synthetic_pointer_traits::explicit_conversion_required<void, U>> inline
synthetic_pointer<void, AM>::operator synthetic_pointer<U, AM>() const
{
    return synthetic_pointer<U, AM>(m_addrmodel);
}

//------
//
template<class AM> inline
bool
synthetic_pointer<void, AM>::equals(std::nullptr_t) const
{
    return m_addrmodel.equals(nullptr);
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<void, U>> inline
bool
synthetic_pointer<void, AM>::equals(U const* p) const
{
    return m_addrmodel.equals(p);
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<void, U>> inline
bool
synthetic_pointer<void, AM>::equals(synthetic_pointer<U, AM> const& p) const
{
    return m_addrmodel.equals(p.m_addrmodel);
}

template<class AM> inline
bool
synthetic_pointer<void, AM>::greater_than(std::nullptr_t) const
{
    return m_addrmodel.greater_than(nullptr);
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<void, U>> inline
bool
synthetic_pointer<void, AM>::greater_than(U const* p) const
{
    return m_addrmodel.greater_than(p);
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<void, U>> inline
bool
synthetic_pointer<void, AM>::greater_than(synthetic_pointer<U, AM> const& p) const
{
    return m_addrmodel.greater_than(p);
}

template<class AM> inline
bool
synthetic_pointer<void, AM>::less_than(std::nullptr_t) const
{
    return m_addrmodel.less_than(nullptr);
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<void, U>> inline
bool
synthetic_pointer<void, AM>::less_than(U const* p) const
{
    return m_addrmodel.less_than(p);
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<void, U>> inline
bool
synthetic_pointer<void, AM>::less_than(synthetic_pointer<U, AM> const& p) const
{
    return m_addrmodel.less_than(p);
}


//--------------------------------------------------------------------------------------------------
//  Class:
//      synthetic_pointer<void const, AM>
//
//  Summary:
//      Partial specialization of synthetic_pointer<T, AM>, removing arithmetic, indexing,
//      and dereferencing operations.
//--------------------------------------------------------------------------------------------------
//
template<class AM>
class synthetic_pointer<void const, AM>
{
  public:
    //- Re-binder alias required for C++11 support.
    //
    template<class U>
    using rebind = synthetic_pointer<U, AM>;

    //- Other aliases required by allocator_traits<T>, pointer_traits<T>, and the containers.
    //
    using difference_type   = typename AM::difference_type;
    using size_type         = typename AM::size_type;
    using pointer           = synthetic_pointer;

  public:
    //- Special member functions - make intentions explicit.
    //
    ~synthetic_pointer() noexcept = default;

    synthetic_pointer() noexcept = default;
    synthetic_pointer(synthetic_pointer&&) noexcept = default;
    synthetic_pointer(synthetic_pointer const&) noexcept = default;

    synthetic_pointer&  operator =(synthetic_pointer&&) noexcept = default;
    synthetic_pointer&  operator =(synthetic_pointer const&) noexcept = default;

    //- User-defined construction.  Allow only implicit conversions at compile time.
    //
    synthetic_pointer(AM am);
    synthetic_pointer(std::nullptr_t);
    template<class U, synthetic_pointer_traits::implicitly_convertible<U, void const> = true>
    synthetic_pointer(U* p);
    template<class U, synthetic_pointer_traits::implicitly_convertible<U, void const> = true>
    synthetic_pointer(synthetic_pointer<U, AM> const& p);

    //- User-defined assignment.
    //
    synthetic_pointer&   operator =(std::nullptr_t);
    template<class U, synthetic_pointer_traits::implicitly_convertible<U, void const> = true>
    synthetic_pointer&   operator =(U* p);
    template<class U, synthetic_pointer_traits::implicitly_convertible<U, void const> = true>
    synthetic_pointer&   operator =(synthetic_pointer<U, AM> const& p);

    //- User-defined conversion.
    //
    explicit    operator bool() const;
    template<class U, synthetic_pointer_traits::implicitly_convertible<void const, U> = true>
                operator U* () const;
    template<class U, synthetic_pointer_traits::explicit_conversion_required<void const, U> = true>
    explicit    operator U* () const;
    template<class U, synthetic_pointer_traits::explicit_conversion_required<void const, U> = true>
    explicit    operator synthetic_pointer<U, AM>() const;

    //- Additional helper functions used to implement the comparison operators.
    //
    bool    equals(std::nullptr_t) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<void const, U> = true>
    bool    equals(U const* p) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<void const, U> = true>
    bool    equals(synthetic_pointer<U, AM> const& p) const;

    bool    greater_than(std::nullptr_t) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<void const, U> = true>
    bool    greater_than(U const* p) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<void const, U> = true>
    bool    greater_than(synthetic_pointer<U, AM> const& p) const;

    bool    less_than(std::nullptr_t) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<void const, U> = true>
    bool    less_than(U const* p) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<void const, U> = true>
    bool    less_than(synthetic_pointer<U, AM> const& p) const;

  private:
    template<class OT, class OAM> friend class synthetic_pointer;   //- For conversion ctor

    AM      m_addrmodel;
};

//--------------------------------------------------------------------------------------------------
//  Facility:   synthetic_pointer<void const,AM>
//--------------------------------------------------------------------------------------------------
//
template<class AM> inline
synthetic_pointer<void const, AM>::synthetic_pointer(AM am)
:   m_addrmodel{am}
{}

template<class AM> inline
synthetic_pointer<void const, AM>::synthetic_pointer(std::nullptr_t)
:   m_addrmodel(nullptr)
{}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_convertible<U, void const>> inline
synthetic_pointer<void const, AM>::synthetic_pointer(U* p)
{
    m_addrmodel.assign_from(p);
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_convertible<U, void const>> inline
synthetic_pointer<void const, AM>::synthetic_pointer(synthetic_pointer<U, AM> const& src)
:   m_addrmodel(src.m_addrmodel)
{}

//------
//
template<class AM> inline
synthetic_pointer<void const, AM>&
synthetic_pointer<void const, AM>::operator =(std::nullptr_t)
{
    m_addrmodel = nullptr;
    return *this;
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_convertible<U, void const>> inline
synthetic_pointer<void const, AM>&
synthetic_pointer<void const, AM>::operator =(U* p)
{
    m_addrmodel.assign_from(p);
    return *this;
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_convertible<U, void const>> inline
synthetic_pointer<void const, AM>&
synthetic_pointer<void const, AM>::operator =(synthetic_pointer<U, AM> const& rhs)
{
    m_addrmodel = rhs.m_addrmodel;
    return *this;
}

//------
//
template<class AM> inline
synthetic_pointer<void const, AM>::operator bool() const
{
    return !m_addrmodel.equals(nullptr);
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_convertible<void const, U>> inline
synthetic_pointer<void const, AM>::operator U* () const
{
    return static_cast<U*>(m_addrmodel.address());
}

template<class AM>
template<class U, synthetic_pointer_traits::explicit_conversion_required<void const, U>> inline
synthetic_pointer<void const, AM>::operator U* () const
{
    return static_cast<U*>(m_addrmodel.address());
}

template<class AM>
template<class U, synthetic_pointer_traits::explicit_conversion_required<void const, U>> inline
synthetic_pointer<void const, AM>::operator synthetic_pointer<U, AM>() const
{
    return synthetic_pointer<U, AM>(m_addrmodel);
}

//------
//
template<class AM> inline
bool
synthetic_pointer<void const, AM>::equals(std::nullptr_t) const
{
    return m_addrmodel.equals(nullptr);
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<void const, U>> inline
bool
synthetic_pointer<void const, AM>::equals(U const* p) const
{
    return m_addrmodel.equals(p);
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<void const, U>> inline
bool
synthetic_pointer<void const, AM>::equals(synthetic_pointer<U, AM> const& p) const
{
    return m_addrmodel.equals(p.m_addrmodel);
}

template<class AM> inline
bool
synthetic_pointer<void const, AM>::greater_than(std::nullptr_t) const
{
    return m_addrmodel.greater_than(nullptr);
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<void const, U>> inline
bool
synthetic_pointer<void const, AM>::greater_than(U const* p) const
{
    return m_addrmodel.greater_than(p);
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<void const, U>> inline
bool
synthetic_pointer<void const, AM>::greater_than(synthetic_pointer<U, AM> const& p) const
{
    return m_addrmodel.greater_than(p);
}

template<class AM> inline
bool
synthetic_pointer<void const, AM>::less_than(std::nullptr_t) const
{
    return m_addrmodel.less_than(nullptr);
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<void const, U>> inline
bool
synthetic_pointer<void const, AM>::less_than(U const* p) const
{
    return m_addrmodel.less_than(p);
}

template<class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<void const, U>> inline
bool
synthetic_pointer<void const, AM>::less_than(synthetic_pointer<U, AM> const& p) const
{
    return m_addrmodel.less_than(p);
}

#endif  //- SYNTHETIC_VOID_POINTER_INTERFACE_H_DEFINED
