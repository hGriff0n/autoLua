

#pragma once

#include "LuaTypeTraits.h"

namespace autoLua {
	namespace impl {

		template <class output>
		void stackDebug(lua_State* L, output&& out) {
			out << "Stack size: " << lua_gettop(L) << endl;
			for ( auto i = 1; i != lua_gettop(L) + 1; ++i ) {
				out << luaL_typename(L, i);
				if ( lua_type(L, i) == LUA_TSTRING )
					out << " - " << lua_tostring(L, i);
				if ( lua_type(L, i) == LUA_TNUMBER )
					out << " - " << lua_tonumber(L, i);
				out << "\n";
			}
		}

		template <typename T>
		void setValue(lua_State* L, T value, int loc = LUA_GLOBALSINDEX) {
			lua_remove(L, lua_gettop(L));
			LuaTypeTraits<T>::pushValue(L, value);
			lua_settable(L, loc);
		}
		template <typename T>
		void setValue(lua_State* L, T value, bool& useGlobals) {
			setValue(L, value, (useGlobals ? LUA_GLOBALSINDEX : -3));

			useGlobals = true;
		}

		void createTables(lua_State* L, int loc, bool throwIfVal = false) {
			if ( lua_isnil(L, -1) ) {
				loc = lua_realindex(L, loc);
				lua_remove(L, lua_gettop(L));
				lua_pushvalue(L, -1);
				lua_pushvalue(L, -1);
				lua_newtable(L);
				lua_settable(L, loc);
				lua_gettable(L, loc);
			} else if ( !lua_istable(L, -1) && throwIfVal )
				throw("Error");

			luaL_remove(L, -2);
		}

		// Constructor
		template <typename T>
		void indexOp(T field, lua_State* L) {
			LuaTypeTraits<T>::pushValue(L, field, 2);
			lua_gettable(L, LUA_GLOBALSINDEX);
		}
		template <int N>
		void indexOp(const char(&field)[N], lua_State* L) {
			indexOp(std::string(field), L);
		}

		template <typename T>
		void indexOp(T field, lua_State* L, bool& useGlobalOp, bool throwIfVal) {
			auto createField = useGlobalOp ? LUA_GLOBALSINDEX : -3;

			createTables(L, createField, throwIfVal);
			if ( !useGlobalOp )
				luaL_remove(L, -2);

			LuaTypeTraits<T>::pushValue(L, field, 2);
			lua_gettable(L, -3);

			useGlobalOp = false;
		}
		template <int N>
		void indexOp(const char(&field)[N], lua_State* L, bool& useGlobalOp, bool throwIfVal) {
			indexOp(std::string(field), L, useGlobalOp, throwIfVal);
		}

	}
}
