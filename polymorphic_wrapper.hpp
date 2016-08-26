#pragma once

#ifndef POLYMORPHIC_WRAPPER_HPP_INCLUDED
#define POLYMORPHIC_WRAPPER_HPP_INCLUDED

#include <cstdint> // std::uintptr_t
#include <new>     // placement new
#include <utility> // std::move, std::forward
#include <cstring> // std::size_t, std::memcpy/memcpy_s, std::memset
#include <cassert> // assert

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning( push )
//  Disable MSVC Compiler Warning C4324 ("'struct_name': structure was padded due to alignment specifier")
#   pragma warning( disable : 4324 )
#endif

// Macro used by polymorphic_wrapper<...> to check invariants.
#define POLYMORPHIC_WRAPPER_ASSERT(x) \
    { \
        assert((x)); \
    }

using ::std::size_t;
using ::std::uintptr_t;

// Common definitions and various utilities used by polymorphic_wrapper<...>.
namespace polymorphic_wrapper_lib
{
    using ::std::move;
    using ::std::forward;

    using ::std::memset;

    inline void memcpy_s(void * dest, size_t dest_size, const void * src, size_t src_size) noexcept
    {
#ifdef _MSC_VER
        errno_t err = ::memcpy_s(dest, dest_size, src, src_size);
        (void)err;
        POLYMORPHIC_WRAPPER_ASSERT(!err);
#else
        POLYMORPHIC_WRAPPER_ASSERT(dest_size >= src_size);
        (void)dest_size;
        ::std::memcpy(dest, src, src_size);
#endif
    }

    namespace ctm // Compile-time math :)
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

    namespace type_traits
    {
        template<typename... Ts>
        struct max_size_of
        {
            static constexpr size_t value = ctm::_max<size_t, sizeof(Ts)...>::value;
        };

        template<typename... Ts>
        struct max_alignment_of
        {
            static constexpr size_t value = ctm::_max<size_t, alignof(Ts)...>::value;
        };

        template<typename... Ts>
        struct max_size_and_alignment_of
        {
            static constexpr size_t size = max_size_of<Ts...>::value;
            static constexpr size_t alignment = max_alignment_of<Ts...>::value;
        };

        template<typename T>
        struct is_polymorphic
        {
            static_assert(sizeof(T) > 0, "incomplete types are not allowed");

            static constexpr bool value = __is_polymorphic(T); // non-standard, but works on most compilers; for portable solution, include <type_traits> and use std::is_polymorphic
        };

        template<typename B, typename D>
        struct is_base_of
        {
            static_assert(sizeof(B) > 0, "incomplete types are not allowed");
            static_assert(sizeof(D) > 0, "incomplete types are not allowed");

            static constexpr bool value = __is_base_of(B, D); // non-standard, but works on most compilers; for portable solution, include <type_traits> and use std::is_base_of
        };
    }
}

// In order to use move constructors of derived objects (which will be owned by polymorphic_wrapper<...> instances)
// from polymorphic_wrapper's own move operations, do the following actions:
//     - inherit the base class of the polymorphic hierarchy from the polymorphic_wrapper_moveable interface;
//     - implement its member function in every derived class that will be used with polymorphic_wrapper<...>
//       (the POLYMORPHIC_WRAPPER_MOVEABLE macro is recommended for providing a typical implementation).
class polymorphic_wrapper_moveable
{
public:
    virtual void _move_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) && = 0;

protected:
    constexpr polymorphic_wrapper_moveable() noexcept { }
    inline ~polymorphic_wrapper_moveable() noexcept { }
};

// In order to use move and copy constructors of derived objects (which will be owned by polymorphic_wrapper<...> instances)
// from polymorphic_wrapper's own move/copy operations, do the following actions:
//     - inherit the base class of the polymorphic hierarchy from the polymorphic_wrapper_copyable interface;
//     - implement its member functions in every derived class that will be used with polymorphic_wrapper<...>
//       (the POLYMORPHIC_WRAPPER_COPYABLE macro is recommended for providing typical implementations).
class polymorphic_wrapper_copyable : public polymorphic_wrapper_moveable
{
public:
    virtual void _copy_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) const = 0;

protected:
    constexpr polymorphic_wrapper_copyable() noexcept { }
    inline ~polymorphic_wrapper_copyable() noexcept { }
};

// Macro that can be used inside the definition of a derived class
// objects of which will be owned by polymorphic_wrapper<...> template instances,
// if move constructor of polymorphic_wrapper<...> instances is reqiured
// to invoke move constructor of this derived class.
// The base class of such hierarchy must inherit from polymorphic_wrapper_moveable.
#define POLYMORPHIC_WRAPPER_MOVEABLE(c) \
    virtual void _move_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) && override \
    { \
        POLYMORPHIC_WRAPPER_ASSERT(!!dest_buffer_begin); \
        POLYMORPHIC_WRAPPER_ASSERT(sizeof(c) <= dest_buffer_size); \
        POLYMORPHIC_WRAPPER_ASSERT(0 == reinterpret_cast<uintptr_t>(dest_buffer_begin) % alignof(c)); \
        (void)dest_buffer_size; \
        ::new (dest_buffer_begin) c(polymorphic_wrapper_lib::move(*this)); \
    }

// Macro that can be used inside the definition of a derived class
// objects of which will be owned by polymorphic_wrapper<...> template instances,
// if move and copy constructors of polymorphic_wrapper<...> instances are reqiured
// to invoke respective move/copy constructors of this derived class.
// The base class of such hierarchy must inherit from polymorphic_wrapper_copyable.
#define POLYMORPHIC_WRAPPER_COPYABLE(c) \
    \
    POLYMORPHIC_WRAPPER_MOVEABLE(c) \
    \
    virtual void _copy_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) const override \
    { \
        POLYMORPHIC_WRAPPER_ASSERT(!!dest_buffer_begin); \
        POLYMORPHIC_WRAPPER_ASSERT(sizeof(c) <= dest_buffer_size); \
        POLYMORPHIC_WRAPPER_ASSERT(0 == reinterpret_cast<uintptr_t>(dest_buffer_begin) % alignof(c)); \
        (void)dest_buffer_size; \
        ::new (dest_buffer_begin) c(*this); \
    }

namespace polymorphic_wrapper_lib
{
    template<class BitwiseCopyablePolymorphicWrapper>
    inline void copy_as_bytes(BitwiseCopyablePolymorphicWrapper & dest, const BitwiseCopyablePolymorphicWrapper & src)
    {
        static_assert(sizeof(dest) >= BitwiseCopyablePolymorphicWrapper::derived_object_max_size, "");
        memcpy_s(static_cast<void *>(&dest), sizeof(dest), static_cast<const void *>(&src), sizeof(src));
    }

    template<class PolymorphicWrapper>
    inline void set_bytes_to_zero(PolymorphicWrapper & dest)
    {
        static_assert(sizeof(dest) >= PolymorphicWrapper::derived_object_max_size, "");
        memset(static_cast<void *>(&dest), 0, sizeof(dest));
    }

    template<class PolymorphicWrapper>
    inline void move_construct(PolymorphicWrapper & uninitialized_dest, PolymorphicWrapper && src) noexcept
    {
        polymorphic_wrapper_moveable * p = static_cast<polymorphic_wrapper_moveable *>(src.object_ptr_safe());
        if (p) {
            uninitialized_dest.DRP_set_offset_to_base(src.DRP_offset_to_base());
            polymorphic_wrapper_lib::move(*p)._move_construct_at(uninitialized_dest.derived_object_bytes_begin(), PolymorphicWrapper::derived_object_max_size);
        } else {
            polymorphic_wrapper_lib::set_bytes_to_zero(uninitialized_dest);
        }
    }

    template<class PolymorphicWrapper>
    inline void copy_construct(PolymorphicWrapper & uninitialized_dest, const PolymorphicWrapper & src)
    {
        const polymorphic_wrapper_copyable * p = static_cast<const polymorphic_wrapper_copyable *>(src.object_ptr_safe());
        if (p) {
            uninitialized_dest.DRP_set_offset_to_base(src.DRP_offset_to_base());
            p->_copy_construct_at(uninitialized_dest.derived_object_bytes_begin(), PolymorphicWrapper::derived_object_max_size);
        } else {
            polymorphic_wrapper_lib::set_bytes_to_zero(uninitialized_dest);
        }
    }
}

// Auxiliary macro for mixins.
#define POLYMORPHIC_WRAPPER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(c) \
    inline c() noexcept { } \
    inline c(const c &) noexcept { } \
    inline c & operator = (const c &) noexcept { return *this; } \
    inline c(c &&) noexcept { } \
    inline c & operator = (c &&) noexcept { return *this; } \
    inline ~c() noexcept { }

// Namespace for polymorphic_wrapper's Moving/Copying Policies.
// These policies specify how a polymorphic_wrapper should move and - if supported - copy its contents.
//
// Move-only policies must provide a member function
//     void MCP_move_construct_from(polymorphic_wrapper &&) noexcept
// to polymorphic_wrapper<...> template instance.
//
// Policies that support both moving and copying must provide both
//     void MCP_move_construct_from(polymorphic_wrapper &&) noexcept
// and
//     void MCP_copy_construct_from(const polymorphic_wrapper &)
// to polymorphic_wrapper.
namespace polymorphic_wrapper_MCP
{
    // Implements moving of polymorphic_wrapper<...> instances as if they were POD
    // (despite they are not, so be careful).
    // Move-constructing from a polymorphic_wrapper that uses this policy also sets
    // all of the polymorphic_wrapper's bits to zero.
    // Does not allow copying.
    template<class PolymorphicWrapperSelf>
    class bitwise_exclusive_move
    {
    public:
        inline void MCP_move_construct_from(PolymorphicWrapperSelf && other) noexcept
        {
            polymorphic_wrapper_lib::copy_as_bytes(static_cast<PolymorphicWrapperSelf &>(*this), other);
            polymorphic_wrapper_lib::set_bytes_to_zero(other);
        }

        inline void MCP_copy_construct_from(const PolymorphicWrapperSelf & other) = delete;

    protected:
        POLYMORPHIC_WRAPPER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(bitwise_exclusive_move)
    };

    // Implements both moving and copying of polymorphic_wrapper<...> instances as if they were POD
    // (despite they are not, so be careful).
    // Bits of moved-from objects are not modified, moving and copying is essentially
    // the same operation with this policy.
    template<class PolymorphicWrapperSelf>
    class bitwise_copying
    {
    public:
        inline void MCP_move_construct_from(PolymorphicWrapperSelf && other) noexcept
        {
            polymorphic_wrapper_lib::copy_as_bytes(static_cast<PolymorphicWrapperSelf &>(*this), other);
        }

        inline void MCP_copy_construct_from(const PolymorphicWrapperSelf & other) noexcept
        {
            polymorphic_wrapper_lib::copy_as_bytes(static_cast<PolymorphicWrapperSelf &>(*this), other);
        }

    protected:
        POLYMORPHIC_WRAPPER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(bitwise_copying)
    };

    // Implements both moving and copying of polymorphic_wrapper<...> instances as if they were POD
    // (despite they are not, so be careful).
    // Move-constructing from a polymorphic_wrapper that uses this policy also sets
    // all of the polymorphic_wrapper's bits to zero.
    // Copy-constructing is supported as well, and it, as expected, leaves bits of the source object unchanged.
    template<class PolymorphicWrapperSelf>
    class bitwise_copy_and_exclusive_move
    {
    public:
        inline void MCP_move_construct_from(PolymorphicWrapperSelf && other) noexcept
        {
            polymorphic_wrapper_lib::copy_as_bytes(static_cast<PolymorphicWrapperSelf &>(*this), other);
            polymorphic_wrapper_lib::set_bytes_to_zero(other);
        }

        inline void MCP_copy_construct_from(const PolymorphicWrapperSelf & other) noexcept
        {
            polymorphic_wrapper_lib::copy_as_bytes(static_cast<PolymorphicWrapperSelf &>(*this), other);
        }

    protected:
        POLYMORPHIC_WRAPPER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(bitwise_copy_and_exclusive_move)
    };

    // Implements moving of polymorphic_wrapper<...> instances using the "virtual move constructor" provided by
    //     - inheriting the base class of the hierarchy from the polymorphic_wrapper_moveable interface
    // and
    //     - adding the POLYMORPHIC_WRAPPER_MOVEABLE(DerivedType) macro to all of derived classes' definitions.
    // Does not allow copying.
    template<class PolymorphicWrapperSelf>
    class virtual_move_constructor_only
    {
    public:
        inline void MCP_move_construct_from(PolymorphicWrapperSelf && other) noexcept
        {
            polymorphic_wrapper_lib::move_construct(static_cast<PolymorphicWrapperSelf &>(*this), polymorphic_wrapper_lib::move(other));
        }

    protected:
        POLYMORPHIC_WRAPPER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(virtual_move_constructor_only)
    };

    // Implements both moving and copying of polymorphic_wrapper<...> instances
    // using the "virtual move constructor" and "virtual copy constructor" provided by
    //     - inheriting the base class of the hierarchy from the polymorphic_wrapper_copyable interface
    // and
    //     - adding the POLYMORPHIC_WRAPPER_COPYABLE(DerivedType) macro to all of derived classes' definitions.
    template<class PolymorphicWrapperSelf>
    class virtual_constructors
    {
    public:
        inline void MCP_move_construct_from(PolymorphicWrapperSelf && other) noexcept
        {
            polymorphic_wrapper_lib::move_construct(static_cast<PolymorphicWrapperSelf &>(*this), polymorphic_wrapper_lib::move(other));
        }

        inline void MCP_copy_construct_from(const PolymorphicWrapperSelf & other)
        {
            polymorphic_wrapper_lib::copy_construct(static_cast<PolymorphicWrapperSelf &>(*this), other);
        }

    protected:
        POLYMORPHIC_WRAPPER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(virtual_constructors)
    };
}

// Namespace for polymorphic_wrapper's Data Representation Policies.
//
// Each policy must contain all necessary member fields required for working polymorphic_wrapper<...> implementation
// and allow the polymorphic_wrapper<...> template to directly access the
//     DRP_derived_bytes
// field provided by this policy.
// This field must be a properly aligned bytes' array of required size on which
// the derived objects owned by polymorphic_wrapper<...> will be constructed.
// Also, a Data Representation Policy implementation must provide the following member functions:
//     size_t DRP_offset_to_base() const noexcept;
//     void DRP_set_offset_to_base(size_t new_offset_value) noexcept;
// These two functions will be used as a setter and a getter of the offset
// between the first byte of the currently stored derived class instance
// and the first byte of its base class subobject.
namespace polymorphic_wrapper_DRP
{
    // Representation of polymorphic_wrapper that stores an aligned array of bytes only.
    // Does not support derived classes with non-zero offsets to their base subobject!
    template<size_t MaxObjectSize, size_t Alignment>
    class bytes_array_only
    {
        static_assert(MaxObjectSize > 0, "");
        static_assert(Alignment > 0, "");

    protected:
        alignas(Alignment) unsigned char DRP_derived_bytes[MaxObjectSize];

    protected:
        constexpr size_t DRP_offset_to_base() const noexcept
        {
            return 0;
        }

        inline void DRP_set_offset_to_base(size_t new_offset_value) noexcept
        {
            POLYMORPHIC_WRAPPER_ASSERT(new_offset_value == 0);
            (void)new_offset_value;
        }

    protected:
        POLYMORPHIC_WRAPPER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(bytes_array_only)
    };

    // Representation of polymorphic_wrapper that stores an offset to base subobject
    // before an aligned array of bytes.
    template<size_t MaxObjectSize, size_t Alignment>
    class offset_before_bytes
    {
        static_assert(MaxObjectSize > 0, "");
        static_assert(Alignment > 0, "");

    private:
        size_t _offset_to_base_value;
    protected:
        alignas(Alignment) unsigned char DRP_derived_bytes[MaxObjectSize];

    protected:
        inline size_t DRP_offset_to_base() const noexcept
        {
            return _offset_to_base_value;
        }

        inline void DRP_set_offset_to_base(size_t new_offset_value) noexcept
        {
            POLYMORPHIC_WRAPPER_ASSERT(new_offset_value < MaxObjectSize);
            _offset_to_base_value = new_offset_value;
        }

    protected:
        POLYMORPHIC_WRAPPER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(offset_before_bytes)
    };

    // Representation of polymorphic_wrapper that stores an offset to base subobject
    // after an aligned array of bytes.
    template<size_t MaxObjectSize, size_t Alignment>
    struct offset_after_bytes
    {
        static_assert(MaxObjectSize > 0, "");
        static_assert(Alignment > 0, "");

    protected:
        alignas(Alignment) unsigned char DRP_derived_bytes[MaxObjectSize];
    private:
        size_t _offset_to_base_value;

    protected:
        inline size_t DRP_offset_to_base() const noexcept
        {
            return _offset_to_base_value;
        }

        inline void DRP_set_offset_to_base(size_t new_offset_value) noexcept
        {
            POLYMORPHIC_WRAPPER_ASSERT(new_offset_value < MaxObjectSize);
            _offset_to_base_value = new_offset_value;
        }

    protected:
        POLYMORPHIC_WRAPPER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS(offset_after_bytes)
    };
}

// Wrapper that stores one object of any type that is derived from BaseType (may include BaseType itself),
// provided that its size and alignment fit into given MaxObjectSize and Alignment values.
//
// Allows the user to work with these objects via pointers/references to BaseType polymorphically
// and still avoid the unwanted dynamic memory allocations.
//
// Be extremely careful with bitwise copying/moving if you choose them as Moving/Copying Policy,
// since far from all classes can work correctly with this kind of operations.
//
// Virtual inheritance is not supported! But that's no big deal since nobody uses it anyway, right? ;)
template<class BaseType, size_t MaxObjectSize, size_t Alignment,
         template<class> class MovingCopyingPolicy = polymorphic_wrapper_MCP::virtual_constructors,
         template<size_t, size_t> class DataRepresentationPolicy = polymorphic_wrapper_DRP::offset_before_bytes>
class polymorphic_wrapper
    : private MovingCopyingPolicy<polymorphic_wrapper<BaseType, MaxObjectSize, Alignment, MovingCopyingPolicy, DataRepresentationPolicy>>
    , private DataRepresentationPolicy<MaxObjectSize, Alignment>
{
    static_assert(polymorphic_wrapper_lib::type_traits::is_polymorphic<BaseType>::value, "base type must be polymorphic");

    template<class PolymorphicWrapper> friend void polymorphic_wrapper_lib::move_construct(PolymorphicWrapper & uninitialized_dest, PolymorphicWrapper && src) noexcept;
    template<class PolymorphicWrapper> friend void polymorphic_wrapper_lib::copy_construct(PolymorphicWrapper & uninitialized_dest, const PolymorphicWrapper & src);

    friend class MovingCopyingPolicy<polymorphic_wrapper<BaseType, MaxObjectSize, Alignment, MovingCopyingPolicy, DataRepresentationPolicy>>;
    friend class DataRepresentationPolicy<MaxObjectSize, Alignment>;

    using MCP_mixin = MovingCopyingPolicy<polymorphic_wrapper<BaseType, MaxObjectSize, Alignment, MovingCopyingPolicy, DataRepresentationPolicy>>;
    using DRP_mixin = DataRepresentationPolicy<MaxObjectSize, Alignment>;

public:
    using base_type = BaseType;
    static constexpr size_t derived_object_max_size = MaxObjectSize;
    static constexpr size_t derived_object_alignment = Alignment;

public:
    inline bool is_constructed() const noexcept
    {
        for (size_t i = 0; i < derived_object_max_size; ++i) {
            if (this->DRP_derived_bytes[i])
                return true;
        }
        return false;
    }

private:
    inline       unsigned char * derived_object_bytes_begin()       noexcept { return &this->DRP_derived_bytes[0]; }
    inline const unsigned char * derived_object_bytes_begin() const noexcept { return &this->DRP_derived_bytes[0]; }

public:
    inline       base_type * object_ptr_unsafe()       noexcept { return reinterpret_cast<      base_type *>(&this->DRP_derived_bytes[this->DRP_offset_to_base()]); }
    inline const base_type * object_ptr_unsafe() const noexcept { return reinterpret_cast<const base_type *>(&this->DRP_derived_bytes[this->DRP_offset_to_base()]); }

    inline       base_type * object_ptr_safe()       noexcept { return this->is_constructed() ? this->object_ptr_unsafe() : nullptr; }
    inline const base_type * object_ptr_safe() const noexcept { return this->is_constructed() ? this->object_ptr_unsafe() : nullptr; }

    inline       base_type & object_ref()       noexcept { POLYMORPHIC_WRAPPER_ASSERT(this->is_constructed()); return *(this->object_ptr_unsafe()); }
    inline const base_type & object_ref() const noexcept { POLYMORPHIC_WRAPPER_ASSERT(this->is_constructed()); return *(this->object_ptr_unsafe()); }

    inline       base_type & operator * ()       noexcept { return this->object_ref(); }
    inline const base_type & operator * () const noexcept { return this->object_ref(); }

    inline       base_type * operator -> ()       noexcept { POLYMORPHIC_WRAPPER_ASSERT(this->is_constructed()); return this->object_ptr_unsafe(); }
    inline const base_type * operator -> () const noexcept { POLYMORPHIC_WRAPPER_ASSERT(this->is_constructed()); return this->object_ptr_unsafe(); }

    inline explicit operator bool() const noexcept { return  this->is_constructed(); }
    inline bool     operator !   () const noexcept { return !this->is_constructed(); }

private:
    template<class DesiredType, typename... CtorArgs>
    inline void impl_construct(CtorArgs&&... args)
    {
        static_assert(polymorphic_wrapper_lib::type_traits::is_base_of<base_type, DesiredType>::value, "constructed object type is not derived from base_type");
        static_assert(sizeof(DesiredType) <= derived_object_max_size, "constructed object does not fit into available memory");
        static_assert(alignof(DesiredType) <= derived_object_alignment, "constructed object's alignment requirements are not met by this wrapper");

        const DesiredType *const test_ptr_d = reinterpret_cast<const DesiredType *>(uintptr_t(-1) / 2);
        const base_type *const test_ptr_b = static_cast<const base_type *>(test_ptr_d);
        const size_t offset = reinterpret_cast<const unsigned char *>(test_ptr_b) - reinterpret_cast<const unsigned char *>(test_ptr_d);

        polymorphic_wrapper other;
        other.DRP_set_offset_to_base(offset);
        POLYMORPHIC_WRAPPER_ASSERT(uintptr_t(other.derived_object_bytes_begin()) % derived_object_alignment == 0);
        POLYMORPHIC_WRAPPER_ASSERT(uintptr_t(this->derived_object_bytes_begin()) % derived_object_alignment == 0);
        ::new (other.derived_object_bytes_begin()) DesiredType(polymorphic_wrapper_lib::forward<CtorArgs>(args)...);

        *this = polymorphic_wrapper_lib::move(other);
    }

public:
    template<class DesiredType, typename... CtorArgs>
    inline void construct(CtorArgs&&... args)
    {
        POLYMORPHIC_WRAPPER_ASSERT(!is_constructed());
        this->impl_construct<DesiredType>(polymorphic_wrapper_lib::forward<CtorArgs>(args)...);
    }

    template<class DesiredType, typename... CtorArgs>
    inline static polymorphic_wrapper make(CtorArgs&&... args)
    {
        polymorphic_wrapper tmp;
        tmp.construct<DesiredType>(polymorphic_wrapper_lib::forward<CtorArgs>(args)...);
        return tmp;
    }

private:
    inline void destroy_object_unsafe() noexcept
    {
        this->object_ptr_unsafe()->~base_type();
    }

public:
    template<class DesiredType, typename... CtorArgs>
    inline void reset(CtorArgs&&... args)
    {
        this->impl_construct<DesiredType>(polymorphic_wrapper_lib::forward<CtorArgs>(args)...);
    }

    inline void reset() noexcept
    {
        this->clear();
    }

    inline void clear() noexcept
    {
        if (is_constructed()) {
            destroy_object_unsafe();
            polymorphic_wrapper_lib::set_bytes_to_zero(*this);
        }
    }

public:
    inline polymorphic_wrapper() noexcept
    {
        polymorphic_wrapper_lib::set_bytes_to_zero(*this);
    }

    inline polymorphic_wrapper(polymorphic_wrapper && other) noexcept
    {
        this->MCP_move_construct_from(polymorphic_wrapper_lib::move(other));
    }

    inline polymorphic_wrapper(const polymorphic_wrapper & other)
        : MCP_mixin(), DRP_mixin()
    {
        this->MCP_copy_construct_from(other);
    }

    inline polymorphic_wrapper & operator = (polymorphic_wrapper && other) noexcept
    {
        if (is_constructed()) {
            destroy_object_unsafe();
        }
        this->MCP_move_construct_from(polymorphic_wrapper_lib::move(other));
        return *this;
    }

    polymorphic_wrapper & operator = (const polymorphic_wrapper & other)
    {
        polymorphic_wrapper temp(other);
        *this = polymorphic_wrapper_lib::move(temp);
        return *this;
    }

    inline ~polymorphic_wrapper() noexcept
    {
        if (is_constructed()) {
            destroy_object_unsafe();
        }
    }
};

#undef POLYMORPHIC_WRAPPER_DEFINE_EMPTY_SPECIAL_MEMBER_FUNCTIONS

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning( pop )
#endif

#endif // POLYMORPHIC_WRAPPER_HPP_INCLUDED
