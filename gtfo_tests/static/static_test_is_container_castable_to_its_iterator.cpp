#include "gtfo/_impl/type_traits/is_container_castable_to_its_iterator.hpp"
#define GTFO_IS_CONTAINER_CASTABLE_TO_ITS_ITERATOR(t) ::gtfo::_tt::is_container_castable_to_its_iterator<t>::value

namespace
{
    struct X { int * begin() { return nullptr; }
               float * end() { return nullptr; }
               operator int * () { return begin(); } };

    struct Y { int * begin() { return nullptr; }
               int * end() { return nullptr; } };

    struct Z { int * begin() { return nullptr; }
               int * end() { return nullptr; }
               operator int * () { return begin(); } };
}

static_assert(!GTFO_IS_CONTAINER_CASTABLE_TO_ITS_ITERATOR(X), "");
static_assert(!GTFO_IS_CONTAINER_CASTABLE_TO_ITS_ITERATOR(Y), "");
static_assert(GTFO_IS_CONTAINER_CASTABLE_TO_ITS_ITERATOR(Z), "");
static_assert(GTFO_IS_CONTAINER_CASTABLE_TO_ITS_ITERATOR(int(&)[42]), "");
