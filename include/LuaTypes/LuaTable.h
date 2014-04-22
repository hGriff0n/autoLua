

#pragma once

#include "LuaTypeTraits.h"

namespace autoLua {

	class LuaTable {
		private:

		public:
			LuaTable() { }

	};

	namespace impl {

		template <>
		struct LuaTypeTraits<LuaTable> {
			using type = LuaTable;

			static type getValue(lua_State* L, int idx = -1) {
				return LuaTable();
			}
			static type popValue(lua_State* L, int idx = -1) {
				return LuaTable();
			}
			static void pushValue(lua_State* L, type& val, int N = 1) {
				for ( auto i = 0; i != N; ++i )
					lua_newtable(L);
			}
			static bool isA(lua_State* L, int idx = -1) {
				return lua_istable(L, idx);
			}
			//static void setValue(lua_State* L, int lookup, type& obj, bool use_metamethods = true) { }
		};

	}

}