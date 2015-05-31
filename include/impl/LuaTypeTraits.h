

#pragma once

#include "impl.h"

#include "lua.hpp"
#include "Exceptions/LuaException.h"
#include <string>

//#pragma warning (once: C4800) there has to be a way to disable warnings

namespace autoLua {

	// tag classes
	typedef lua_CFunction LuaFunction;    // Why do I have this typedef???

	namespace impl {

		// TypeTraits class to abstract away c++/lua type system mismatch
		// Provides a structure for type check, pushing, and popping values on the stack

		// By default: Throws LuaUndefinedBehavior
		// May change definitions (especially for isA) if behavior is undesired
		template <typename T>
		struct LuaTypeTraits {
			using type = T;

			static type getValue(lua_State* L, int idx = -1) {
				throw LuaUndefinedBehavior("LuaTypeTraits<T>::getValue");
			}

			static type popValue(lua_State* L, int idx = -1) {
				throw LuaUndefinedBehavior("LuaTypeTraits<T>::popValue");
			}

			static void pushValue(lua_State* L, type& val, int N = 1) {
				throw LuaUndefinedBehavior("LuaTypeTraits<T>::pushValue");
			}

			static bool isA(lua_State* L, int idx = -1) {
				throw LuaUndefinedBehavior("LuaTypeTraits<T>::isA");
			}
			//static void setValue(lua_State* L, int lookup, type& obj, bool use_metamethods = true) { }
		};

		/* typedef nullptr_t LuaNil_t;

		template <typename T>
		struct LuaTypeTraits<T*> {
			using type = T*;

			static type getValue(lua_State* L, int idx = -1) {
			    if (LuaTypeTraits<LuaNil>::isA(L, idx))
				    return LuaTypeTraits<LuaNil>::getValue(L, idx);

				return LuaTypeTraits<T>::getValue(L, idx);
			}

			static type popValue(lua_State* L, int idx = -1) {
			    if (LuaTypeTraits<LuaNil>::isA(L, idx))
				    return LuaTypeTraits<LuaNil>::popValue(L, idx);

				return LuaTypeTraits<T>::popValue(L, idx);
			}

			static void pushValue(lua_State* L, type val, int N = 1) {
				if ( val == nullptr )
					return LuaTypeTraits<LuaNil>::pushValue(L, LuaNil(), N);

				LuaTypeTraits<T>::pushValue(L, *val, N);
			}

			static bool isA(lua_State* L, int idx = -1) {
				return LuaTypeTraits<T>::isA(L, idx);
			}
		}; */

		// Specialization for int
		template <>
		struct LuaTypeTraits<int> {
			using type = int;

			static type getValue(lua_State* L, int idx = -1) {
				if ( !isA(L, idx) )
					throw LuaTypeError("LuaTypeTraits<int>::getValue", "Value not convertible to an int");   // way to make this static ???

				return lua_tointeger(L, idx);
			}

			static type popValue(lua_State* L, int idx = -1) {
				auto x = LuaTypeTraits<type>::getValue(L, idx);
				luaL_remove(L, idx);
				return x;
			}

			static void pushValue(lua_State* L, type val, int N = 1) {
				for ( auto i = 0; i != N; ++i )
					lua_pushinteger(L, val);
			}

			static bool isA(lua_State* L, int idx = -1) {
				return lua_isnumber(L, idx);
			}
		};
		
		// Specialization for lua_Number(double)
		template <>
		struct LuaTypeTraits<lua_Number> {
			using type = lua_Number;

			static type getValue(lua_State* L, int idx = -1) {
				if ( !isA(L, idx) )
					throw LuaTypeError("LuaTypeTraits<lua_Number>::getValue", "Value not convertible to a lua_Number");

				return luaL_checknumber(L, lua_realindex(L,idx));
			}

			static type popValue(lua_State* L, int idx = -1) {
				auto x = LuaTypeTraits<type>::getValue(L, idx);
				luaL_remove(L, idx);
				return x;
			}

			static void pushValue(lua_State* L, type val, int N = 1) {
				for ( auto i = 0; i != N; ++i )
					lua_pushnumber(L, val);
			}

			static bool isA(lua_State* L, int idx = -1) {
				return lua_isnumber(L, idx);
			}
		};

		// Specialization for LuaFunction
		template<>
		struct LuaTypeTraits<LuaFunction> {
			using type = LuaFunction;

			static type getValue(lua_State* L, int idx = -1) {
				if ( !isA(L, idx) )
					throw LuaTypeError("LuaTypeTraits<LuaFunction>::getValue", "Value not convertible to a LuaFunction");
				
				return lua_tocfunction(L,idx);
			}

			static type popValue(lua_State* L, int idx = -1) {
				lua_CFunction x = LuaTypeTraits<type>::getValue(L, idx);
				luaL_remove(L, idx);
				return x;
			}

			static void pushValue(lua_State* L, type val, int N = 1) {
				for ( auto i = 0; i != N; ++i )
					lua_pushcfunction(L, val);
			}

			static bool isA(lua_State* L, int idx = -1) {
				return lua_iscfunction(L, idx);
			}
		};

		// Specialization for bool
		template<>
		struct LuaTypeTraits<bool> {
			using type = bool;

			static type getValue(lua_State* L, int idx = -1) {
				if ( !isA(L, idx) )
					throw LuaTypeError("LuaTypeTraits<bool>::getValue", "Value not convertible to a bool");

				return lua_toboolean(L, idx);
			}

			static type popValue(lua_State* L, int idx = -1) {
				bool x = LuaTypeTraits<type>::getValue(L, idx);
				luaL_remove(L, idx);
				return x;
			}

			static void pushValue(lua_State* L, type val, int N = 1) {
				for ( auto i = 0; i != N; ++i )
					lua_pushboolean(L, val);
			}

			static bool isA(lua_State* L, int idx = -1) {
				return lua_isboolean(L, idx);
			}
		};

		// Specialization for string
		template<>
		struct LuaTypeTraits<std::string> {
			using type = std::string;

			static type getValue(lua_State* L, int idx = -1) {
				if ( !isA(L, idx) )
					throw LuaTypeError("LuaTypeTraits<string>::getValue", "Value not convertible to a string");
			
				return luaL_checkstring(L, lua_realindex(L, idx));
			}

			static type popValue(lua_State* L, int idx = -1) {
				auto x = LuaTypeTraits<type>::getValue(L, idx);
				luaL_remove(L, idx);
				return x;
			}

			static void pushValue(lua_State* L, type val, int N = 1) {
				for ( auto i = 0; i != N; ++i )
					lua_pushstring(L, val.c_str());
			}

			static bool isA(lua_State* L, int idx = -1) {
				return lua_isstring(L, idx);
			}
		};

		// Specialization for const char* (defined in terms of LuaTypeTraits<std::string>)
		// may be more useful to define the other way around (LuaTypeTraits<string> in terms of LuaTypeTraits<const char*>)
		template<>
		struct LuaTypeTraits<const char*> {
			using type = const char*;

			static type getValue(lua_State* L, int idx = -1) {
				return LuaTypeTraits<std::string>::getValue(L, idx).c_str();
			}

			static type popValue(lua_State* L, int idx = -1) {
				return LuaTypeTraits<std::string>::popValue(L, idx).c_str();
			}

			static void pushValue(lua_State* L, type val, int N = 1) {
				LuaTypeTraits<std::string>::pushValue(L, val, N);
			}

			static bool isA(lua_State* L, int idx = -1) {
				return LuaTypeTraits<std::string>::isA(L, idx);
			}
		};

		/* Possible future specialization that treats nullptr as the C++ equivalent of nil
		   This would also entail the elimination of the file "LuaTypes/LuaNil.h"
		typedef std::nullptr_t LuaNil;
		
		template<>
		struct LuaTypeTraits<LuaNil> {
		    using type = LuaNil;

		    static type getValue(lua_State* L, int idx = -1) {
		        return nullptr;
		    }

		    static type popValue(lua_State* L, int idx = -1) {
		        luaL_remove(L, idx);     // should I remove the value or should I check that it is nil first ???
		        return nullptr;
		    }

		    static void pushValue(lua_State* L, type val, int N = 1) {
		        for ( auto i = 0; i != N; ++i )
		            lua_pushnil(L);
		    }

		    static bool isA(lua_State* L, int idx = -1) {
		        return lua_isnil(L, idx);
		    }
		}
		*/
	}

}