

#pragma once

#include "lua.hpp"

#include <iomanip>

namespace autoLua {
	namespace debug {

		template <class _stream>
		void flush(_stream& out) {
			out << "<---------------------------------------->\n" << std::endl;
		}

		template <class _stream>
		void printHead(lua_State* L, _stream& debugger) {
			debugger << "<---lua_State debug information--->\n";
		}

		template <class _stream>
		void printBody(lua_State* L, _stream& debugger) {
			debugger << "\n";
		}

		template <class _stream>
		void printIndex(lua_State* L, int idx, _stream& debugger) {
			debugger << "::at idx " << idx << "[" << lua_negindex(L, idx) << "] | ";// << std::setw(9);
			debugger << "{" << std::setw(7) << std::right << luaL_typename(L, idx) << "} -> ";
			switch ( lua_type(L, idx) ) {
				case LUA_TNIL:
					debugger << "nil";
					break;
				case LUA_TNUMBER:
					debugger << lua_tonumber(L, idx);
					break;
				case LUA_TBOOLEAN:
					debugger << (lua_toboolean(L, idx) ? "true" : "false");
					break;
				case LUA_TSTRING:
					debugger << "\"" << lua_tostring(L, idx) << "\"";
					break;
				default:
					break;
			}
			debugger << "\n";
		}

		//template <typename _lua, class _stream>
		template <class _stream>
		void debugStack(lua_State* L, _stream& debugger) {
			printHead(L, debugger);
			for ( auto i = 1; i != lua_gettop(L) + 1; ++i )
				printIndex(L, i, debugger);
			printBody(L, debugger);
		}

		template <class _stream>
		void debugLua(lua_State* L, _stream& debugger) {
			debugStack(L, debugger);
			flush(debugger);
		}

	}
}