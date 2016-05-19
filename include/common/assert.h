#pragma once

#define assert(cond) ( (cond) ? (void) 0 : _static_assert_func(#cond, __FILE__, __LINE__, __func__) )

void _static_assert_func(const char* name, const char* file, unsigned int line, const char* func);
