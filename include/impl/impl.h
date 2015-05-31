

#pragma once

#include <initializer_list>
// do I even need this file (aside from the lua definitions) ???

struct lua_State;

// functions for mass comparison of values
template <typename T>
bool equalsOne(T obj, std::initializer_list<T> comp_list) {
	for ( auto x : comp_list )
		if ( obj == x ) return true;
	return false;
}

template <typename T>
bool notEquals(T obj, std::initializer_list<T> comp_list) {
	return !equalsOne(obj, comp_list);
}

// convert a negative lua index into the positive index that points to the same spot
int lua_realindex(lua_State*, int);
int lua_negindex(lua_State*, int);

#define luaL_remove(L, idx) lua_remove(L, lua_realindex(L, idx))
