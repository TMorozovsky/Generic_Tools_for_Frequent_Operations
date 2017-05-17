#ifndef GTFO_FILE_INCLUDED_CONTAINER_MATRIX_EXCEPTIONS_HPP
#define GTFO_FILE_INCLUDED_CONTAINER_MATRIX_EXCEPTIONS_HPP

#include <stdexcept>

namespace gtfo
{
inline namespace container
{
    class bad_matrix_initializer_list : public ::std::invalid_argument
    {
    public:
        bad_matrix_initializer_list() : ::std::invalid_argument("invalid matrix initializer list") { }
    };

    class bad_matrix_filler_row : public ::std::invalid_argument
    {
    public:
        bad_matrix_filler_row() : ::std::invalid_argument("invalid matrix filler row") { }
    };

    class bad_matrix_index : public ::std::invalid_argument
    {
    public:
        explicit bad_matrix_index(const char * message) : ::std::invalid_argument(message) { }
    };

    class bad_matrix_row_index : public bad_matrix_index
    {
    public:
        bad_matrix_row_index() : bad_matrix_index("invalid matrix row index") { }
    };

    class bad_matrix_column_index : public bad_matrix_index
    {
    public:
        bad_matrix_column_index() : bad_matrix_index("invalid matrix column index") { }
    };
}
}

#endif // GTFO_FILE_INCLUDED_CONTAINER_MATRIX_EXCEPTIONS_HPP
