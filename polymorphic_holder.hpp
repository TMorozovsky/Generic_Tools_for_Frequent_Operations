#pragma once

#ifndef POLYMORPHIC_HOLDER_HPP_INCLUDED
#define POLYMORPHIC_HOLDER_HPP_INCLUDED

#if !defined(_MSC_VER) && !defined(__clang__) && !defined(__GNUG__)
#   define POLYMORPHIC_HOLDER_USE_STD_TYPE_TRAITS
#endif

#include <cstdint> // std::uintptr_t
#include <cstring> // std::size_t, std::memcpy/memcpy_s, std::memset
#include <utility> // std::move, std::forward
#include <new>     // placement new

#ifdef POLYMORPHIC_HOLDER_USE_STD_TYPE_TRAITS
#   include <type_traits> // std::is_polymorphic, std::is_base_of
#endif

#ifndef POLYMORPHIC_HOLDER_ASSERT
#   include <cassert> // assert

//  Macro used by polymorphic_holder<...> for assertions.
//  Can be "overridden" by the user if they define this macro before including current header.
#   define POLYMORPHIC_HOLDER_ASSERT(x) \
        { \
            assert((x)); \
        }
#endif

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning( push )
//  Disable MSVC Compiler Warning C4324 ("'struct_name': structure was padded due to alignment specifier")
#   pragma warning( disable : 4324 )
#endif

using ::std::size_t;
using ::std::uintptr_t;

// Common definitions and various utilities used by polymorphic_holder<...>.
namespace polymorphic_holder_lib
{
    using ::std::move;
    using ::std::forward;

    using ::std::memset;

    inline void memcpy_s(void * dest, size_t dest_size, const void * src, size_t src_size) noexcept
    {
        POLYMORPHIC_HOLDER_ASSERT(dest_size >= src_size);
#ifdef _MSC_VER
        errno_t err = ::memcpy_s(dest, dest_size, src, src_size);
        POLYMORPHIC_HOLDER_ASSERT(!err);
        (void)err;
#else
        (void)dest_size;
        ::std::memcpy(dest, src, src_size);
#endif
    }

    inline namespace type_traits
    {
        template<typename T>
        struct is_polymorphic
        {
            static_assert(sizeof(T) > 0, "incomplete types are not allowed");

#ifndef POLYMORPHIC_HOLDER_USE_STD_TYPE_TRAITS
            static constexpr bool value = __is_polymorphic(T);
#else
            static constexpr bool value = ::std::is_polymorphic<T>::value;
#endif
        };

        template<typename B, typename D>
        struct is_base_of
        {
            static_assert(sizeof(B) > 0, "incomplete types are not allowed");
            static_assert(sizeof(D) > 0, "incomplete types are not allowed");

#ifndef POLYMORPHIC_HOLDER_USE_STD_TYPE_TRAITS
            static constexpr bool value = __is_base_of(B, D);
#else
            static constexpr bool value = ::std::is_base_of<B, D>::value;
#endif
        };

        template<typename T> struct is_unsigned_integral_type                         { static constexpr bool value = false; };
        template<>           struct is_unsigned_integral_type<unsigned char>          { static constexpr bool value = true; };
        template<>           struct is_unsigned_integral_type<unsigned short>         { static constexpr bool value = true; };
        template<>           struct is_unsigned_integral_type<unsigned int>           { static constexpr bool value = true; };
        template<>           struct is_unsigned_integral_type<unsigned long int>      { static constexpr bool value = true; };
        template<>           struct is_unsigned_integral_type<unsigned long long int> { static constexpr bool value = true; };
    }
}

// Utilities that are not used by the polymorphic_holder<...> implementation
// but might be useful for client code.
namespace polymorphic_holder_utils
{
    namespace detail
    {
        // Compile-time math :)
        namespace ctm
        {
            template<typename T, T... args>
            struct _max;

            template<typename T, T single_arg>
            struct _max<T, single_arg>
            {
                static constexpr T value = single_arg;
            };

            template<typename T, T arg0, T arg1, T... tail_args>
            struct _max<T, arg0, arg1, tail_args...>
            {
            private:
                static constexpr T _other_max = _max<T, arg1, tail_args...>::value;
            public:
                static constexpr T value = (arg0 > _other_max) ? arg0 : _other_max;
            };
        }
    }

    template<typename... Ts>
    struct max_size_of
    {
        static constexpr size_t value = detail::ctm::_max<size_t, sizeof(Ts)...>::value;
    };

    template<typename... Ts>
    struct max_alignment_of
    {
        static constexpr size_t value = detail::ctm::_max<size_t, alignof(Ts)...>::value;
    };

    template<typename... Ts>
    struct type_traits
    {
        static constexpr size_t max_size = max_size_of<Ts...>::value;
        static constexpr size_t max_alignment = max_alignment_of<Ts...>::value;
    };
}

namespace polymorphic_holder_lib
{
    // Base interface for polymorphic_holder_moveable_only, polymorphic_holder_copyable_only
    // and polymorphic_holder_copyable_and_moveable.
    class i_moveable
    {
    public:
        // Move-constructs a derived object at given uninitialized, properly aligned buffer.
        virtual void move_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) && noexcept = 0;

    protected:
        constexpr i_moveable() noexcept = default;
        inline ~i_moveable() noexcept { }
    };

    // Base interface for polymorphic_holder_copyable_only and polymorphic_holder_copyable_and_moveable.
    class i_copyable
    {
    public:
        // Copy-constructs a derived object at given uninitialized, properly aligned buffer.
        virtual void copy_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) const = 0;

    protected:
        constexpr i_copyable() noexcept = default;
        inline ~i_copyable() noexcept { }
    };
}

namespace polymorphic_holder_utils
{
    // In order to use move constructors of derived objects (which will be owned by polymorphic_holder<...> instances)
    // from polymorphic_holder's own move operations, do the following actions:
    //     - inherit the base class of the polymorphic hierarchy from polymorphic_holder_utils::moveable_only;
    //     - implement its member function in every derived class that will be used with polymorphic_holder<...>
    //       (the POLYMORPHIC_HOLDER_MOVEABLE_ONLY macro is recommended for providing a typical implementation).
    class moveable_only : public polymorphic_holder_lib::i_moveable
    {
    public:
        void copy_construct_at(unsigned char *, size_t) const = delete;

    protected:
        constexpr moveable_only() noexcept = default;
        inline ~moveable_only() noexcept {}
    };

    // In order to use copy constructors of derived objects (which will be owned by polymorphic_holder<...> instances)
    // from polymorphic_holder's own move and copy operations, do the following actions:
    //     - inherit the base class of the polymorphic hierarchy from the polymorphic_holder_utils::copyable_only interface;
    //     - implement its member function in every derived class that will be used with polymorphic_holder<...>
    //       (the POLYMORPHIC_HOLDER_COPYABLE_ONLY macro is recommended for providing a typical implementation).
    class copyable_only : public polymorphic_holder_lib::i_copyable
    {
    public:
        void move_construct_at(unsigned char *, size_t) && = delete;

    protected:
        constexpr copyable_only() noexcept = default;
        inline ~copyable_only() noexcept {}
    };

    // In order to use move and copy constructors of derived objects (which will be owned by polymorphic_holder<...> instances)
    // from polymorphic_holder's own move/copy operations, do the following actions:
    //     - inherit the base class of the polymorphic hierarchy from the polymorphic_holder_copyable_and_moveable interface;
    //     - implement both of its member functions in every derived class that will be used with polymorphic_holder<...>
    //       (the POLYMORPHIC_HOLDER_COPYABLE_AND_MOVEABLE macro is recommended for providing typical implementations).
    class copyable_and_moveable : public polymorphic_holder_lib::i_moveable, public polymorphic_holder_lib::i_copyable
    {
    protected:
        constexpr copyable_and_moveable() noexcept = default;
        inline ~copyable_and_moveable() noexcept {}
    };
}

// Helper macro that shall be used inside a class definition
// in order to provide a typical implementation of the move_construct_at() virtual function.
#define POLYMORPHIC_HOLDER_OVERRIDE_MOVE_CONSTRUCT_AT(c) \
    virtual void move_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) && noexcept override \
    { \
        POLYMORPHIC_HOLDER_ASSERT(dest_buffer_begin != nullptr); \
        POLYMORPHIC_HOLDER_ASSERT(sizeof(c) <= dest_buffer_size); \
        POLYMORPHIC_HOLDER_ASSERT(0 == reinterpret_cast<uintptr_t>(dest_buffer_begin) % alignof(c)); \
        (void)dest_buffer_size; \
        ::new (dest_buffer_begin) c(polymorphic_holder_lib::move(*this)); \
    }

// Helper macro that shall be used inside a class definition
// in order to provide a typical implementation of the copy_construct_at() virtual function.
#define POLYMORPHIC_HOLDER_OVERRIDE_COPY_CONSTRUCT_AT(c) \
    virtual void copy_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) const override \
    { \
        POLYMORPHIC_HOLDER_ASSERT(dest_buffer_begin != nullptr); \
        POLYMORPHIC_HOLDER_ASSERT(sizeof(c) <= dest_buffer_size); \
        POLYMORPHIC_HOLDER_ASSERT(0 == reinterpret_cast<uintptr_t>(dest_buffer_begin) % alignof(c)); \
        (void)dest_buffer_size; \
        ::new (dest_buffer_begin) c(*this); \
    }

// Helper macro that can be used inside a class definition in order to delete the copy_construct_at() function.
#define POLYMORPHIC_HOLDER_DELETE_COPY_CONSTRUCT_AT() \
    void copy_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) const = delete;

// Helper macro that can be used inside a class definition in order to delete the move_construct_at() function.
#define POLYMORPHIC_HOLDER_DELETE_MOVE_CONSTRUCT_AT() \
    void move_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) const = delete;

// Macro that can be used inside the definition of a derived class
// objects of which will be owned by polymorphic_holder<...> template instances,
// if move constructor of polymorphic_holder<...> instances
// is reqiured to invoke move constructor of this derived class.
// The base class of such hierarchy must inherit from polymorphic_holder_moveable_only.
#define POLYMORPHIC_HOLDER_MOVEABLE_ONLY(c) \
    POLYMORPHIC_HOLDER_OVERRIDE_MOVE_CONSTRUCT_AT(c) \
    POLYMORPHIC_HOLDER_DELETE_COPY_CONSTRUCT_AT()

// Macro that can be used inside the definition of a derived class
// objects of which will be owned by polymorphic_holder<...> template instances,
// if both move and copy constructors of polymorphic_holder<...> instances
// are reqiured to invoke copy constructor of this derived class.
// The base class of such hierarchy must inherit from polymorphic_holder_copyable_only.
#define POLYMORPHIC_HOLDER_COPYABLE_ONLY(c) \
    POLYMORPHIC_HOLDER_OVERRIDE_COPY_CONSTRUCT_AT(c) \
    POLYMORPHIC_HOLDER_DELETE_MOVE_CONSTRUCT_AT()

// Macro that can be used inside the definition of a derived class
// objects of which will be owned by polymorphic_holder<...> template instances,
// if move and copy constructors of polymorphic_holder<...> instances
// are reqiured to invoke respective move/copy constructors of this derived class.
// The base class of such hierarchy must inherit from polymorphic_holder_copyable_and_moveable.
#define POLYMORPHIC_HOLDER_COPYABLE_AND_MOVEABLE(c) \
    POLYMORPHIC_HOLDER_OVERRIDE_MOVE_CONSTRUCT_AT(c) \
    POLYMORPHIC_HOLDER_OVERRIDE_COPY_CONSTRUCT_AT(c)

namespace polymorphic_holder_lib
{
    template<class BitwiseCopyablePolymorphicHolder>
    inline void copy_as_bytes(BitwiseCopyablePolymorphicHolder & dest, const BitwiseCopyablePolymorphicHolder & src)
    {
        static_assert(sizeof(dest) >= BitwiseCopyablePolymorphicHolder::max_object_size, "");
        polymorphic_holder_lib::memcpy_s(static_cast<void *>(&dest), sizeof(dest), static_cast<const void *>(&src), sizeof(src));
    }

    template<class PolymorphicHolder>
    inline void set_bytes_to_zero(PolymorphicHolder & dest)
    {
        static_assert(sizeof(dest) >= PolymorphicHolder::max_object_size, "");
        polymorphic_holder_lib::memset(static_cast<void *>(&dest), 0, sizeof(dest));
    }

    template<class PolymorphicHolder>
    inline void move_construct(PolymorphicHolder & uninitialized_dest, PolymorphicHolder && src) noexcept
    {
        i_moveable * p = static_cast<i_moveable *>(src.object_ptr_safe());
        if (p) {
            uninitialized_dest.DRP_set_offset_to_base(src.DRP_offset_to_base());
            polymorphic_holder_lib::move(*p).move_construct_at(uninitialized_dest.object_bytes_begin(), PolymorphicHolder::max_object_size);
        } else {
            polymorphic_holder_lib::set_bytes_to_zero(uninitialized_dest);
        }
    }

    template<class PolymorphicHolder>
    inline void copy_construct(PolymorphicHolder & uninitialized_dest, const PolymorphicHolder & src)
    {
        const i_copyable * p = static_cast<const i_copyable *>(src.object_ptr_safe());
        if (p) {
            uninitialized_dest.DRP_set_offset_to_base(src.DRP_offset_to_base());
            p->copy_construct_at(uninitialized_dest.object_bytes_begin(), PolymorphicHolder::max_object_size);
        } else {
            polymorphic_holder_lib::set_bytes_to_zero(uninitialized_dest);
        }
    }
}

// Auxiliary macro for mixins.
#define POLYMORPHIC_HOLDER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(c) \
    inline c() noexcept { } \
    inline c(const c &) noexcept { } \
    inline c & operator = (const c &) noexcept { return *this; } \
    inline c(c &&) noexcept { } \
    inline c & operator = (c &&) noexcept { return *this; } \
    inline ~c() noexcept { }

// Namespace for polymorphic_holder's Moving/Copying Policies.
// These policies specify how a polymorphic_holder should move and - if supported - copy its contents.
//
// Move-only policies must provide a member function
//     void MCP_move_construct_from(polymorphic_holder &&) noexcept
// to polymorphic_holder<...> template instance.
//
// Policies that support both moving and copying must provide both
//     void MCP_move_construct_from(polymorphic_holder &&) noexcept
// and
//     void MCP_copy_construct_from(const polymorphic_holder &)
// to polymorphic_holder.
namespace polymorphic_holder_MCP
{
    // Implements moving of polymorphic_holder<...> instances as if they were POD
    // (despite they are not, so be careful).
    // Move-constructing from a polymorphic_holder that uses this policy also sets
    // all of the polymorphic_holder's bits to zero.
    // Does not allow copying.
    template<class PolymorphicHolderSelf>
    class bitwise_exclusive_move
    {
    public:
        inline void MCP_move_construct_from(PolymorphicHolderSelf && other) noexcept
        {
            polymorphic_holder_lib::copy_as_bytes(static_cast<PolymorphicHolderSelf &>(*this), other);
            polymorphic_holder_lib::set_bytes_to_zero(other);
        }

        inline void MCP_copy_construct_from(const PolymorphicHolderSelf & other) = delete;

    protected:
        POLYMORPHIC_HOLDER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(bitwise_exclusive_move)
    };

    // Implements both moving and copying of polymorphic_holder<...> instances as if they were POD
    // (despite they are not, so be careful).
    // Bits of moved-from objects are not modified, moving and copying is essentially
    // the same operation with this policy.
    template<class PolymorphicHolderSelf>
    class bitwise_copying
    {
    public:
        inline void MCP_move_construct_from(PolymorphicHolderSelf && other) noexcept
        {
            polymorphic_holder_lib::copy_as_bytes(static_cast<PolymorphicHolderSelf &>(*this), other);
        }

        inline void MCP_copy_construct_from(const PolymorphicHolderSelf & other) noexcept
        {
            polymorphic_holder_lib::copy_as_bytes(static_cast<PolymorphicHolderSelf &>(*this), other);
        }

    protected:
        POLYMORPHIC_HOLDER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(bitwise_copying)
    };

    // Implements both moving and copying of polymorphic_holder<...> instances as if they were POD
    // (despite they are not, so be careful).
    // Move-constructing from a polymorphic_holder that uses this policy also sets
    // all of the polymorphic_holder's bits to zero.
    // Copy-constructing is supported as well. As expected, it leaves bits of the source object unchanged.
    template<class PolymorphicHolderSelf>
    class bitwise_copy_and_exclusive_move
    {
    public:
        inline void MCP_move_construct_from(PolymorphicHolderSelf && other) noexcept
        {
            polymorphic_holder_lib::copy_as_bytes(static_cast<PolymorphicHolderSelf &>(*this), other);
            polymorphic_holder_lib::set_bytes_to_zero(other);
        }

        inline void MCP_copy_construct_from(const PolymorphicHolderSelf & other) noexcept
        {
            polymorphic_holder_lib::copy_as_bytes(static_cast<PolymorphicHolderSelf &>(*this), other);
        }

    protected:
        POLYMORPHIC_HOLDER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(bitwise_copy_and_exclusive_move)
    };

    // Implements moving of polymorphic_holder<...> instances using the "virtual move constructor" provided by
    //     - inheriting the base class of the hierarchy from the polymorphic_holder_moveable_only interface
    // and
    //     - adding the POLYMORPHIC_HOLDER_MOVEABLE_ONLY(DerivedType) macro to all of derived classes' definitions.
    // Does not allow copying.
    template<class PolymorphicHolderSelf>
    class virtual_move_constructor_only
    {
    public:
        inline void MCP_move_construct_from(PolymorphicHolderSelf && other) noexcept
        {
            polymorphic_holder_lib::move_construct(static_cast<PolymorphicHolderSelf &>(*this), polymorphic_holder_lib::move(other));
        }

        inline void MCP_copy_construct_from(const PolymorphicHolderSelf & other) = delete;

    protected:
        POLYMORPHIC_HOLDER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(virtual_move_constructor_only)
    };

    // Implements copying of polymorphic_holder<...> instances using the "virtual copy constructor" provided by
    //     - inheriting the base class of the hierarchy from the polymorphic_holder_copyable_only interface
    // and
    //     - adding the POLYMORPHIC_HOLDER_COPYABLE_ONLY(DerivedType) macro to all of derived classes' definitions.
    // Moving is expressed through copy operations as well.
    template<class PolymorphicHolderSelf>
    class virtual_copy_constructor_only
    {
    public:
        inline void MCP_copy_construct_from(const PolymorphicHolderSelf & other)
        {
            polymorphic_holder_lib::copy_construct(static_cast<PolymorphicHolderSelf &>(*this), other);
        }

        inline void MCP_move_construct_from(PolymorphicHolderSelf && other) noexcept
        {
            this->MCP_copy_construct_from(other);
        }

    protected:
        POLYMORPHIC_HOLDER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(virtual_copy_constructor_only)
    };

    // Implements both moving and copying of polymorphic_holder<...> instances
    // using the "virtual move constructor" and "virtual copy constructor" provided by
    //     - inheriting the base class of the hierarchy from the polymorphic_holder_copyable_only
    //       or polymorphic_holder_copyable_and_moveable interface
    // and
    //     - adding the POLYMORPHIC_HOLDER_COPYABLE_ONLY(DerivedType)
    //       or POLYMORPHIC_HOLDER_COPYABLE_AND_MOVEABLE(DerivedType) macro
    //       to all of derived classes' definitions.
    template<class PolymorphicHolderSelf>
    class virtual_constructors
    {
    public:
        inline void MCP_move_construct_from(PolymorphicHolderSelf && other) noexcept
        {
            polymorphic_holder_lib::move_construct(static_cast<PolymorphicHolderSelf &>(*this), polymorphic_holder_lib::move(other));
        }

        inline void MCP_copy_construct_from(const PolymorphicHolderSelf & other)
        {
            polymorphic_holder_lib::copy_construct(static_cast<PolymorphicHolderSelf &>(*this), other);
        }

    protected:
        POLYMORPHIC_HOLDER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(virtual_constructors)
    };
}

// Namespace for polymorphic_holder's Data Representation Policies.
//
// A Data Representation Policy will be inherited by polymorphic_holder<...> implementation.
// It must implement the following "interface":
//
// (pseudocode)
// template<size_t MaxObjectSize, size_t ObjectAlignment>
// class DataRepresentationPolicy
// {
// protected (member fields):
//     alignas(ObjectAlignment) unsigned char DRP_object_bytes[MaxObjectSize];
// protected (member functions):
//     size_t DRP_offset_to_base() const noexcept;
//     void DRP_set_offset_to_base(size_t new_offset_value) noexcept;
// };
//
// The DRP_object_bytes field must be a properly aligned bytes' array of required size on which
// the derived objects owned by polymorphic_holder<...> will be constructed.
//
// The DRP_offset_to_base() and DRP_set_offset_to_base() functions will be used as a setter and a getter
// of the offset between the first byte of the currently stored derived class instance
// and the first byte of its base class subobject.
namespace polymorphic_holder_DRP
{
    // Instances of polymorphic_holder<...> will be inherited from implementations that are defined
    // in this auxiliary namespace.
    //
    // The classes and class templates in the enclosing namespace (polymorphic_holder_DRP) are just wrappers,
    // each of them defines a template alias "implementation" which has exactly two template parameters:
    // size_t MaxObjectSize and size_t Alignment.
    // A policy wrapper is allowed to be either a class or a template that takes some additional parameters
    // which will be forwarded to the implementation.
    //
    // This approach allows the client, for instance, to use either
    //     offset_before_bytes
    // or
    //     offset_before_bytes_typed<unsigned char>
    // as Data Representation Policy when parameterizing the polymorphic_holder template,
    // because both
    //     class offset_before_bytes
    // and
    //     template<typename OffsetType>
    //     class offset_before_bytes_typed
    // define similar nested ...::implementation templates.
    namespace _implementation
    {
        // Implementation of the bytes_array_only policy.
        template<size_t MaxObjectSize, size_t ObjectAlignment>
        class bytes_array_only_impl
        {
            static_assert(MaxObjectSize > 0, "MaxObjectSize must be greater than zero");
            static_assert(ObjectAlignment > 0, "ObjectAlignment must be greater than zero");

        protected:
            alignas(ObjectAlignment) unsigned char DRP_object_bytes[MaxObjectSize];

        protected:
            constexpr size_t DRP_offset_to_base() const noexcept
            {
                return 0;
            }

            inline void DRP_set_offset_to_base(size_t new_offset_value) noexcept
            {
                POLYMORPHIC_HOLDER_ASSERT(new_offset_value == 0);
                (void)new_offset_value;
            }

        protected:
            POLYMORPHIC_HOLDER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(bytes_array_only_impl)
        };

        // Implementation of the offset_before_bytes and offset_before_bytes_typed<...> policies.
        template<size_t MaxObjectSize, size_t ObjectAlignment, typename OffsetType>
        class offset_before_bytes_impl
        {
            static_assert(MaxObjectSize > 0, "MaxObjectSize must be greater than zero");
            static_assert(ObjectAlignment > 0, "ObjectAlignment must be greater than zero");

            static_assert(sizeof(OffsetType) <= sizeof(size_t), "OffsetType must not be bigger than size_t");
            static_assert(polymorphic_holder_lib::is_unsigned_integral_type<OffsetType>::value, "OffsetType must be unsigned");

        private:
            OffsetType _offset_to_base_value;
        protected:
            alignas(ObjectAlignment) unsigned char DRP_object_bytes[MaxObjectSize];

        protected:
            inline size_t DRP_offset_to_base() const noexcept
            {
                return static_cast<size_t>(_offset_to_base_value);
            }

            inline void DRP_set_offset_to_base(size_t new_offset_value) noexcept
            {
                POLYMORPHIC_HOLDER_ASSERT(new_offset_value < MaxObjectSize);
                POLYMORPHIC_HOLDER_ASSERT(new_offset_value <= static_cast<size_t>(OffsetType(-1)));
                _offset_to_base_value = static_cast<OffsetType>(new_offset_value);
            }

        protected:
            POLYMORPHIC_HOLDER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(offset_before_bytes_impl)
        };

        // Implementation of the offset_after_bytes and offset_after_bytes_typed<...> policies.
        template<size_t MaxObjectSize, size_t ObjectAlignment, typename OffsetType>
        class offset_after_bytes_impl
        {
            static_assert(MaxObjectSize > 0, "MaxObjectSize must be greater than zero");
            static_assert(ObjectAlignment > 0, "ObjectAlignment must be greater than zero");

            static_assert(sizeof(OffsetType) <= sizeof(size_t), "OffsetType must not be bigger than size_t");
            static_assert(polymorphic_holder_lib::is_unsigned_integral_type<OffsetType>::value, "OffsetType must be unsigned");

        protected:
            alignas(ObjectAlignment) unsigned char DRP_object_bytes[MaxObjectSize];
        private:
            OffsetType _offset_to_base_value;

        protected:
            inline size_t DRP_offset_to_base() const noexcept
            {
                return static_cast<size_t>(_offset_to_base_value);
            }

            inline void DRP_set_offset_to_base(size_t new_offset_value) noexcept
            {
                POLYMORPHIC_HOLDER_ASSERT(new_offset_value < MaxObjectSize);
                POLYMORPHIC_HOLDER_ASSERT(new_offset_value <= static_cast<size_t>(OffsetType(-1)));
                _offset_to_base_value = static_cast<OffsetType>(new_offset_value);
            }

        protected:
            POLYMORPHIC_HOLDER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(offset_after_bytes_impl)
        };
    }

    // Data Representation Policy for polymorphic_holder that stores an aligned array of bytes only.
    // Does not support derived classes with non-zero offsets to their base subobject!
    class bytes_array_only
    {
    public:
        template<size_t MaxObjectSize, size_t ObjectAlignment>
        using implementation = _implementation::bytes_array_only_impl<MaxObjectSize, ObjectAlignment>;
    };

    // Data Representation Policy for polymorphic_holder that stores an offset to base subobject
    // before an aligned array of bytes.
    //
    // Type of the stored offset field is size_t.
    // If you want to use some other unsigned integral type instead, check out offset_before_bytes_typed.
    class offset_before_bytes
    {
    public:
        template<size_t MaxObjectSize, size_t ObjectAlignment>
        using implementation = _implementation::offset_before_bytes_impl<MaxObjectSize, ObjectAlignment, size_t>;
    };


    // Data Representation Policy for polymorphic_holder that stores an offset to base subobject
    // before an aligned array of bytes.
    //
    // Type of the stored offset field is OffsetType,
    // which must be some fundamental unsigned integral type that is not bigger than size_t.
    template<typename OffsetType>
    class offset_before_bytes_typed
    {
    public:
        template<size_t MaxObjectSize, size_t ObjectAlignment>
        using implementation = _implementation::offset_before_bytes_impl<MaxObjectSize, ObjectAlignment, OffsetType>;
    };

    // Data Representation Policy for polymorphic_holder that stores an offset to base subobject
    // after an aligned array of bytes.
    //
    // Type of the stored offset field is size_t.
    // If you want to use some other unsigned integral type instead, check out offset_after_bytes_typed.
    class offset_after_bytes
    {
    public:
        template<size_t MaxObjectSize, size_t ObjectAlignment>
        using implementation = _implementation::offset_after_bytes_impl<MaxObjectSize, ObjectAlignment, size_t>;
    };

    // Data Representation Policy for polymorphic_holder that stores an offset to base subobject
    // after an aligned array of bytes.
    //
    // Type of the stored offset field is OffsetType,
    // which must be some fundamental unsigned integral type that is not bigger than size_t.
    template<typename OffsetType>
    class offset_after_bytes_typed
    {
    public:
        template<size_t MaxObjectSize, size_t ObjectAlignment>
        using implementation = _implementation::offset_after_bytes_impl<MaxObjectSize, ObjectAlignment, OffsetType>;
    };
}

// Holder that stores one object of any type that is derived from BaseType (may also be BaseType itself),
// provided that its size and alignment meet given MaxObjectSize and ObjectAlignment requirements.
//
// Allows the user to work with these objects via pointers/references to BaseType polymorphically
// and still avoid the unwanted dynamic memory allocations.
//
// Be extremely careful with bitwise copying/moving if you choose them as Moving/Copying Policy,
// since far from all classes can work correctly with this kind of operations.
//
// Virtual inheritance is not supported! But that's no big deal since nobody uses it anyway, right? ;)
template<class BaseType, size_t MaxObjectSize, size_t ObjectAlignment,
         template<class> class MovingCopyingPolicy = polymorphic_holder_MCP::virtual_constructors,
         class DataRepresentationPolicy = polymorphic_holder_DRP::offset_before_bytes>
class polymorphic_holder
    : private MovingCopyingPolicy<polymorphic_holder<BaseType, MaxObjectSize, ObjectAlignment, MovingCopyingPolicy, DataRepresentationPolicy>>
    , private DataRepresentationPolicy::template implementation<MaxObjectSize, ObjectAlignment>
{
    static_assert(polymorphic_holder_lib::is_polymorphic<BaseType>::value, "base type must be polymorphic");

    template<class PolymorphicHolder> friend void polymorphic_holder_lib::move_construct(PolymorphicHolder & uninitialized_dest, PolymorphicHolder && src) noexcept;
    template<class PolymorphicHolder> friend void polymorphic_holder_lib::copy_construct(PolymorphicHolder & uninitialized_dest, const PolymorphicHolder & src);

    friend class MovingCopyingPolicy<polymorphic_holder<BaseType, MaxObjectSize, ObjectAlignment, MovingCopyingPolicy, DataRepresentationPolicy>>;
    friend typename DataRepresentationPolicy::template implementation<MaxObjectSize, ObjectAlignment>;

    using MCP_mixin = MovingCopyingPolicy<polymorphic_holder<BaseType, MaxObjectSize, ObjectAlignment, MovingCopyingPolicy, DataRepresentationPolicy>>;
    using DRP_mixin = typename DataRepresentationPolicy::template implementation<MaxObjectSize, ObjectAlignment>;

public:
    using base_type = BaseType;
    static constexpr size_t max_object_size = MaxObjectSize;
    static constexpr size_t object_alignment = ObjectAlignment;

public:
    inline bool is_constructed() const noexcept
    {
        for (size_t i = 0; i < max_object_size; ++i) {
            if (this->DRP_object_bytes[i])
                return true;
        }
        return false;
    }

private:
    inline       unsigned char * object_bytes_begin()       noexcept { return &this->DRP_object_bytes[0]; }
    inline const unsigned char * object_bytes_begin() const noexcept { return &this->DRP_object_bytes[0]; }

public:
    inline       base_type * object_ptr_unsafe()       noexcept { return reinterpret_cast<      base_type *>(&this->DRP_object_bytes[this->DRP_offset_to_base()]); }
    inline const base_type * object_ptr_unsafe() const noexcept { return reinterpret_cast<const base_type *>(&this->DRP_object_bytes[this->DRP_offset_to_base()]); }

    inline       base_type * object_ptr_safe()       noexcept { return this->is_constructed() ? this->object_ptr_unsafe() : nullptr; }
    inline const base_type * object_ptr_safe() const noexcept { return this->is_constructed() ? this->object_ptr_unsafe() : nullptr; }

    inline       base_type & object_ref()       noexcept { POLYMORPHIC_HOLDER_ASSERT(this->is_constructed()); return *(this->object_ptr_unsafe()); }
    inline const base_type & object_ref() const noexcept { POLYMORPHIC_HOLDER_ASSERT(this->is_constructed()); return *(this->object_ptr_unsafe()); }

    inline       base_type & operator * ()       noexcept { return this->object_ref(); }
    inline const base_type & operator * () const noexcept { return this->object_ref(); }

    inline       base_type * operator -> ()       noexcept { POLYMORPHIC_HOLDER_ASSERT(this->is_constructed()); return this->object_ptr_unsafe(); }
    inline const base_type * operator -> () const noexcept { POLYMORPHIC_HOLDER_ASSERT(this->is_constructed()); return this->object_ptr_unsafe(); }

    inline explicit operator bool() const noexcept { return  this->is_constructed(); }
    inline bool     operator !   () const noexcept { return !this->is_constructed(); }

private:
    template<class DesiredType, typename... CtorArgs>
    inline void impl_construct(CtorArgs&&... args)
    {
        static_assert(polymorphic_holder_lib::is_base_of<base_type, DesiredType>::value, "constructed object type is not derived from base_type");
        static_assert(sizeof(DesiredType) <= max_object_size, "constructed object does not fit into available memory");
        static_assert(alignof(DesiredType) <= object_alignment, "constructed object's alignment requirements are not met by this holder");

        const DesiredType *const test_ptr_d = reinterpret_cast<const DesiredType *>(uintptr_t(-1) / 2);
        const base_type *const test_ptr_b = static_cast<const base_type *>(test_ptr_d);
        const size_t offset = reinterpret_cast<const unsigned char *>(test_ptr_b) - reinterpret_cast<const unsigned char *>(test_ptr_d);

        polymorphic_holder other;
        other.DRP_set_offset_to_base(offset);
        POLYMORPHIC_HOLDER_ASSERT(uintptr_t(other.object_bytes_begin()) % object_alignment == 0);
        POLYMORPHIC_HOLDER_ASSERT(uintptr_t(this->object_bytes_begin()) % object_alignment == 0);
        ::new (other.object_bytes_begin()) DesiredType(polymorphic_holder_lib::forward<CtorArgs>(args)...);

        *this = polymorphic_holder_lib::move(other);
    }

public:
    template<class DesiredType, typename... CtorArgs>
    inline void construct(CtorArgs&&... args)
    {
        POLYMORPHIC_HOLDER_ASSERT(!this->is_constructed());
        this->impl_construct<DesiredType>(polymorphic_holder_lib::forward<CtorArgs>(args)...);
    }

    template<class DesiredType, typename... CtorArgs>
    inline static polymorphic_holder make(CtorArgs&&... args)
    {
        polymorphic_holder tmp;
        tmp.construct<DesiredType>(polymorphic_holder_lib::forward<CtorArgs>(args)...);
        return tmp;
    }

private:
    inline void destroy_object_unsafe() noexcept
    {
        POLYMORPHIC_HOLDER_ASSERT(this->is_constructed());
        this->object_ptr_unsafe()->~base_type();
    }

public:
    template<class DesiredType, typename... CtorArgs>
    inline void reset(CtorArgs&&... args)
    {
        this->impl_construct<DesiredType>(polymorphic_holder_lib::forward<CtorArgs>(args)...);
    }

    inline void reset() noexcept
    {
        this->clear();
    }

    inline void clear() noexcept
    {
        if (this->is_constructed()) {
            this->destroy_object_unsafe();
            polymorphic_holder_lib::set_bytes_to_zero(*this);
        }
    }

public:
    inline polymorphic_holder() noexcept
    {
        polymorphic_holder_lib::set_bytes_to_zero(*this);
    }

    inline polymorphic_holder(polymorphic_holder && other) noexcept
    {
        this->MCP_move_construct_from(polymorphic_holder_lib::move(other));
    }

    inline polymorphic_holder(const polymorphic_holder & other)
    {
        this->MCP_copy_construct_from(other);
    }

    inline polymorphic_holder & operator = (polymorphic_holder && other) noexcept
    {
        if (this->is_constructed()) {
            this->destroy_object_unsafe();
        }
        this->MCP_move_construct_from(polymorphic_holder_lib::move(other));
        return *this;
    }

    inline polymorphic_holder & operator = (const polymorphic_holder & other)
    {
        polymorphic_holder temp(other);
        *this = polymorphic_holder_lib::move(temp);
        return *this;
    }

    inline ~polymorphic_holder() noexcept
    {
        if (this->is_constructed()) {
            this->destroy_object_unsafe();
        }
    }
};

#undef POLYMORPHIC_HOLDER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning( pop )
#endif

#endif // POLYMORPHIC_HOLDER_HPP_INCLUDED
