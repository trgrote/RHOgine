#ifndef DEFINES_H
#define DEFINES_H

#include <cassert>
#include <vector>

#include <cstdio>
#include <cstdarg>
#include <cstdlib>

#if defined(DEBUG)

#define assert_msg( cond, msg ) do { if ( !(cond) ) {\
    printStack();\
    fprintf( stderr, "Assertion failed: %s\n", #cond );\
    fprintf( stderr, "\t" msg );\
    fprintf( stderr, "\n\tIn %s:%d\n", __FILE__, __LINE__ );\
	FILE * pFile = fopen ("assert.txt","w");\
	printStack(pFile);\
	fprintf( pFile, "Assertion failed: %s\n", #cond );\
    fprintf( pFile, "\t" msg );\
    fprintf( pFile, "\n\tIn %s:%d\n", __FILE__, __LINE__ );\
	fclose( pFile );\
    system("PAUSE");\
    exit(EXIT_FAILURE);\
    }\
    } while(0)

#define xassert_msg( cond, msg, ... ) do { if ( !(cond) ) {\
    printStack();\
    fprintf( stderr, "Assertion failed: %s\n", #cond );\
    fprintf( stderr, "\t" msg, __VA_ARGS__ );\
    fprintf( stderr, "\n\tIn %s:%d\n", __FILE__, __LINE__ );\
	FILE * pFile = fopen ("assert.txt","w");\
	printStack(pFile);\
	fprintf( pFile, "Assertion failed: %s\n", #cond );\
    fprintf( pFile, "\t" msg, __VA_ARGS__  );\
    fprintf( pFile, "\n\tIn %s:%d\n", __FILE__, __LINE__ );\
	fclose( pFile );\
    system("PAUSE");\
    exit(EXIT_FAILURE);\
    }\
    } while(0)
#else
    #define assert_msg( cond, msg )         (cond)
    #define xassert_msg( cond, msg, ... )   (cond)
#endif

void printStack();
void printStack(FILE * file);

class ErrorContext
{
public:
    ErrorContext(const char *name, const char *data = 0);

    ~ErrorContext();

    static void init();
    static void shutdown();
};

#endif
