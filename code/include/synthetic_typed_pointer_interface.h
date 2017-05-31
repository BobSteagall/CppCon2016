//==================================================================================================
//  File:
//      synthetic_typed_pointer_interface.h
//
//  Summary:
//      Defines synthetic_pointer<T, AM>.
//==================================================================================================
//
#ifndef SYNTHETIC_TYPED_POINTER_INTERFACE_H_DEFINED
#define SYNTHETIC_TYPED_POINTER_INTERFACE_H_DEFINED

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <type_traits>

template<class T, class AM>
class synthetic_pointer
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
    using element_type      = T;
    using value_type        = T;
    using reference         = T&;
    using pointer           = synthetic_pointer;
    using iterator_category = std::random_access_iterator_tag;

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
    template<class U, synthetic_pointer_traits::implicitly_convertible<U, T> = true>
    synthetic_pointer(U* p);
    template<class U, synthetic_pointer_traits::implicitly_convertible<U, T> = true>
    synthetic_pointer(synthetic_pointer<U, AM> const& p);

    //- User-defined assignment.
    //
    synthetic_pointer&   operator =(std::nullptr_t);
    template<class U, synthetic_pointer_traits::implicitly_convertible<U, T> = true>
    synthetic_pointer&   operator =(U* p);
    template<class U, synthetic_pointer_traits::implicitly_convertible<U, T> = true>
    synthetic_pointer&   operator =(synthetic_pointer<U, AM> const& p);

    //- User-defined conversion.
    //
    explicit    operator bool() const;
    template<class U, synthetic_pointer_traits::implicitly_convertible<T, U> = true>
                operator U* () const;
    template<class U, synthetic_pointer_traits::explicit_conversion_required<T, U> = true>
    explicit    operator U* () const;
    template<class U, synthetic_pointer_traits::explicit_conversion_required<T, U> = true>
    explicit    operator synthetic_pointer<U, AM>() const;

    //- De-referencing and indexing.
    //
    T*  operator ->() const;
    T&  operator  *() const;
    T&  operator [](size_type n) const;

    //- Pointer arithmetic operators.
    //
    difference_type          operator -(const synthetic_pointer& p) const;
    synthetic_pointer        operator -(difference_type n) const;
    synthetic_pointer        operator +(difference_type n) const;

    synthetic_pointer&       operator ++();
    synthetic_pointer const  operator ++(int);
    synthetic_pointer&       operator --();
    synthetic_pointer const  operator --(int);
    synthetic_pointer&       operator +=(difference_type n);
    synthetic_pointer&       operator -=(difference_type n);

    //- Helper function required by pointer_traits<T>.
    //
    static  synthetic_pointer     pointer_to(element_type& e);

    //- Additional helper functions used to implement the comparison operators.
    //
    bool    equals(std::nullptr_t) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<T, U> = true>
    bool    equals(U const* p) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<T, U> = true>
    bool    equals(synthetic_pointer<U, AM> const& p) const;

    bool    greater_than(std::nullptr_t) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<T, U> = true>
    bool    greater_than(U const* p) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<T, U> = true>
    bool    greater_than(synthetic_pointer<U, AM> const& p) const;

    bool    less_than(std::nullptr_t) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<T, U> = true>
    bool    less_than(U const* p) const;
    template<class U, synthetic_pointer_traits::implicitly_comparable<T, U> = true>
    bool    less_than(synthetic_pointer<U, AM> const& p) const;

  private:
    template<class OT, class OAM> friend class synthetic_pointer;   //- For conversion ctor

    AM      m_addrmodel;
};

//--------------------------------------------------------------------------------------------------
//  Facility:   synthetic_pointer<T,AM>
//--------------------------------------------------------------------------------------------------
template<class T, class AM> inline
synthetic_pointer<T, AM>::synthetic_pointer(AM am)
:   m_addrmodel{am}
{}

template<class T, class AM> inline
synthetic_pointer<T, AM>::synthetic_pointer(std::nullptr_t)
:   m_addrmodel(nullptr)
{}

template<class T, class AM>
template<class U, synthetic_pointer_traits::implicitly_convertible<U, T>> inline
synthetic_pointer<T, AM>::synthetic_pointer(U* p)
{
    m_addrmodel.assign_from(p);
}

template<class T, class AM>
template<class U, synthetic_pointer_traits::implicitly_convertible<U, T>> inline
synthetic_pointer<T, AM>::synthetic_pointer(synthetic_pointer<U, AM> const& src)
:   m_addrmodel(src.m_addrmodel)
{}

//------
//
template<class T, class AM> inline
synthetic_pointer<T, AM>&
synthetic_pointer<T, AM>::operator =(std::nullptr_t)
{
    m_addrmodel = nullptr;
    return *this;
}

template<class T, class AM>
template<class U, synthetic_pointer_traits::implicitly_convertible<U, T>> inline
synthetic_pointer<T, AM>&
synthetic_pointer<T, AM>::operator =(U* p)
{
    m_addrmodel.assign_from(p);
    return *this;
}

template<class T, class AM>
template<class U, synthetic_pointer_traits::implicitly_convertible<U, T>> inline
synthetic_pointer<T, AM>&
synthetic_pointer<T, AM>::operator =(synthetic_pointer<U, AM> const& rhs)
{
    m_addrmodel = rhs.m_addrmodel;
    return *this;
}

//------
//
template<class T, class AM> inline
synthetic_pointer<T, AM>::operator bool() const
{
    return !m_addrmodel.equals(nullptr);
}

template<class T, class AM>
template<class U, synthetic_pointer_traits::implicitly_convertible<T, U>> inline
synthetic_pointer<T, AM>::operator U* () const
{
    return static_cast<U*>(m_addrmodel.address());
}

template<class T, class AM>
template<class U, synthetic_pointer_traits::explicit_conversion_required<T, U>> inline
synthetic_pointer<T, AM>::operator U* () const
{
    return static_cast<U*>(m_addrmodel.address());
}

template<class T, class AM>
template<class U, synthetic_pointer_traits::explicit_conversion_required<T, U>> inline
synthetic_pointer<T, AM>::operator synthetic_pointer<U, AM>() const
{
    return synthetic_pointer<U, AM>(m_addrmodel);
}

//------
//
template<class T, class AM> inline
T*
synthetic_pointer<T, AM>::operator ->() const
{
    return static_cast<T*>(m_addrmodel.address());
}

template<class T, class AM> inline
T&
synthetic_pointer<T, AM>::operator *() const
{
    return *static_cast<T*>(m_addrmodel.address());
}

template<class T, class AM> inline
T&
synthetic_pointer<T, AM>::operator [](size_type n) const
{
    return static_cast<T*>(m_addrmodel.address())[n];
}

template<class T, class AM> inline
typename synthetic_pointer<T, AM>::difference_type
synthetic_pointer<T, AM>::operator -(synthetic_pointer const& rhs) const
{
    uint8_t const*  p1 = static_cast<uint8_t const*>(m_addrmodel.address());
    uint8_t const*  p2 = static_cast<uint8_t const*>(rhs.m_addrmodel.address());

    return (p1 - p2) / sizeof(T);
}

template<class T, class AM> inline
synthetic_pointer<T, AM>
synthetic_pointer<T, AM>::operator -(difference_type n) const
{
    synthetic_pointer   tmp{*this};
    tmp.m_addrmodel.decrement(n*sizeof(T));
    return tmp;
}

template<class T, class AM> inline
synthetic_pointer<T, AM>
synthetic_pointer<T, AM>::operator +(difference_type n) const
{
    synthetic_pointer   tmp{*this};
    tmp.m_addrmodel.increment(n*sizeof(T));
    return tmp;
}

template<class T, class AM> inline
synthetic_pointer<T, AM>&
synthetic_pointer<T, AM>::operator ++()
{
    m_addrmodel.increment(sizeof(T));
    return *this;
}

template<class T, class AM> inline
synthetic_pointer<T, AM> const
synthetic_pointer<T, AM>::operator ++(int)
{
    synthetic_pointer   tmp{*this};
    m_addrmodel.increment(sizeof(T));
    return tmp;
}

template<class T, class AM> inline
synthetic_pointer<T, AM>&
synthetic_pointer<T, AM>::operator --()
{
    m_addrmodel.decrement(sizeof(T));
    return *this;
}

template<class T, class AM> inline
synthetic_pointer<T, AM> const
synthetic_pointer<T, AM>::operator --(int)
{
    synthetic_pointer   tmp{*this};
    m_addrmodel.decrement(sizeof(T));
    return tmp;
}

template<class T, class AM> inline
synthetic_pointer<T, AM>&
synthetic_pointer<T, AM>::operator +=(difference_type n)
{
    m_addrmodel.increment(n * sizeof(T));
    return *this;
}

template<class T, class AM> inline
synthetic_pointer<T, AM>&
synthetic_pointer<T, AM>::operator -=(difference_type n)
{
    m_addrmodel.decrement(n * sizeof(T));
    return *this;
}

//------
//
template<class T, class AM> inline
bool
synthetic_pointer<T, AM>::equals(std::nullptr_t) const
{
    return m_addrmodel.equals(nullptr);
}

template<class T, class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<T, U>> inline
bool
synthetic_pointer<T, AM>::equals(U const* p) const
{
    return m_addrmodel.equals(p);
}

template<class T, class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<T, U>> inline
bool
synthetic_pointer<T, AM>::equals(synthetic_pointer<U, AM> const& p) const
{
    return m_addrmodel.equals(p.m_addrmodel);
}

template<class T, class AM> inline
bool
synthetic_pointer<T, AM>::greater_than(std::nullptr_t) const
{
    return m_addrmodel.greater_than(nullptr);
}

template<class T, class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<T, U>> inline
bool
synthetic_pointer<T, AM>::greater_than(U const* p) const
{
    return m_addrmodel.greater_than(p);
}

template<class T, class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<T, U>> inline
bool
synthetic_pointer<T, AM>::greater_than(synthetic_pointer<U, AM> const& p) const
{
    return m_addrmodel.greater_than(p);
}

template<class T, class AM> inline
bool
synthetic_pointer<T, AM>::less_than(std::nullptr_t) const
{
    return m_addrmodel.less_than(nullptr);
}

template<class T, class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<T, U>> inline
bool
synthetic_pointer<T, AM>::less_than(U const* p) const
{
    return m_addrmodel.less_than(p);
}

template<class T, class AM>
template<class U, synthetic_pointer_traits::implicitly_comparable<T, U>> inline
bool
synthetic_pointer<T, AM>::less_than(synthetic_pointer<U, AM> const& p) const
{
    return m_addrmodel.less_than(p);
}

template<class T, class AM> inline
synthetic_pointer<T, AM>
synthetic_pointer<T, AM>::pointer_to(element_type& e)
{
    return synthetic_pointer(&e);
}

#endif  //- SYNTHETIC_TYPED_POINTER_INTERFACE_H_DEFINED
