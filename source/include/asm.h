#pragma once
#ifndef ASM_H
#define ASM_H

#include <sstream>

namespace assembly
{
	class Assembly
	{
	public:
		void startOfFunction(std::wstringstream& st);
		void endOfFunction(std::wstringstream& st);
		void callFunction(std::wstringstream& st,int address);
	};

} // namespace assembly


#endif