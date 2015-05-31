#pragma once

#include "LuaState.h"
#include "FunctionWrapper.h"

#include "LuaTypes\LuaTuple.h"
#include "LuaTypes\LuaNil.h"
#include "LuaTypes\LuaTable.h"


// Could they work with only a LuaStack
template <typename T>
T get(autoLua::LuaState& L, int N = -1) {
	if (!validIndex(L, N))
		throw autoLua::LuaException("get<T>", "N is an invalid index");

	if (autoLua::impl::LuaTypeTraits<T>::isA(L, N))
		return autoLua::impl::LuaTypeTraits<T>::popValue(L, N);

	throw autoLua::LuaTypeError("get<T>", "Nth element is not of type T");
}

//getFirst
template <typename T>
T fst(autoLua::LuaState& L) {
	for (auto idx = 0; idx > -lua_gettop(L); --idx)
		if (autoLua::impl::LuaTypeTraits<T>::isA(L, idx - 1))
			return autoLua::impl::LuaTypeTraits<T>::popValue(L, idx - 1);

	throw autoLua::LuaTypeError("getFirst<T>", "No elements of type T on the stack");
}

//getLast
template <typename T>
T lst(autoLua::LuaState& L) {
	for (auto idx = -lua_gettop(L); idx < 0; ++idx)
		if (autoLua::impl::LuaTypeTraits<T>::isA(L, idx))
			return autoLua::impl::LuaTypeTraits<T>::popValue(L, idx);

	throw autoLua::LuaTypeError("getLast<T>", "No elements of type T on the stack");
}

template <typename T>
T after(autoLua::LuaState& L, int N = 1) {
	N = lua_realindex(L, N + 1);

	if ( !validIndex(L, N) )
		throw autoLua::LuaStackError("after<T>", "N is an invalid index");

	for ( ; N < lua_gettop(L); ++N )
		if ( autoLua::impl::LuaTypeTraits<T>::isA(L, N) )
			return autoLua::impl::LuaTypeTraits<T>::popValue(L, N);

	throw autoLua::LuaTypeError("after<T>", "No elements of type T after index N");
}

template <typename T>
T before(autoLua::LuaState& L, int N = -1) {
	N = lua_realindex(L, N - 1);

	if ( !validIndex(L, N) )
		throw autoLua::LuaStackError("before<T>", "N is an invalid index");

	for ( ; N > 0; --N )
		if ( autoLua::impl::LuaTypeTraits<T>::isA(L, N) )
			return autoLua::impl::LuaTypeTraits<T>::popValue(L, N);

	throw autoLua::LuaTypeError("before<T>", "No elements of type T before index N");
}


template <typename T>
std::string toString(autoLua::LuaState& L, T val) {
	impl::LuaTypeTraits<T>::pushValue(L, val);
	return get<std::string>(L);
}


int abs(int n) {
	int mask = n >> (sizeof(int) - 1);
	return (mask ^ n) - mask;
}

bool validIndex(autoLua::LuaState& L, int N) {
	return abs(N) <= lua_gettop(L) && N != 0;
}
