

#pragma once

#include "impl\function_impl.h"

namespace autoLua {

	template <typename... T> //typename std::enable_if<std::is_even<sizeof...(T)>>::type = 0>
	class LuaTable {
		private:
			friend struct impl::LuaTypeTraits<LuaTable<T...>>;

			std::tuple<T...> values;
			const int numPairs = (sizeof...(T) / 2);		// possibly public in the future

			void fillTable(lua_State* L) {
				auto table_pos = lua_gettop(L);
				impl::_push(L, values);
				for ( auto i = 0; i != numPairs; ++i )
					lua_settable(L, table_pos);
			}
	
		public:
			LuaTable(T... ts) : values(std::make_tuple(ts)) { }
			LuaTable(std::tuple<T...> _vals) : values(_vals) { }
	};
	// override for templating on void (should I override LuaTypeTraits to???)
	template <>
	class LuaTable<void> {
		private:
			friend struct impl::LuaTypeTraits<LuaTable<void>>;

			const int numPairs = 0;

			void fillTable(lua_State* L) { }
		public:
			LuaTable() { }
	};

	namespace impl {

		template <typename... T>
		std::tuple<T...> _getLuaTable(lua_State* L, int idx) {
			idx = lua_realindex(L, idx);
			while ( lua_next(L, idx) ) {
				lua_pushvalue(L, -2);
			}
			// currently slightly wrong (now: value, key -> expected: key, value) quick future fix
			return _getArgs<T...>(L);
		}

		template <typename... T>
		struct LuaTypeTraits<LuaTable<T...>> {
			using type = LuaTable<T...>;

			static type getValue(lua_State* L, int idx = -1) {
				return LuaTable<T...>(_getLuaTable(L, idx));
			}
			static type popValue(lua_State* L, int idx = -1) {
				auto x = LuaTypeTraits<LuaTable<T...>>::getValue(L, idx);
				luaL_remove(L, idx);
				return x;
			}
			static void pushValue(lua_State* L, type& val, int N = 1) {
				for ( auto i = 0; i != N; ++i ) {
					lua_createtable(L, val.numPairs, val.numPairs);
					val.fillTable(L);
				}
			}
			static bool isA(lua_State* L, int idx = -1) {
				return lua_istable(L, idx);
			}
			//static void setValue(lua_State* L, int lookup, type& obj, bool use_metamethods = true) { }
		};

	}

}