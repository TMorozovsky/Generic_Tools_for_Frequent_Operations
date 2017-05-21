#include "gtfo/_impl/type_traits/is_dereferenceable.hpp"
#define GTFO_IS_DEREFERENCEABLE(t) ::gtfo::_tt::is_dereferenceable<t>::value

namespace
{
    struct A { };
    struct B { private: void operator * () { } };
    struct C { void operator * () { } };
    struct D { void operator * () const { } };
}

static_assert(!GTFO_IS_DEREFERENCEABLE(A &), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(B &), "");
static_assert( GTFO_IS_DEREFERENCEABLE(      C &), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(const C &), "");
static_assert( GTFO_IS_DEREFERENCEABLE(      D &), "");
static_assert( GTFO_IS_DEREFERENCEABLE(const D &), "");

static_assert(!GTFO_IS_DEREFERENCEABLE(int), "");
static_assert( GTFO_IS_DEREFERENCEABLE(int *), "");
static_assert( GTFO_IS_DEREFERENCEABLE(const int *), "");
static_assert( GTFO_IS_DEREFERENCEABLE(const int *const), "");
static_assert( GTFO_IS_DEREFERENCEABLE(const int *const &), "");

static_assert(!GTFO_IS_DEREFERENCEABLE(               void *), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(         const void *), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(      volatile void *), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(const volatile void *), "");

static_assert(!GTFO_IS_DEREFERENCEABLE(               void *const), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(         const void *const), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(      volatile void *const), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(const volatile void *const), "");

static_assert(!GTFO_IS_DEREFERENCEABLE(               void *volatile), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(         const void *volatile), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(      volatile void *volatile), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(const volatile void *volatile), "");

static_assert(!GTFO_IS_DEREFERENCEABLE(               void *const volatile), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(         const void *const volatile), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(      volatile void *const volatile), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(const volatile void *const volatile), "");

static_assert(!GTFO_IS_DEREFERENCEABLE(               void * &), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(         const void * &), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(      volatile void * &), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(const volatile void * &), "");

static_assert(!GTFO_IS_DEREFERENCEABLE(               void *const &), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(         const void *const &), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(      volatile void *const &), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(const volatile void *const &), "");

static_assert(!GTFO_IS_DEREFERENCEABLE(               void *volatile &), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(         const void *volatile &), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(      volatile void *volatile &), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(const volatile void *volatile &), "");

static_assert(!GTFO_IS_DEREFERENCEABLE(               void *const volatile &), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(         const void *const volatile &), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(      volatile void *const volatile &), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(const volatile void *const volatile &), "");

