

#pragma once

#include "Helpers\LuaVariable.h"
#include "Helpers\LuaStack.h"

// Wrapper of lua_State that provides a central hub for working with lua

// TODO:

namespace autoLua {

	class LuaState {
		private:
			lua_State* L;
			bool stack_trace_on_debug;
			impl::LuaRegistry* registry;

			static lua_State* defaultSetup();

		public:
			LuaState(bool = false);
			LuaState(const std::function<lua_State*(void)>& setupLua, bool debug = false)
					: L(setupLua()), stack_trace_on_debug(debug) {
				registry = lua_newregister(L);
			}
			~LuaState();
	
			// get underlying lua_State* implicitly for reverse compatibility with standard lua library functions
			operator lua_State*();
			// temporary method for LuaMetatable work
			operator impl::LuaRegistry*();

			// get direct access to the lua stack
			LuaStack operator*(void);

			template <typename T>
			LuaVariable operator[](T name) {
				return LuaVariable(L, name, registry);
			}

			LuaStack run(std::string);
			//LuaState& load(std::string);

			// bind c++ class to lua (subject to change)
			template <class obj>
			LuaState& expose() {
				return *this;
			}
			template <class obj>
			LuaState& expose(obj&& placeholder) {
				return expose<obj>();
			}

	};

}