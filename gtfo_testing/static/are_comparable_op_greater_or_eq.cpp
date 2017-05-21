#include "gtfo/_impl/type_traits/are_comparable_op_greater_or_eq.hpp"
#define GTFO_CAN_INVOKE_OP_GREATER_OR_EQ(t1,t2) ::gtfo::_tt::detail::can_invoke_op_greater_or_eq<t1,t2>::value
#define GTFO_RESULT_OF_OP_GREATER_OR_EQ(t1,t2) typename ::gtfo::_tt::detail::result_of_op_greater_or_eq<t1,t2>::type
#define GTFO_ARE_COMPARABLE_OP_GREATER_OR_EQ(t1,t2) ::gtfo::_tt::are_comparable_op_greater_or_eq<t1,t2>::value

#define GTFO_RESULT_OF_OP_GREATER_OR_EQ_IS(t1,t2,t3) \
    ::gtfo::_tt::is_same \
    < \
        GTFO_RESULT_OF_OP_GREATER_OR_EQ(t1,t2), \
        t3 \
    >::value

#define GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(t) ::gtfo::_tt::can_be_used_in_boolean_context<t>::value

#ifdef __clang__
#   define UNUSED __attribute__((unused))
#else
#   define UNUSED
#endif

namespace
{
    struct Bool { operator bool() { return false; } };

    struct AL { };
    struct AR { };

    struct BL { };
    struct BR { };
    inline UNUSED void operator >= (BL, BR) { }

    struct CL { };
    struct CR { };
    inline UNUSED int operator >= (CL, CR) { return 42; }

    struct DL { };
    struct DR { };
    inline UNUSED Bool operator >= (DL, DR) { return Bool(); }
}

static_assert(GTFO_ARE_COMPARABLE_OP_GREATER_OR_EQ(int, int), "");
static_assert(GTFO_ARE_COMPARABLE_OP_GREATER_OR_EQ(int, float), "");
static_assert(GTFO_ARE_COMPARABLE_OP_GREATER_OR_EQ(int, unsigned long), "");

static_assert(!GTFO_CAN_INVOKE_OP_GREATER_OR_EQ(AL, AR), "");
static_assert(!GTFO_ARE_COMPARABLE_OP_GREATER_OR_EQ(AL, AR), "");

static_assert(GTFO_CAN_INVOKE_OP_GREATER_OR_EQ(BL, BR), "");
static_assert(GTFO_RESULT_OF_OP_GREATER_OR_EQ_IS(BL, BR, void), "");
static_assert(!GTFO_ARE_COMPARABLE_OP_GREATER_OR_EQ(BL, BR), "");

static_assert(GTFO_CAN_INVOKE_OP_GREATER_OR_EQ(CL, CR), "");
static_assert(GTFO_RESULT_OF_OP_GREATER_OR_EQ_IS(CL, CR, int), "");
static_assert(GTFO_ARE_COMPARABLE_OP_GREATER_OR_EQ(CL, CR), "");

static_assert(GTFO_CAN_INVOKE_OP_GREATER_OR_EQ(DL, DR), "");
static_assert(GTFO_RESULT_OF_OP_GREATER_OR_EQ_IS(DL, DR, Bool), "");
static_assert(GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(Bool), "");
static_assert(GTFO_ARE_COMPARABLE_OP_GREATER_OR_EQ(DL, DR), "");
