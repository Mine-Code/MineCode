#pragma once
#ifndef ASM_H
#define ASM_H

#include <sstream>

namespace assembly
{
	class Assembly
	{
	public:
		void startOfFunction(std::stringstream& st);
		void endOfFunction(std::stringstream& st);
		void callFunction(std::stringstream& st,int address);
	};

} // namespace assembly


#endif