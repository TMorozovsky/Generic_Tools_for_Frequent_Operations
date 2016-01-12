#include "gtfo/_impl/type_traits/are_comparable_op_n_eq.hpp"
#define GTFO_CAN_INVOKE_OP_N_EQ(t1,t2) ::gtfo::_tt::helpers::can_invoke_op_n_eq<t1,t2>::value
#define GTFO_RESULT_OF_OP_N_EQ(t1,t2) typename ::gtfo::_tt::helpers::result_of_op_n_eq<t1,t2>::type
#define GTFO_ARE_COMPARABLE_OP_N_EQ(t1,t2) ::gtfo::_tt::are_comparable_op_n_eq<t1,t2>::value

#define GTFO_RESULT_OF_OP_N_EQ_IS(t1,t2,t3) \
    ::gtfo::_tt::is_same \
    < \
        GTFO_RESULT_OF_OP_N_EQ(t1,t2), \
        t3 \
    >::value

#define GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(t) ::gtfo::_tt::can_be_used_in_boolean_context<t>::value

namespace
{
    struct Bool { operator bool() { return false; } };

    struct AL { };
    struct AR { };

    struct BL { };
    struct BR { };
    inline void operator != (BL, BR) { }

    struct CL { };
    struct CR { };
    inline int operator != (CL, CR) { return 42; }

    struct DL { };
    struct DR { };
    inline Bool operator != (DL, DR) { return Bool(); }
}

static_assert(GTFO_ARE_COMPARABLE_OP_N_EQ(int, int), "");
static_assert(GTFO_ARE_COMPARABLE_OP_N_EQ(int, float), "");
static_assert(GTFO_ARE_COMPARABLE_OP_N_EQ(int, unsigned long), "");

static_assert(!GTFO_CAN_INVOKE_OP_N_EQ(AL, AR), "");
static_assert(!GTFO_ARE_COMPARABLE_OP_N_EQ(AL, AR), "");

static_assert(GTFO_CAN_INVOKE_OP_N_EQ(BL, BR), "");
static_assert(GTFO_RESULT_OF_OP_N_EQ_IS(BL, BR, void), "");
static_assert(!GTFO_ARE_COMPARABLE_OP_N_EQ(BL, BR), "");

static_assert(GTFO_CAN_INVOKE_OP_N_EQ(CL, CR), "");
static_assert(GTFO_RESULT_OF_OP_N_EQ_IS(CL, CR, int), "");
static_assert(GTFO_ARE_COMPARABLE_OP_N_EQ(CL, CR), "");

static_assert(GTFO_CAN_INVOKE_OP_N_EQ(DL, DR), "");
static_assert(GTFO_RESULT_OF_OP_N_EQ_IS(DL, DR, Bool), "");
static_assert(GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(Bool), "");
static_assert(GTFO_ARE_COMPARABLE_OP_N_EQ(DL, DR), "");
