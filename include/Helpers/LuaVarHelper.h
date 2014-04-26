

#pragma once

#include "LuaCaller.h"
#include "LuaStack.h"
#include "LuaStackGuard.h"
#include "impl/LuaRegistry.h"

#include <bitset>

namespace autoLua {

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

	// Helper class that interacts with the lua variable heiarchy

	class LuaVarHelper {
		private:
			lua_State* L;
			std::bitset<3> flags;
			impl::LuaRegistry* registry;
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
			LuaVarHelper(lua_State* lua, T field, impl::LuaRegistry* records) : L(lua), flags(7), registry(records) {
				impl::LuaTypeTraits<T>::pushValue(L, field, 2);
				lua_gettable(L, LUA_GLOBALSINDEX);
			}
			~LuaVarHelper() { }

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

				impl::LuaTypeTraits<T>::pushValue(L, field, 2);
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
				impl::LuaTypeTraits<T>::pushValue(L, val);
				setToValue();
			}
			void operator=(LuaVarHelper& other) {

			}
			template <typename Ret, typename... Args>
			void operator=(std::function<Ret(Args...)>& func) {
				registerFunction(registry, std::forward<WrapperPtr>(makeWrapper(L, func)));
				setToValue();
			}
			template <typename... Ret, typename... Args>
			void operator=(std::function<std::tuple<Ret...>(Args...)>& func) {
				registerFunction(registry, std::forward<WrapperPtr>(makeWrapper(L, func)));
				setToValue();
			}

			template <typename... Args>
			LuaStack operator()(Args&&... args) {
				/* auto delta = */
				LuaCaller(L).call(std::forward<Args>(args)...);
				return L;
			}

			template <typename T>
			operator T() {
				return (T)LuaStack(L);
			}


	};

}