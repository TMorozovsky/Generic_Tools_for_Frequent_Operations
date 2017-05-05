#include "gtfo/_impl/type_traits/are_comparable_op_eq.hpp"
#define GTFO_CAN_INVOKE_OP_EQ(t1,t2) ::gtfo::_tt::helpers::can_invoke_op_eq<t1,t2>::value
#define GTFO_RESULT_OF_OP_EQ(t1,t2) typename ::gtfo::_tt::helpers::result_of_op_eq<t1,t2>::type
#define GTFO_ARE_COMPARABLE_OP_EQ(t1,t2) ::gtfo::_tt::are_comparable_op_eq<t1,t2>::value

#define GTFO_RESULT_OF_OP_EQ_IS(t1,t2,t3) \
    ::gtfo::_tt::is_same \
    < \
        GTFO_RESULT_OF_OP_EQ(t1,t2), \
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
    struct PrivateBool { private: operator bool() { return false; } };
    struct NotBool { };

    struct AL { };
    struct AR { };

    struct BL { };
    struct BR { };
    inline UNUSED void operator == (BL, BR) { }

    struct CL { };
    struct CR { };
    inline UNUSED bool operator == (CL, CR) { return false; }

    struct DL { };
    struct DR { };
    inline UNUSED Bool operator == (DL, DR) { return Bool(); }

    struct EL { };
    struct ER { };
    inline UNUSED PrivateBool operator == (EL, ER) { return PrivateBool(); }

    struct FL { };
    struct FR { };
    inline UNUSED NotBool operator == (FL, FR) { return NotBool(); }
}

static_assert(GTFO_ARE_COMPARABLE_OP_EQ(int, int), "");
static_assert(GTFO_ARE_COMPARABLE_OP_EQ(int, float), "");
static_assert(GTFO_ARE_COMPARABLE_OP_EQ(int, unsigned long), "");

static_assert(!GTFO_CAN_INVOKE_OP_EQ(AL, AR), "");
static_assert(!GTFO_ARE_COMPARABLE_OP_EQ(AL, AR), "");

static_assert(GTFO_CAN_INVOKE_OP_EQ(BL, BR), "");
static_assert(GTFO_RESULT_OF_OP_EQ_IS(BL, BR, void), "");
static_assert(!GTFO_ARE_COMPARABLE_OP_EQ(BL, BR), "");

static_assert(GTFO_CAN_INVOKE_OP_EQ(CL, CR), "");
static_assert(GTFO_RESULT_OF_OP_EQ_IS(CL, CR, bool), "");
static_assert(GTFO_ARE_COMPARABLE_OP_EQ(CL, CR), "");

static_assert(GTFO_CAN_INVOKE_OP_EQ(DL, DR), "");
static_assert(GTFO_RESULT_OF_OP_EQ_IS(DL, DR, Bool), "");
static_assert(GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(Bool), "");
static_assert(GTFO_ARE_COMPARABLE_OP_EQ(DL, DR), "");

static_assert(GTFO_CAN_INVOKE_OP_EQ(EL, ER), "");
static_assert(GTFO_RESULT_OF_OP_EQ_IS(EL, ER, PrivateBool), "");
static_assert(!GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(PrivateBool), "");
static_assert(!GTFO_ARE_COMPARABLE_OP_EQ(EL, ER), "");

static_assert(GTFO_CAN_INVOKE_OP_EQ(FL, FR), "");
static_assert(GTFO_RESULT_OF_OP_EQ_IS(FL, FR, NotBool), "");
static_assert(!GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(NotBool), "");
static_assert(!GTFO_ARE_COMPARABLE_OP_EQ(FL, FR), "");
