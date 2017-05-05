#include "gtfo/_impl/type_traits/has_begin.hpp"
#define GTFO_HAS_BEGIN(t) ::gtfo::_tt::has_begin<t>::value

static_assert(GTFO_HAS_BEGIN(int(&)[42]), "");

namespace
{
    struct X { void begin() const { } };
    struct Y { };
}

static_assert(GTFO_HAS_BEGIN(X), "");
static_assert(!GTFO_HAS_BEGIN(Y), "");
