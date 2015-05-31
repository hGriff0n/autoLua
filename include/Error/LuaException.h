#pragma once

#include <string>

namespace autoLua {

	class LuaException {
		public:
		using msgType = std::string;

		private:
		    msgType loc, msg;

		public:
		    LuaException(msgType nLoc, msgType nMsg) : loc(nLoc), msg(nMsg) {}

		    virtual msgType error() {
			    return "Error in function " + loc + " -> " + msg;
		    }
	};

	class LuaTypeError : public LuaException {
		public:
		    LuaTypeError(msgType nLoc, msgType nMsg) : LuaException(nLoc, nMsg) {}
	};

	class LuaStackError : public LuaException {
		public:
		    LuaStackError(msgType nLoc, msgType nMsg) : LuaException(nLoc, nMsg) {}
	};

	class LuaPanicError : public LuaException {
		public:
		    LuaPanicError(msgType nLoc, msgType nMsg) : LuaException(nLoc, nMsg) {}
	};

	class LuaUndefinedBehavior : public LuaException {
		public:
		    LuaUndefinedBehavior(msgType nLoc) : LuaException(nLoc, "Operation is undefined") {}
	};

}