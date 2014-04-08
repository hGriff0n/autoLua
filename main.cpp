

#include "LuaState.h"
#include "FunctionWrapper.h"

#include <iostream>

using namespace autoLua;
using namespace impl;
using namespace std;

// possible to bind variadic functions ???
// slightly hackish implementation of multiple return wrapper (uses _intTags struct, would prefer use of _Indices)

int main(int argc, const char* argv[]) {
	LuaState L;
	lua_State* lua = *L;
	
	// functions to test
	function<int(int,int,int)> summer = [](int x, int y, int z) { return x + y + z; };
	function<void(string, int)> printer = [] (string x, int y) {
		cout << "string param " << x << "\ninteger param " << y << endl;
	};
	function<tuple<double, double>(double)> splitter = [] (double x) -> tuple<int, int> {
		double y = 0;
		double z = 10.0 * modf(x, &y);
		return make_tuple(z, y);
	};
	function<int(void)> writer = [] () -> int {
		cout << "Writing from C++ in lua called from C++!\n";
		return 1;
	};
	function<int(lua_State*)> adder = [] (lua_State* L) -> int {
		int x = luaL_checkinteger(L, -1) , y = luaL_checkinteger(L, -2);
		lua_pushinteger(L, (x + y));
		return 1;
	};
	

	// test general wrapping (Passed)
	// test wrapping with void return (Passed)
	// test wrapping over lua_CFunction (Passed)
	// test wrapping with no arguments (Passed)
	// test wrapping with multiple return (Passed)

	auto someSplits = makeWrapper(lua, splitter);
	lua_setglobal(lua, "splits");
	
	//_print_tuple(std::make_tuple(5, "Hello", 3, 2));

	// test suite

	lua_getglobal(lua, "splits");

	lua_pushnumber(lua, 5.3);	// prints "5" & "2" oddly
								// prints "5" & "4" when I pass 5.4 though

	lua_pcall(lua, 1, 2, 0);
	
	cout << LuaTypeTraits<double>::getValue(lua, -1) << endl;
	cout << LuaTypeTraits<double>::getValue(lua, -2) << endl;

	cin.get();
}