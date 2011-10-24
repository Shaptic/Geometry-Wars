#include "Errors.h"

/* Sometimes, errors happen. When they do, we must be ready.
 * A cross-platform (sort-of) approach is below. When an 
 * error arises, call handleError() with your error message
 * as an argument, and receive the follwing:
 * Windows: A message box with your error and an "OK" box.
 * UNIX:    A message written to the console, because there
 *              are too many window systems in *nix.
 * If the "quit" argument is set to "true" (true by default),
 * the program will exit, otherwise it will continue as normal.
 */
void handleError(const char* errorMessage, const bool quit)
{
#ifdef _WIN32
    MessageBoxA(NULL, errorMessage, "Error!", MB_ICONEXCLAMATION | MB_OK | MB_ICONERROR);
#else
    if(quit)
        fprintf(stderr, "[-] FATAL! %s", errorMessage);
        fprintf(stderr, "Press ENTER to exit...");
    else
        fprintf(stderr, "[-] A non fatal error occured: %s", errorMessage);
        fprintf(stderr, "Press ENTER to continue...");
    gets();
#endif // _WIN32

    if(quit)
        exit(1);
}
