//==================================================================================================
//  File:
//      segmented_private_storage_model.h
//
//  Summary:
//      Defines a very simple heap class for testing rhx_allocator.
//==================================================================================================
//
#ifndef SEGMENTED_PRIVATE_STORAGE_MODEL_H_DEFINED
#define SEGMENTED_PRIVATE_STORAGE_MODEL_H_DEFINED

#include <cstddef>
#include <cstdint>
#include "segmented_addressing_model.h"

class segmented_private_storage_model
{
  public:
    using difference_type  = std::ptrdiff_t;
    using size_type        = std::size_t; 
    using addressing_model = segmented_addressing_model<segmented_private_storage_model>;

  public:
    enum : size_type
    {
        max_segments = 8,           //- Don't need many for testing
        max_size     = 1u << 22     //- 4MB segments
    };

    static  void    allocate_segment(size_type segment, size_type size = max_size);
    static  void    deallocate_segment(size_type segment);
    static  void    clear_segments();
    static  void    swap_buffers();

    static  uint8_t*            segment_address(size_type segment) noexcept;
    static  addressing_model    segment_pointer(size_type segment, size_type offset=0) noexcept;
    static  size_type           segment_size(size_type segment) noexcept;

    static  constexpr   size_type   first_segment();
    static  constexpr   size_type   max_segment_count();
    static  constexpr   size_type   max_segment_size();

  private:
    friend class segmented_addressing_model<segmented_private_storage_model>;
    
    static  uint8_t*            sm_segment_addr[max_segments + 2];
    static  addressing_model    sm_segment_data[max_segments + 2];
    static  size_type           sm_segment_size[max_segments + 2];
    static  uint8_t*            sm_shadow_addr[max_segments + 2];
};


inline auto
segmented_private_storage_model::segment_address(size_type segment) noexcept -> uint8_t*
{
    return sm_segment_addr[segment];
}

inline auto
segmented_private_storage_model::segment_pointer(size_type segment, size_type offset) noexcept 
-> addressing_model
{
    return addressing_model{static_cast<uint32_t>(segment), static_cast<uint32_t>(offset)};
}

inline auto
segmented_private_storage_model::segment_size(size_type segment) noexcept -> size_type
{
    return sm_segment_size[segment];
}

constexpr inline auto
segmented_private_storage_model::first_segment() -> size_type
{
    return 2;
}

constexpr inline auto
segmented_private_storage_model::max_segment_count() -> size_type
{
    return max_segments;
}

constexpr inline auto
segmented_private_storage_model::max_segment_size() -> size_type
{
    return max_size;
}

#endif  //- SEGMENTED_PRIVATE_STORAGE_MODEL_H_DEFINED
