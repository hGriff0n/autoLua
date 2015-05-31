

#pragma once

// might have to include some files here, but it seems to compile fine

namespace autoLua {

	class LuaNil {};

	namespace impl {

		template <typename T> struct LuaTypeTraits;

		template <>
		struct LuaTypeTraits<LuaNil> {
			using type = LuaNil;

			static type getValue(lua_State* L, int idx = -1) {
				return LuaNil();
			}

			static type popValue(lua_State* L, int idx = -1) {
				luaL_remove(L, idx);     // should I remove the value or should I check that it is nil first ???
				return LuaNil();
			}

			static void pushValue(lua_State* L, type val, int N = 1) {
				for ( auto i = 0; i != N; ++i )
					lua_pushnil(L);
			}

			static bool isA(lua_State* L, int idx = -1) {
				return lua_isnil(L, idx);
			}

			//static void setValue(lua_State* L, int lookup, type& obj, bool use_metamethods = true) { }
		};

		template <>
		struct LuaTypeTraits<std::nullptr_t> {
			using type = std::nullptr_t;

			static type getValue(lua_State* L, int idx = -1) {
				return nullptr;
			}

			static type popValue(lua_State* L, int idx = -1) {
				luaL_remove(L, idx);     // should I remove the value or should I check that it is nil first ???
				return nullptr;
			}

			static void pushValue(lua_State* L, type val, int N = 1) {
				for ( auto i = 0; i != N; ++i )
					lua_pushnil(L);
			}

			static bool isA(lua_State* L, int idx = -1) {
				return lua_isnil(L, idx);
			}
		};

	}

}