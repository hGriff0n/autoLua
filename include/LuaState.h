

#pragma once

#include "Helpers\LuaVariable.h"

// Wrapper of lua_State that provides a central hub for working with lua

// TODO:

namespace autoLua {

	class LuaState {
		lua_State* L;
		impl::LuaRegistry* registry;

		static lua_State* defaultSetup();
		static int defaultPanic(lua_State*);

		public:
		    LuaState();// bool = false);
			LuaState(const std::function<lua_State*( void )>&);//, bool = false);
			~LuaState();
	
			// implicit cast to lua_State* for reverse compatibility with standard lua library functions
			operator lua_State*();
			// temporary method for LuaMetatable work
			operator impl::LuaRegistry*();

			// get direct access to the lua stack
			LuaStack operator*(void);

			// global index
			template <typename T>
			LuaVariable operator[](T name) {
				return LuaVariable(L, name, registry);
			}

			// run the lua code
			LuaStack run(std::string);

			// load the lua file
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