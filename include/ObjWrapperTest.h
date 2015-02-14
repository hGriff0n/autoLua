

#pragma once

#include <iostream>
#include <string>
#include "impl\LuaTypeTraits.h"
#include "Helpers\LuaBinder.h"

class Log {
	private:
		int _level;

	public:
		Log(int level) : _level(level) { }

		void setLevel(int level) {
			_level = level;
		}

		int getLevel() const {
			return _level;
		}

		void log(int level, const std::string& format) {
			if ( _level & level )
				std::cout << format << std::endl;
		}

};

/*
l = Log(3)
print(l.level)
l.level = 4
l.log(2,"%d")

l2 = AltLog(3)
print(l.getLevel())
l.setLevel(4)
l.log(2,"%d")
*/

namespace autoLua {



}
