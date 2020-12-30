#pragma once
#ifndef MYASSERT_H
#define MYASSERT_H

#define assert(cond,msg)\
    if(!(cond))syntaxError(ctx,msg)

#endif