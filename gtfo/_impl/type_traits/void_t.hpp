#ifndef GTFO_FILE_INCLUDED_TYPE_TRAITS_VOID_T_HPP
#define GTFO_FILE_INCLUDED_TYPE_TRAITS_VOID_T_HPP

namespace gtfo
{
    namespace _tt
    {
        namespace helpers
        {
            template<typename...>
            struct voider
            {
                using type = void;
            };
        }

        template<typename... T>
        using void_t = typename helpers::voider<T...>::type;
    }
}

#endif // GTFO_FILE_INCLUDED_TYPE_TRAITS_VOID_T_HPP
