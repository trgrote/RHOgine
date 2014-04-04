#include "defines.h"

static std::vector<const char *> *_error_context_name = NULL;
static std::vector<const char *> *_error_context_data = NULL;

///////////////////////////////////////////////////////////////////////////////

void printStack()
{
    for ( unsigned int i = 0 ;
        i < _error_context_name->size() ;
        ++i )
    {
        char const * name = _error_context_name->at( i );
        char const * data = _error_context_data->at( i );

        fprintf ( stderr,
            "%s %s\n",
            name,
            ( data ? data : "" ) );
    }
}

///////////////////////////////////////////////////////////////////////////////

void printStack(FILE * file)
{
    for ( unsigned int i = 0 ;
        i < _error_context_name->size() ;
        ++i )
    {
        char const * name = _error_context_name->at( i );
        char const * data = _error_context_data->at( i );

        fprintf ( 
			file,
            "%s %s\n",
            name,
            ( data ? data : "" ) );
    }
}

///////////////////////////////////////////////////////////////////////////////

ErrorContext::ErrorContext(const char *name, const char *data )
{
    if ( _error_context_name && _error_context_data )
    {
        _error_context_name->push_back(name);
        _error_context_data->push_back(data);
    }
}

///////////////////////////////////////////////////////////////////////////////

ErrorContext::~ErrorContext()
{
    if ( _error_context_name && _error_context_data )
    {
        _error_context_name->pop_back();
        _error_context_data->pop_back();
    }
}

///////////////////////////////////////////////////////////////////////////////

void ErrorContext::init()
{
    _error_context_name = new std::vector<const char *>;
    _error_context_data = new std::vector<const char *>;
}

///////////////////////////////////////////////////////////////////////////////

void ErrorContext::shutdown()
{
    delete _error_context_data;
	delete _error_context_name;
}

///////////////////////////////////////////////////////////////////////////////
