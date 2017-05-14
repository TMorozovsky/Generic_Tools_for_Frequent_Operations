#ifndef GTFO_FILE_INCLUDED_FUNCTIONAL_NOT_HPP
#define GTFO_FILE_INCLUDED_FUNCTIONAL_NOT_HPP

#include "gtfo/_impl/functional/function_object_holder.hpp"
#include "gtfo/_impl/type_traits/mem_fun_invoke_is_nothrow_callable.hpp"
#include "gtfo/_impl/type_traits/mem_fun_invoke_call_result.hpp"
#include "gtfo/_impl/type_traits/is_nothrow_logically_negatable.hpp"
#include "gtfo/_impl/type_traits/result_of_logical_negation.hpp"

namespace gtfo
{
    namespace detail
    {
        namespace functional
        {
            // Auxiliary type trait for not_t<> which tells whether one of not_t<>'s overloaded function call operators can be marked as noexcept.
            // Takes a (possibly, cv-qualified and/or ref-qualified) type of the respective function_object_holder that stores the wrapped function
            // and a pack of argument types which will be passed to that function via function_object_holder's "invoke()" member function.
            template<typename FunctionObjectHolder, typename... Arguments>
            struct is_not_t_call_noexcept
            {
                static_assert(_tt::mem_fun_invoke_is_callable<FunctionObjectHolder, Arguments...>::value,
                              "cannot invoke the wrapped function with given arguments");

                using result_of_fun_holder_invocation = _tt::mem_fun_invoke_call_result_t<FunctionObjectHolder, Arguments...>;

                static_assert(_tt::is_logically_negatable<result_of_fun_holder_invocation>::value,
                              "the function can be invoked but operator ! cannot be applied to the return type");

                static constexpr bool value = _tt::mem_fun_invoke_is_nothrow_callable<FunctionObjectHolder, Arguments...>::value &&
                                              _tt::is_nothrow_logically_negatable< result_of_fun_holder_invocation >::value &&
                                              _tt::is_nothrow_move_constructible< _tt::result_of_logical_negation_t<result_of_fun_holder_invocation> >::value;
            };
        }
    }

    inline namespace functional
    {
        // Wrapper that holds some function-like object and forwards all calls to it,
        // returning the result of operator ! applied to its return value.
        template<typename F>
        class not_t : protected detail::functional::function_object_holder<_tt::decay_t<F>>
        {
        protected:
            using _fun_holder = detail::functional::function_object_holder<_tt::decay_t<F>>;

            using _fun_holder::invoke;

        public:
            constexpr          not_t()                             noexcept(_tt::is_nothrow_default_constructible<_fun_holder>::value)                   : _fun_holder()                  { }

            constexpr explicit not_t(const _tt::decay_t<F> &  arg) noexcept(_tt::is_nothrow_constructible<_fun_holder, const _tt::decay_t<F> & >::value) : _fun_holder(arg)               { }
            constexpr explicit not_t(      _tt::decay_t<F> && arg) noexcept(_tt::is_nothrow_constructible<_fun_holder,       _tt::decay_t<F> &&>::value) : _fun_holder(::gtfo::move(arg)) { }

            constexpr not_t(const not_t &  other) noexcept(_tt::is_nothrow_copy_constructible<_fun_holder>::value) : _fun_holder(static_cast<const _fun_holder & >(other))               { }
            constexpr not_t(      not_t && other) noexcept(_tt::is_nothrow_move_constructible<_fun_holder>::value) : _fun_holder(static_cast<      _fun_holder &&>(::gtfo::move(other))) { }

        public:
            template<typename... Args>
            GTFO_CONSTEXPR_FUNCTION
                auto
                operator () (Args&&... args) &
                noexcept( detail::functional::is_not_t_call_noexcept<_fun_holder &, Args...>::value )
                -> decltype(!(this->invoke(::gtfo::forward<Args>(args)...)))
            {
                return      !(this->invoke(::gtfo::forward<Args>(args)...));
            }

            template<typename... Args>
            constexpr
                auto
                operator () (Args&&... args) const &
                noexcept( detail::functional::is_not_t_call_noexcept<const _fun_holder &, Args...>::value )
                -> decltype(!(this->invoke(::gtfo::forward<Args>(args)...)))
            {
                return      !(this->invoke(::gtfo::forward<Args>(args)...));
            }

            template<typename... Args>
            GTFO_CONSTEXPR_FUNCTION
                auto
                operator () (Args&&... args) volatile &
                noexcept( detail::functional::is_not_t_call_noexcept<volatile _fun_holder &, Args...>::value )
                -> decltype(!(this->invoke(::gtfo::forward<Args>(args)...)))
            {
                return      !(this->invoke(::gtfo::forward<Args>(args)...));
            }

            template<typename... Args>
            constexpr
                auto
                operator () (Args&&... args) const volatile &
                noexcept( detail::functional::is_not_t_call_noexcept<const volatile _fun_holder &, Args...>::value )
                -> decltype(!(this->invoke(::gtfo::forward<Args>(args)...)))
            {
                return      !(this->invoke(::gtfo::forward<Args>(args)...));
            }

            template<typename... Args>
            GTFO_CONSTEXPR_FUNCTION
                auto
                operator () (Args&&... args) &&
                noexcept( detail::functional::is_not_t_call_noexcept<_fun_holder, Args...>::value )
                -> decltype(!(::gtfo::move(*this).invoke(::gtfo::forward<Args>(args)...)))
            {
                return      !(::gtfo::move(*this).invoke(::gtfo::forward<Args>(args)...));
            }

            template<typename... Args>
            constexpr
                auto
                operator () (Args&&... args) const &&
                noexcept( detail::functional::is_not_t_call_noexcept<const _fun_holder, Args...>::value )
                -> decltype(!(::gtfo::move(*this).invoke(::gtfo::forward<Args>(args)...)))
            {
                return      !(::gtfo::move(*this).invoke(::gtfo::forward<Args>(args)...));
            }

            template<typename... Args>
            GTFO_CONSTEXPR_FUNCTION
                auto
                operator () (Args&&... args) volatile &&
                noexcept( detail::functional::is_not_t_call_noexcept<volatile _fun_holder, Args...>::value )
                -> decltype(!(::gtfo::move(*this).invoke(::gtfo::forward<Args>(args)...)))
            {
                return      !(::gtfo::move(*this).invoke(::gtfo::forward<Args>(args)...));
            }

            template<typename... Args>
            constexpr
                auto
                operator () (Args&&... args) const volatile &&
                noexcept( detail::functional::is_not_t_call_noexcept<const volatile _fun_holder, Args...>::value )
                -> decltype(!(::gtfo::move(*this).invoke(::gtfo::forward<Args>(args)...)))
            {
                return      !(::gtfo::move(*this).invoke(::gtfo::forward<Args>(args)...));
            }
        };

        // Returns a functional object which holds some other function-like object
        // and forwards all calls to it, returning the result of operator ! applied to its return value.
        template<typename F>
        constexpr
            not_t<_tt::decay_t<F>>
            not_(F && f)
            noexcept( ::gtfo::_tt::is_nothrow_constructible<not_t<_tt::decay_t<F>>, F>::value &&
                      ::gtfo::_tt::is_nothrow_move_constructible< not_t<_tt::decay_t<F>> >::value )
        {
            return              not_t<_tt::decay_t<F>>(::gtfo::forward<F>(f));
        }
    }
}

#endif // GTFO_FILE_INCLUDED_FUNCTIONAL_NOT_HPP
