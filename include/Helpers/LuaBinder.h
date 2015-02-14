

#pragma once

#include "impl\LuaTypeTraits.h"

// bind c++ objects to lua variables (define LuaTypeTraits<T> to use LuaBinder<T> so that users don't have to declare both) 
// c++ objects through lua
// tie it all into LuaRegistry

namespace autoLua {

	/*
	::lua object representation::
	metatable {
		__this,			(from SweetLua)
		__owner,		(c++ or lua owned)
		__type,			(from SweetLua)

		__metatable,	(LuaClass from LuaStream)
		__gc			(LuaClass from LuaStream)

		__tostring,		(LuaClass from LuaStream)	::optional

		__call,			(LuaClass from LuaStream)
		__index,		(LuaClass from LuaStream)
		__newindex,		(my own work)
	}
	{ methods }

	default lua metamethods:
	__index,	__newindex,		__mode,
	__call,		__metatable,	__tostring,
	__len,		__gc,			__unm,
	__add,		__sub,			__mul,
	__div,		__mod,			__pow,
	__concat,	__eq,			__lt,
	_le


	have methods be part of the metatable ???
		would look name up in methods table if it doesn't exist outside
	need to have ability to make member variables public / private
		public would wrap getters / setters
	need to have ability to handle c++ / lua ownership
		determine is this class owned by c++ or lua
		if owned by c++, does the class still exist
	how should garbage collection for c++ owned classes be handled ???
	*/

	template <class obj>
	struct LuaBinder {
		using type = obj;  using pointer = type*;  using ref = type&;

		static int refLocation = -1;

		static void registerClass(lua_State* L) {
			// setup class metatable
			// store class in LuaRegistry

			// store function - or - location in LuaRegistry / here
		}

		static void deregisterClass(lua_State* L) {

		}

		// very basic implementation here. doesn't ensure longevity or anything like that
		static pointer getObj(lua_State* L, int idx = -1) {
			return (pointer)lua_touserdata(L, lua_realindex(L, idx));
		}
		static pointer popObj(lua_State* L, int idx = -1) {
			pointer tmp = LuaBinder<obj>::getObj(L, idx);
			luaL_remove(L, idx);
			return tmp;
		}
		static void pushObj(lua_State* L, ref val) {
			lua_pushlightuserdata(L, (void*)val);
		}
		static void isA(lua_State* L, int idx = -1) {

		}

	};

}