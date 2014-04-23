

#pragma once

#include "Helpers/LuaVarHelper.h"
#include "Helpers/LuaStack.h"

// Wrapper of lua_State that provides a central hub for working with lua

// TODO:
// call lua functions with args
// custom opening of lua_State*

namespace autoLua {

	class LuaState {
		private:
			lua_State* L;
			bool stack_trace_on_debug;

		public:
			LuaState(bool debug = false) : L(luaL_newstate()), stack_trace_on_debug(debug) { }
			// custom 'newstate' function
			~LuaState() {
				lua_close(L);
				L = nullptr;
			}
	
			operator lua_State*() {
				return L;
			}
			LuaStack operator*(void) {
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
			LuaStack operator()(std::string code) {
				luaL_dostring(L, code.c_str());
				return L;
			}

	};

}