

#pragma once

#include "Helpers\LuaVarHelper.h"
#include "Helpers\LuaStack.h"

// Wrapper of lua_State that provides a central hub for working with lua

// TODO:

namespace autoLua {

	class LuaState {
		private:
			lua_State* L;
			bool stack_trace_on_debug;
			impl::LuaRegistry* registry;

			static lua_State* defaultSetup() {
				return luaL_newstate();
			}

		public:
			LuaState(bool debug = false) :
					L(LuaState::defaultSetup()), stack_trace_on_debug(debug) {
				registry = lua_newregister(L);
			}
			LuaState(const std::function<lua_State*(void)>& setupLua, bool debug = false)
					: L(setupLua()), stack_trace_on_debug(debug) { }
			~LuaState() {
				lua_closeregister(registry);
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
				return LuaVarHelper(L, name, registry);
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