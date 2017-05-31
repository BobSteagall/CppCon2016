//==================================================================================================
//  File:
//      synthetic_pointer_compare_ops.h
//
//  Summary:
//      Comparison operators for the synthetic_pointer<T, AM> class template.
//==================================================================================================
//
#ifndef SYNTHETIC_POINTER_COMPARE_OPS_H_DEFINED
#define SYNTHETIC_POINTER_COMPARE_OPS_H_DEFINED

template<class T, class U, class AM> inline bool
operator ==(synthetic_pointer<T, AM> p1, synthetic_pointer<U, AM> p2)
{
    return p1.equals(p2);
}

template<class T, class U, class AM> inline bool
operator ==(synthetic_pointer<T, AM> p1, U p2)
{
    return p1.equals(p2);
}

template<class T, class U, class AM> inline bool
operator ==(T p1, synthetic_pointer<U, AM> p2)
{
    return p2.equals(p1);
}

//------
//
template<class T, class U, class AM> inline bool
operator !=(synthetic_pointer<T, AM> p1, synthetic_pointer<U, AM> p2)
{
    return !p1.equals(p2);
}

template<class T, class U, class AM> inline bool
operator !=(synthetic_pointer<T, AM> p1, U p2)
{
    return !p1.equals(p2);
}

template<class T, class U, class AM> inline bool
operator !=(T p1, synthetic_pointer<U, AM> p2)
{
    return !p2.equals(p1);
}

//------
//
template<class T, class U, class AM> inline bool
operator <(synthetic_pointer<T, AM> p1, synthetic_pointer<U, AM> p2)
{
    return p1.less_than(p2);
}

template<class T, class U, class AM> inline bool
operator <(synthetic_pointer<T, AM> p1, U p2)
{
    return p1.less_than(p2);
}

template<class T, class U, class AM> inline bool
operator <(T p1, synthetic_pointer<U, AM> p2)
{
    return p2.greater_than(p1);
}

//------
//
template<class T, class U, class AM> inline bool
operator <=(synthetic_pointer<T, AM> p1, synthetic_pointer<U, AM> p2)
{
    return !p1.greater_than(p2);
}

template<class T, class U, class AM> inline bool
operator <=(synthetic_pointer<T, AM> p1, U p2)
{
    return !p1.greater_than(p2);
}

template<class T, class U, class AM> inline bool
operator <=(T p1, synthetic_pointer<U, AM> p2)
{
    return !p2.less_than(p1);
}

//------
//
template<class T, class U, class AM> inline bool
operator >(synthetic_pointer<T, AM> p1, synthetic_pointer<U, AM> p2)
{
    return p1.greater_than(p2);
}

template<class T, class U, class AM> inline bool
operator >(synthetic_pointer<T, AM> p1, U p2)
{
    return p1.greater_than(p2);
}

template<class T, class U, class AM> inline bool
operator >(T p1, synthetic_pointer<U, AM> p2)
{
    return p2.less_than(p1);
}

//------
//
template<class T, class U, class AM> inline bool
operator >=(synthetic_pointer<T, AM> p1, synthetic_pointer<U, AM> p2)
{
    return !p1.less_than(p2);
}

template<class T, class U, class AM> inline bool
operator >=(synthetic_pointer<T, AM> p1, U p2)
{
    return !p1.less_than(p2);
}

template<class T, class U, class AM> inline bool
operator >=(T p1, synthetic_pointer<U, AM> p2)
{
    return !p2.greater_than(p1);
}

#endif  //- SYNTHETIC_POINTER_COMPARE_OPS_H_DEFINED
