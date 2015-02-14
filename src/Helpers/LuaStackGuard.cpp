

#include "Helpers\LuaStackGuard.h"

autoLua::LuaStackGuard::LuaStackGuard(lua_State* lua, int restore_delta) : L(lua), restore_to(lua_gettop(lua)) {
	changeRestoreDelta(restore_delta);
}
autoLua::LuaStackGuard::~LuaStackGuard() {
	lua_settop(L, restore_to);
}

void autoLua::LuaStackGuard::changeRestoreDelta(int new_delta) {
	restore_to += new_delta;	// legacy code. not sure about how to implement it here
}