

#pragma once

#include "impl/LuaTypeTraits.h"

namespace autoLua {

	class LuaNil {
		private:

		public:
			LuaNil() { }

	};

	namespace impl {

		template <>
		struct LuaTypeTraits<LuaNil> {
			using type = LuaNil;

			static type getValue(lua_State* L, int idx = -1) {
				return LuaNil();
			}
			static type popValue(lua_State* L, int idx = -1) {
				return LuaNil();
			}
			static void pushValue(lua_State* L, type& val, int N = 1) {
				for ( auto i = 0; i != N; ++i )
					lua_pushnil(L);
			}
			static bool isA(lua_State* L, int idx = -1) {
				return lua_isnil(L, idx);
			}
			//static void setValue(lua_State* L, int lookup, type& obj, bool use_metamethods = true) { }
		};

	}

}