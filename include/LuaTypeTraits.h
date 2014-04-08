

#pragma once

#include "lua.hpp"
#include <string>

// create function for mass comparison of values
// like " i =|= (1, 2, 3) " <- read: " i == 1 || i == 2 | i == 3 "

/*
template <typename T>
bool operator==(T& obj, std::initializer_list<T> comp_list) {

}
template <typename T>
bool operator!=(T& obj, std::initializer_list<T> comp_list) {
	return !operator==(obj,comp_list);
}
*/

// convert a negative lua index into the positive index that points to the same spot
int lua_absindex(lua_State* L, int idx) {
	// equals_all<LUA_GLOBALSINDEX>(idx);
	// idx != { LUA_GLOBALSINDEX }
	if ( idx < 0 && idx != LUA_GLOBALSINDEX )
		return lua_gettop(L) + idx + 1;
	return idx;
}

// get field from a lua table
// needs correction to be type-independent for name
#define lua_getfromtable(L,idx,name) \
	lua_pushstring(L,(name));\
	lua_gettable(L,lua_absindex(L,(idx)))


namespace autoLua {
	namespace impl {

		template <typename T>
		struct LuaTypeTraits {
			static T getValue(lua_State* L, int idx) { }
			static void pushValue(lua_State* L, T& val) { }
		};

		template <>
		struct LuaTypeTraits<int> {
			static int getValue(lua_State* L, int idx) {
				return luaL_checkint(L, lua_absindex(L,idx));
			}
			static void pushValue(lua_State* L, int val) {
				lua_pushinteger(L, val);
			}
		};
		
		template <>
		struct LuaTypeTraits<lua_Number> {
			static lua_Number getValue(lua_State* L, int idx) {
				return luaL_checknumber(L, lua_absindex(L,idx));
			}
			static void pushValue(lua_State* L, lua_Number val) {
				lua_pushnumber(L, val);
			}
		};

		template<>
		struct LuaTypeTraits<std::string> {
			static std::string getValue(lua_State* L, int idx) {
				return luaL_checkstring(L, lua_absindex(L, idx));
			}
			static void pushValue(lua_State* L, std::string val) {
				lua_pushstring(L, val.c_str());
			}
		};
		

	}
}