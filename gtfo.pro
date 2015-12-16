SOURCES += \
    gtfo/_impl/tests/runtime/runtime_tests.cpp \
    gtfo/_impl/tests/static/static_tests.cpp

QMAKE_CXXFLAGS += -std=c++11 -Wall -pedantic

HEADERS += \
    gtfo/type_traits.hpp \
    gtfo/numeric.hpp \
    gtfo/numeric/accumulate.hpp \
    gtfo/numeric/adjacent_difference.hpp \
    gtfo/algorithm.hpp \
    gtfo/algorithm/for_each.hpp \
    gtfo/numeric/inner_product.hpp \
    gtfo/_impl/type_traits.hpp \
    gtfo/_impl/type_traits/_common_definitions.hpp \
    gtfo/_impl/type_traits/has_begin.hpp \
    gtfo/_impl/type_traits/has_end.hpp \
    gtfo/_impl/type_traits/result_of_begin.hpp \
    gtfo/_impl/type_traits/result_of_end.hpp \
    gtfo/_impl/type_traits/is_dereferenceable.hpp \
    gtfo/_impl/type_traits/result_of_dereferencing.hpp \
    gtfo/_impl/type_traits/value_of_dereferenced.hpp \
    gtfo/_impl/type_traits/is_equality_comparable.hpp \
    gtfo/_impl/type_traits/is_incrementable.hpp \
    gtfo/_impl/type_traits/is_iterator.hpp \
    gtfo/_impl/type_traits/is_container.hpp \
    gtfo/_impl/type_traits/iterator_of_container.hpp \
    gtfo/_impl/type_traits/value_from_container.hpp \
    gtfo/_impl/type_traits/is_invokable_fun0.hpp \
    gtfo/_impl/type_traits/is_invokable_fun1.hpp \
    gtfo/_impl/type_traits/is_invokable_fun2.hpp \
    gtfo/_impl/type_traits/are_addable.hpp \
    gtfo/_impl/type_traits/are_subtractable.hpp \
    gtfo/_impl/type_traits/are_multipliable.hpp \
    gtfo/_impl/type_traits/result_of_fun0.hpp \
    gtfo/_impl/type_traits/result_of_fun1.hpp \
    gtfo/_impl/type_traits/result_of_fun2.hpp \
    gtfo/_impl/type_traits/result_of_addition.hpp \
    gtfo/_impl/type_traits/result_of_subtraction.hpp \
    gtfo/_impl/type_traits/result_of_multiplication.hpp \
    gtfo/_impl/type_traits/is_assignable.hpp \
    gtfo/_impl/type_traits/common_type_2.hpp \
    gtfo/_impl/type_traits/is_container_castable_to_its_iterator.hpp \
    gtfo/_impl/type_traits/result_of_container_iterator_dereferencing.hpp