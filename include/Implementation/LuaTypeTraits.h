

#pragma once

#include "lua.hpp"
#include <string>

// create function for mass comparison of values
// like " i =|= (1, 2, 3) " <- read: " i == 1 || i == 2 | i == 3 "
template <typename T>
bool equalsOne(T obj, std::initializer_list<T> comp_list) {
	for ( auto x : comp_list )
		if ( obj == x ) return true;
	return false;
}
template <typename T>
bool notEquals(T obj, std::initializer_list<T> comp_list) {
	return !equalsOne(obj, comp_list);
}

// convert a negative lua index into the positive index that points to the same spot
int lua_absindex(lua_State* L, int idx) {
	if ( idx < 0 && notEquals(idx, { LUA_GLOBALSINDEX }) )
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
			using type = T;

			static type getValue(lua_State* L, int idx = -1) { }
			static type popValue(lua_State* L, int idx = -1) { }
			static void pushValue(lua_State* L, type& val) { }
		};

		template <>
		struct LuaTypeTraits<int> {
			using type = int;

			static type getValue(lua_State* L, int idx = -1) {
				return luaL_checkint(L, lua_absindex(L,idx));
			}
			static type popValue(lua_State* L, int idx = -1) {
				auto x = LuaTypeTraits<type>::getValue(L, idx);
				lua_remove(L, idx);
				return x;
			}
			static void pushValue(lua_State* L, type val) {
				lua_pushinteger(L, val);
			}
		};
		
		template <>
		struct LuaTypeTraits<lua_Number> {
			using type = lua_Number;

			static type getValue(lua_State* L, int idx = -1) {
				return luaL_checknumber(L, lua_absindex(L,idx));
			}
			static type popValue(lua_State* L, int idx = -1) {
				auto x = LuaTypeTraits<type>::getValue(L, idx);
				lua_remove(L, idx);
				return x;
			}
			static void pushValue(lua_State* L, type val) {
				lua_pushnumber(L, val);
			}
		};

		template<>
		struct LuaTypeTraits<std::string> {
			using type = std::string;

			static type getValue(lua_State* L, int idx = -1) {
				return luaL_checkstring(L, lua_absindex(L, idx));
			}
			static type popValue(lua_State* L, int idx = -1) {
				auto x = LuaTypeTraits<type>::getValue(L, idx);
				lua_remove(L, idx);
				return x;
			}
			static void pushValue(lua_State* L, type val) {
				lua_pushstring(L, val.c_str());
			}
		};

	}
}