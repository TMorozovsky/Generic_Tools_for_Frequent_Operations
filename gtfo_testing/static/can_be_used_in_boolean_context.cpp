#include "gtfo/_impl/type_traits/can_be_used_in_boolean_context.hpp"

#define GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(t) ::gtfo::_tt::can_be_used_in_boolean_context<t>::value

namespace
{
    struct Bool { operator bool() { return false; } };
    struct PrivateBool { private: operator bool() { return false; } };
    struct NotBool { };
}

static_assert(!GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(void), "");
static_assert( GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(bool), "");
static_assert( GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(const bool &), "");
static_assert( GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(volatile bool &&), "");
static_assert( GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(int), "");
static_assert( GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(unsigned char), "");

static_assert( GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(Bool), "");
#if !defined(GTFO_NEED_WORKAROUNDS_FOR_OLD_MSVC) && !defined(GTFO_NEED_WORKAROUNDS_FOR_MSVC_CLANG) // a bug in VC 2012 and VC/clang
static_assert(!GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(PrivateBool), "");
#endif
#ifndef GTFO_NEED_WORKAROUNDS_FOR_MSVC_CLANG // a bug in VC/clang
static_assert(!GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(NotBool), "");
#endif
