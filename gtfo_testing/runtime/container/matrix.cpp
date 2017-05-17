#include "gtfo/container/matrix.hpp"
#include <iostream>

inline std::ostream & operator << (std::ostream & os, gtfo::matrix<int>::iterator)
{
    return os << "<matrix iterator>";
}
inline std::ostream & operator << (std::ostream & os, gtfo::matrix<int>::const_iterator)
{
    return os << "<matrix const_iterator>";
}
inline std::ostream & operator << (std::ostream & os, gtfo::matrix<int>::reverse_iterator)
{
    return os << "<matrix reverse_iterator>";
}
inline std::ostream & operator << (std::ostream & os, gtfo::matrix<int>::const_reverse_iterator)
{
    return os << "<matrix const_reverse_iterator>";
}

#include "gtfo_testing/runtime/runtime_tests.hpp"
#include <iomanip>
using namespace std;
using gtfo::matrix;

template<typename Matrix>
inline ostream & print_matrix(const Matrix & matr, ostream & os = cout)
{
    for (auto row : matr) {
        for (const auto & element : row)
            cout << setw(3) << element << ' ';
        cout << endl;
    }
    return os;
}

static void test_mutable_forward_loop()
{
    cout << (__func__) << endl;
    matrix<int> mi = { { 1, 2, 3 }, { 4, 5, 6 } };
    for (auto row : mi) {
        for (int & val : row)
            ++val;
    }
    print_matrix(mi);
    cout << endl;
}

static void test_const_forward_loop()
{
    cout << (__func__) << endl;
    const matrix<int> mi = { { 1, 2, 3 }, { 4, 5, 6 } };
    print_matrix(mi);
    cout << endl;
}

static void test_mutable_reverse_loop()
{
    cout << (__func__) << endl;
    matrix<int> mi = { { 0, 0, 0 }, { 0, 0, 0 } };
    int next_value = 1;
    for (auto row_reverse_it = mi.rbegin(), end = mi.rend(); row_reverse_it != end; ++row_reverse_it) {
        for (int & val : *row_reverse_it)
            val = next_value++;
    }
    print_matrix(mi);
    cout << endl;
}

static void test_const_reverse_loop()
{
    cout << (__func__) << endl;
    const matrix<int> mi = { { 1, 2, 3 }, { 4, 5, 6 } };
    for (auto row_reverse_it = mi.rbegin(), end = mi.rend(); row_reverse_it != end; ++row_reverse_it) {
        for (int val : *row_reverse_it)
            cout << val << ' ';
        cout << endl;
    }
    cout << endl;
}

static void test_row_reverse_iterator_construct()
{
    cout << (__func__) << endl;
    matrix<int> mi = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
    {
        matrix<int>::iterator it = mi.begin();
        auto rit = matrix<int>::reverse_iterator(it);
        GTFO_TEST_ASSERT_EQ(rit, mi.rend());
        GTFO_TEST_ASSERT_EQ(rit.base(), it);
    }
}

static void test_row_reverse_iterator_to_base()
{
    cout << (__func__) << endl;
    matrix<int> mi = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
    {
        const matrix<int>::const_reverse_iterator rit = mi.rbegin();
        const matrix<int>::const_iterator it = rit.base();
        GTFO_TEST_ASSERT_EQ(it, mi.end());
    }
    {
        const matrix<int>::reverse_iterator rit = mi.rbegin() + 1;
        const matrix<int>::iterator it = rit.base();
        GTFO_TEST_ASSERT_EQ(it, mi.end() - 1);
    }
    {
        matrix<int>::reverse_iterator rit = mi.rend();
        matrix<int>::iterator it = rit.base();
        GTFO_TEST_ASSERT_EQ(it, mi.begin());
    }
}

static void test_erase_row()
{
    cout << (__func__) << endl;
    matrix<int> mi = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
    mi.erase_row(mi.end());
    GTFO_TEST_ASSERT_EQ(mi, matrix<int>({ { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } }));
    mi.erase_row(mi.begin() + 1);
    GTFO_TEST_ASSERT_EQ(mi, matrix<int>({ { 1, 2, 3 }, { 7, 8, 9 } }));
    mi.erase_row(mi.begin());
    GTFO_TEST_ASSERT_EQ(mi, matrix<int>({ { 7, 8, 9 } }));
    mi.erase_row(mi.begin());
    GTFO_TEST_ASSERT_EQ(mi.rows(), 0u);
    GTFO_TEST_ASSERT_EQ(mi.cols(), 3u);
    GTFO_TEST_ASSERT_EQ(mi.overall_size(), 0u);
}

static void test_erase_rows()
{
    cout << (__func__) << endl;
    matrix<int> mi = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
    mi.erase_rows(mi.begin() + 1, mi.begin() + 1);
    GTFO_TEST_ASSERT_EQ(mi, matrix<int>({ { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } }));
    mi.erase_rows(mi.begin() + 3, mi.end());
    GTFO_TEST_ASSERT_EQ(mi, matrix<int>({ { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } }));
    mi.erase_rows(mi.begin(), mi.end() - 1);
    GTFO_TEST_ASSERT_EQ(mi, matrix<int>({ { 7, 8, 9 } }));
    mi.erase_rows(mi.begin(), mi.end());
    GTFO_TEST_ASSERT_EQ(mi.rows(), 0u);
    GTFO_TEST_ASSERT_EQ(mi.cols(), 3u);
    GTFO_TEST_ASSERT_EQ(mi.overall_size(), 0u);
}

static void test_resize_rows_to_zero()
{
    cout << (__func__) << endl;
    matrix<int> mi = { { 1, 2, 3 }, { 4, 5, 6 } };
    mi.resize_rows(0);
    GTFO_TEST_ASSERT_EQ(mi.rows(), 0u);
    GTFO_TEST_ASSERT_EQ(mi.cols(), 3u);
    GTFO_TEST_ASSERT_EQ(mi.overall_size(), 0u);
}

static void test_resize_rows_down()
{
    cout << (__func__) << endl;
    matrix<int> mi = { { 1, 2, 3 }, { 4, 5, 6 } };
    mi.resize_rows(1);
    GTFO_TEST_ASSERT_EQ(mi, matrix<int>({ { 1, 2, 3 } }));
}

static void test_resize_rows_equal()
{
    cout << (__func__) << endl;
    matrix<int> mi = { { 1, 2, 3 }, { 4, 5, 6 } };
    mi.resize_rows(2);
    GTFO_TEST_ASSERT_EQ(mi, matrix<int>({ { 1, 2, 3 }, { 4, 5, 6 } }));
}

static void test_resize_rows_up_default_filler_element()
{
    cout << (__func__) << endl;
    matrix<int> mi = { { 1, 2, 3 }, { 4, 5, 6 } };
    mi.resize_rows(3);
    GTFO_TEST_ASSERT_EQ(mi, matrix<int>({ { 1, 2, 3 }, { 4, 5, 6 }, { 0, 0, 0 } }));
}

static void test_resize_rows_up_custom_filler_element()
{
    cout << (__func__) << endl;
    matrix<int> mi = { { 1, 2, 3 }, { 4, 5, 6 } };
    mi.resize_rows(3, 42);
    GTFO_TEST_ASSERT_EQ(mi, matrix<int>({ { 1, 2, 3 }, { 4, 5, 6 }, { 42, 42, 42 } }));
}

static void test_resize_rows_up_custom_filler_row()
{
    cout << (__func__) << endl;
    matrix<int> mi = { { 1, 2, 3 }, { 4, 5, 6 } };
    const int source_array[] = { 5, 6, 7, 8, 9, 10, 11, 12 };
    mi.resize_rows(5, source_array + 3, source_array + 6);
    GTFO_TEST_ASSERT_EQ(mi, matrix<int>({ { 1, 2, 3 }, { 4, 5, 6 }, { 8, 9, 10 }, { 8, 9, 10 }, { 8, 9, 10 } }));
}

static void test_resize_rows_up_custom_filler_initializer_list()
{
    cout << (__func__) << endl;
    matrix<int> mi = { { 1, 2, 3 }, { 4, 5, 6 } };
    mi.resize_rows(8, { 11, 12, 13 });
    GTFO_TEST_ASSERT_EQ(mi, matrix<int>({ { 1, 2, 3 }, { 4, 5, 6 }, { 11, 12, 13 }, { 11, 12, 13 }, { 11, 12, 13 }, { 11, 12, 13 }, { 11, 12, 13 }, { 11, 12, 13 } }));
}

static void test_resize_cols_to_zero()
{
    cout << (__func__) << endl;
    matrix<int> mi = { { 1, 2, 3 }, { 4, 5, 6 } };
    mi.resize_cols(0);
    GTFO_TEST_ASSERT_EQ(mi.rows(), 2u);
    GTFO_TEST_ASSERT_EQ(mi.cols(), 0u);
    GTFO_TEST_ASSERT_EQ(mi.overall_size(), 0u);
}

static void test_resize_cols_down()
{
    cout << (__func__) << endl;
    matrix<int> mi = { { 1, 2, 3, 4, 77, 88, 99 },
                       { 5, 6, 7, 8, 66, 77, 88 },
                       { 9, 10, 11, 12, 55, 66, 77 } };
    mi.resize_cols(4);
    GTFO_TEST_ASSERT_EQ(mi, matrix<int>({ { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 } }));
}

static void test_resize_cols_equal()
{
    cout << (__func__) << endl;
    matrix<int> mi = { { 1, 2, 3 }, { 4, 5, 6 } };
    mi.resize_cols(3);
    GTFO_TEST_ASSERT_EQ(mi, matrix<int>({ { 1, 2, 3 }, { 4, 5, 6 } }));
}

static void test_resize_cols_up_default_filler_element()
{
    cout << (__func__) << endl;
    matrix<int> mi = { { 1, 2, 3 }, { 4, 5, 6 } };
    mi.resize_cols(4);
    GTFO_TEST_ASSERT_EQ(mi, matrix<int>({ { 1, 2, 3, 0 }, { 4, 5, 6, 0 } }));
    mi.resize_cols(6);
    GTFO_TEST_ASSERT_EQ(mi, matrix<int>({ { 1, 2, 3, 0, 0, 0 }, { 4, 5, 6, 0, 0, 0 } }));
    mi.resize_cols(15);
    GTFO_TEST_ASSERT_EQ(mi, matrix<int>({ { 1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }));
    print_matrix(mi) << endl;
}

GTFO_TEST_FUN_BEGIN
{
    try {
        test_mutable_forward_loop();
        test_const_forward_loop();
        test_mutable_reverse_loop();
        test_const_reverse_loop();

        test_row_reverse_iterator_construct();
        test_row_reverse_iterator_to_base();

        test_erase_row();
        test_erase_rows();

        test_resize_rows_to_zero();
        test_resize_rows_down();
        test_resize_rows_equal();
        test_resize_rows_up_default_filler_element();
        test_resize_rows_up_custom_filler_element();
        test_resize_rows_up_custom_filler_row();
        test_resize_rows_up_custom_filler_initializer_list();

        test_resize_cols_to_zero();
        test_resize_cols_down();
        test_resize_cols_equal();
        test_resize_cols_up_default_filler_element();
        //test_resize_cols_up_custom_filler_element();
        //test_resize_cols_up_custom_filler_row();
        //test_resize_cols_up_custom_filler_initializer_list();

        cout << "Success!" << endl;
    }
    catch (const exception & e) {
        cerr << "Exception caught:" << endl << e.what() << endl;
        GTFO_TEST_ASSERT(!"An exception was thrown during testing of the matrix template!");
    }
}
GTFO_TEST_FUN_END
