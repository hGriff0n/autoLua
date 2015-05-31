

#pragma once

#include "impl\function_impl.h"
//#include "LuaTypes\Tuple.h"


#include <iostream>
#include "debugTesting.h"

// Helper class that handles interaction with the lua stack (often for developer's use)

namespace autoLua {

	class LuaStack {
		lua_State* L;

		public:
			LuaStack(lua_State* lua) : L(lua) { }
			~LuaStack() {
				L = nullptr;
			}

			// used to have an error here when chained with LuaTuple (see main.cpp)
			// pops and returns the element at the top of the stack if it convertible to type T
			template <typename T>
			operator T() {
				//debug::debugStack(L, std::cout);
				//std::cin.get();
				return impl::LuaTypeTraits<T>::popValue(L);
			}

			// fills the given tuple by popping elements off the stack (change name??? have to change LuaTuple::operator=)
			template <typename... T>
			void move(std::tuple<T&...>& args) {
				args = impl::_getArgs<T...>(L);
			}

			// pushes an element onto the stack
			template <typename T>
			LuaStack& operator<<(T value) {
				impl::LuaTypeTraits<T>::pushValue(L, value);
				return *this;
			}

			// pushes a string literal onto the stack
			template <int N>
			LuaStack& operator<<(const char(&name)[N]) {
				return *this << std::string(name);
			}

			// extracts the top element of the stack into the passed variable
			template <typename T>
			LuaStack& operator>>(T& value) {
				value = impl::LuaTypeTraits<T>::popValue(L);
				return *this;
			}

	};

}