

#include "LuaState.h"

namespace autoLua {

	LuaState::LuaState() : LuaState(LuaState::defaultSetup) { }

	LuaState::LuaState(const std::function<lua_State*( void )>& setupLua) : L(setupLua()) { //, stack_trace_on_debug(debug) {
		registry = lua_newregister(L);
		//lua_atpanic(L, defaultPanic);
	}

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

	int LuaState::defaultPanic(lua_State* L) {
		return 0;
	}

}
