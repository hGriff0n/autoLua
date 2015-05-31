

#include "Helpers\LuaVariable.h"

namespace autoLua {

	// sets the variable on the stack, using metamethods if desired
	void LuaVariable::setStackField(int alt_index) {
		(flags[0] ? &lua_settable : &lua_rawset)(L, flags[1] ? LUA_GLOBALSINDEX : alt_index );
		//auto setter = flags[0] ? &lua_settable : &lua_rawset;
		//setter(L, flags[1] ? LUA_GLOBALSINDEX : alt_index);
	}

	// gets the variable onto the stack, using metamethods if desired
	void LuaVariable::getStackField(int index) {
		(flags[0] ? &lua_gettable : &lua_rawget)(L, index);
		//auto getter = flags[0] ? &lua_gettable : &lua_rawget;
		//getter(L, index);
	}

	// sets the variable on the stack
	void LuaVariable::setToValue() {
		luaL_remove(L, -2);
		setStackField(-3);
	}

	// checks for a table at the 
	void LuaVariable::checkTables() {
		// this code is organized poorly. a table won't be created if the element is not nil (regardless of whether flags[2] is set or not)

		if ( lua_isnil(L, -1) ) {
			auto loc = lua_realindex(L, flags[1] ? LUA_GLOBALSINDEX : -3);
			luaL_remove(L, -1);
			lua_pushvalue(L, -1);
			lua_pushvalue(L, -1);
			lua_newtable(L);
			lua_settable(L, loc);
			lua_gettable(L, loc);
		} else if ( !lua_istable(L, -1) && flags[2] )
			throw LuaTypeError("LuaVariable::checkTables", "Not a table");        // Not entirely desired (or accurate) but works for now

		luaL_remove(L, -2);
	}

	// might have to roll a custom solution when the class binding is finished
	std::string LuaVariable::type() {
		return lua_typename(L, lua_type(L, -1));
	}

}
