//==================================================================================================
//  File:
//      segmented_private_storage_model.h
//
//  Summary:
//      Defines a very simple heap class for testing rhx_allocator.
//==================================================================================================
//
#include <cstring>
#include <utility>
#include "segmented_private_storage_model.h"

uint8_t*   
    segmented_private_storage_model::sm_segment_addr[max_segments + 2];

segmented_private_storage_model::addressing_model  
    segmented_private_storage_model::sm_segment_data[max_segments + 2];

segmented_private_storage_model::size_type  
    segmented_private_storage_model::sm_segment_size[max_segments + 2];

uint8_t*   
    segmented_private_storage_model::sm_shadow_addr[max_segments + 2];

void
segmented_private_storage_model::allocate_segment(size_type segment, size_type size)
{
    if (segment >= first_segment()  &&  segment <= max_segments  &&  
        size <= max_size  &&  sm_segment_addr[segment] == nullptr)
    {
        sm_shadow_addr[segment] = new uint8_t[size];
        memset(sm_shadow_addr[segment], 0, size);

        sm_segment_addr[segment] = new uint8_t[size];
        memset(sm_segment_addr[segment], 0, size);

        sm_segment_size[segment] = size;
    }
}

void
segmented_private_storage_model::deallocate_segment(size_type segment)
{
    if (sm_segment_addr[segment] != nullptr)
    {
        delete [] sm_shadow_addr[segment];
        delete [] sm_segment_addr[segment];
        sm_segment_addr[segment] = nullptr;
        sm_segment_size[segment] = 0;
    }
}

void
segmented_private_storage_model::clear_segments()
{
    for (size_type i = first_segment();  i <= max_segments;  ++i)
    {
        deallocate_segment(i);
    }
}

void
segmented_private_storage_model::swap_buffers()
{
    for (size_type i = first_segment();  i <= max_segments;  ++i)
    {
        memcpy(sm_shadow_addr[i], sm_segment_addr[i], sm_segment_size[i]);
        std::swap(sm_shadow_addr[i], sm_segment_addr[i]);
    }
}
