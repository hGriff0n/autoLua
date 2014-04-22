

#pragma once

#include "LuaCaller.h"
#include "FunctionWrapper.h"

#include "impl/LuaVarImpl.h"	// will be phased out later

#include <bitset>

namespace autoLua {

	class LuaState;
	/*
	usage:
		value
		key
		table

	operator[]:
		key
		key
		table
	
	*/

	// extract the `impl` functions inline
	class LuaVarHelper {
		private:
			lua_State* L;
			std::bitset<3> flags;
			/*
			all flags default to on
			create enum for indexes ???
			[0] = use lua_gettable/settable over lua_rawget/set for lookup
			[1] = curent variable is in global
			[2] = throw exception on table creation in op[]
			*/

			//friend class LuaState;
		protected:
			void setStackField(int alt_index) {
				auto setter = flags[0] ? &lua_settable : &lua_rawset;
				setter(L, flags[1] ? LUA_GLOBALSINDEX : alt_index);
			}
			void getStackField(int index) {
				auto getter = flags[0] ? &lua_gettable : &lua_rawget;
				getter(L, index);
			}
			void setToValue() {
				luaL_remove(L, -2);
				setStackField(-3);
			}
			void checkTables() {
				if ( lua_isnil(L, -1) ) {
					auto loc = lua_realindex(L, flags[1] ? LUA_GLOBALSINDEX : -3);
					lua_remove(L, lua_gettop(L));
					lua_pushvalue(L, -1);
					lua_pushvalue(L, -1);
					lua_newtable(L);
					lua_settable(L, loc);
					lua_gettable(L, loc);
				} else if ( !lua_istable(L, -1) && flags[2] )
					throw("Error");

				luaL_remove(L, -2);
			}

		public:
		template <typename T>
			LuaVarHelper(lua_State* lua, T field) : L(lua), flags(6) {
				LuaTypeTraits<T>::pushValue(L, field, 2);
				lua_gettable(L, LUA_GLOBALSINDEX);
			}
			~LuaVarHelper() {
				L = nullptr;
			}

			std::string type() {
				return lua_typename(L, lua_type(L, -1));
			}
			template <typename T>
			bool isA() {
				return impl::LuaTypeTraits<T>::isA(L);
			}
			template <typename T>
			bool isSameTypeAs(T& obj) {
				return isA<T>();
			}

			template <typename T>
			LuaVarHelper& operator[](T field) {
				checkTables();
				if ( !flags[1] )
					luaL_remove(L, -2);

				LuaTypeTraits<T>::pushValue(L, field, 2);
				getStackField(-3);

				flags[1] = 0;
				return *this;
			}
			template <int N>
			LuaVarHelper& operator[](const char(&name)[N]) {
				return (*this)[std::string(name)];
			};

			template <typename T>
			void operator=(T val) {
				LuaTypeTraits<T>::pushValue(L, val);
				setToValue();
			}
			void operator=(LuaVarHelper& other) {

			}
			template <typename Ret, typename... Args>
			void operator=(std::function<Ret(Args...)>& func) {

			}
			template <typename... Ret, typename... Args>
			void operator=(std::function<std::tuple<Ret...>(Args...)>& func) {

			}

			template <typename... Args>
			LuaConverter operator()(Args&&... args) {
				if ( !LuaTypeTraits<LuaFunction>::isA(L) ) throw;
				return LuaCaller(L)(std::forward<Args>(args)...);
			}

			template <typename T>
			operator T() {
				return (T)LuaConverter(L);
			}


	};

}