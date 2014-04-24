

#pragma once

#include "lua.hpp"

namespace autoLua {
	namespace impl {

		// StackGuard class to prevent the lua stack from 'accidentally' growing during use (RAII)

		class LuaStackGuard {
			private:
				lua_State* L;
				int restore_to;

			public:
				// Optional int is the number of extra values that the Guard should leave on the stack
				// Passing a two will cause the StackGuard to allow two values to be left on the stack after destruction
				LuaStackGuard(lua_State* lua, int restore_delta = 0) : L(lua), restore_to(lua_gettop(lua)) {
					changeRestoreDelta(restore_delta);
				}
				~LuaStackGuard() {
					lua_settop(L, restore_to);
				}

				void changeRestoreDelta(int new_delta) {
					restore_to += new_delta;	// legacy code. not sure about how to implement it here
				}
		};

	}
}