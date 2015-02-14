

#pragma once

#include "LuaCaller.h"
#include "LuaStack.h"
#include "LuaStackGuard.h"
#include "impl\LuaRegistry.h"

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

	class LuaVariable {
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
			void setStackField(int);
			void getStackField(int);
			void setToValue();
			void checkTables();

		public:
			template <typename T>
			LuaVariable(lua_State* lua, T field, impl::LuaRegistry* records) : L(lua), flags(7), registry(records) {
				impl::LuaTypeTraits<T>::pushValue(L, field, 2);
				lua_gettable(L, LUA_GLOBALSINDEX);
			}
			~LuaVariable() { }

			std::string type();
			template <typename T>
			bool isA() {
				return impl::LuaTypeTraits<T>::isA(L);
			}
			template <typename T>
			bool isSameTypeAs(T& obj) {
				return isA<T>();
			}

			template <typename T>
			LuaVariable& operator[](T field) {
				checkTables();
				if ( !flags[1] )
					luaL_remove(L, -2);

				impl::LuaTypeTraits<T>::pushValue(L, field, 2);
				getStackField(-3);

				flags[1] = 0;
				return *this;
			}

			template <typename T>
			void operator=(T val) {
				impl::LuaTypeTraits<T>::pushValue(L, val);
				setToValue();
			}
			void operator=(LuaVariable other) {
				lua_pushvalue(L, -4 + flags[1]);
				setToValue();
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