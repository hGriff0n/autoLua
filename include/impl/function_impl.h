

#pragma once

#include "LuaTypeTraits.h"

#include <tuple>
#include <functional>

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
		template <typename... Args>
		std::tuple<Args...> _getArgs(lua_State* L) {
			return std::make_tuple<Args...>(LuaTypeTraits<Args>::popValue(L, -1)...);
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

		// Helpers for tuple override of _push
		template <int N, typename... Args>
		void _push_tuple(lua_State* L, _intTags<N> x, std::tuple<Args...> values) {
			_push(L, std::get<std::tuple_size<std::tuple<Args...>>::value - N>(values));
			_push_tuple(L, _intTags<N - 1>(), std::forward<std::tuple<Args...>>(values));
		}
		template <typename... Args>
		void _push_tuple(lua_State* L, _intTags<1> x, std::tuple<Args...> values) {
			_push(L, std::get<std::tuple_size<std::tuple<Args...>>::value - 1>(values));
		}

		// base class for polymorphic storage of FunctionWrapper in lua_State
		struct BaseFunctionWrapper {
			virtual ~BaseFunctionWrapper() { }
			virtual int execute(lua_State*) = 0;
		};

	}
}