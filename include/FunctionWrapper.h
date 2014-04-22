

#pragma once

#include "impl/function_impl.h"

namespace autoLua {

	namespace impl {

		// base class for polymorphic storage of FunctionWrapper in lua_State
		struct BaseFunctionWrapper {
			virtual ~BaseFunctionWrapper() { }
			virtual int execute(lua_State*) = 0;
		};

		// thunker for calling of c++ functions from lua
		// this function is what actually gets stored in the lua_State
		int luaDispatch(lua_State* L) {
			BaseFunctionWrapper* func = (BaseFunctionWrapper*)lua_touserdata(L, lua_upvalueindex(1));
			return func->execute(L);
		}

		// outside class to handle thunker execution
		// moved outside of FunctionWrapper because I wasn't able to successfully specialize for a void return
		template <typename Ret>
		struct _WrapperImpl {
			template <int N, typename... Args>
			static int execute(lua_State* L, const std::function<Ret(Args...)>& func) {
				_push(L, std::forward<Ret>(_unpack(func, _getArgs<Args...>(L))));
				return N;
			}
		};

		// override for void returns
		template <>
		struct _WrapperImpl<void> {
			template <int N, typename... Args>
			static int execute(lua_State* L, const std::function<void(Args...)>& func) {
				_unpack(func, _getArgs<Args...>(L));
				return N;
			}
		};

		// adds a override for a lua_CFunction
		template <>
		struct _WrapperImpl<int> {
			template <int N, typename... Args>
			static int execute(lua_State* L, const std::function<int(Args...)>& func) {
				_push(L, std::forward<int>(_unpack(func, _getArgs<Args...>(L))));
				return N;
			}
			template <int N>
			static int execute(lua_State* L, const std::function<int(lua_State*)>& func) {
				return func(L);
			}
		};

	}

	// Wraps a C++ function for lua to call
	template <int N, typename Ret, typename... Args>
	class FunctionWrapper : public impl::BaseFunctionWrapper {
		private:
		std::function<Ret(Args...)> func;

		public:
		FunctionWrapper(lua_State* L, const std::function<Ret(Args...)>& _func) : /*lua(L),*/ func(_func) {
			lua_pushlightuserdata(L, (void*)static_cast<impl::BaseFunctionWrapper*>(this));
			lua_pushcclosure(L, &impl::luaDispatch, 1);
		}

		int execute(lua_State* L) {
			return impl::_WrapperImpl<Ret>::execute<N>(L, func);
		}
	};

	// Factories for creation of FunctionWrappers
	template <typename... Args>
	FunctionWrapper<0, void, Args...> makeWrapper(lua_State* L, const std::function<void(Args...)>& func) {
		return FunctionWrapper<0, void, Args...>(L, func);
	}
	template <typename Ret, typename... Args>
	FunctionWrapper<1, Ret, Args...> makeWrapper(lua_State* L, const std::function<Ret(Args...)>& func) {
		return FunctionWrapper<1, Ret, Args...>(L, func);
	}
	template <typename... Ret, typename... Args>
	FunctionWrapper<sizeof...(Ret), std::tuple<Ret...>, Args...>
		makeWrapper(lua_State* L, const std::function<std::tuple<Ret...>(Args...)>& func) {
			return FunctionWrapper<sizeof...(Ret), std::tuple<Ret...>, Args...>(L, func);
	}

}