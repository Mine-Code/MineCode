#pragma once
#ifndef MYASSERT_H
#define MYASSERT_H

#include <string>

#define assert(cond,msg)\
    if(!(cond))syntaxError(ctx,msg)

#endif