

#pragma once

#include "impl.h"

#include <iostream>

namespace autoLua {

	// automatically converts the values on the lua stack to c++ types
	// accomplished through templated operator T() (override for tuple<Args...>)
	// slight problem tuple override (see error message in main.cpp)
	struct LuaConverter {
		private:
			lua_State* L;

		public:
			LuaConverter(lua_State* lua) : L(lua) { }
			~LuaConverter() { L = nullptr; }

			// tries to convert LuaConverter to std::tuple when I don't want it to (and it shouldn't)
			template <typename T>
			operator T() {
				return LuaTypeTraits<T>::popValue(L);
			}

			template <typename... T>
			void move(std::tuple<T&...>& args) {
				args = impl::_getArgs<T...>(L);
			}

	};

	template <typename T, typename std::enable_if<std::is_same<T,int>::value,int>::type = 0>
	int test(T x) {
		return x;
	}

}