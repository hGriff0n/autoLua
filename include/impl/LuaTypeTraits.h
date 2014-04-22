

#pragma once

#include "lua.hpp"
#include <string>

// disable C4800

// functions for mass comparison of values
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
int lua_realindex(lua_State* L, int idx) {
	if ( idx < 0 && notEquals(idx, { LUA_GLOBALSINDEX }) )
		return lua_gettop(L) + idx + 1;
	return idx;
}
int lua_negindex(lua_State* L, int idx) {
	if ( idx > 0 )
		idx -= (lua_gettop(L) + 1);
	return idx;
}

#define luaL_remove(L, idx) lua_remove(L, lua_realindex(L, idx))


// convert LuaTypeTraits::pushValue to use for loop to push multiple times
namespace autoLua {

	// tag classes
	typedef lua_CFunction LuaFunction;
	class LuaNil;

	namespace impl {

		template <typename T>
		struct LuaTypeTraits {
			using type = T;

			static type getValue(lua_State* L, int idx = -1) { }
			static type popValue(lua_State* L, int idx = -1) { }
			static void pushValue(lua_State* L, type& val, int N = 1) { }
			static bool isA(lua_State* L, int idx = -1) { }
			//static void setValue(lua_State* L, int lookup, type& obj, bool use_metamethods = true) { }
		};

		template <>
		struct LuaTypeTraits<int> {
			using type = int;

			static type getValue(lua_State* L, int idx = -1) {
				return luaL_checkint(L, lua_realindex(L,idx));
			}
			static type popValue(lua_State* L, int idx = -1) {
				auto x = LuaTypeTraits<type>::getValue(L, idx);
				lua_remove(L, idx);
				return x;
			}
			static void pushValue(lua_State* L, type val, int N = 1) {
				for ( auto i = 0; i != N; ++i )
					lua_pushinteger(L, val);
			}
			static bool isA(lua_State* L, int idx = -1) {
				return lua_isnumber(L, idx);
			}
		};
		
		template <>
		struct LuaTypeTraits<lua_Number> {
			using type = lua_Number;

			static type getValue(lua_State* L, int idx = -1) {
				return luaL_checknumber(L, lua_realindex(L,idx));
			}
			static type popValue(lua_State* L, int idx = -1) {
				auto x = LuaTypeTraits<type>::getValue(L, idx);
				lua_remove(L, idx);
				return x;
			}
			static void pushValue(lua_State* L, type val, int N = 1) {
				for ( auto i = 0; i != N; ++i )
					lua_pushnumber(L, val);
			}
			static bool isA(lua_State* L, int idx = -1) {
				lua_isnumber(L, idx);
			}
		};

		template<>
		struct LuaTypeTraits<std::string> {
			using type = std::string;

			static type getValue(lua_State* L, int idx = -1) {
				return luaL_checkstring(L, lua_realindex(L, idx));
			}
			static type popValue(lua_State* L, int idx = -1) {
				auto x = LuaTypeTraits<type>::getValue(L, idx);
				lua_remove(L, idx);
				return x;
			}
			static void pushValue(lua_State* L, type val, int N = 1) {
				for ( auto i = 0; i != N; ++i )
					lua_pushstring(L, val.c_str());
			}
			static bool isA(lua_State* L, int idx = -1) {
				return lua_isstring(L, idx);
			}
		};

		template<>
		struct LuaTypeTraits<LuaFunction> {
			using type = LuaFunction;

			static type getValue(lua_State* L, int idx = -1) {
				return lua_tocfunction(L,idx);
			}
			static type popValue(lua_State* L, int idx = -1) {
				lua_CFunction x = LuaTypeTraits<type>::getValue(L, idx);
				lua_remove(L, idx);
				return x;
			}
			static void pushValue(lua_State* L, type val, int N = 1) {
				for ( auto i = 0; i != N; ++i )
					lua_pushcfunction(L, val);
			}
			static bool isA(lua_State* L, int idx = -1) {
				return lua_iscfunction(L, idx);
			}
		};

	}
}