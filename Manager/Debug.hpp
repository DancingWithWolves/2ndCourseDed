#pragma once
#include <cstdio>



#define DEBUG



const char divider_str_in[] = "\n===========================================================================================\\\n";
const char divider_str_out[] = "===========================================================================================/\n\n";
FILE* LOG = nullptr;

#define MSG_TO_LOG(format, args...) \
    LOG = fopen(log_name, "at");    \
    fprintf(LOG, "%s", divider_str_in);   \
    fprintf(LOG, format, ##args);   \
    fprintf(LOG, "%s", divider_str_out);   \
    fclose(LOG);

#define meow __FILE__, __PRETTY_FUNCTION__, __LINE__

#ifdef DEBUG

#define soft_assert(cond) assert(cond);
#define ON_DEBUG(param) param

#else

#define soft_assert(cond)\
if(!(cond)) MSG_TO_LOG("Assert %s failed! File %s, function %s, line %d!\n", #cond, __FILE__, __PRETTY_FUNCTION__, __LINE__);
#define ON_DEBUG(param) ;

#endif

