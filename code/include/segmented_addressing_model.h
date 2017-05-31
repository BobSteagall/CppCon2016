//==================================================================================================
//  File:
//      segmented_addressing_model.h
//
//  Summary:
//      Defines a simple segmented addressing model as a class template.
//==================================================================================================
//
#ifndef SEGMENTED_ADDRESSING_MODEL_H_DEFINED
#define SEGMENTED_ADDRESSING_MODEL_H_DEFINED

#include <cstddef>
#include <cstdint>

//--------------------------------------------------------------------------------------------------
//  Class:
//      segmented_addressing_model
//
//  Summary:
//      This class implements a based (segment:offset) addressing model.
//--------------------------------------------------------------------------------------------------
//
template<typename SM>
class segmented_addressing_model
{
  public:
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

  public:
    ~segmented_addressing_model() = default;

    segmented_addressing_model() noexcept = default;
    segmented_addressing_model(segmented_addressing_model&&) noexcept = default;
    segmented_addressing_model(segmented_addressing_model const&) noexcept = default;
    segmented_addressing_model(std::nullptr_t) noexcept;

    segmented_addressing_model& operator =(segmented_addressing_model&&) noexcept = default;
    segmented_addressing_model& operator =(segmented_addressing_model const&) noexcept = default;
    segmented_addressing_model& operator =(std::nullptr_t) noexcept;

    void*       address() const noexcept;
    size_type   offset() const noexcept;
    size_type   segment() const noexcept;

    bool        equals(std::nullptr_t) const noexcept;
    bool        equals(void const* p) const noexcept;
    bool        equals(segmented_addressing_model const& other) const noexcept;

    bool        greater_than(std::nullptr_t) const noexcept;
    bool        greater_than(void const* p) const noexcept;
    bool        greater_than(segmented_addressing_model const& other) const noexcept;

    bool        less_than(std::nullptr_t) const noexcept;
    bool        less_than(void const* p) const noexcept;
    bool        less_than(segmented_addressing_model const& other) const noexcept;

    void        assign_from(void const* p);

    void        decrement(difference_type dec) noexcept;
    void        increment(difference_type inc) noexcept;

  private:
    friend  SM;
   
    enum : uint64_t
    {
        offset_zero = 0u,
        offset_mask = (~offset_zero) >> 16
    };

    struct addr_bits
    {
        uint16_t    m_word1;
        uint16_t    m_word2;
        uint16_t    m_word3;
        uint16_t    m_segment;
    };

  private:
    union
    {
        uint64_t    m_addr;
        addr_bits   m_bits;
    };

  private:
      segmented_addressing_model(size_type segment, size_type offset) noexcept;
};

template<typename SM> inline
segmented_addressing_model<SM>::segmented_addressing_model(std::nullptr_t) noexcept
:   m_addr{0u}
{}

template<typename SM> inline
segmented_addressing_model<SM>&
segmented_addressing_model<SM>::operator =(std::nullptr_t) noexcept
{
    m_addr = 0u;
    return *this;
}

template<typename SM> inline
void*
segmented_addressing_model<SM>::address() const noexcept
{
    return SM::sm_segment_addr[m_bits.m_segment] + (m_addr & offset_mask);
}

template<typename SM> inline
typename segmented_addressing_model<SM>::size_type
segmented_addressing_model<SM>::offset() const noexcept
{
    return m_addr & offset_mask;
}

template<typename SM> inline
typename segmented_addressing_model<SM>::size_type
segmented_addressing_model<SM>::segment() const noexcept
{
    return m_bits.m_segment;
}

template<typename SM> inline
bool
segmented_addressing_model<SM>::equals(std::nullptr_t) const noexcept
{
    return m_addr == 0;
}

template<typename SM> inline
bool
segmented_addressing_model<SM>::equals(void const* p) const noexcept
{
    return address() == p;
}

template<typename SM> inline
bool
segmented_addressing_model<SM>::equals(segmented_addressing_model const& other) const noexcept
{
    return address() == other.address();
}

template<typename SM> inline
bool
segmented_addressing_model<SM>::greater_than(std::nullptr_t) const noexcept
{
    return address() != nullptr;
}

template<typename SM> inline
bool
segmented_addressing_model<SM>::greater_than(void const* p) const noexcept
{
    return address() > p;
}

template<typename SM> inline
bool
segmented_addressing_model<SM>::greater_than(segmented_addressing_model const& other) const noexcept
{
    return address() > other.address();
}

template<typename SM> inline
bool
segmented_addressing_model<SM>::less_than(std::nullptr_t) const noexcept
{
    return false;
}

template<typename SM> inline
bool
segmented_addressing_model<SM>::less_than(void const* p) const noexcept
{
    return address() < p;
}

template<typename SM> inline
bool
segmented_addressing_model<SM>::less_than(segmented_addressing_model const& other) const noexcept
{
    return address() < other.address();
}

template<typename SM>
void
segmented_addressing_model<SM>::assign_from(void const* p) 
{
    uint8_t const*  pnull = nullptr;
    uint8_t const*  pbyte = static_cast<uint8_t const*>(p);

    size_type   sb = SM::first_segment();
    size_type   se = sb + SM::max_segment_count();

    for (size_type i = sb;  i < se;  ++i)
    {
        uint8_t const*  pbottom = SM::sm_segment_addr[i];

        if (pbottom != nullptr)
        {
            uint8_t const*  ptop = pbottom + SM::sm_segment_size[i];

            if (pbottom <= pbyte  &&  pbyte < ptop)
            {
                m_addr            = pbyte - pbottom;
                m_bits.m_segment = static_cast<uint16_t>(i);
                return;
            }
        }
    }

    m_addr = pbyte - pnull;
}

template<typename SM> inline
void
segmented_addressing_model<SM>::decrement(difference_type dec) noexcept
{
    m_addr -= dec;
}

template<typename SM> inline
void
segmented_addressing_model<SM>::increment(difference_type inc) noexcept
{
    m_addr += inc;
}

template<typename SM> inline
segmented_addressing_model<SM>::segmented_addressing_model(size_type seg, size_type off) noexcept
:   m_addr{off}
{
    m_bits.m_segment = static_cast<uint16_t>(seg);
}

#endif  //- SEGMENTED_ADDRESSING_MODEL_H_DEFINED
