#include "gtfo/functional/not_.hpp"
#include "gtfo_testing/runtime/runtime_tests.hpp"

#include <vector>
#include <iostream>

namespace
{
    struct is_even
    {
        bool operator()(int x) const noexcept
        {
            return x % 2 == 0;
        }
    };

    bool is_odd_fun(int x) noexcept
    {
        return x % 2 == 1;
    }

    struct is_value_not_default_constructible
    {
    private:
        int _value;

    public:
        is_value_not_default_constructible() = delete;
        explicit is_value_not_default_constructible(int value) noexcept : _value(value) { }
        is_value_not_default_constructible(const is_value_not_default_constructible &) noexcept = default;

        bool operator()(int x) const noexcept
        {
            return x == _value;
        }
    };

    struct is_five_final final
    {
        bool operator()(int x) const noexcept
        {
            return x == 5;
        }
    };
}

GTFO_TEST_FUN_BEGIN
{
    using gtfo::functional::not_t;
    using gtfo::functional::not_;

    std::vector<int> values { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    std::vector<int> test_results;
    auto apply_pred_and_test = [&values, &test_results] (auto pred, std::initializer_list<int> il) {
        test_results.clear();
        for (int value : values) {
            if (pred(value))
                test_results.push_back(value);
        }

        GTFO_TEST_ASSERT_EQ(test_results, il);
    };

    apply_pred_and_test(is_even(),        { 0, 2, 4, 6, 8 });
    apply_pred_and_test(not_t<is_even>(), { 1, 3, 5, 7, 9 });

    is_even is_even_obj;
    apply_pred_and_test(not_(is_even_obj), { 1, 3, 5, 7, 9 });

    apply_pred_and_test(not_(is_even()), { 1, 3, 5, 7, 9 });

    apply_pred_and_test(not_(is_odd_fun),  { 0, 2, 4, 6, 8 });
    apply_pred_and_test(not_(&is_odd_fun), { 0, 2, 4, 6, 8 });

    apply_pred_and_test(not_t<bool (int)>(is_odd_fun),    { 0, 2, 4, 6, 8 });
    apply_pred_and_test(not_t<bool (int)>(&is_odd_fun),   { 0, 2, 4, 6, 8 });

    apply_pred_and_test(not_t<bool(*)(int)>(is_odd_fun),  { 0, 2, 4, 6, 8 });
    apply_pred_and_test(not_t<bool(*)(int)>(&is_odd_fun), { 0, 2, 4, 6, 8 });

    apply_pred_and_test(not_t<bool(&)(int)>(is_odd_fun),  { 0, 2, 4, 6, 8 });
    apply_pred_and_test(not_t<bool(&)(int)>(&is_odd_fun), { 0, 2, 4, 6, 8 });

    apply_pred_and_test(not_t<is_value_not_default_constructible>(is_value_not_default_constructible(5)), { 0, 1, 2, 3, 4, 6, 7, 8, 9 });
    apply_pred_and_test(not_(is_value_not_default_constructible(3)), { 0, 1, 2, 4, 5, 6, 7, 8, 9 });
    apply_pred_and_test(not_(is_five_final()), { 0, 1, 2, 3, 4, 6, 7, 8, 9 });

    apply_pred_and_test(not_([](int x) { return x == 4 || x == 7; }), { 0, 1, 2, 3, 5, 6, 8, 9 });

    int mutable_x = 1;
    apply_pred_and_test(not_([&mutable_x](int x) mutable {
        if (x == mutable_x) {
            mutable_x *= 2;
            return true;
        }
        return false;
    }), { 0, 3, 5, 6, 7, 9 });

    apply_pred_and_test(not_(not_t<bool (int)>(is_odd_fun)), { 1, 3, 5, 7, 9 });
    apply_pred_and_test(not_(not_(is_value_not_default_constructible(3))), { 3 });
    apply_pred_and_test(not_(not_(is_five_final())), { 5 });
}
GTFO_TEST_FUN_END