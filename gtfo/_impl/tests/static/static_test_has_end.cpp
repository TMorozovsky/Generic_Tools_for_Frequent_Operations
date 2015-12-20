#include "gtfo/_impl/type_traits/has_end.hpp"
#define GTFO_HAS_END(t) ::gtfo::_tt::has_end<t>::value

static_assert(GTFO_HAS_END(int(&)[42]), "");

namespace
{
    struct X { void end() const { } };
    struct Y { };
}

static_assert(GTFO_HAS_END(X), "");
static_assert(!GTFO_HAS_END(Y), "");
