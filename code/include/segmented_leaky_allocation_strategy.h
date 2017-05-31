//==================================================================================================
//  File:
//      segmented_leaky_allocation_strategy.h
//
//  Summary:
//      Defines a very allocation strategy class for testing rhx_allocator.
//==================================================================================================
//
#ifndef SEGMENTED_LEAKY_ALLOCATION_STRATEGY_H_DEFINED
#define SEGMENTED_LEAKY_ALLOCATION_STRATEGY_H_DEFINED

#include <cstddef>
#include <cstdint>

#include "synthetic_pointer_interface.h"

//--------------------------------------------------------------------------------------------------
//  Class:
//      segmented_heap<SM>
//
//  Summary:
//      This class implements a simple leaky allocation strategy for testing purposes.
//--------------------------------------------------------------------------------------------------
//
template<class SM>
class segmented_leaky_allocation_strategy
{
  public:
    using storage_model         = SM;
    using addressing_model      = typename SM::addressing_model;

    using difference_type       = typename SM::difference_type;
    using size_type             = typename SM::size_type;

    using void_pointer          = synthetic_pointer<void, addressing_model>;
    using const_void_pointer    = synthetic_pointer<void const, addressing_model>;

    template<class T>
    using rebind_pointer        = synthetic_pointer<T, addressing_model>;

  public:
    size_type       max_size() const;

    void_pointer    allocate(size_type n);
    void            deallocate(void_pointer p);

    static  void    swap_buffers();

  private:
    enum : size_type 
    {
        segment_count = 4
    };

    static  difference_type     round_up(difference_type x, difference_type r);
    static  void                init_segments();

    static  size_type   sm_curr_segment;
    static  size_type   sm_curr_offset;
};

template<class SM>
typename segmented_leaky_allocation_strategy<SM>::size_type     segmented_leaky_allocation_strategy<SM>::sm_curr_segment = 0;
template<class SM>
typename segmented_leaky_allocation_strategy<SM>::size_type     segmented_leaky_allocation_strategy<SM>::sm_curr_offset = 0;


template<class SM> inline
typename segmented_leaky_allocation_strategy<SM>::size_type
segmented_leaky_allocation_strategy<SM>::max_size() const
{
    return storage_model::max_segment_size() / 2;
}

template<class SM>
typename segmented_leaky_allocation_strategy<SM>::void_pointer
segmented_leaky_allocation_strategy<SM>::allocate(size_type n)
{
    if (sm_curr_segment == 0)
    {
        init_segments();
    }

    size_type   chunk_size   = round_up(n, 16u);
    size_type   chunk_offset = sm_curr_offset;

    if ((chunk_offset + chunk_size) > storage_model::max_segment_size())
    {
        ++sm_curr_segment;
        chunk_offset   = 0;
        sm_curr_offset = chunk_size;
    }
    else
    {
        sm_curr_offset += chunk_size;
    }

    return storage_model::segment_pointer(sm_curr_segment, chunk_offset);
}

template<class SM> inline
void
segmented_leaky_allocation_strategy<SM>::deallocate(void_pointer)
{}

template<class SM> inline
void
segmented_leaky_allocation_strategy<SM>::swap_buffers()
{
    storage_model::swap_buffers();
}

template<class SM> inline
typename segmented_leaky_allocation_strategy<SM>::difference_type
segmented_leaky_allocation_strategy<SM>::round_up(difference_type x, difference_type r)
{
    return (x % r) ? (x + r - (x % r)) : x;
}

template<class SM>
void
segmented_leaky_allocation_strategy<SM>::init_segments()
{
    for (size_type i = 0, j = storage_model::first_segment();  i < segment_count;  ++i, ++j)
    {
        storage_model::allocate_segment(j);
    }
    sm_curr_segment = storage_model::first_segment();
}

#endif  //- SEGMENTED_LEAKY_ALLOCATION_STRATEGY_H_DEFINED
