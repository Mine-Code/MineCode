#pragma once
#ifndef MYASSERT_H
#define MYASSERT_H

#include <string>

#define assert(cond,msg)\
void assert_fail(char*,char*,int,std::string);

#endif