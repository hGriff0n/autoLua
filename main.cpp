

#include "LuaState.h"
#include "FunctionWrapper.h"
#include "LuaTuple.h"

#include <iostream>

using namespace autoLua;
using namespace std;

// possible to bind variadic functions ???

int main(int argc, const char* argv[]) {
	LuaState L;
	
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

	auto someSplits = makeWrapper(L, splitter);
	lua_setglobal(L, "splits");

	// test suite

	double x, y;
	auto tmp = L["splits"](5.3);
	luaTie(x,y) = tmp;
	
	cout << x << endl;
	cout << y << endl;

	cin.get();
}