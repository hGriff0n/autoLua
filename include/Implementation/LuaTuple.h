

#pragma once

#include <tuple>
#include "LuaConverter.h"

namespace autoLua {

	template <typename... T>
	class LuaTuple {
		private:
			std::tuple<T&...> _tuple;

		public:
			LuaTuple(T&... args) : _tuple(args...) { }

			void operator=(LuaConverter& conv) {
				conv.move(_tuple);
			}

	};

	// just in case there are problems with std::tie
	template <typename... Args>
	LuaTuple<Args...> luaTie(Args&... args) {
		return LuaTuple<Args...>(args...);
	}

}