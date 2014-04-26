

#pragma once

// change "impl/function_impl.h" to "impl/stack_impl.h" ??? (also change some function names)
#include "impl/function_impl.h"

namespace autoLua {

	// Helper class that handles calling lua functions

	class LuaCaller {
		private:
			lua_State* L;

		protected:
			template <typename... Args>
			lua_State* checkPushCall(Args&&... args) {
				if ( !LuaTypeTraits<LuaFunction>::isA(L) ) throw("Not a function");
				impl::_push(L, std::make_tuple(args...));
				lua_pcall(L, sizeof...(Args), LUA_MULTRET, 0);
				return L;
			}

		public:
			LuaCaller(lua_State* lua) : L(lua) { }
			~LuaCaller() { L = nullptr; }

			template <typename... Args>
			int call(Args&&... args) {
				auto former = lua_gettop(L) - 1;
				return (lua_gettop(checkPushCall(std::forward<Args>(args)...)) - former);
			}

			template <typename... Args>
			lua_State* operator()(Args&&... args) {
				return checkPushCall(std::forward<Args>(args)...);
			}
	};

}