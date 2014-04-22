

#include "LuaState.h"
#include "FunctionWrapper.h"

#include "impl/Tuple.h"
#include "LuaTypes/LuaNil.h"
#include "Luatypes/LuaTable.h"

#include <iostream>

using namespace autoLua;
using namespace autoLua::impl;
using namespace std;

// possible to bind variadic functions ???

#define showLine(x) cout << x << endl
#define showPause(x) showLine(x); cin.get()


int main(int argc, const char* argv[]) {
	LuaState L;
	double x, y, z;

	// demonstrating current function wrapping syntax
	function<tuple<double, double, double>(double, double)> combiner = [] (double x, double y) -> tuple<double, double, double> {
		return make_tuple(x, y, x + y);
	};
	auto someCombo = makeWrapper(L, combiner);
	lua_setglobal(L, "combo");


	// luaTie(x,y,z) = L["combo"](5.3, 4.2);
	// top throws error C4716 'LuaTypeTraits<LuaTuple<double,double,double>>::popValue
	// must return a value', yet the bottom doesn't !!!???
	{ auto tmp = L["combo"](5.3, 4.2); luaTie(x, y, z) = tmp; }

	showLine(x);
	showLine(y);
	showLine(z);


	// demonstrating recent addition
	L["test"]["randVal"]["randKey"] = 5.5;
	luaL_remove(L, -2);	// needs to go into LuaVarHelper destructor

	showLine((double)L["test"]["randVal"]["randKey"]);

	// should go in destructor
	luaL_remove(L, -2);
	luaL_remove(L, -2);

	// cout << LuaStackPos<-1>::getValue(lua) << endl;

	cin.get();
}