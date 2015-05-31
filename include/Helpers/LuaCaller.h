

#pragma once

// change "impl/function_impl.h" to "impl/stack_impl.h" ??? (also change some function names)
#include "impl/function_impl.h"
#include "Exceptions/LuaException.h"
#include <iostream>
#include "debugTesting.h"

namespace autoLua {

	// Helper class that handles calling lua functions

	class LuaCaller {
		lua_State* L;

		protected:
			template <typename... Args>
			lua_State* checkPushCall(Args&&... args) {
				if ( !LuaTypeTraits<LuaFunction>::isA(L) ) throw LuaTypeError("LuaCaller::checkPushCall", "Not a function");   // Basic exception support. Will update later

				impl::_push(L, std::make_tuple(args...));
				if ( int error = lua_pcall(L, sizeof...( Args ), LUA_MULTRET, 0) ) {    // add "int error = " if i want to keep track of the error code
					debug::debugStack(L, std::cout);
					std::cin.get();
					//LuaPanicError e("LuaCaller::checkPushCall", LuaTypeTraits<std::string>::popValue(L));
					//std::cout << e.error() << std::endl;
					//std::cin.get();
					//throw e;
				}

				return L;
			}

		public:
			LuaCaller(lua_State* lua) : L(lua) { }
			~LuaCaller() { L = nullptr; }

			// direct wrapper of the lua api
			template <typename... Args>
			int call(Args&&... args) {
				auto former = lua_gettop(L) - 1;
				return lua_gettop(checkPushCall(std::forward<Args>(args)...)) - former;
			}

			// Enables "var = luaFunction(args)" statements by returning a lua_State*
			template <typename... Args>
			lua_State* operator()(Args&&... args) {
				return checkPushCall(std::forward<Args>(args)...);
			}
	};

}