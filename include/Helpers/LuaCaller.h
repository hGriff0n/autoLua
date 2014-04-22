

#pragma once

#include "LuaConverter.h"

namespace autoLua {

	// Caller helper class
	class LuaCaller {
		private:
			lua_State* L;

		protected:
			template <typename... Args>
			LuaConverter pushAndCall(int N, std::tuple<Args...>&& obj) {
				impl::_push(L, obj);
				lua_pcall(L, N, LUA_MULTRET, 0);
				return L;
			}

		public:
			LuaCaller(lua_State* lua) : L(lua) { }
			~LuaCaller() { L = nullptr; }

			template <typename... Args>
			LuaConverter operator()(Args... args) {
				return pushAndCall(sizeof...(Args), std::make_tuple(args...));
			}
	};

}