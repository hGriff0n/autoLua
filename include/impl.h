

#pragma once

#include "LuaTypeTraits.h"

#include <tuple>
#include <functional>

#include <iostream>

namespace autoLua {
	namespace impl {

		// tag class
		template <size_t... Is>
		struct _Indices { };

		// Factory. Recursively inherits from itself
		template <size_t N, size_t... Is>
		struct _IndicesBuilder : _IndicesBuilder<N - 1, N - 1, Is...> { };
		template <size_t... Is>
		struct _IndicesBuilder<0, Is...> {
			using type = _Indices<Is...>;
		};

		// Secondary Tag Class (for use in _push override for tuple. ideally should be done using _Indices)
		template <int N> struct _intTags { };


		// Pack args into a tuple
		// adjust so it uses pseudo indices instead of real indices somehow ???
		template <typename... Args, size_t... N>
		std::tuple<Args...> _getArgs(lua_State* L, _Indices<N...> idx) {
			return std::make_tuple(LuaTypeTraits<Args>::getValue(L, N + 1)...);
		}
		template <typename... Args>
		std::tuple<Args...> _getArgs(lua_State* L) {
			typename _IndicesBuilder<sizeof...(Args)>::type idxs;
			return _getArgs<Args...>(L, idxs);
		}


		// Unpack args and call function
		template <typename Ret, int... N, typename... Args>
		Ret _unpack(std::function<Ret(Args...)> functor, std::tuple<Args...> args, _Indices<N...> idx) {
			return functor(std::get<N>(args)...);
		}
		template <typename Ret, typename... Args>
		Ret _unpack(std::function<Ret(Args...)> functor, std::tuple<Args...> args) {
			typename _IndicesBuilder<sizeof...(Args)>::type idxs;
			return _unpack(functor, args, idxs);
		}


		// Push return onto lua stack
		template <typename T>
		void _push(lua_State* L, T value) {
			LuaTypeTraits<T>::pushValue(L, value);
		}
		template <typename... Args>
		void _push(lua_State* L, std::tuple<Args...> values) {
			_push_tuple(L, _intTags<sizeof...(Args)>(), std::forward<std::tuple<Args...>>(values));
		}

		template <int N, typename... Args>
		void _push_tuple(lua_State* L, _intTags<N> x, std::tuple<Args...> values) {
			_push(L, std::get<std::tuple_size<std::tuple<Args...>>::value - N>(values));
			_push_tuple(L, _intTags<N - 1>(), std::forward<std::tuple<Args...>>(values));
		}
		template <typename... Args>
		void _push_tuple(lua_State* L, _intTags<1> x, std::tuple<Args...> values) {
			_push(L, std::get<std::tuple_size<std::tuple<Args...>>::value - 1>(values));
		}

	}
}