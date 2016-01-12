#include "gtfo/_impl/type_traits/are_comparable_pred.hpp"
#define GTFO_CAN_INVOKE_PRED(p,t1,t2) ::gtfo::_tt::is_invokable_fun2<p,t1,t2>::value
#define GTFO_RESULT_OF_PRED(p,t1,t2) typename ::gtfo::_tt::result_of_fun2<p,t1,t2>::type
#define GTFO_ARE_COMPARABLE_PRED(p,t1,t2) ::gtfo::_tt::are_comparable_pred<p,t1,t2>::value

#define GTFO_RESULT_OF_PRED_IS(p,t1,t2,t3) \
    ::gtfo::_tt::is_same \
    < \
        GTFO_RESULT_OF_PRED(p,t1,t2), \
        t3 \
    >::value

#define GTFO_CAN_BE_USED_IN_BOOLEAN_CONTEXT(t) ::gtfo::_tt::can_be_used_in_boolean_context<t>::value

#include "gtfo/_impl/type_traits/are_comparable_op_eq.hpp"

namespace
{
    struct Voider
    {
        template<typename Lhs, typename Rhs>
        typename gtfo::_tt::enable_if
        <
            gtfo::_tt::helpers::can_invoke_op_eq<Lhs, Rhs>::value,
            void
        >::type
        operator()(const Lhs & lhs, const Rhs & rhs) const
        {
            (void)lhs; (void)rhs;
        }
    };

    struct Comparator
    {
        template<typename Lhs, typename Rhs>
        typename gtfo::_tt::enable_if
        <
            gtfo::_tt::helpers::can_invoke_op_eq<Lhs, Rhs>::value,
            typename gtfo::_tt::helpers::result_of_op_eq<Lhs, Rhs>::type
        >::type
        operator()(const Lhs & lhs, const Rhs & rhs) const
        {
            return lhs == rhs;
        }
    };

    struct Bool { operator bool() { return false; } };

    struct AL { };
    struct AR { };

    struct BL { };
    struct BR { };
    inline void operator == (BL, BR) { }

    struct CL { };
    struct CR { };
    inline bool operator == (CL, CR) { return false; }

    struct DL { };
    struct DR { };
    inline Bool operator == (DL, DR) { return Bool(); }
}

static_assert(GTFO_RESULT_OF_PRED_IS(Voider, int, int, void), "");

static_assert(!GTFO_ARE_COMPARABLE_PRED(Voider, int, int), "");
static_assert(!GTFO_ARE_COMPARABLE_PRED(Voider, int, float), "");
static_assert(!GTFO_ARE_COMPARABLE_PRED(Voider, int, unsigned long), "");

static_assert(GTFO_ARE_COMPARABLE_PRED(Comparator, int, int), "");
static_assert(GTFO_ARE_COMPARABLE_PRED(Comparator, int, float), "");
static_assert(GTFO_ARE_COMPARABLE_PRED(Comparator, int, unsigned long), "");

static_assert(!GTFO_CAN_INVOKE_PRED(Voider, AL, AR), "");
static_assert(!GTFO_ARE_COMPARABLE_PRED(Voider, AL, AR), "");
static_assert(!GTFO_CAN_INVOKE_PRED(Comparator, AL, AR), "");
static_assert(!GTFO_ARE_COMPARABLE_PRED(Comparator, AL, AR), "");

static_assert(GTFO_CAN_INVOKE_PRED(Voider, BL, BR), "");
static_assert(!GTFO_ARE_COMPARABLE_PRED(Voider, BL, BR), "");
static_assert(GTFO_CAN_INVOKE_PRED(Comparator, BL, BR), "");
static_assert(GTFO_RESULT_OF_PRED_IS(Comparator, BL, BR, void), "");
static_assert(!GTFO_ARE_COMPARABLE_PRED(Comparator, BL, BR), "");

static_assert(GTFO_CAN_INVOKE_PRED(Voider, CL, CR), "");
static_assert(!GTFO_ARE_COMPARABLE_PRED(Voider, CL, CR), "");
static_assert(GTFO_CAN_INVOKE_PRED(Comparator, CL, CR), "");
static_assert(GTFO_RESULT_OF_PRED_IS(Comparator, CL, CR, bool), "");
static_assert(GTFO_ARE_COMPARABLE_PRED(Comparator, CL, CR), "");

static_assert(GTFO_CAN_INVOKE_PRED(Voider, DL, DR), "");
static_assert(!GTFO_ARE_COMPARABLE_PRED(Voider, DL, DR), "");
static_assert(GTFO_CAN_INVOKE_PRED(Comparator, DL, DR), "");
static_assert(GTFO_RESULT_OF_PRED_IS(Comparator, DL, DR, Bool), "");
static_assert(GTFO_ARE_COMPARABLE_PRED(Comparator, DL, DR), "");
