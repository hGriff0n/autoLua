

#pragma once

#include "lua.hpp"

namespace autoLua {
	namespace debug {

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

	}
}