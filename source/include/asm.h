#pragma once
#ifndef ASM_H
#define ASM_H

namespace assembly
{
	class Assembly
	{
	public:
		std::stringstream startOfFunction();
		std::stringstream endOfFunction();
		std::stringstream callFunction(int address);
	};

} // namespace assembly


#endif