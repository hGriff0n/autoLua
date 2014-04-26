

#pragma once

#include "impl/function_impl.h"

// Helper class that interacts directly with the lua stack
// Replaced LuaConverter (which might be brought back due to it's highly tailored nature)

// slight problem with tuple override (see error message in main.cpp)

namespace autoLua {

	class LuaStack {
		private:
			lua_State* L;

		public:
			LuaStack(lua_State* lua) : L(lua) { }
			~LuaStack() {
				L = nullptr;
			}

			// tries to convert LuaStack to std::tuple when I don't want it to (and it shouldn't)
			// see error comment in main.cpp
			template <typename T>
			operator T() {
				return impl::LuaTypeTraits<T>::popValue(L);
			}
			template <typename... T>
			void move(std::tuple<T&...>& args) {
				args = impl::_getArgs<T...>(L);
			}

			template <typename T>
			LuaStack& operator=(T value) {
				impl::LuaTypeTraits<T>::pushValue(L, value);
				return *this;
			}
			template <int N>
			LuaStack& operator=(const char(&name)[N]) {
				return (*this = std::string(name));
			};

	};

}