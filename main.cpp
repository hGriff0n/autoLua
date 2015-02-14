

#include "LuaState.h"
#include "FunctionWrapper.h"

#include "LuaTypes\Tuple.h"
#include "LuaTypes\LuaNil.h"
#include "LuaTypes\LuaTable.h"

#include "Helpers\LuaStackGuard.h"

#include "debugTesting.h"

#include <iostream>

using namespace autoLua;
using namespace autoLua::impl;
using namespace std;


// TODO:
// Improve LuaTable
// Add Exceptions and Error Handling
// C++/lua object binding/wrapping/etc.
// Start filling in github readme with information about the interface and project
// implement LuaStackGuard within the various Lua_ classes
// LuaThreadState class
// Correct autoLua::impl::_getLuaTable implementation as mentioned in the function
// Reduce number of copies made with LuaVariable
// Use std pointers instead of raw ones in Lua_ classes (I'm already including <memory>)

// possible to bind variadic functions ???
// possible to bind lambdas (templated too) ???
// Completely merge working files with the github files (general commit)
// mark issue in github for assigning LuaStack to a tuple (fixed???)

#define showLine(x) cout << x << endl
#define showPause(x) showLine(x); cin.get()


int main(int argc, const char* argv[]) {
	LuaState L;
	double x, y, z;
	function<tuple<double, double, double>(double, double)> combiner = [] (double x, double y) -> tuple<double, double, double> {
		return make_tuple(x, y, x + y);
	};

	{
		LuaStackGuard guard(L);

		L["tmp"]["combo"] = combiner;
		L["tmp"]["t"] = 5;
		L["foo"] = L["tmp"]["t"];
		showLine((int)L["foo"]);
	}

	{
		LuaStackGuard guard(L);

		luaTie(x, y, z) = L["tmp"]["combo"](5.3, 4.2);    // The error has mysteriously disappeard (C4716)
		// if it does reappear: auto tmp = L["tmp"]["combo"](5.3, 4.2); luaTie(x, y, z) = tmp;
	}

	showLine(x);
	showLine(y);
	showLine(z);
	showLine("");

	{
		LuaStackGuard guard(L);

		*L << "Hello" << makeLuaTable("Goodbye", 5, 3, 4);
		makeMetatable(L);

		debug::debugStack(L, cout);
	}
	debug::debugStack(L, cout);
	showLine("The stack has been cleared!!!");

	// cout << get<-1>(L) << endl;

	cin.get();
}