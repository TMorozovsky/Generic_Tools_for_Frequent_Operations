#include "gtfo/_impl/type_traits/is_dereferenceable.hpp"
#define GTFO_IS_DEREFERENCEABLE(t) ::gtfo::_tt::is_dereferenceable<t>::value

namespace
{
    struct X { };
    struct Y { private: void operator * () { } };
    struct Z { void operator * () { } };
}

static_assert(!GTFO_IS_DEREFERENCEABLE(int), "");
static_assert(GTFO_IS_DEREFERENCEABLE(int *), "");
static_assert(!GTFO_IS_DEREFERENCEABLE(X &), "");
#ifndef GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC
static_assert(!GTFO_IS_DEREFERENCEABLE(Y &), "");
#endif
static_assert(GTFO_IS_DEREFERENCEABLE(Z &), "");
