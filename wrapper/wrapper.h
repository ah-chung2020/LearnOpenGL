#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <assert.h>

#ifdef DEBUG
#define GL_CALL(func) func;check_error();
#else
#define GL_CALL(func) func;
#endif

void check_error();