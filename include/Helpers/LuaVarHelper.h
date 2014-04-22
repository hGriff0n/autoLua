

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
			bool useGlobals, throwIfVal;
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
				void(*setter)(lua_State*, int) = (flags[0] ? &lua_settable : &lua_rawset);
				setter(L, (flags[1] ? LUA_GLOBALSINDEX : alt_index));
			}
			void getStackField(int alt_index) {
				void(*getter)(lua_State*, int) = (flags[0] ? &lua_gettable : &lua_rawget);
				getter(L, (flags[1] ? LUA_GLOBALSINDEX : alt_index));
			}
			void checkTables() {
				if ( lua_istable(L, -1) )
					return;
				else if ( lua_isnil(L, -1) ) {
					//stackDebug(L);

					lua_remove(L, lua_gettop(L));
					lua_pushvalue(L, -1);
					lua_pushvalue(L, -1);
					lua_newtable(L);
					setStackField(-5);
					getStackField(-3);
					return;
				} else
					throw("No implicit tables");
			}

		public:
		template <typename T>
			LuaVarHelper(lua_State* lua, T field) : L(lua), flags(7), useGlobals(true), throwIfVal(false) {
				impl::indexOp(field, L);
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
				impl::indexOp(field, L, useGlobals, throwIfVal);
				return *this;
			}
			template <int N>
			LuaVarHelper& operator[](const char(&name)[N]) {
				return (*this)[std::string(name)];
			};

			template <typename T>
			void operator=(T val) {
				impl::setValue(L, val, useGlobals);
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