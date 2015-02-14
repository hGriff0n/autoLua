

#include "impl/impl.h"
#include "lua.hpp"

//#include <algorithm>

// convert a negative lua index into the positive index that points to the same spot
int lua_realindex(lua_State* L, int idx) {
	// if ( idx < 0 && std::none_of(begin(),end(),[idx](int n) { return idx == n; }) )
	if ( idx < 0 && notEquals(idx, { LUA_GLOBALSINDEX }) )
		return lua_gettop(L) + idx + 1;
	return idx;
}
int lua_negindex(lua_State* L, int idx) {
	if ( idx > 0 )
		idx -= (lua_gettop(L) + 1);
	return idx;
}
