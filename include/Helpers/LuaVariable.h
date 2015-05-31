

#pragma once

#include "LuaCaller.h"
#include "LuaStack.h"
#include "LuaStackGuard.h"
#include "impl\LuaRegistry.h"

#include <bitset>

#include <iostream>
#include "debugTesting.h"

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

	// How will I go about maintaining stack order

	class LuaVariable {
		lua_State* L;
		std::bitset<3> flags;
		impl::LuaRegistry* registry;
		//LuaStackGuard guard;
		/*
			all flags default to on (1)
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
			LuaVariable(lua_State* lua, T field, impl::LuaRegistry* records) : L(lua), flags(7), registry(records) {//, guard(lua) {
			//LuaVariable(lua_State* lua, T field, impl::LuaRegistry* records) : L(lua), flags(3), registry(records) {
				impl::LuaTypeTraits<T>::pushValue(L, field, 2);
				lua_gettable(L, LUA_GLOBALSINDEX);
			}

			~LuaVariable() { }


			// variable type functions
			std::string type();

			template <typename T>
			bool isA() {
				return impl::LuaTypeTraits<T>::isA(L);
			}

			template <typename T>
			bool isSameTypeAs(T& obj) {
				return isA<T>();
			}


			/* table index
			template <typename T>
			LuaVariable& operator[](T field) {
				checkTables();
				if (!flags[1])            // remove the reference to the last table
					luaL_remove(L, -2);

				impl::LuaTypeTraits<T>::pushValue(L, field, 2);
				getStackField(-3);        // get the element at the index (while keeping the index in case a set operation is desired)

				flags[1] = 0;
				return *this;
			} */

			template <typename T>
			LuaVariable& operator[](T field) {
				// if the item is a nil value or it is not a table and the error flag is not set
				if ( impl::LuaTypeTraits<LuaNil>::isA(L) || !( impl::LuaTypeTraits<LuaTable<T>>::isA(L) || flags[2] ) ) {
					lua_pop(L, 1);                                          // Remove the old value
					lua_pushvalue(L, -1);                                   // Push a copy of the key
					lua_newtable(L);                                        // Create a new table
					lua_settable(L, flags[1] ? LUA_GLOBALSINDEX : -4);      // Set the value
					lua_gettable(L, flags[1] ? LUA_GLOBALSINDEX : -2);      // Get the new value

				} else if ( !LuaTypeTraits<LuaTable<T>>::isA(L) )
					throw LuaTypeError("LuaVariable::operator[]", "Cannot index a non-table element");     // Change exception name ???

				else
					luaL_remove(L, -2);                                     // Remove the old key


				impl::LuaTypeTraits<T>::pushValue(L, field, 2);             // Push the keys on the stack

				// need to take flags[0] into account here (change to getStackField ???)
				lua_gettable(L, -3);                                        // Get the value from the table
				if (!flags[1]) luaL_remove(L, -4);                          // Remove the old table

				flags[1] = false;
				return *this;
			}
			
			// set the variable's value
			template <typename T>
			LuaVariable& operator=(T val) {
				lua_pop(L, 1);

				impl::LuaTypeTraits<T>::pushValue(L, val);
				setStackField(-3);

				// leaves the table from which the variable was taken on the stack
				// if the return type is void
				    // lua_pop(L, 1);

				return *this;
			}

			LuaVariable& operator=(LuaVariable& other) {
				lua_pop(L, 1);                          // Remove the old value
				lua_pushvalue(L, -1);                   // Push a copy of the key
				lua_pushvalue(L, -4 + flags[1]);        // Push a copy of the new value
				setStackField(-4);                      // Set the value

				return *this;
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


			// invoke the function (if the variable is a function)
			template <typename... Args>
			LuaStack operator()(Args&&... args) {
				/* auto delta = */
				return LuaCaller(L)(std::forward<Args>(args)...);
			}

			
			// get the variable's value
			template <typename T>
			operator T() {
				return (T)LuaStack(L);
			}


	};

}