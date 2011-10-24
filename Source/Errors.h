/*****************************************************
*                                                    *
*                     Errors.h                       *
*                                                    *
*   A simple function to display a an error message  *
*   incase of any problems.                          *
*                                                    *
*   FUNCTIONS:                                       *
*                                                    *
*       - handleError(errorMessage, quit = true)     *
*           If using Windows, this will create a     *
*           window with the errorMessage C-string    *
*           displayed to the user. If using any      *
*           other OS, the error message will be      *
*           displayed on stderr.                     *
*           Upon pressing okay (Windows) the program *
*           will quit if "quit" is set to true,      *
*           otherwise it'll continue. On another OS, *
*           it will wait for the user to press ENTER *
*           to either quit or continue.              *
*                                                    *
*****************************************************/
#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif // __cplusplus

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#ifdef __cplusplus
#include <cstdio>
#else 
#include <stdio.h>
#endif // __cplusplus
#endif // _WIN32

void handleError(const char* errorMessage, const bool quit = true);

#endif // ERROR_HANDLER_H