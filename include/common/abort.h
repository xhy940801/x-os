#pragma once

#define abort() _abort(__FILE__, __LINE__, __func__)

void _abort(const char* file, unsigned int line, const char* func);