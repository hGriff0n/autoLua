

#pragma once

#include "Helpers/LuaStack.h"

namespace autoLua {

	template <typename... T>
	class LuaTuple {
		private:
			std::tuple<T&...> _tuple;

		public:
			LuaTuple(T&... args) : _tuple(args...) { }

			void operator=(LuaStack conv) {
				conv.move(_tuple);
			}

	};

	template <typename... Args>
	LuaTuple<Args...> luaTie(Args&... args) {
		return LuaTuple<Args...>(args...);
	}

}