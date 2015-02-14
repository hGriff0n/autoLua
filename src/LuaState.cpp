

#include "LuaState.h"

namespace autoLua {

	LuaState::LuaState(bool debug) : LuaState(LuaState::defaultSetup, debug) { }
	LuaState::~LuaState() {
		lua_closeregister(registry);
		lua_close(L);
		L = nullptr;
	}

	LuaState::operator lua_State*() {
		return L;
	}
	LuaState::operator impl::LuaRegistry*() {
		return registry;
	}
	LuaStack LuaState::operator*(void) {
		return L;
	}

	LuaStack LuaState::run(std::string code) {
		luaL_dostring(L, code.c_str());
		return L;
	}

	lua_State* LuaState::defaultSetup() {
		return luaL_newstate();
	}

}
