

#pragma once

#include "Helpers/LuaVarHelper.h"

// TODO:
// call lua functions with args
// index lua variables (tables)
// custom opening of lua_State*

namespace autoLua {

	class LuaState {
		private:
			lua_State* L;

		public:
			LuaState() : L(luaL_newstate()) { }
	
			operator lua_State*() {
				return L;
			}

			template <typename T>
			LuaVarHelper operator[](T name) {
				return LuaVarHelper(L, name);
			}
			template <int N>
			LuaVarHelper operator[](const char(&name)[N]) {
				return (*this)[std::string(name)];
			};

			// call(std::string func)
			LuaConverter operator()(std::string code) {
				luaL_dostring(L, code.c_str());
				return L;
			}

	};

}