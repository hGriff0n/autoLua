

#pragma once

#include "impl.h"

#include "lua.hpp"
#include <string>

// disable C4800

namespace autoLua {

	// tag classes
	typedef lua_CFunction LuaFunction;

	namespace impl {

		// TypeTraits class to abstract away c++/lua type system mismatch

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
				luaL_remove(L, idx);
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
				luaL_remove(L, idx);
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
				luaL_remove(L, idx);
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
				luaL_remove(L, idx);
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

		template<>
		struct LuaTypeTraits<bool> {
			using type = bool;

			static type getValue(lua_State* L, int idx = -1) {
				return lua_toboolean(L, idx);
			}
			static type popValue(lua_State* L, int idx = -1) {
				bool x = LuaTypeTraits<type>::getValue(L, idx);
				luaL_remove(L, idx);
				return x;
			}
			static void pushValue(lua_State* L, type val, int N = 1) {
				for ( auto i = 0; i != N; ++i )
					lua_pushboolean(L, val);
			}
			static bool isA(lua_State* L, int idx = -1) {
				return lua_isboolean(L, idx);
			}
		};

		// defined in terms of LuaTypeTraits<std::string>
		// may be more useful to define the other way around (LuaTypeTraits<string> in terms of LuaTypeTraits<const char*>)
		template<>
		struct LuaTypeTraits<const char*> {
			using type = const char*;

			static type getValue(lua_State* L, int idx = -1) {
				return LuaTypeTraits<std::string>::getValue(L, idx).c_str();
			}
			static type popValue(lua_State* L, int idx = -1) {
				return LuaTypeTraits<std::string>::popValue(L, idx).c_str();
			}
			static void pushValue(lua_State* L, type val, int N = 1) {
				LuaTypeTraits<std::string>::pushValue(L, val, N);
			}
			static bool isA(lua_State* L, int idx = -1) {
				return LuaTypeTraits<std::string>::isA(L, idx);
			}
		};

	}

}