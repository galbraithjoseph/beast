//
// Copyright (c) 2013-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BEAST_STATIC_BUFFER_HPP
#define BEAST_STATIC_BUFFER_HPP

#include <beast/config.hpp>
#include <boost/asio/buffer.hpp>
#include <algorithm>
#include <cstddef>
#include <cstring>

namespace beast {

/** A circular @b DynamicBuffer with a fixed size internal buffer.

    This implements a circular dynamic buffer. Calls to @ref prepare
    never require moving memory. The buffer sequences returned may
    be up to length two.
    Ownership of the underlying storage belongs to the derived class.

    @note Variables are usually declared using the template class
    @ref static_buffer; however, to reduce the number of instantiations
    of template functions receiving static stream buffer arguments in a
    deduced context, the signature of the receiving function should use
    @ref static_buffer_base.

    When used with @ref static_buffer this implements a dynamic
    buffer using no memory allocations.

    @see @ref static_buffer
*/
class static_buffer_base
{
    char* begin_;
    std::size_t in_off_ = 0;
    std::size_t in_size_ = 0;
    std::size_t out_size_ = 0;
    std::size_t capacity_;

    template<bool, class T>
    class buffers_type;

    static_buffer_base(static_buffer_base const& other) = delete;
    static_buffer_base& operator=(static_buffer_base const&) = delete;

public:
    /// The type used to represent the input sequence as a list of buffers.
#if BEAST_DOXYGEN
    using const_buffers_type = implementation_defined;
#else
    using const_buffers_type =
        buffers_type<true, boost::asio::const_buffer>;
#endif

    /// The type used to represent the mutable input sequence as a list of buffers.
#if BEAST_DOXYGEN
    using mutable_data_type = implementation_defined;
#else
    using mutable_data_type =
        buffers_type<true, boost::asio::mutable_buffer>;
#endif

    /// The type used to represent the output sequence as a list of buffers.
#if BEAST_DOXYGEN
    using mutable_buffers_type = implementation_defined;
#else
    using mutable_buffers_type =
        buffers_type<false, boost::asio::mutable_buffer>;
#endif

    /** Constructor

        This creates a dynamic buffer using the provided storage area.

        @param p A pointer to valid storage of at least `n` bytes.

        @param size The number of valid bytes pointed to by `p`.
    */
    static_buffer_base(void* p, std::size_t size);

    /// Return the size of the input sequence.
    std::size_t
    size() const
    {
        return in_size_;
    }

    /// Return the maximum sum of the input and output sequence sizes.
    std::size_t
    max_size() const
    {
        return capacity_;
    }

    /// Return the maximum sum of input and output sizes that can be held without an allocation.
    std::size_t
    capacity() const
    {
        return capacity_;
    }

    /** Get a list of buffers that represent the input sequence.
    */
    const_buffers_type
    data() const;

    /** Get a list of mutable buffers that represent the input sequence.
    */
    mutable_data_type
    mutable_data();

    /** Get a list of buffers that represent the output sequence, with the given size.

        @param size The number of bytes to request.

        @throws std::length_error if the size would exceed the capacity.
    */
    mutable_buffers_type
    prepare(std::size_t size);

    /** Move bytes from the output sequence to the input sequence.

        @param size The nubmer of bytes to commit. If this is greater
        than the size of the output sequence, the entire output
        sequence is committed.
    */
    void
    commit(std::size_t size);

    /** Remove bytes from the input sequence.

        @param size The number of bytes to consume. If this is greater
        than the size of the input sequence, the entire input sequence
        is consumed.
    */
    void
    consume(std::size_t size);

protected:
    /** Constructor

        The buffer will be in an undefined state. It is necessary
        for the derived class to call @ref reset in order to
        initialize the object.
    */
    static_buffer_base();

    /** Reset the pointed-to buffer.

        This function resets the internal state to the buffer provided.
        All input and output sequences are invalidated. This function
        allows the derived class to construct its members before
        initializing the static buffer.

        @param p A pointer to valid storage of at least `n` bytes.

        @param size The number of valid bytes pointed to by `p`.
    */
    void
    reset(void* p, std::size_t size);
};

//------------------------------------------------------------------------------

/** A circular @b DynamicBuffer with a fixed size internal buffer.

    This implements a circular dynamic buffer. Calls to @ref prepare
    never require moving memory. The buffer sequences returned may
    be up to length two.
    Ownership of the underlying storage belongs to the derived class.

    @tparam N The number of bytes in the internal buffer.

    @note To reduce the number of template instantiations when passing
    objects of this type in a deduced context, the signature of the
    receiving function should use @ref static_buffer_base instead.

    @see @ref static_buffer_base
*/
template<std::size_t N>
class static_buffer : public static_buffer_base
{
    char buf_[N];

public:
    /// Constructor
    static_buffer(static_buffer const&);

    /// Constructor
    static_buffer()
        : static_buffer_base(buf_, N)
    {
    }

    /// Assignment
    static_buffer& operator=(static_buffer const&);

    /// Returns the @ref static_buffer_base portion of this object
    static_buffer_base&
    base()
    {
        return *this;
    }

    /// Returns the @ref static_buffer_base portion of this object
    static_buffer_base const&
    base() const
    {
        return *this;
    }
};

} // beast

#include <beast/core/impl/static_buffer.ipp>

#endif
