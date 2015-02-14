

#include "Helpers\LuaVariable.h"

namespace autoLua {

	void LuaVariable::setStackField(int alt_index) {
		auto setter = flags[0] ? &lua_settable : &lua_rawset;
		setter(L, flags[1] ? LUA_GLOBALSINDEX : alt_index);
	}
	void LuaVariable::getStackField(int index) {
		auto getter = flags[0] ? &lua_gettable : &lua_rawget;
		getter(L, index);
	}
	void LuaVariable::setToValue() {
		luaL_remove(L, -2);
		setStackField(-3);
	}
	void LuaVariable::checkTables() {
		if ( lua_isnil(L, -1) ) {
			auto loc = lua_realindex(L, flags[1] ? LUA_GLOBALSINDEX : -3);
			lua_remove(L, lua_gettop(L));
			lua_pushvalue(L, -1);
			lua_pushvalue(L, -1);
			lua_newtable(L);
			lua_settable(L, loc);
			lua_gettable(L, loc);
		} else if ( !lua_istable(L, -1) && flags[2] )
			throw("Error");

		luaL_remove(L, -2);
	}

	std::string LuaVariable::type() {
		return lua_typename(L, lua_type(L, -1));
	}

}
