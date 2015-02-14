

#pragma once

//#include "impl\function_impl.h"
#include "impl\LuaRegistry.h"

namespace autoLua {

	/*
	template <typename... T>
	class LuaBasicTable {
		private:
			std::tuple<T...> values;

		public:
			...
	}

	class LuaMetatable {
		private:
			LuaBasicTable<> methods;

		public:
			...
	}

	template <typename... T>
	class LuaCustomTable : public LuaBasicTable<T...> {
		private:
			LuaMetatable meta;

		public:
			...
	}
	*/

	// Rework LuaTable to not be templated (to allow for adding items later on) ???

	// registerFunction(registry,std::forward<WrapperPtr>(makeWrapper(L, func)));
	/*
	namespace impl {
		class LuaRegistry;
	}
	*/

	// Find way to merge with LuaTable ???
	// builds table on stack (current execution)
	class LuaMetatable {
		private:
			impl::LuaRegistry* reg;

		public:
			LuaMetatable();
			LuaMetatable(impl::LuaRegistry*);

			template <typename Ret, typename... Args>
			void registerMethod(std::string name, std::function<Ret(Args...)>& func) {
				lua_State* L = getRegisterState(reg);
				impl::LuaTypeTraits<std::string>::pushValue(L, name);
				registerFunction(reg, std::forward<WrapperPtr>(makeWrapper(L, func)));
				lua_settable(L, -3);
			}

			template <typename... Ret, typename... Args>
			void registerMethod(std::string name, std::function<std::tuple<Ret...>(Args...)>& func) {
				lua_State* L = getRegisterState(reg);
				impl::LuaTypeTraits<std::string>::pushValue(L, name);
				registerFunction(reg, std::forward<WrapperPtr>(makeWrapper(L, func)));
				lua_settable(L, -3);
			}

			template <typename T>
			void registerValue(std::string name, T val) {
				lua_State* L = getRegisterState(reg);
				impl::LuaTypeTraits<std::string>::pushValue(L, name);
				impl::LuaTypeTraits<T>::pushValue(L, val);
				lua_settable(L, -3);
			}

			void registerCode(std::string, std::string);
			void setTable(lua_State*);

			// "standard" custom predefs
			enum {
				read_only,
				hidden,
				constructor
			};
	};

	LuaMetatable makeMetatable(impl::LuaRegistry*);
	LuaMetatable makeMetatable(impl::LuaRegistry*, std::string);
	LuaMetatable makeMetatable(impl::LuaRegistry*, std::initializer_list<int>);
	LuaMetatable makeMetatable(impl::LuaRegistry*, std::string, std::initializer_list<int>);
 
	template <typename... T>
	class LuaTable {
		private:
			std::tuple<T...> values;
			LuaMetatable meta;
	
		public:
			LuaTable(T... ts) : values(std::make_tuple(ts...)), meta() { }
			LuaTable(std::tuple<T...> _vals) : values(_vals), meta() { }
			LuaTable(LuaMetatable _m, T... ts) : values(std::make_tuple(ts...)), meta(_m) { }
			LuaTable(LuaMetatable _m, std::tuple<T...> _vals) : values(_vals), meta(_m) { }

			static const int num_pairs = sizeof...(T) / 2;

			void fillTable(lua_State* L) {
				auto table_pos = lua_gettop(L);
				impl::_push(L, values);
				for ( auto i = 0; i != num_pairs; ++i )
					lua_settable(L, table_pos);
				meta.setTable(L);
			}

			static LuaTable<T...> grabTable(lua_State* L, int idx) {
				// get metatable
				return _getLuaTable<T...>(L, idx);
			}
	};
	
	// override for templating on void (should I override LuaTypeTraits to???)
	template <>
	class LuaTable<void> {
		private:
			LuaMetatable meta;

		public:
			LuaTable() : meta() { }
			LuaTable(LuaMetatable _m) : meta(_m) { }

			static const int num_pairs = 0;

			void fillTable(lua_State* L) { }

			static LuaTable<void> grabTable(lua_State* L, int idx) {
				if ( !impl::LuaTypeTraits<LuaTable<void>>::isA(L, idx) ) throw;
				return LuaTable<void>();
			}
	};
	
	template <typename... T>
	LuaTable<T...> makeLuaTable(T... ts) {
		return LuaTable<T...>(ts...);
	}

	namespace impl {

		template <typename... T>
		std::tuple<T...> _getLuaTable(lua_State* L, int idx) {
			idx = lua_realindex(L, idx);
			while ( lua_next(L, idx) ) {
				lua_pushvalue(L, -2);
			}
			// currently slightly wrong (now: value, key -> expected: key, value) quick future fix
			return _getArgs<T...>(L);
		}
		
		template <typename... T>
		struct LuaTypeTraits<LuaTable<T...>> {
			using type = LuaTable<T...>;

			static type getValue(lua_State* L, int idx = -1) {
				return type::grabTable(L, idx);
			}
			static type popValue(lua_State* L, int idx = -1) {
				auto x = LuaTypeTraits<type>::getValue(L, idx);
				luaL_remove(L, idx);
				return x;
			}
			static void pushValue(lua_State* L, type& val, int N = 1) {
				for ( auto i = 0; i != N; ++i ) {
					lua_createtable(L, 0, 0);
					val.fillTable(L);
				}
			}
			static bool isA(lua_State* L, int idx = -1) {
				return lua_istable(L, idx);
			}
			//static void setValue(lua_State* L, int lookup, type& obj, bool use_metamethods = true) { }
		};
		
	}

}