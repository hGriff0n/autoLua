

#pragma once

#include "lua.hpp"

namespace autoLua {

	class LuaState {
		private:
			lua_State* L;

		public:
			LuaState() : L(luaL_newstate()) { }

			operator lua_State*() {
				return L;
			}

	};

}