

#include "LuaTypes\LuaTable.h"
#include "LuaTypes\LuaNil.h"

namespace autoLua {

	LuaMetatable::LuaMetatable() : reg(nullptr) { }
	LuaMetatable::LuaMetatable(impl::LuaRegistry* _r) : reg(_r) {
		//luaL_newmetatable(getRegisterState(_r), name.c_str());
		lua_newtable(getRegisterState(_r));
	}

	void LuaMetatable::registerCode(std::string name, std::string code) {
		lua_State* L = getRegisterState(reg);
		impl::LuaTypeTraits<std::string>::pushValue(L, name);
		luaL_dostring(L, std::string("return " + code).c_str());
		lua_settable(L, -3);
	}
	void LuaMetatable::setTable(lua_State* L) {
		if ( reg != nullptr ) {
			lua_insert(L, lua_realindex(L,-2));
			lua_setmetatable(L, -2);
		}
	}

	LuaMetatable makeMetatable(impl::LuaRegistry* _r) {
		return makeMetatable(_r, "", { });
	}
	LuaMetatable makeMetatable(impl::LuaRegistry* _r, std::string name) {
		return makeMetatable(_r, name, { });
	}
	LuaMetatable makeMetatable(impl::LuaRegistry* _r, std::initializer_list<int> predefs) {
		return makeMetatable(_r, "", predefs);
	}
	LuaMetatable makeMetatable(impl::LuaRegistry* _r, std::string name, std::initializer_list<int> predefs) {
		LuaMetatable meta(_r);
		for ( auto i : predefs )
			switch ( i ) {
				case LuaMetatable::read_only:
					meta.registerValue("__newindex", LuaNil());
					break;
				case LuaMetatable::constructor:
					meta.registerCode("__call", std::string("function(...) return " + name + ".new(...) end"));
					break;
				case LuaMetatable::hidden:
					meta.registerValue("__metatable", LuaNil());
					break;
				default:
					break;
		}
		return meta;
	}

}