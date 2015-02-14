

#pragma once

#include "lua.hpp"

namespace autoLua {

		// StackGuard class to prevent the lua stack from 'accidentally' growing during use (RAII)

		class LuaStackGuard {
			private:
				lua_State* L;
				int restore_to;

			public:
				// Optional int is the number of extra values that the Guard should leave on the stack
				// Passing a two will cause the StackGuard to allow two values to be left on the stack after destruction
				LuaStackGuard(lua_State*, int = 0);
				~LuaStackGuard();

				void changeRestoreDelta(int);
		};

}