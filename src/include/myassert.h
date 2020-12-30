#pragma once
#ifndef MYASSERT_H
#define MYASSERT_H

#include <string>

#define assert(cond,msg)\
    if(!(cond))assert_fail(__FILE__,__func__,__LINE__,msg)

void assert_fail(char*,char*,int,std::string);

#endif