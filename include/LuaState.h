

#pragma once

#include "lua.hpp"
#include "LuaCaller.h"

namespace autoLua {

	class LuaState {
		private:
			lua_State* L;

		public:
			LuaState() : L(luaL_newstate()) { }

			operator lua_State*() {
				return L;
			}
			
			LuaCaller operator[](std::string func) {
				lua_getglobal(L, func.c_str());
				return L;
			}


	};

}