#include "gtfo/_impl/type_traits/is_equality_comparable.hpp"
#define GTFO_CAN_INVOKE_COMPARISON_EQ(t1,t2) ::gtfo::_tt::helpers::can_invoke_comparison_eq<t1,t2>::value
#define GTFO_CAN_INVOKE_COMPARISON_N_EQ(t1,t2) ::gtfo::_tt::helpers::can_invoke_comparison_n_eq<t1,t2>::value
#define GTFO_RESULT_OF_EQ_COMPARISON(t1,t2) typename ::gtfo::_tt::helpers::result_of_eq_comparison<t1,t2>::type
#define GTFO_RESULT_OF_N_EQ_COMPARISON(t1,t2) typename ::gtfo::_tt::helpers::result_of_n_eq_comparison<t1,t2>::type
#define GTFO_IS_EQUALITY_COMPARABLE(t1,t2) ::gtfo::_tt::is_equality_comparable<t1,t2>::value

#define GTFO_RESULT_OF_EQ_COMPARISON_IS(t1,t2,t3) \
    ::gtfo::_tt::is_same \
    < \
        GTFO_RESULT_OF_EQ_COMPARISON(t1,t2), \
        t3 \
    >::value
#define GTFO_RESULT_OF_N_EQ_COMPARISON_IS(t1,t2,t3) \
    ::gtfo::_tt::is_same \
    < \
        GTFO_RESULT_OF_N_EQ_COMPARISON(t1,t2), \
        t3 \
    >::value

#define GTFO_CAN_BE_USED_IN_A_BOOLEAN_CONTEXT(t) ::gtfo::_tt::helpers::can_be_used_in_a_boolean_context<t>::value

namespace
{
    struct Bool { operator bool() { return false; } };
    struct PrivateBool { private: operator bool() { return false; } };
    struct NotBool { };

    struct AL { };
    struct AR { };

    struct BL { };
    struct BR { };
    void operator == (BL, BR) { }
    void operator != (BL, BR) { }

    struct CL { };
    struct CR { };
    bool operator == (CL, CR) { return false; }
    void operator != (CL, CR) { }

    struct DL { };
    struct DR { };
    void operator == (DL, DR) { }
    bool operator != (DL, DR) { return false; }

    struct EL { };
    struct ER { };
    bool operator == (EL, ER) { return false; }
    bool operator != (EL, ER) { return false; }

    struct FL { };
    struct FR { };
    Bool operator == (FL, FR) { return Bool(); }
    Bool operator != (FL, FR) { return Bool(); }

    struct GL { };
    struct GR { };
    PrivateBool operator == (GL, GR) { return PrivateBool(); }
    PrivateBool operator != (GL, GR) { return PrivateBool(); }

    struct HL { };
    struct HR { };
    NotBool operator == (HL, HR) { return NotBool(); }
    Bool operator != (HL, HR) { return Bool(); }
}

static_assert(GTFO_IS_EQUALITY_COMPARABLE(int, int), "");
static_assert(GTFO_IS_EQUALITY_COMPARABLE(int, float), "");
static_assert(GTFO_IS_EQUALITY_COMPARABLE(int, unsigned long), "");

static_assert(!GTFO_CAN_INVOKE_COMPARISON_EQ(AL, AR), "");
static_assert(!GTFO_CAN_INVOKE_COMPARISON_N_EQ(AL, AR), "");
static_assert(!GTFO_IS_EQUALITY_COMPARABLE(AL, AR), "");

static_assert(GTFO_CAN_INVOKE_COMPARISON_EQ(BL, BR), "");
static_assert(GTFO_CAN_INVOKE_COMPARISON_N_EQ(BL, BR), "");
static_assert(GTFO_RESULT_OF_EQ_COMPARISON_IS(BL, BR, void), "");
static_assert(GTFO_RESULT_OF_N_EQ_COMPARISON_IS(BL, BR, void), "");
static_assert(!GTFO_IS_EQUALITY_COMPARABLE(BL, BR), "");

static_assert(GTFO_CAN_INVOKE_COMPARISON_EQ(CL, CR), "");
static_assert(GTFO_CAN_INVOKE_COMPARISON_N_EQ(CL, CR), "");
static_assert(GTFO_RESULT_OF_EQ_COMPARISON_IS(CL, CR, bool), "");
static_assert(GTFO_RESULT_OF_N_EQ_COMPARISON_IS(CL, CR, void), "");
static_assert(!GTFO_IS_EQUALITY_COMPARABLE(CL, CR), "");

static_assert(GTFO_CAN_INVOKE_COMPARISON_EQ(DL, DR), "");
static_assert(GTFO_CAN_INVOKE_COMPARISON_N_EQ(DL, DR), "");
static_assert(GTFO_RESULT_OF_EQ_COMPARISON_IS(DL, DR, void), "");
static_assert(GTFO_RESULT_OF_N_EQ_COMPARISON_IS(DL, DR, bool), "");
static_assert(!GTFO_IS_EQUALITY_COMPARABLE(DL, DR), "");

static_assert(GTFO_CAN_INVOKE_COMPARISON_EQ(EL, ER), "");
static_assert(GTFO_CAN_INVOKE_COMPARISON_N_EQ(EL, ER), "");
static_assert(GTFO_RESULT_OF_EQ_COMPARISON_IS(EL, ER, bool), "");
static_assert(GTFO_RESULT_OF_N_EQ_COMPARISON_IS(EL, ER, bool), "");
static_assert(GTFO_IS_EQUALITY_COMPARABLE(EL, ER), "");

static_assert(GTFO_CAN_INVOKE_COMPARISON_EQ(FL, FR), "");
static_assert(GTFO_CAN_INVOKE_COMPARISON_N_EQ(FL, FR), "");
static_assert(GTFO_RESULT_OF_EQ_COMPARISON_IS(FL, FR, Bool), "");
static_assert(GTFO_RESULT_OF_N_EQ_COMPARISON_IS(FL, FR, Bool), "");
static_assert(GTFO_CAN_BE_USED_IN_A_BOOLEAN_CONTEXT(Bool), "");
static_assert(GTFO_IS_EQUALITY_COMPARABLE(FL, FR), "");

static_assert(GTFO_CAN_INVOKE_COMPARISON_EQ(GL, GR), "");
static_assert(GTFO_CAN_INVOKE_COMPARISON_N_EQ(GL, GR), "");
static_assert(GTFO_RESULT_OF_EQ_COMPARISON_IS(GL, GR, PrivateBool), "");
static_assert(GTFO_RESULT_OF_N_EQ_COMPARISON_IS(GL, GR, PrivateBool), "");
// unfortunately, a bug in MSVC 2012 :(
#if !defined(_MSC_VER) || _MSC_VER >= 1900
static_assert(!GTFO_CAN_BE_USED_IN_A_BOOLEAN_CONTEXT(PrivateBool), "");
static_assert(!GTFO_IS_EQUALITY_COMPARABLE(GL, GR), "");
#endif

static_assert(GTFO_CAN_INVOKE_COMPARISON_EQ(HL, HR), "");
static_assert(GTFO_CAN_INVOKE_COMPARISON_N_EQ(HL, HR), "");
static_assert(GTFO_RESULT_OF_EQ_COMPARISON_IS(HL, HR, NotBool), "");
static_assert(GTFO_RESULT_OF_N_EQ_COMPARISON_IS(HL, HR, Bool), "");
static_assert(!GTFO_CAN_BE_USED_IN_A_BOOLEAN_CONTEXT(NotBool), "");
static_assert(!GTFO_IS_EQUALITY_COMPARABLE(HL, HR), "");
