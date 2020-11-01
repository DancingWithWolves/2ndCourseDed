#pragma once
#include <cstdio>

#define soft_assert(cond)\
if(!(cond)) printf("Assert %s failed! File %s, function %s, line %d!\n", #cond, __FILE__, __PRETTY_FUNCTION__, __LINE__);
