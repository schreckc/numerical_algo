#ifndef ERROR_HPP
#define ERROR_HPP

// #include <stdio.h>
// #include <stdlib.h>
// #include <stdarg.h>
#include <assert.h>
// #include <stdint.h>
// #include <stdbool.h>
#include <iostream>
#include "ExceptionSimu.hpp"

//****************************************************

#ifndef __MODE_DEBUG  
#define __MODE_DEBUG 3
#endif

#if __MODE_DEBUG > 0

#include <assert.h>

void close();

#define ERROR(cond, msg, debug_info) if (!(cond)) {std::cout<<"ERROR: "<<msg<<"\n"<<debug_info<<std::endl;assert(cond);}

#define WARNING(cond, msg, debug_info) if (!(cond)) {std::cout<<"WARNING: "<<msg<<"\n"<<debug_info<<std::endl;}

//#define INFO(debug_info) if (__MODE_DEBUG == 3) {std::cout<<"INFO: \n"<<debug_info<<std::endl;}
#define INFO(level, debug_info) if (__MODE_DEBUG >= level) {std::cout<<"INFO: \n"<<debug_info<<std::endl;}

#define TEST(cond) if (!cond) {close(); assert(cond);}

#define IS_DEF(nombre) assert(!std::isnan(nombre) && !std::isinf(nombre))

//*********************

#else

#define ERROR(cond, msg, debug_info)do {if (!(cond)) {std::cerr<<"\n***ERREUR*** : \n"<<msg<<"\n"<<std::endl; throw exception_paper();}}while(0)

#define WARNING(cond, msg, debug_info) do {if (!(cond)) {std::cerr<<"\n****WARNING*** : \n"<<msg<<"\n"<<std::endl;}while(0)

//#define INFO(debug_info)
#define INFO(level, debug_info)

#define TEST(cond)

#define IS_DEF(nombre)


#endif

//****************************************************


#endif
