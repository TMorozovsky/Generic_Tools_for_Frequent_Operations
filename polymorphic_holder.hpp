#ifndef POLYMORPHIC_HOLDER_HPP_INCLUDED
#define POLYMORPHIC_HOLDER_HPP_INCLUDED

#if defined(_MSC_VER) || defined(__clang__) || defined(__GNUG__)
#   define POLYMORPHIC_HOLDER_USE_TYPE_TRAITS_COMPILER_EXTENSIONS
#endif

#include <cstdint> // std::uintptr_t
#include <cstring> // std::size_t, std::memcpy/memcpy_s, std::memset
#include <utility> // std::move, std::forward
#include <new>     // placement new

#ifndef POLYMORPHIC_HOLDER_USE_TYPE_TRAITS_COMPILER_EXTENSIONS
#   include <type_traits> // std::is_polymorphic, std::is_base_of
#endif

#include <cassert> // assert

//  Macro used by polymorphic_holder<...> for assertions.
#define POLYMORPHIC_HOLDER_ASSERT(x) \
    { \
        assert((x)); \
    }

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

#ifdef POLYMORPHIC_HOLDER_USE_TYPE_TRAITS_COMPILER_EXTENSIONS
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

#ifdef POLYMORPHIC_HOLDER_USE_TYPE_TRAITS_COMPILER_EXTENSIONS
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

    // Interface that must be implemented by a hierarchy of nothrow-moveable polymorphic objects.
    // This interface will be used by polymorphic_holder to move-construct its owned object
    // when the polymorphic_holder itself is being moved.
    // Essentially, this interface declares a "virtual move constructor" in the owned objects' hierarchy.
    class i_nothrow_moveable
    {
    public:
        // Move-constructs a derived object at given uninitialized, properly aligned buffer.
        virtual void move_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) && noexcept = 0;

    protected:
        constexpr i_nothrow_moveable() noexcept = default;
        inline ~i_nothrow_moveable() noexcept { }
    };

    // Interface that must be implemented by a hierarchy of moveable polymorphic objects
    // move constructors of which might throw exceptions.
    // This interface will be used by polymorphic_holder to move-construct its owned object
    // when the polymorphic_holder itself is being moved.
    // Essentially, this interface declares a "virtual move constructor" in the owned objects' hierarchy.
    class i_throwing_moveable
    {
    public:
        // Move-constructs a derived object at given uninitialized, properly aligned buffer. Might throw exceptions.
        virtual void move_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) && = 0;

    protected:
        constexpr i_throwing_moveable() noexcept = default;
        inline ~i_throwing_moveable() noexcept { }
    };

    // Interface that must be implemented by a hierarchy of nothrow-copyable polymorphic objects.
    // This interface will be used by polymorphic_holder to copy-construct its owned object
    // when the polymorphic_holder itself is being copied.
    // Essentially, this interface declares a "virtual copy constructor" in the owned objects' hierarchy.
    class i_nothrow_copyable
    {
    public:
        // Copy-constructs a derived object at given uninitialized, properly aligned buffer.
        virtual void copy_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) const noexcept = 0;

    protected:
        constexpr i_nothrow_copyable() noexcept = default;
        inline ~i_nothrow_copyable() noexcept { }
    };

    // Interface that must be implemented by a hierarchy of copyable polymorphic objects
    // copy constructors of which might throw exceptions.
    // This interface will be used by polymorphic_holder to copy-construct its owned object
    // when the polymorphic_holder itself is being copied.
    // Essentially, this interface declares a "virtual copy constructor" in the owned objects' hierarchy.
    class i_throwing_copyable
    {
    public:
        // Copy-constructs a derived object at given uninitialized, properly aligned buffer.
        virtual void copy_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) const = 0;

    protected:
        constexpr i_throwing_copyable() noexcept = default;
        inline ~i_throwing_copyable() noexcept { }
    };
}

// Utilities that are not used by polymorphic_holder directly but can be useful for client code.
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

    // "Returns" common max_size and max_alignment for all argument types.
    template<typename... Ts>
    struct type_traits
    {
        static constexpr size_t max_size = max_size_of<Ts...>::value;
        static constexpr size_t max_alignment = max_alignment_of<Ts...>::value;
    };

    // Interface that must be inherited by the base class of a polymorphic hierarchy
    // and implemented in each derived class via using the POLYMORPHIC_HOLDER_NOTHROW_MOVEABLE_ONLY macro
    // to allow polymorphic_holder objects to use the nothrow move constructors
    // of those derived objects. Copying will not be allowed.
    class nothrow_moveable_only : public polymorphic_holder_lib::i_nothrow_moveable
    {
    public:
        void copy_construct_at(unsigned char *, size_t) const = delete;

    protected:
        constexpr nothrow_moveable_only() noexcept = default;
        inline ~nothrow_moveable_only() noexcept { }
    };

    // Interface that must be inherited by the base class of a polymorphic hierarchy
    // and implemented in each derived class via using the POLYMORPHIC_HOLDER_THROWING_MOVEABLE_ONLY macro
    // to allow polymorphic_holder objects to use the throwing move constructors
    // of those derived objects. Copying will not be allowed.
    class throwing_moveable_only : public polymorphic_holder_lib::i_throwing_moveable
    {
    public:
        void copy_construct_at(unsigned char *, size_t) const = delete;

    protected:
        constexpr throwing_moveable_only() noexcept = default;
        inline ~throwing_moveable_only() noexcept { }
    };

    // Interface that must be inherited by the base class of a polymorphic hierarchy
    // and implemented in each derived class via using the POLYMORPHIC_HOLDER_NOTHROW_COPYABLE_ONLY macro
    // to allow polymorphic_holder objects to use the nothrow copy constructors
    // of those derived objects.
    class nothrow_copyable_only : public polymorphic_holder_lib::i_nothrow_copyable
    {
    public:
        void move_construct_at(unsigned char *, size_t) && = delete;

    protected:
        constexpr nothrow_copyable_only() noexcept = default;
        inline ~nothrow_copyable_only() noexcept { }
    };

    // Interface that must be inherited by the base class of a polymorphic hierarchy
    // and implemented in each derived class via using the POLYMORPHIC_HOLDER_THROWING_COPYABLE_ONLY macro
    // to allow polymorphic_holder objects to use the throwing copy constructors
    // of those derived objects.
    class throwing_copyable_only : public polymorphic_holder_lib::i_throwing_copyable
    {
    public:
        void move_construct_at(unsigned char *, size_t) && = delete;

    protected:
        constexpr throwing_copyable_only() noexcept = default;
        inline ~throwing_copyable_only() noexcept { }
    };

    // Interface that must be inherited by the base class of a polymorphic hierarchy
    // and implemented in each derived class via using the POLYMORPHIC_HOLDER_NOTHROW_MOVEABLE_AND_NOTHROW_COPYABLE macro
    // to allow polymorphic_holder objects to use the nothrow copy constructors and nothrow move constructors
    // of those derived objects.
    class nothrow_moveable_and_nothrow_copyable : public polymorphic_holder_lib::i_nothrow_moveable, public polymorphic_holder_lib::i_nothrow_copyable
    {
    protected:
        constexpr nothrow_moveable_and_nothrow_copyable() noexcept = default;
        inline ~nothrow_moveable_and_nothrow_copyable() noexcept { }
    };

    // Interface that must be inherited by the base class of a polymorphic hierarchy
    // and implemented in each derived class via using the POLYMORPHIC_HOLDER_NOTHROW_MOVEABLE_AND_THROWING_COPYABLE macro
    // to allow polymorphic_holder objects to use the throwing copy constructors and nothrow move constructors
    // of those derived objects.
    class nothrow_moveable_and_throwing_copyable : public polymorphic_holder_lib::i_nothrow_moveable, public polymorphic_holder_lib::i_throwing_copyable
    {
    protected:
        constexpr nothrow_moveable_and_throwing_copyable() noexcept = default;
        inline ~nothrow_moveable_and_throwing_copyable() noexcept { }
    };

    // Interface that must be inherited by the base class of a polymorphic hierarchy
    // and implemented in each derived class via using the POLYMORPHIC_HOLDER_THROWING_MOVEABLE_AND_THROWING_COPYABLE macro
    // to allow polymorphic_holder objects to use the throwing copy constructors and throwing move constructors
    // of those derived objects.
    class throwing_moveable_and_throwing_copyable : public polymorphic_holder_lib::i_throwing_moveable, public polymorphic_holder_lib::i_throwing_copyable
    {
    protected:
        constexpr throwing_moveable_and_throwing_copyable() noexcept = default;
        inline ~throwing_moveable_and_throwing_copyable() noexcept { }
    };
}

// Helper macro that can be used inside a class definition
// in order to provide a typical implementation of the move_construct_at() noexcept virtual function.
#define POLYMORPHIC_HOLDER_OVERRIDE_NOTHROW_MOVE_CONSTRUCT_AT(c) \
    virtual void move_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) && noexcept override \
    { \
        static_assert(polymorphic_holder_lib::is_base_of<polymorphic_holder_lib::i_nothrow_moveable, c>::value, \
                      "base class of this polymorphic hierarchy must inherit from one of the \"nothrow moveable\" interfaces " \
                      "from namespace polymorphic_holder_utils"); \
        static_assert(noexcept(c(polymorphic_holder_lib::move(*this))), "move construction of objects of this class is required to be noexcept"); \
        POLYMORPHIC_HOLDER_ASSERT(dest_buffer_begin != nullptr); \
        POLYMORPHIC_HOLDER_ASSERT(sizeof(c) <= dest_buffer_size); \
        POLYMORPHIC_HOLDER_ASSERT(0 == reinterpret_cast<uintptr_t>(dest_buffer_begin) % alignof(c)); \
        (void)dest_buffer_size; \
        ::new (dest_buffer_begin) c(polymorphic_holder_lib::move(*this)); \
    }

// Helper macro that can be used inside a class definition
// in order to provide a typical implementation of the throwing move_construct_at() virtual function.
#define POLYMORPHIC_HOLDER_OVERRIDE_THROWING_MOVE_CONSTRUCT_AT(c) \
    virtual void move_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) && override \
    { \
        static_assert(polymorphic_holder_lib::is_base_of<polymorphic_holder_lib::i_throwing_moveable, c>::value, \
                      "base class of this polymorphic hierarchy must inherit from one of the \"throwing moveable\" interfaces " \
                      "from namespace polymorphic_holder_utils"); \
        POLYMORPHIC_HOLDER_ASSERT(dest_buffer_begin != nullptr); \
        POLYMORPHIC_HOLDER_ASSERT(sizeof(c) <= dest_buffer_size); \
        POLYMORPHIC_HOLDER_ASSERT(0 == reinterpret_cast<uintptr_t>(dest_buffer_begin) % alignof(c)); \
        (void)dest_buffer_size; \
        ::new (dest_buffer_begin) c(polymorphic_holder_lib::move(*this)); \
    }

// Helper macro that can be used inside a class definition
// in order to provide a typical implementation of the copy_construct_at() noexcept virtual function.
#define POLYMORPHIC_HOLDER_OVERRIDE_NOTHROW_COPY_CONSTRUCT_AT(c) \
    virtual void copy_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) const noexcept override \
    { \
        static_assert(polymorphic_holder_lib::is_base_of<polymorphic_holder_lib::i_nothrow_copyable, c>::value, \
                      "base class of this polymorphic hierarchy must inherit from one of the \"nothrow copyable\" interfaces " \
                      "from namespace polymorphic_holder_utils"); \
        static_assert(noexcept(c(*this)), "copy construction of objects of this class is required to be noexcept"); \
        POLYMORPHIC_HOLDER_ASSERT(dest_buffer_begin != nullptr); \
        POLYMORPHIC_HOLDER_ASSERT(sizeof(c) <= dest_buffer_size); \
        POLYMORPHIC_HOLDER_ASSERT(0 == reinterpret_cast<uintptr_t>(dest_buffer_begin) % alignof(c)); \
        (void)dest_buffer_size; \
        ::new (dest_buffer_begin) c(*this); \
    }

// Helper macro that can be used inside a class definition
// in order to provide a typical implementation of the throwing copy_construct_at() virtual function.
#define POLYMORPHIC_HOLDER_OVERRIDE_THROWING_COPY_CONSTRUCT_AT(c) \
    virtual void copy_construct_at(unsigned char * dest_buffer_begin, size_t dest_buffer_size) const override \
    { \
        static_assert(polymorphic_holder_lib::is_base_of<polymorphic_holder_lib::i_throwing_copyable, c>::value, \
                      "base class of this polymorphic hierarchy must inherit from one of the \"throwing copyable\" interfaces " \
                      "from namespace polymorphic_holder_utils"); \
        POLYMORPHIC_HOLDER_ASSERT(dest_buffer_begin != nullptr); \
        POLYMORPHIC_HOLDER_ASSERT(sizeof(c) <= dest_buffer_size); \
        POLYMORPHIC_HOLDER_ASSERT(0 == reinterpret_cast<uintptr_t>(dest_buffer_begin) % alignof(c)); \
        (void)dest_buffer_size; \
        ::new (dest_buffer_begin) c(*this); \
    }

// Helper macro that can be used inside a class definition in order to delete the copy_construct_at() function.
#define POLYMORPHIC_HOLDER_DELETE_COPY_CONSTRUCT_AT() \
    void copy_construct_at(unsigned char *, size_t) const = delete;

// Helper macro that can be used inside a class definition in order to delete the move_construct_at() function.
#define POLYMORPHIC_HOLDER_DELETE_MOVE_CONSTRUCT_AT() \
    void move_construct_at(unsigned char *, size_t) && = delete;

// Macro that can be used inside definition of a derived class
// instances of which will be owned by polymorphic_holder<...> objects
// if polymorphic_holder's move operations are reqiured to invoke the
// noexcept move constructor of that derived class.
// The base class of such hierarchy must inherit from polymorphic_holder_utils::nothrow_moveable_only.
#define POLYMORPHIC_HOLDER_NOTHROW_MOVEABLE_ONLY(c) \
    POLYMORPHIC_HOLDER_OVERRIDE_NOTHROW_MOVE_CONSTRUCT_AT(c) \
    POLYMORPHIC_HOLDER_DELETE_COPY_CONSTRUCT_AT()

// Macro that can be used inside definition of a derived class
// instances of which will be owned by polymorphic_holder<...> objects
// if polymorphic_holder's move operations are reqiured to invoke the
// throwing move constructor of that derived class.
// The base class of such hierarchy must inherit from polymorphic_holder_utils::throwing_moveable_only.
#define POLYMORPHIC_HOLDER_THROWING_MOVEABLE_ONLY(c) \
    POLYMORPHIC_HOLDER_OVERRIDE_THROWING_MOVE_CONSTRUCT_AT(c) \
    POLYMORPHIC_HOLDER_DELETE_COPY_CONSTRUCT_AT()

// Macro that can be used inside definition of a derived class
// instances of which will be owned by polymorphic_holder<...> objects
// if polymorphic_holder's move and copy operations are reqiured to invoke the
// noexcept copy constructor of that derived class.
// The base class of such hierarchy must inherit from polymorphic_holder_utils::nothrow_copyable_only.
#define POLYMORPHIC_HOLDER_NOTHROW_COPYABLE_ONLY(c) \
    POLYMORPHIC_HOLDER_OVERRIDE_NOTHROW_COPY_CONSTRUCT_AT(c) \
    POLYMORPHIC_HOLDER_DELETE_MOVE_CONSTRUCT_AT()

// Macro that can be used inside definition of a derived class
// instances of which will be owned by polymorphic_holder<...> objects
// if polymorphic_holder's move and copy operations are reqiured to invoke the
// throwing copy constructor of that derived class.
// The base class of such hierarchy must inherit from polymorphic_holder_utils::throwing_copyable_only.
#define POLYMORPHIC_HOLDER_THROWING_COPYABLE_ONLY(c) \
    POLYMORPHIC_HOLDER_OVERRIDE_THROWING_COPY_CONSTRUCT_AT(c) \
    POLYMORPHIC_HOLDER_DELETE_MOVE_CONSTRUCT_AT()

// Macro that can be used inside definition of a derived class
// instances of which will be owned by polymorphic_holder<...> objects
// if polymorphic_holder's move and copy operations are reqiured to invoke the
// nothrow move constructor and nothrow copy constructor of that derived class, respectively.
// The base class of such hierarchy must inherit from polymorphic_holder_utils::nothrow_moveable_and_nothrow_copyable.
#define POLYMORPHIC_HOLDER_NOTHROW_MOVEABLE_AND_NOTHROW_COPYABLE(c) \
    POLYMORPHIC_HOLDER_OVERRIDE_NOTHROW_MOVE_CONSTRUCT_AT(c) \
    POLYMORPHIC_HOLDER_OVERRIDE_NOTHROW_COPY_CONSTRUCT_AT(c)

// Macro that can be used inside definition of a derived class
// instances of which will be owned by polymorphic_holder<...> objects
// if polymorphic_holder's move and copy operations are reqiured to invoke the
// nothrow move constructor and throwing copy constructor of that derived class, respectively.
// The base class of such hierarchy must inherit from polymorphic_holder_utils::nothrow_moveable_and_throwing_copyable.
#define POLYMORPHIC_HOLDER_NOTHROW_MOVEABLE_AND_THROWING_COPYABLE(c) \
    POLYMORPHIC_HOLDER_OVERRIDE_NOTHROW_MOVE_CONSTRUCT_AT(c) \
    POLYMORPHIC_HOLDER_OVERRIDE_THROWING_COPY_CONSTRUCT_AT(c)

// Macro that can be used inside definition of a derived class
// instances of which will be owned by polymorphic_holder<...> objects
// if polymorphic_holder's move and copy operations are reqiured to invoke the
// throwing move constructor and throwing copy constructor of that derived class, respectively.
// The base class of such hierarchy must inherit from polymorphic_holder_utils::throwing_moveable_and_throwing_copyable.
#define POLYMORPHIC_HOLDER_THROWING_MOVEABLE_AND_THROWING_COPYABLE(c) \
    POLYMORPHIC_HOLDER_OVERRIDE_THROWING_MOVE_CONSTRUCT_AT(c) \
    POLYMORPHIC_HOLDER_OVERRIDE_THROWING_COPY_CONSTRUCT_AT(c)

namespace polymorphic_holder_lib
{
    template<class BitwiseCopyablePolymorphicHolder>
    inline void copy_as_bytes(BitwiseCopyablePolymorphicHolder & dest, const BitwiseCopyablePolymorphicHolder & src) noexcept
    {
        static_assert(sizeof(dest) >= BitwiseCopyablePolymorphicHolder::max_object_size, "");
        polymorphic_holder_lib::memcpy_s(static_cast<void *>(&dest), sizeof(dest), static_cast<const void *>(&src), sizeof(src));
    }

    template<class PolymorphicHolder>
    inline void set_bytes_to_zero(PolymorphicHolder & dest) noexcept
    {
        static_assert(sizeof(dest) >= PolymorphicHolder::max_object_size, "");
        polymorphic_holder_lib::memset(static_cast<void *>(&dest), 0, sizeof(dest));
    }

    // Guard that will be used inside a code block where throwing construction takes place.
    // If an exception is thrown during construction
    // before guard's set_constructed() member function was invoked
    // its destructor will fill PolymorphicHolder's bytes with zeroes
    // in order to prevent it from remaining partially constructed.
    template<class PolymorphicHolder>
    class scoped_throwing_construction_guard
    {
    private:
        PolymorphicHolder & _ref_polymorphic_holder;
        bool _is_constructed;

    public:
        inline explicit scoped_throwing_construction_guard(PolymorphicHolder & ref_polymorphic_holder) noexcept
            : _ref_polymorphic_holder(ref_polymorphic_holder), _is_constructed(false)
        {
        }

        scoped_throwing_construction_guard              (const scoped_throwing_construction_guard &) = delete;
        scoped_throwing_construction_guard & operator = (const scoped_throwing_construction_guard &) = delete;
        scoped_throwing_construction_guard              (scoped_throwing_construction_guard &&)      = delete;
        scoped_throwing_construction_guard & operator = (scoped_throwing_construction_guard &&)      = delete;

        // Invoke this function when throwing construction has successfully been finished
        // to disable the guard.
        inline void set_constructed() noexcept
        {
            POLYMORPHIC_HOLDER_ASSERT(_is_constructed == false);
            _is_constructed = true;
        }

        inline ~scoped_throwing_construction_guard()
        {
            if (!_is_constructed) {
                polymorphic_holder_lib::set_bytes_to_zero(_ref_polymorphic_holder);
            }
        }
    };

    template<class PolymorphicHolder>
    inline void nothrow_move_construct(PolymorphicHolder & uninitialized_dest, PolymorphicHolder && src) noexcept
    {
        i_nothrow_moveable * p = static_cast<i_nothrow_moveable *>(src.object_ptr_safe());
        if (p) {
            polymorphic_holder_lib::move(*p).move_construct_at(uninitialized_dest.object_bytes_begin(), PolymorphicHolder::max_object_size);
            uninitialized_dest.DRP_set_offset_to_base(src.DRP_offset_to_base());
        } else {
            polymorphic_holder_lib::set_bytes_to_zero(uninitialized_dest);
        }
    }

    template<class PolymorphicHolder>
    inline void throwing_move_construct(PolymorphicHolder & uninitialized_dest, PolymorphicHolder && src)
    {
        i_throwing_moveable * p = static_cast<i_throwing_moveable *>(src.object_ptr_safe());
        if (p) {
            polymorphic_holder_lib::scoped_throwing_construction_guard<PolymorphicHolder> guard(uninitialized_dest);
            polymorphic_holder_lib::move(*p).move_construct_at(uninitialized_dest.object_bytes_begin(), PolymorphicHolder::max_object_size);
            guard.set_constructed();
            uninitialized_dest.DRP_set_offset_to_base(src.DRP_offset_to_base());
        } else {
            polymorphic_holder_lib::set_bytes_to_zero(uninitialized_dest);
        }
    }

    template<class PolymorphicHolder>
    inline void nothrow_copy_construct(PolymorphicHolder & uninitialized_dest, const PolymorphicHolder & src) noexcept
    {
        const i_nothrow_copyable * p = static_cast<const i_nothrow_copyable *>(src.object_ptr_safe());
        if (p) {
            p->copy_construct_at(uninitialized_dest.object_bytes_begin(), PolymorphicHolder::max_object_size);
            uninitialized_dest.DRP_set_offset_to_base(src.DRP_offset_to_base());
        } else {
            polymorphic_holder_lib::set_bytes_to_zero(uninitialized_dest);
        }
    }

    template<class PolymorphicHolder>
    inline void throwing_copy_construct(PolymorphicHolder & uninitialized_dest, const PolymorphicHolder & src)
    {
        const i_throwing_copyable * p = static_cast<const i_throwing_copyable *>(src.object_ptr_safe());
        if (p) {
            polymorphic_holder_lib::scoped_throwing_construction_guard<PolymorphicHolder> guard(uninitialized_dest);
            p->copy_construct_at(uninitialized_dest.object_bytes_begin(), PolymorphicHolder::max_object_size);
            guard.set_constructed();
            uninitialized_dest.DRP_set_offset_to_base(src.DRP_offset_to_base());
        } else {
            polymorphic_holder_lib::set_bytes_to_zero(uninitialized_dest);
        }
    }
}

// Namespace for polymorphic_holder's Moving/Copying Policies.
//
// These policies specify how a polymorphic_holder should move and - if supported - copy its contents.
// A Moving/Copying Policy will be inherited by polymorphic_holder<...> implementation.
//
// Move-only policies must provide a member function
//     void MCP_move_construct_from(polymorphic_holder &&)
// to polymorphic_holder<...> template instances.
//
// Policies that support both moving and copying must provide both
//     void MCP_move_construct_from(polymorphic_holder &&)
// and
//     void MCP_copy_construct_from(const polymorphic_holder &)
// to polymorphic_holder.
//
// These construction functions must either be noexcept or guarantee that
// if an exception is thrown during construction, the destination polymorphic_holder's data buffer
// will be filled with zeroes, so that it will not contain any partially constructed data.
//
// Also, each Moving/Copying Policy must provide static boolean constants
//     static constexpr bool MCP_is_nothrow_move_constructible
// and
//     static constexpr bool MCP_is_nothrow_copy_constructible
// that will tell whether the respective construction functions are noexcept.
//
// The MCP_move_construct_from() function will be used by polymorphic_holder's
// move constructor and move assignment operator, while MCP_copy_construct_from() will be used
// by polymorphic_holder's copy constructor and copy assignment operator.
namespace polymorphic_holder_MCP
{
    // Stub that can be used instead of a valid Moving/Copying Policy in those rare cases
    // where no support of moving or copying is required at all.
    template<class PolymorphicHolderSelf>
    class no_moving_or_copying
    {
    protected:
        static constexpr bool MCP_is_nothrow_move_constructible = false;
        static constexpr bool MCP_is_nothrow_copy_constructible = false;

        void MCP_move_construct_from(PolymorphicHolderSelf &&) = delete;
        void MCP_copy_construct_from(const PolymorphicHolderSelf &) = delete;
    };

    // Implements moving of polymorphic_holder<...> instances as if they were POD.
    // Move-constructing from a polymorphic_holder which uses this policy also sets all of its bits to zero.
    // Does not allow copying.
    template<class PolymorphicHolderSelf>
    class bitwise_exclusive_move
    {
    protected:
        static constexpr bool MCP_is_nothrow_move_constructible = true;
        static constexpr bool MCP_is_nothrow_copy_constructible = false;

        inline void MCP_move_construct_from(PolymorphicHolderSelf && other) noexcept
        {
            polymorphic_holder_lib::copy_as_bytes(static_cast<PolymorphicHolderSelf &>(*this), other);
            polymorphic_holder_lib::set_bytes_to_zero(other);
        }

        void MCP_copy_construct_from(const PolymorphicHolderSelf &) = delete;

        constexpr bitwise_exclusive_move() noexcept = default;
        inline ~bitwise_exclusive_move() noexcept { }
    };

    // Implements both moving and copying of polymorphic_holder<...> instances as if they were POD.
    // Bits of moved-from objects are not modified, moving and copying is essentially
    // the same operation with this policy.
    template<class PolymorphicHolderSelf>
    class bitwise_copying
    {
    protected:
        static constexpr bool MCP_is_nothrow_move_constructible = true;
        static constexpr bool MCP_is_nothrow_copy_constructible = true;

        inline void MCP_move_construct_from(PolymorphicHolderSelf && other) noexcept
        {
            polymorphic_holder_lib::copy_as_bytes(static_cast<PolymorphicHolderSelf &>(*this), other);
        }

        inline void MCP_copy_construct_from(const PolymorphicHolderSelf & other) noexcept
        {
            polymorphic_holder_lib::copy_as_bytes(static_cast<PolymorphicHolderSelf &>(*this), other);
        }

        constexpr bitwise_copying() noexcept = default;
        inline ~bitwise_copying() noexcept { }
    };

    // Implements moving of polymorphic_holder<...> instances
    // using the noexcept "virtual move constructor"
    // provided by
    //     - inheriting the base class of a polymorphic hierarchy from
    //       polymorphic_holder_utils::nothrow_moveable_only
    // and
    //     - adding the POLYMORPHIC_HOLDER_NOTHROW_MOVEABLE_ONLY(DerivedType) macro
    //       to all of derived classes' definitions.
    // Does not allow copying.
    template<class PolymorphicHolderSelf>
    class nothrow_moveable_only
    {
    protected:
        static constexpr bool MCP_is_nothrow_move_constructible = true;
        static constexpr bool MCP_is_nothrow_copy_constructible = false;

        inline void MCP_move_construct_from(PolymorphicHolderSelf && other) noexcept
        {
            polymorphic_holder_lib::nothrow_move_construct(static_cast<PolymorphicHolderSelf &>(*this), polymorphic_holder_lib::move(other));
        }

        void MCP_copy_construct_from(const PolymorphicHolderSelf &) = delete;

        constexpr nothrow_moveable_only() noexcept = default;
        inline ~nothrow_moveable_only() noexcept { }
    };

    // Implements moving of polymorphic_holder<...> instances
    // using the throwing "virtual move constructor"
    // provided by
    //     - inheriting the base class of a polymorphic hierarchy from
    //       polymorphic_holder_utils::throwing_moveable_only
    // and
    //     - adding the POLYMORPHIC_HOLDER_THROWING_MOVEABLE_ONLY(DerivedType) macro
    //       to all of derived classes' definitions.
    // Does not allow copying.
    template<class PolymorphicHolderSelf>
    class throwing_moveable_only
    {
    protected:
        static constexpr bool MCP_is_nothrow_move_constructible = false;
        static constexpr bool MCP_is_nothrow_copy_constructible = false;

        inline void MCP_move_construct_from(PolymorphicHolderSelf && other)
        {
            polymorphic_holder_lib::throwing_move_construct(static_cast<PolymorphicHolderSelf &>(*this), polymorphic_holder_lib::move(other));
        }

        void MCP_copy_construct_from(const PolymorphicHolderSelf &) = delete;

        constexpr throwing_moveable_only() noexcept = default;
        inline ~throwing_moveable_only() noexcept { }
    };

    // Implements copying of polymorphic_holder<...> instances
    // using the noexcept "virtual copy constructor"
    // provided by
    //     - inheriting the base class of a polymorphic hierarchy from
    //       polymorphic_holder_utils::nothrow_copyable_only
    // and
    //     - adding the POLYMORPHIC_HOLDER_NOTHROW_COPYABLE_ONLY(DerivedType) macro
    //       to all of derived classes' definitions.
    // Moving is the same operation as copying.
    template<class PolymorphicHolderSelf>
    class nothrow_copyable_only
    {
    protected:
        static constexpr bool MCP_is_nothrow_move_constructible = true;
        static constexpr bool MCP_is_nothrow_copy_constructible = true;

        inline void MCP_copy_construct_from(const PolymorphicHolderSelf & other) noexcept
        {
            polymorphic_holder_lib::nothrow_copy_construct(static_cast<PolymorphicHolderSelf &>(*this), other);
        }

        inline void MCP_move_construct_from(PolymorphicHolderSelf && other) noexcept
        {
            this->MCP_copy_construct_from(other);
        }

        constexpr nothrow_copyable_only() noexcept = default;
        inline ~nothrow_copyable_only() noexcept { }
    };

    // Implements copying of polymorphic_holder<...> instances
    // using the throwing "virtual copy constructor"
    // provided by
    //     - inheriting the base class of a polymorphic hierarchy from
    //       polymorphic_holder_utils::throwing_copyable_only
    // and
    //     - adding the POLYMORPHIC_HOLDER_THROWING_COPYABLE_ONLY(DerivedType) macro
    //       to all of derived classes' definitions.
    // Moving is the same operation as copying.
    template<class PolymorphicHolderSelf>
    class throwing_copyable_only
    {
    protected:
        static constexpr bool MCP_is_nothrow_move_constructible = false;
        static constexpr bool MCP_is_nothrow_copy_constructible = false;

        inline void MCP_copy_construct_from(const PolymorphicHolderSelf & other)
        {
            polymorphic_holder_lib::throwing_copy_construct(static_cast<PolymorphicHolderSelf &>(*this), other);
        }

        inline void MCP_move_construct_from(PolymorphicHolderSelf && other)
        {
            this->MCP_copy_construct_from(other);
        }

        constexpr throwing_copyable_only() noexcept = default;
        inline ~throwing_copyable_only() noexcept { }
    };

    // Implements both moving and copying of polymorphic_holder<...> instances
    // using the noexcept "virtual move constructor" and noexcept "virtual copy constructor"
    // provided by
    //     - inheriting the base class of a polymorphic hierarchy from
    //       polymorphic_holder_utils::nothrow_moveable_and_nothrow_copyable
    // and
    //     - adding the POLYMORPHIC_HOLDER_NOTHROW_MOVEABLE_AND_NOTHROW_COPYABLE(DerivedType) macro
    //       to all of derived classes' definitions.
    template<class PolymorphicHolderSelf>
    class nothrow_moveable_and_nothrow_copyable
    {
    protected:
        static constexpr bool MCP_is_nothrow_move_constructible = true;
        static constexpr bool MCP_is_nothrow_copy_constructible = true;

        inline void MCP_move_construct_from(PolymorphicHolderSelf && other) noexcept
        {
            polymorphic_holder_lib::nothrow_move_construct(static_cast<PolymorphicHolderSelf &>(*this), polymorphic_holder_lib::move(other));
        }

        inline void MCP_copy_construct_from(const PolymorphicHolderSelf & other) noexcept
        {
            polymorphic_holder_lib::nothrow_copy_construct(static_cast<PolymorphicHolderSelf &>(*this), other);
        }

        constexpr nothrow_moveable_and_nothrow_copyable() noexcept = default;
        inline ~nothrow_moveable_and_nothrow_copyable() noexcept { }
    };

    // Implements both moving and copying of polymorphic_holder<...> instances
    // using the noexcept "virtual move constructor" and throwing "virtual copy constructor"
    // provided by
    //     - inheriting the base class of a polymorphic hierarchy from
    //       polymorphic_holder_utils::nothrow_moveable_and_throwing_copyable
    // and
    //     - adding the POLYMORPHIC_HOLDER_NOTHROW_MOVEABLE_AND_THROWING_COPYABLE(DerivedType) macro
    //       to all of derived classes' definitions.
    template<class PolymorphicHolderSelf>
    class nothrow_moveable_and_throwing_copyable
    {
    protected:
        static constexpr bool MCP_is_nothrow_move_constructible = true;
        static constexpr bool MCP_is_nothrow_copy_constructible = false;

        inline void MCP_move_construct_from(PolymorphicHolderSelf && other) noexcept
        {
            polymorphic_holder_lib::nothrow_move_construct(static_cast<PolymorphicHolderSelf &>(*this), polymorphic_holder_lib::move(other));
        }

        inline void MCP_copy_construct_from(const PolymorphicHolderSelf & other)
        {
            polymorphic_holder_lib::throwing_copy_construct(static_cast<PolymorphicHolderSelf &>(*this), other);
        }

        constexpr nothrow_moveable_and_throwing_copyable() noexcept = default;
        inline ~nothrow_moveable_and_throwing_copyable() noexcept { }
    };

    // Implements both moving and copying of polymorphic_holder<...> instances
    // using the throwing "virtual move constructor" and throwing "virtual copy constructor"
    // provided by
    //     - inheriting the base class of a polymorphic hierarchy from
    //       polymorphic_holder_utils::throwing_moveable_and_throwing_copyable
    // and
    //     - adding the POLYMORPHIC_HOLDER_THROWING_MOVEABLE_AND_THROWING_COPYABLE(DerivedType) macro
    //       to all of derived classes' definitions.
    template<class PolymorphicHolderSelf>
    class throwing_moveable_and_throwing_copyable
    {
    protected:
        static constexpr bool MCP_is_nothrow_move_constructible = false;
        static constexpr bool MCP_is_nothrow_copy_constructible = false;

        inline void MCP_move_construct_from(PolymorphicHolderSelf && other)
        {
            polymorphic_holder_lib::throwing_move_construct(static_cast<PolymorphicHolderSelf &>(*this), polymorphic_holder_lib::move(other));
        }

        inline void MCP_copy_construct_from(const PolymorphicHolderSelf & other)
        {
            polymorphic_holder_lib::throwing_copy_construct(static_cast<PolymorphicHolderSelf &>(*this), other);
        }

        constexpr throwing_moveable_and_throwing_copyable() noexcept = default;
        inline ~throwing_moveable_and_throwing_copyable() noexcept { }
    };
}

// Namespace for polymorphic_holder's Data Representation Policies.
//
// A Data Representation Policy will be inherited by polymorphic_holder<...> implementation.
// It must comply with the following template:
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
// the derived objects owned by polymorphic_holder will be constructed.
//
// The DRP_offset_to_base() and DRP_set_offset_to_base() functions will be used to get/set value
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
    namespace detail
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
            constexpr bytes_array_only_impl() noexcept                                         = default;
            inline bytes_array_only_impl(const bytes_array_only_impl &) noexcept               { }
            inline bytes_array_only_impl & operator = (const bytes_array_only_impl &) noexcept { return *this; }
            inline bytes_array_only_impl(bytes_array_only_impl &&) noexcept                    { }
            inline bytes_array_only_impl & operator = (bytes_array_only_impl &&) noexcept      { return *this; }
            inline ~bytes_array_only_impl() noexcept                                           { }
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
            constexpr offset_before_bytes_impl() noexcept                                            = default;
            inline offset_before_bytes_impl(const offset_before_bytes_impl &) noexcept               { }
            inline offset_before_bytes_impl & operator = (const offset_before_bytes_impl &) noexcept { return *this; }
            inline offset_before_bytes_impl(offset_before_bytes_impl &&) noexcept                    { }
            inline offset_before_bytes_impl & operator = (offset_before_bytes_impl &&) noexcept      { return *this; }
            inline ~offset_before_bytes_impl() noexcept                                              { }
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
            constexpr offset_after_bytes_impl() noexcept                                           = default;
            inline offset_after_bytes_impl(const offset_after_bytes_impl &) noexcept               { }
            inline offset_after_bytes_impl & operator = (const offset_after_bytes_impl &) noexcept { return *this; }
            inline offset_after_bytes_impl(offset_after_bytes_impl &&) noexcept                    { }
            inline offset_after_bytes_impl & operator = (offset_after_bytes_impl &&) noexcept      { return *this; }
            inline ~offset_after_bytes_impl() noexcept                                             { }
        };
    }

    // Data Representation Policy for polymorphic_holder that stores an aligned array of bytes only.
    // Does not support derived classes with non-zero offsets to their base subobject!
    class bytes_array_only
    {
    public:
        template<size_t MaxObjectSize, size_t ObjectAlignment>
        using implementation = detail::bytes_array_only_impl<MaxObjectSize, ObjectAlignment>;

        bytes_array_only() = delete;
        ~bytes_array_only() = delete;
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
        using implementation = detail::offset_before_bytes_impl<MaxObjectSize, ObjectAlignment, size_t>;

        offset_before_bytes() = delete;
        ~offset_before_bytes() = delete;
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
        using implementation = detail::offset_before_bytes_impl<MaxObjectSize, ObjectAlignment, OffsetType>;

        offset_before_bytes_typed() = delete;
        ~offset_before_bytes_typed() = delete;
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
        using implementation = detail::offset_after_bytes_impl<MaxObjectSize, ObjectAlignment, size_t>;

        offset_after_bytes() = delete;
        ~offset_after_bytes() = delete;
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
        using implementation = detail::offset_after_bytes_impl<MaxObjectSize, ObjectAlignment, OffsetType>;

        offset_after_bytes_typed() = delete;
        ~offset_after_bytes_typed() = delete;
    };
}

namespace polymorphic_holder_lib
{
    struct nothrow_tag { };
    struct throwing_tag { };

    template<bool nothrow_flag> struct get_noexcept_specification_tag;
    template<>                  struct get_noexcept_specification_tag<true>  { using result = polymorphic_holder_lib::nothrow_tag; };
    template<>                  struct get_noexcept_specification_tag<false> { using result = polymorphic_holder_lib::throwing_tag; };

    template<class BaseType, class DerivedType>
    inline size_t get_offset_to_base_from_derived() noexcept
    {
        static_assert(polymorphic_holder_lib::is_base_of<BaseType, DerivedType>::value, "invalid base and derived types");

        using derived_type_ptr = const DerivedType *;
        using base_type_ptr = const BaseType *;

        const derived_type_ptr test_ptr_d = reinterpret_cast<derived_type_ptr>(uintptr_t(-1) / 2);
        const base_type_ptr test_ptr_b = static_cast<base_type_ptr>(test_ptr_d);
        const size_t offset = reinterpret_cast<const unsigned char *>(test_ptr_b) - reinterpret_cast<const unsigned char *>(test_ptr_d);

        return offset;
    }

#ifndef POLYMORPHIC_HOLDER_DISABLE_OVERLOADED_PENDING_MEMBER_FUNCTION_CALL_OPERATOR
    namespace detail
    {
        template<class BaseType, typename DecoratedBaseType, typename ResultType, typename ... ArgTypes>
        struct pending_member_function_call_traits;

        template<class BaseType, typename ResultType, typename ... ArgTypes>
        struct pending_member_function_call_traits<BaseType,                          BaseType,                  ResultType, ArgTypes...> {
            using base_object_pointer                                               = BaseType *;
            using pointer_to_member_function = ResultType (BaseType::*) (ArgTypes...);
        };
        template<class BaseType, typename ResultType, typename ... ArgTypes>
        struct pending_member_function_call_traits<BaseType,                          const BaseType,            ResultType, ArgTypes...> {
            using base_object_pointer                                               = const BaseType *;
            using pointer_to_member_function = ResultType (BaseType::*) (ArgTypes...) const;
        };
        template<class BaseType, typename ResultType, typename ... ArgTypes>
        struct pending_member_function_call_traits<BaseType,                          volatile BaseType,         ResultType, ArgTypes...> {
            using base_object_pointer                                               = volatile BaseType *;
            using pointer_to_member_function = ResultType (BaseType::*) (ArgTypes...) volatile;
        };
        template<class BaseType, typename ResultType, typename ... ArgTypes>
        struct pending_member_function_call_traits<BaseType,                          const volatile BaseType,   ResultType, ArgTypes...> {
            using base_object_pointer                                               = const volatile BaseType *;
            using pointer_to_member_function = ResultType (BaseType::*) (ArgTypes...) const volatile;
        };
        template<class BaseType, typename ResultType, typename ... ArgTypes>
        struct pending_member_function_call_traits<BaseType,                          BaseType &,                ResultType, ArgTypes...> {
            using base_object_pointer                                               = BaseType *;
            using pointer_to_member_function = ResultType (BaseType::*) (ArgTypes...) &;
        };
        template<class BaseType, typename ResultType, typename ... ArgTypes>
        struct pending_member_function_call_traits<BaseType,                          const BaseType &,          ResultType, ArgTypes...> {
            using base_object_pointer                                               = const BaseType *;
            using pointer_to_member_function = ResultType (BaseType::*) (ArgTypes...) const &;
        };
        template<class BaseType, typename ResultType, typename ... ArgTypes>
        struct pending_member_function_call_traits<BaseType,                          volatile BaseType &,       ResultType, ArgTypes...> {
            using base_object_pointer                                               = volatile BaseType *;
            using pointer_to_member_function = ResultType (BaseType::*) (ArgTypes...) volatile &;
        };
        template<class BaseType, typename ResultType, typename ... ArgTypes>
        struct pending_member_function_call_traits<BaseType,                          const volatile BaseType &, ResultType, ArgTypes...> {
            using base_object_pointer                                               = const volatile BaseType *;
            using pointer_to_member_function = ResultType (BaseType::*) (ArgTypes...) const volatile &;
        };

        template<class BaseType, typename DecoratedBaseType, typename ResultType, typename ... ArgTypes>
        class pending_member_function_call_impl
        {
            using traits_type = detail::pending_member_function_call_traits<BaseType, DecoratedBaseType, ResultType, ArgTypes...>;

            using base_object_pointer        = typename traits_type::base_object_pointer;
            using pointer_to_member_function = typename traits_type::pointer_to_member_function;

        private:
            const base_object_pointer        _p_base_object;
            const pointer_to_member_function _p_member_function;

        public:
            inline pending_member_function_call_impl(base_object_pointer p_base_object, pointer_to_member_function p_member_function) noexcept
                : _p_base_object(p_base_object), _p_member_function(p_member_function)
            {
                POLYMORPHIC_HOLDER_ASSERT(p_base_object != nullptr);
                POLYMORPHIC_HOLDER_ASSERT(p_member_function != nullptr);
            }

            inline ~pending_member_function_call_impl() noexcept
            {
            }

            template<typename ... ForwardedArgs>
            inline ResultType operator () (ForwardedArgs&&... args) const
                noexcept(noexcept((_p_base_object->*_p_member_function)(polymorphic_holder_lib::forward<ForwardedArgs>(args)...)))
            {
                return (_p_base_object->*_p_member_function)(polymorphic_holder_lib::forward<ForwardedArgs>(args)...);
            }
        };

    }

    template<class BaseType, typename ResultType, typename ... ArgTypes>
    using pending_member_function_call                       = detail::pending_member_function_call_impl<BaseType, BaseType, ResultType, ArgTypes...>;

    template<class BaseType, typename ResultType, typename ... ArgTypes>
    using pending_const_member_function_call                 = detail::pending_member_function_call_impl<BaseType, const BaseType, ResultType, ArgTypes...>;

    template<class BaseType, typename ResultType, typename ... ArgTypes>
    using pending_volatile_member_function_call              = detail::pending_member_function_call_impl<BaseType, volatile BaseType, ResultType, ArgTypes...>;

    template<class BaseType, typename ResultType, typename ... ArgTypes>
    using pending_const_volatile_member_function_call        = detail::pending_member_function_call_impl<BaseType, const volatile BaseType, ResultType, ArgTypes...>;

    template<class BaseType, typename ResultType, typename ... ArgTypes>
    using pending_lvalue_member_function_call                = detail::pending_member_function_call_impl<BaseType, BaseType &, ResultType, ArgTypes...>;

    template<class BaseType, typename ResultType, typename ... ArgTypes>
    using pending_const_lvalue_member_function_call          = detail::pending_member_function_call_impl<BaseType, const BaseType &, ResultType, ArgTypes...>;

    template<class BaseType, typename ResultType, typename ... ArgTypes>
    using pending_volatile_lvalue_member_function_call       = detail::pending_member_function_call_impl<BaseType, volatile BaseType &, ResultType, ArgTypes...>;

    template<class BaseType, typename ResultType, typename ... ArgTypes>
    using pending_const_volatile_lvalue_member_function_call = detail::pending_member_function_call_impl<BaseType, const volatile BaseType &, ResultType, ArgTypes...>;
#endif // !defined POLYMORPHIC_HOLDER_DISABLE_OVERLOADED_PENDING_MEMBER_FUNCTION_CALL_OPERATOR
}

// Holder that stores one object of any type derived from BaseType (may also be BaseType itself),
// provided that its size and alignment meet given MaxObjectSize and ObjectAlignment requirements.
//
// Allows the user to work with these objects via pointers/references to BaseType polymorphically
// and still avoid the unwanted dynamic memory allocations.
//
// See namespace polymorphic_holder_MCP to choose a Moving/Copying Policy
// that will specify how polymorphic_holder instances shall be moved and copied.
// Be extremely careful with bitwise copying/moving if you choose them as Moving/Copying Policy,
// since far from all classes can work correctly with this kind of operations.
//
// See namespace polymorphic_holder_DRP to choose a Data Representation Policy
// that will specify how polymorphic_holder's internal state shall be stored.
//
// Virtual inheritance is not supported! But that's no big deal since nobody uses it anyway, right? ;)
template<class BaseType, size_t MaxObjectSize, size_t ObjectAlignment,
         template<class> class MovingCopyingPolicy = polymorphic_holder_MCP::no_moving_or_copying,
         class DataRepresentationPolicy = polymorphic_holder_DRP::bytes_array_only>
class polymorphic_holder
    : private MovingCopyingPolicy<polymorphic_holder<BaseType, MaxObjectSize, ObjectAlignment, MovingCopyingPolicy, DataRepresentationPolicy>>
    , private DataRepresentationPolicy::template implementation<MaxObjectSize, ObjectAlignment>
{
    static_assert(polymorphic_holder_lib::is_polymorphic<BaseType>::value, "base type must be polymorphic");

    template<class PolymorphicHolder> friend void polymorphic_holder_lib::nothrow_move_construct(PolymorphicHolder & uninitialized_dest, PolymorphicHolder && src) noexcept;
    template<class PolymorphicHolder> friend void polymorphic_holder_lib::throwing_move_construct(PolymorphicHolder & uninitialized_dest, PolymorphicHolder && src);
    template<class PolymorphicHolder> friend void polymorphic_holder_lib::nothrow_copy_construct(PolymorphicHolder & uninitialized_dest, const PolymorphicHolder & src) noexcept;
    template<class PolymorphicHolder> friend void polymorphic_holder_lib::throwing_copy_construct(PolymorphicHolder & uninitialized_dest, const PolymorphicHolder & src);

    friend class MovingCopyingPolicy<polymorphic_holder<BaseType, MaxObjectSize, ObjectAlignment, MovingCopyingPolicy, DataRepresentationPolicy>>;
    friend typename DataRepresentationPolicy::template implementation<MaxObjectSize, ObjectAlignment>;

    using mcp_type = MovingCopyingPolicy<polymorphic_holder<BaseType, MaxObjectSize, ObjectAlignment, MovingCopyingPolicy, DataRepresentationPolicy>>;

public:
    using base_type = BaseType;
    static constexpr size_t max_object_size = MaxObjectSize;
    static constexpr size_t object_alignment = ObjectAlignment;

public:
    // Constructs a polymorphic_holder that doesn't store any object.
    inline polymorphic_holder() noexcept
    {
        polymorphic_holder_lib::set_bytes_to_zero(*this);
    }

    // Move-constructs a polymorphic_holder using the move construction function
    // provided by polymorphic_holder's Moving/Copying Policy.
    inline polymorphic_holder(polymorphic_holder && other) noexcept(mcp_type::MCP_is_nothrow_move_constructible)
    {
        static_assert(noexcept(this->MCP_move_construct_from(polymorphic_holder_lib::move(other))) == mcp_type::MCP_is_nothrow_move_constructible,
                      "conflicting \"MCP_is_nothrow_move_constructible\" static constant in current Moving/Copying Policy");
        this->MCP_move_construct_from(polymorphic_holder_lib::move(other));
    }

    // Copy-constructs a polymorphic_holder using the copy construction function
    // provided by polymorphic_holder's Moving/Copying Policy.
    inline polymorphic_holder(const polymorphic_holder & other) noexcept(mcp_type::MCP_is_nothrow_copy_constructible)
    {
        static_assert(noexcept(this->MCP_copy_construct_from(other)) == mcp_type::MCP_is_nothrow_copy_constructible,
                      "conflicting \"MCP_is_nothrow_copy_constructible\" static constant in current Moving/Copying Policy");
        this->MCP_copy_construct_from(other);
    }

    // Destroys the stored object from the BaseType hierarchy (if any).
    inline ~polymorphic_holder() noexcept
    {
        this->destroy_object_safe();
    }

    // Move-assigns contents of other polymorphic_holder to *this.
    //
    // This effectively destroys the derived object previously stored in *this
    // and move-constructs a new one using the move construction function provided by
    // polymorphic_holder's Moving/Copying Policy.
    //
    // If move construction cannot throw, move assignment gives no-throw guarantee as well.
    //
    // If move construction may throw, only basic exception safety is guaranteed:
    // the destination object (*this) will be left empty in case of exception, its previous contents will be destroyed.
    inline polymorphic_holder & operator = (polymorphic_holder && other) noexcept(mcp_type::MCP_is_nothrow_move_constructible)
    {
        this->destroy_object_safe();
        this->MCP_move_construct_from(polymorphic_holder_lib::move(other));
        return *this;
    }

    // Copy-assigns contents of other polymorphic_holder to *this.
    //
    // This effectively destroys the derived object previously stored in *this
    // and copy-constructs a new one using the copy construction function provided by
    // polymorphic_holder's Moving/Copying Policy.
    //
    // If copy construction cannot throw, copy assignment gives no-throw guarantee as well.
    //
    // If copy construction may throw but move construction cannot,
    // a temporary object is copy-constructed and then move-assigned to *this,
    // thus guaranteeing strong exception safety.
    //
    // If both copy construction and move construction may throw, only basic guarantee is given:
    // the destination object (*this) will be left empty in case of exception, its previous contents will be destroyed.
    inline polymorphic_holder & operator = (const polymorphic_holder & other) noexcept(mcp_type::MCP_is_nothrow_copy_constructible)
    {
        using copy_tag = typename polymorphic_holder_lib::get_noexcept_specification_tag<mcp_type::MCP_is_nothrow_copy_constructible>::result;
        using move_tag = typename polymorphic_holder_lib::get_noexcept_specification_tag<mcp_type::MCP_is_nothrow_move_constructible>::result;
        static_assert(noexcept(this->impl_copy_assign(other, copy_tag(), move_tag())) == mcp_type::MCP_is_nothrow_copy_constructible, "");
        this->impl_copy_assign(other, copy_tag(), move_tag());
        return *this;
    }

    // Returns a reference to the stored object from the BaseType hierarchy.
    // The behavior is undefined if this polymorphic_holder is empty.
    inline       base_type & operator * ()       noexcept { return this->object_ref(); }
    inline const base_type & operator * () const noexcept { return this->object_ref(); }

    // Returns a pointer to the stored object from the BaseType hierarchy.
    // The behavior is undefined if this polymorphic_holder is empty.
    inline       base_type * operator -> ()       noexcept { POLYMORPHIC_HOLDER_ASSERT(this->owns_object()); return this->object_ptr_unsafe(); }
    inline const base_type * operator -> () const noexcept { POLYMORPHIC_HOLDER_ASSERT(this->owns_object()); return this->object_ptr_unsafe(); }

#ifndef POLYMORPHIC_HOLDER_DISABLE_OVERLOADED_PENDING_MEMBER_FUNCTION_CALL_OPERATOR
    template<typename ResultType, typename ... ArgTypes>
    inline polymorphic_holder_lib::pending_member_function_call<base_type, ResultType, ArgTypes...>
        operator ->* (ResultType (base_type::*p_mem_fun)(ArgTypes...)) noexcept
    {
        POLYMORPHIC_HOLDER_ASSERT(this->owns_object());
        return polymorphic_holder_lib::pending_member_function_call<base_type, ResultType, ArgTypes...>(this->object_ptr_unsafe(), p_mem_fun);
    }

    template<typename ResultType, typename ... ArgTypes>
    inline polymorphic_holder_lib::pending_const_member_function_call<base_type, ResultType, ArgTypes...>
        operator ->* (ResultType (base_type::*p_mem_fun)(ArgTypes...) const) noexcept
    {
        POLYMORPHIC_HOLDER_ASSERT(this->owns_object());
        return polymorphic_holder_lib::pending_const_member_function_call<base_type, ResultType, ArgTypes...>(this->object_ptr_unsafe(), p_mem_fun);
    }

    template<typename ResultType, typename ... ArgTypes>
    inline polymorphic_holder_lib::pending_volatile_member_function_call<base_type, ResultType, ArgTypes...>
        operator ->* (ResultType (base_type::*p_mem_fun)(ArgTypes...) volatile) noexcept
    {
        POLYMORPHIC_HOLDER_ASSERT(this->owns_object());
        return polymorphic_holder_lib::pending_volatile_member_function_call<base_type, ResultType, ArgTypes...>(this->object_ptr_unsafe(), p_mem_fun);
    }

    template<typename ResultType, typename ... ArgTypes>
    inline polymorphic_holder_lib::pending_const_volatile_member_function_call<base_type, ResultType, ArgTypes...>
        operator ->* (ResultType (base_type::*p_mem_fun)(ArgTypes...) const volatile) noexcept
    {
        POLYMORPHIC_HOLDER_ASSERT(this->owns_object());
        return polymorphic_holder_lib::pending_const_volatile_member_function_call<base_type, ResultType, ArgTypes...>(this->object_ptr_unsafe(), p_mem_fun);
    }

    template<typename ResultType, typename ... ArgTypes>
    inline polymorphic_holder_lib::pending_lvalue_member_function_call<base_type, ResultType, ArgTypes...>
        operator ->* (ResultType (base_type::*p_mem_fun)(ArgTypes...) &) noexcept
    {
        POLYMORPHIC_HOLDER_ASSERT(this->owns_object());
        return polymorphic_holder_lib::pending_lvalue_member_function_call<base_type, ResultType, ArgTypes...>(this->object_ptr_unsafe(), p_mem_fun);
    }

    template<typename ResultType, typename ... ArgTypes>
    inline polymorphic_holder_lib::pending_const_lvalue_member_function_call<base_type, ResultType, ArgTypes...>
        operator ->* (ResultType (base_type::*p_mem_fun)(ArgTypes...) const &) noexcept
    {
        POLYMORPHIC_HOLDER_ASSERT(this->owns_object());
        return polymorphic_holder_lib::pending_const_lvalue_member_function_call<base_type, ResultType, ArgTypes...>(this->object_ptr_unsafe(), p_mem_fun);
    }

    template<typename ResultType, typename ... ArgTypes>
    inline polymorphic_holder_lib::pending_volatile_lvalue_member_function_call<base_type, ResultType, ArgTypes...>
        operator ->* (ResultType (base_type::*p_mem_fun)(ArgTypes...) volatile &) noexcept
    {
        POLYMORPHIC_HOLDER_ASSERT(this->owns_object());
        return polymorphic_holder_lib::pending_volatile_lvalue_member_function_call<base_type, ResultType, ArgTypes...>(this->object_ptr_unsafe(), p_mem_fun);
    }

    template<typename ResultType, typename ... ArgTypes>
    inline polymorphic_holder_lib::pending_const_volatile_lvalue_member_function_call<base_type, ResultType, ArgTypes...>
        operator ->* (ResultType (base_type::*p_mem_fun)(ArgTypes...) const volatile &) noexcept
    {
        POLYMORPHIC_HOLDER_ASSERT(this->owns_object());
        return polymorphic_holder_lib::pending_const_volatile_lvalue_member_function_call<base_type, ResultType, ArgTypes...>(this->object_ptr_unsafe(), p_mem_fun);
    }
#endif // !defined POLYMORPHIC_HOLDER_DISABLE_OVERLOADED_PENDING_MEMBER_FUNCTION_CALL_OPERATOR

    // Checks whether this polymorphic_holder stores an object from the BaseType hierarchy.
    inline explicit operator bool() const noexcept { return  this->owns_object(); }
    inline bool     operator !   () const noexcept { return !this->owns_object(); }

    // Constructs a new object of DesiredType that must be derived from polymorphic_holder's BaseType.
    // sizeof(DesiredType) and alignof(DesiredType) must comply with
    // polymorphic_holder's MaxObjectSize and ObjectAlignment.
    //
    // Precondition: *this polymorphic_holder object must be initially empty.
    // If you are not sure whether a polymorphic_holder object is empty,
    // use its reset() member function instead.
    //
    // May throw if and only if the required constructor of DesiredType throws. Otherwise is noexcept.
    //
    // If an exception is thrown during construction, *this object is guaranteed to be left in empty state.
    template<class DesiredType, typename... CtorArgs>
    inline void construct(CtorArgs&&... args) noexcept(noexcept(DesiredType(polymorphic_holder_lib::forward<CtorArgs>(args)...)))
    {
        POLYMORPHIC_HOLDER_ASSERT(!this->owns_object());
        constexpr bool construction_nothrow_flag = noexcept(DesiredType(polymorphic_holder_lib::forward<CtorArgs>(args)...));
        using construction_specification_tag = typename polymorphic_holder_lib::get_noexcept_specification_tag<construction_nothrow_flag>::result;
        this->impl_construct<DesiredType>(construction_specification_tag(), polymorphic_holder_lib::forward<CtorArgs>(args)...);
    }

    // Constructs a new object of DesiredType that must be derived from polymorphic_holder's BaseType.
    // sizeof(DesiredType) and alignof(DesiredType) must comply with
    // polymorphic_holder's MaxObjectSize and ObjectAlignment.
    //
    // The old object previously owned by *this polymorphic_holder (if any) will be destroyed.
    //
    // If the required constructor of DesiredType cannot throw, gives no-throw guarantee.
    //
    // If DesiredType's constructor may throw but polymorphic_holder's move construction/assignment cannot,
    // creates a temporary polymorphic_holder object and then move-assigns it to *this,
    // thus guaranteeing strong exception safety.
    //
    // If both DesiredType's constructor and polymorphic_holder's move construction/assignment may throw,
    // gives basic guarantee: the destination polymorphic_holder object will be left empty in case of exception,
    // with its previous contents destroyed.
    template<class DesiredType, typename... CtorArgs>
    inline void reset(CtorArgs&&... args) noexcept(noexcept(DesiredType(polymorphic_holder_lib::forward<CtorArgs>(args)...)))
    {
        constexpr bool construction_nothrow_flag = noexcept(DesiredType(polymorphic_holder_lib::forward<CtorArgs>(args)...));
        using construction_specification_tag = typename polymorphic_holder_lib::get_noexcept_specification_tag<construction_nothrow_flag>::result;
        using move_specification_tag = typename polymorphic_holder_lib::get_noexcept_specification_tag<mcp_type::MCP_is_nothrow_move_constructible>::result;
        this->impl_reset<DesiredType>(construction_specification_tag(), move_specification_tag(), polymorphic_holder_lib::forward<CtorArgs>(args)...);
    }

    // Same as clear().
    inline void reset() noexcept
    {
        this->clear();
    }

    // Destroys the derived class' instance that was previously owned by *this polymorphic_holder (if any),
    // leaving the polymorphic_holder in empty state.
    inline void clear() noexcept
    {
        if (this->owns_object()) {
            this->destroy_object_unsafe();
            polymorphic_holder_lib::set_bytes_to_zero(*this);
        }
    }

    // Creates and returns a polymorphic_holder that owns a new object of DesiredType
    // constructed with given args.
    //
    // DesiredType must be derived from polymorphic_holder's BaseType.
    // sizeof(DesiredType) and alignof(DesiredType) must comply with
    // polymorphic_holder's MaxObjectSize and ObjectAlignment.
    template<class DesiredType, typename... CtorArgs>
    inline static polymorphic_holder make(CtorArgs&&... args) noexcept(noexcept(DesiredType(polymorphic_holder_lib::forward<CtorArgs>(args)...)) && mcp_type::MCP_is_nothrow_move_constructible)
    {
        polymorphic_holder temp;
        temp.construct<DesiredType>(polymorphic_holder_lib::forward<CtorArgs>(args)...);
        return temp;
    }

    // Creates and returns an empty polymorphic_holder.
    inline static polymorphic_holder make() noexcept(mcp_type::MCP_is_nothrow_move_constructible)
    {
        return polymorphic_holder();
    }

private:
    // Checks whether an object of some type derived from BaseType
    // is stored inside this polymorphic_holder.
    inline bool owns_object() const noexcept
    {
        static_assert(sizeof(this->DRP_object_bytes) == max_object_size, "invalid size of DRP_object_bytes array");
        for (size_t i = 0; i < max_object_size; ++i) {
            if (this->DRP_object_bytes[i])
                return true;
        }
        return false;
    }

    inline unsigned char * object_bytes_begin() noexcept { return &this->DRP_object_bytes[0]; }

    inline       base_type * object_ptr_unsafe()       noexcept { return reinterpret_cast<      base_type *>(&this->DRP_object_bytes[this->DRP_offset_to_base()]); }
    inline const base_type * object_ptr_unsafe() const noexcept { return reinterpret_cast<const base_type *>(&this->DRP_object_bytes[this->DRP_offset_to_base()]); }

    inline       base_type * object_ptr_safe()       noexcept { return this->owns_object() ? this->object_ptr_unsafe() : nullptr; }
    inline const base_type * object_ptr_safe() const noexcept { return this->owns_object() ? this->object_ptr_unsafe() : nullptr; }

    inline       base_type & object_ref()       noexcept { POLYMORPHIC_HOLDER_ASSERT(this->owns_object()); return *(this->object_ptr_unsafe()); }
    inline const base_type & object_ref() const noexcept { POLYMORPHIC_HOLDER_ASSERT(this->owns_object()); return *(this->object_ptr_unsafe()); }

    // Destroys the derived object owned by this polymorphic_holder.
    // Precondition: such object must exist when this function is called.
    inline void destroy_object_unsafe() noexcept
    {
        POLYMORPHIC_HOLDER_ASSERT(this->owns_object());
        this->object_ptr_unsafe()->~base_type();
    }

    // Destroys the derived object owned by this polymorphic_holder.
    // Does nothing if the polymorphic_holder is empty.
    inline void destroy_object_safe() noexcept
    {
        if (this->owns_object()) {
            this->destroy_object_unsafe();
        }
    }

private: // impl_construct<DesiredType>(construction_specification_tag, args)
         //
         // Implementation of polymorphic_holder's member function construct<DesiredType>(CtorArgs&&... args).
         // Dispatches on exception specification tag of DesiredType's constructor which takes given args pack.

    // DesiredType's constructor cannot throw, so we just construct an object inplace with no-throw guarantee.
    template<class DesiredType, typename... CtorArgs>
    inline void impl_construct(polymorphic_holder_lib::nothrow_tag, CtorArgs&&... args) noexcept
    {
        static_assert(polymorphic_holder_lib::is_base_of<base_type, DesiredType>::value, "constructed object type is not derived from base_type");
        static_assert(sizeof(DesiredType) <= max_object_size, "constructed object does not fit into available memory");
        static_assert(alignof(DesiredType) <= object_alignment, "constructed object's alignment requirements are not met by this polymorphic_holder");

        static_assert(noexcept(DesiredType(polymorphic_holder_lib::forward<CtorArgs>(args)...)), "");
        POLYMORPHIC_HOLDER_ASSERT(uintptr_t(this->object_bytes_begin()) % object_alignment == 0);
        ::new (this->object_bytes_begin()) DesiredType(polymorphic_holder_lib::forward<CtorArgs>(args)...);

        const size_t offset = polymorphic_holder_lib::get_offset_to_base_from_derived<base_type, DesiredType>();
        this->DRP_set_offset_to_base(offset);
    }

    // DesiredType's constructor may throw, so we use a scoped guard to ensure that *this object
    // will not be left in a partially constructed state if an exception is thrown during construction.
    template<class DesiredType, typename... CtorArgs>
    inline void impl_construct(polymorphic_holder_lib::throwing_tag, CtorArgs&&... args)
    {
        static_assert(polymorphic_holder_lib::is_base_of<base_type, DesiredType>::value, "constructed object type is not derived from base_type");
        static_assert(sizeof(DesiredType) <= max_object_size, "constructed object does not fit into available memory");
        static_assert(alignof(DesiredType) <= object_alignment, "constructed object's alignment requirements are not met by this polymorphic_holder");

        polymorphic_holder_lib::scoped_throwing_construction_guard<polymorphic_holder> guard(*this);
        POLYMORPHIC_HOLDER_ASSERT(uintptr_t(this->object_bytes_begin()) % object_alignment == 0);
        ::new (this->object_bytes_begin()) DesiredType(polymorphic_holder_lib::forward<CtorArgs>(args)...);
        guard.set_constructed();

        const size_t offset = polymorphic_holder_lib::get_offset_to_base_from_derived<base_type, DesiredType>();
        this->DRP_set_offset_to_base(offset);
    }

private: // impl_copy_assign(const polymorphic_holder & other, copy_construction_specification_tag, move_assignment_specification_tag)
         //
         // Implementation of polymorphic_holder's copy assignment operator.
         // Dispatches on two exception specification tags:
         // 1) is polymorphic_holder's copy construction noexcept?
         // 2) is polymorphic_holder's move assignment noexcept?

    // Copy construction cannot throw, so we can simply destroy the old object
    // and construct a new one inplace with no-throw guarantee.
    template<typename move_assignment_specification_tag>
    inline void impl_copy_assign(const polymorphic_holder & other, polymorphic_holder_lib::nothrow_tag, move_assignment_specification_tag) noexcept
    {
        this->destroy_object_safe();
        static_assert(noexcept(this->MCP_copy_construct_from(other)), "");
        this->MCP_copy_construct_from(other);
    }

    // Copy construction may throw but move assignment cannot,
    // so we create a temporary copy of other object and move-assign it to *this,
    // thus guaranteeing strong exception safety.
    inline void impl_copy_assign(const polymorphic_holder & other, polymorphic_holder_lib::throwing_tag, polymorphic_holder_lib::nothrow_tag)
    {
        polymorphic_holder temp(other);
        static_assert(noexcept(*this = polymorphic_holder_lib::move(temp)), "");
        *this = polymorphic_holder_lib::move(temp);
    }

    // In the worst case, when even move construction may throw, we can give only basic guarantee:
    // if copy construction fails, the destination object (*this) will be left empty.
    inline void impl_copy_assign(const polymorphic_holder & other, polymorphic_holder_lib::throwing_tag, polymorphic_holder_lib::throwing_tag)
    {
        this->destroy_object_safe();
        this->MCP_copy_construct_from(other);
    }

private: // impl_reset<DesiredType>(construction_specification_tag, move_specification_tag, args)
         //
         // Implementation of polymorphic_holder's member function reset<DesiredType>(CtorArgs&&... args).
         // Dispatches on two exception specification tags:
         // 1) is DesiredType's constructor (the one which takes given args pack) noexcept?
         // 2) is polymorphic_holder's move assignment operator noexcept?

    // DesiredType's constructor is noexcept, so we don't care whether polymorphic_holder's move assignment operator is noexcept;
    // we just destroy the old object and construct a new one inplace with no-throw guarantee.
    template<class DesiredType, typename move_specification_tag, typename... CtorArgs>
    inline void impl_reset(polymorphic_holder_lib::nothrow_tag, move_specification_tag, CtorArgs&&... args) noexcept
    {
        static_assert(noexcept(DesiredType(polymorphic_holder_lib::forward<CtorArgs>(args)...)), "");
        this->destroy_object_safe();
        this->impl_construct<DesiredType>(polymorphic_holder_lib::nothrow_tag(), polymorphic_holder_lib::forward<CtorArgs>(args)...);
    }

    // DesiredType's constructor may throw, but polymorphic_holder's move assignment operator will not,
    // so we construct a temporary object and move-assign it to *this, thus guaranteeing strong exception safety.
    template<class DesiredType, typename... CtorArgs>
    inline void impl_reset(polymorphic_holder_lib::throwing_tag, polymorphic_holder_lib::nothrow_tag, CtorArgs&&... args)
    {
        polymorphic_holder temp;
        static_assert(!noexcept(DesiredType(polymorphic_holder_lib::forward<CtorArgs>(args)...)), "");
        static_assert(noexcept(*this = polymorphic_holder_lib::move(temp)), "");
        temp.impl_construct<DesiredType>(polymorphic_holder_lib::throwing_tag(), polymorphic_holder_lib::forward<CtorArgs>(args)...);
        *this = polymorphic_holder_lib::move(temp);
    }

    // In the worst case, when both DesiredType's constructor and polymorphic_holder's move assignment operator may throw,
    // we can give only basic guarantee: if DesiredType's construction fails, the destination object (*this) will be left empty.
    template<class DesiredType, typename... CtorArgs>
    inline void impl_reset(polymorphic_holder_lib::throwing_tag, polymorphic_holder_lib::throwing_tag, CtorArgs&&... args)
    {
        this->destroy_object_safe();
        this->impl_construct<DesiredType>(polymorphic_holder_lib::throwing_tag(), polymorphic_holder_lib::forward<CtorArgs>(args)...);
    }
};

#if defined(_MSC_VER) && !defined(__clang__)
#   pragma warning( pop )
#endif

#if defined(_MSC_VER) || defined(__clang__) || defined(__GNUG__)
#   undef POLYMORPHIC_HOLDER_USE_TYPE_TRAITS_COMPILER_EXTENSIONS
#endif

#endif // POLYMORPHIC_HOLDER_HPP_INCLUDED
