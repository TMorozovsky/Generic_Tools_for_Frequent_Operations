#ifndef GTFO_FILE_INCLUDED_FUNCTIONAL_FUNCTION_OBJECT_HOLDER_HPP
#define GTFO_FILE_INCLUDED_FUNCTIONAL_FUNCTION_OBJECT_HOLDER_HPP

#include "gtfo/_impl/type_traits/_type_traits_definitions.hpp"
#include "gtfo/_impl/utility.hpp"

namespace gtfo
{
    namespace detail
    {
        namespace functional
        {
            template<typename F, bool F_is_class, bool F_is_final_class>
            class _function_object_holder_impl;

            template<typename F>
            class _function_object_holder_impl<F, true, false> : protected F
            {
                static_assert(_tt::is_class<F>::value, "invalid dispatch: class expected");

            protected:
                constexpr          _function_object_holder_impl()               noexcept(_tt::is_nothrow_default_constructible<F>::value) : F()               { }

                constexpr explicit _function_object_holder_impl(const F &  arg) noexcept(_tt::is_nothrow_copy_constructible<F>::value) : F(arg)               { }
                constexpr explicit _function_object_holder_impl(      F && arg) noexcept(_tt::is_nothrow_move_constructible<F>::value) : F(::gtfo::move(arg)) { }

                constexpr _function_object_holder_impl(const _function_object_holder_impl &  other) noexcept(_tt::is_nothrow_copy_constructible<F>::value) : F(static_cast<const F & >(other))               { }
                constexpr _function_object_holder_impl(      _function_object_holder_impl && other) noexcept(_tt::is_nothrow_move_constructible<F>::value) : F(static_cast<      F &&>(::gtfo::move(other))) { }

            protected:
                GTFO_CONSTEXPR_FUNCTION                F &  _get_fun()                &  noexcept { return static_cast<               F &>(*this); }
                constexpr               const          F &  _get_fun() const          &  noexcept { return static_cast<const          F &>(*this); }
                GTFO_CONSTEXPR_FUNCTION volatile       F &  _get_fun() volatile       &  noexcept { return static_cast<volatile       F &>(*this); }
                constexpr               const volatile F &  _get_fun() const volatile &  noexcept { return static_cast<const volatile F &>(*this); }

                GTFO_CONSTEXPR_FUNCTION                F && _get_fun()                && noexcept { return static_cast<               F &&>(::gtfo::move(*this)); }
                constexpr               const          F && _get_fun() const          && noexcept { return static_cast<const          F &&>(::gtfo::move(*this)); }
                GTFO_CONSTEXPR_FUNCTION volatile       F && _get_fun() volatile       && noexcept { return static_cast<volatile       F &&>(::gtfo::move(*this)); }
                constexpr               const volatile F && _get_fun() const volatile && noexcept { return static_cast<const volatile F &&>(::gtfo::move(*this)); }
            };

            template<typename F>
            class _function_object_holder_impl<F, true, true>
            {
                static_assert(_tt::is_final<F>::value, "invalid dispatch: final class expected");

            private:
                F _stored_f;

            protected:
                constexpr          _function_object_holder_impl()               noexcept(_tt::is_nothrow_default_constructible<F>::value) : _stored_f()               { }

                constexpr explicit _function_object_holder_impl(const F &  arg) noexcept(_tt::is_nothrow_copy_constructible<F>::value) : _stored_f(arg)               { }
                constexpr explicit _function_object_holder_impl(      F && arg) noexcept(_tt::is_nothrow_move_constructible<F>::value) : _stored_f(::gtfo::move(arg)) { }

                constexpr _function_object_holder_impl(const _function_object_holder_impl &  other) noexcept(_tt::is_nothrow_copy_constructible<F>::value) : _stored_f(other._stored_f)               { }
                constexpr _function_object_holder_impl(      _function_object_holder_impl && other) noexcept(_tt::is_nothrow_move_constructible<F>::value) : _stored_f(::gtfo::move(other._stored_f)) { }

            protected:
                GTFO_CONSTEXPR_FUNCTION                F &  _get_fun()                &  noexcept { return _stored_f; }
                constexpr               const          F &  _get_fun() const          &  noexcept { return _stored_f; }
                GTFO_CONSTEXPR_FUNCTION volatile       F &  _get_fun() volatile       &  noexcept { return _stored_f; }
                constexpr               const volatile F &  _get_fun() const volatile &  noexcept { return _stored_f; }

                GTFO_CONSTEXPR_FUNCTION                F && _get_fun()                && noexcept { return ::gtfo::move(_stored_f); }
                constexpr               const          F && _get_fun() const          && noexcept { return ::gtfo::move(_stored_f); }
                GTFO_CONSTEXPR_FUNCTION volatile       F && _get_fun() volatile       && noexcept { return ::gtfo::move(_stored_f); }
                constexpr               const volatile F && _get_fun() const volatile && noexcept { return ::gtfo::move(_stored_f); }
            };

            template<typename F>
            class _function_object_holder_impl<F, false, false>
            {
                static_assert(!_tt::is_class<F>::value, "invalid dispatch: non-class type expected");

            private:
                F _stored_f;

            protected:
                _function_object_holder_impl() = delete;

                constexpr explicit _function_object_holder_impl(const F &  arg) noexcept(_tt::is_nothrow_copy_constructible<F>::value) : _stored_f(arg)               { }
                constexpr explicit _function_object_holder_impl(      F && arg) noexcept(_tt::is_nothrow_move_constructible<F>::value) : _stored_f(::gtfo::move(arg)) { }

                constexpr _function_object_holder_impl(const _function_object_holder_impl &  other) noexcept(_tt::is_nothrow_copy_constructible<F>::value) : _stored_f(other._stored_f)               { }
                constexpr _function_object_holder_impl(      _function_object_holder_impl && other) noexcept(_tt::is_nothrow_move_constructible<F>::value) : _stored_f(::gtfo::move(other._stored_f)) { }

            protected:
                GTFO_CONSTEXPR_FUNCTION                F &  _get_fun()                &  noexcept { return _stored_f; }
                constexpr               const          F &  _get_fun() const          &  noexcept { return _stored_f; }
                GTFO_CONSTEXPR_FUNCTION volatile       F &  _get_fun() volatile       &  noexcept { return _stored_f; }
                constexpr               const volatile F &  _get_fun() const volatile &  noexcept { return _stored_f; }

                GTFO_CONSTEXPR_FUNCTION                F && _get_fun()                && noexcept { return ::gtfo::move(_stored_f); }
                constexpr               const          F && _get_fun() const          && noexcept { return ::gtfo::move(_stored_f); }
                GTFO_CONSTEXPR_FUNCTION volatile       F && _get_fun() volatile       && noexcept { return ::gtfo::move(_stored_f); }
                constexpr               const volatile F && _get_fun() const volatile && noexcept { return ::gtfo::move(_stored_f); }
            };

            template<typename F>
            class function_object_holder : protected _function_object_holder_impl< _tt::decay_t<F>,
                                                                                   _tt::is_class<_tt::decay_t<F>>::value,
                                                                                   _tt::is_class<_tt::decay_t<F>>::value && _tt::is_final<_tt::decay_t<F>>::value >
            {
            private:
                using base_type = _function_object_holder_impl< _tt::decay_t<F>,
                                                                _tt::is_class<_tt::decay_t<F>>::value,
                                                                _tt::is_class<_tt::decay_t<F>>::value && _tt::is_final<_tt::decay_t<F>>::value >;
                using F_type = _tt::decay_t<F>;

            protected:
                using base_type::_get_fun;

            protected:
                constexpr          function_object_holder()                    noexcept(_tt::is_nothrow_default_constructible<base_type>::value)          : base_type()                  { }

                constexpr explicit function_object_holder(const F_type &  arg) noexcept(_tt::is_nothrow_constructible<base_type, const F_type & >::value) : base_type(arg)               { }
                constexpr explicit function_object_holder(      F_type && arg) noexcept(_tt::is_nothrow_constructible<base_type,       F_type &&>::value) : base_type(::gtfo::move(arg)) { }

                constexpr function_object_holder(const function_object_holder &  other) noexcept(_tt::is_nothrow_copy_constructible<base_type>::value) : base_type(static_cast<const base_type & >(other))               { }
                constexpr function_object_holder(      function_object_holder && other) noexcept(_tt::is_nothrow_move_constructible<base_type>::value) : base_type(static_cast<      base_type &&>(::gtfo::move(other))) { }

            public:
                template<typename... Args>
                GTFO_CONSTEXPR_FUNCTION
                    auto
                    invoke(Args&&... args) &
                    noexcept(noexcept(::gtfo::_tt::declval<function_object_holder &>()._get_fun()(::gtfo::forward<Args>(args)...)))
                    -> decltype(this->_get_fun()(::gtfo::forward<Args>(args)...))
                {
                    return      this->_get_fun()(::gtfo::forward<Args>(args)...);
                }

                template<typename... Args>
                constexpr
                    auto
                    invoke(Args&&... args) const &
                    noexcept(noexcept(::gtfo::_tt::declval<const function_object_holder &>()._get_fun()(::gtfo::forward<Args>(args)...)))
                    -> decltype(this->_get_fun()(::gtfo::forward<Args>(args)...))
                {
                    return      this->_get_fun()(::gtfo::forward<Args>(args)...);
                }

                template<typename... Args>
                GTFO_CONSTEXPR_FUNCTION
                    auto
                    invoke(Args&&... args) volatile &
                    noexcept(noexcept(::gtfo::_tt::declval<volatile function_object_holder &>()._get_fun()(::gtfo::forward<Args>(args)...)))
                    -> decltype(this->_get_fun()(::gtfo::forward<Args>(args)...))
                {
                    return      this->_get_fun()(::gtfo::forward<Args>(args)...);
                }

                template<typename... Args>
                constexpr
                    auto
                    invoke(Args&&... args) const volatile &
                    noexcept(noexcept(::gtfo::_tt::declval<const volatile function_object_holder &>()._get_fun()(::gtfo::forward<Args>(args)...)))
                    -> decltype(this->_get_fun()(::gtfo::forward<Args>(args)...))
                {
                    return      this->_get_fun()(::gtfo::forward<Args>(args)...);
                }

                template<typename... Args>
                GTFO_CONSTEXPR_FUNCTION
                    auto
                    invoke(Args&&... args) &&
                    noexcept(noexcept(::gtfo::_tt::declval<function_object_holder>()._get_fun()(::gtfo::forward<Args>(args)...)))
                    -> decltype((::gtfo::move(*this))._get_fun()(::gtfo::forward<Args>(args)...))
                {
                    return      (::gtfo::move(*this))._get_fun()(::gtfo::forward<Args>(args)...);
                }

                template<typename... Args>
                constexpr
                    auto
                    invoke(Args&&... args) const &&
                    noexcept(noexcept(::gtfo::_tt::declval<const function_object_holder>()._get_fun()(::gtfo::forward<Args>(args)...)))
                    -> decltype((::gtfo::move(*this))._get_fun()(::gtfo::forward<Args>(args)...))
                {
                    return      (::gtfo::move(*this))._get_fun()(::gtfo::forward<Args>(args)...);
                }

                template<typename... Args>
                GTFO_CONSTEXPR_FUNCTION
                    auto
                    invoke(Args&&... args) volatile &&
                    noexcept(noexcept(::gtfo::_tt::declval<volatile function_object_holder>()._get_fun()(::gtfo::forward<Args>(args)...)))
                    -> decltype((::gtfo::move(*this))._get_fun()(::gtfo::forward<Args>(args)...))
                {
                    return      (::gtfo::move(*this))._get_fun()(::gtfo::forward<Args>(args)...);
                }

                template<typename... Args>
                constexpr
                    auto
                    invoke(Args&&... args) const volatile &&
                    noexcept(noexcept(::gtfo::_tt::declval<const volatile function_object_holder>()._get_fun()(::gtfo::forward<Args>(args)...)))
                    -> decltype((::gtfo::move(*this))._get_fun()(::gtfo::forward<Args>(args)...))
                {
                    return      (::gtfo::move(*this))._get_fun()(::gtfo::forward<Args>(args)...);
                }
            };
        }
    }
}

#endif // GTFO_FILE_INCLUDED_FUNCTIONAL_FUNCTION_OBJECT_HOLDER_HPP
