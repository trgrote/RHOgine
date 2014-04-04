#ifndef FIRST_COME_SINGLETON_H
#define FIRST_COME_SINGLETON_H

#include "defines.h"

#include <typeinfo>

namespace rho
{

/// \brief First Come, First Serve Singleton
///	General Premise is that a Singleton of this type exists in a traditional scope. i.e. in the Main Game Class.
/// And will leave scope or be deleted within the same scope/context it was created. i.e. in a the Destructor of a class.
/// \b
/// Once a single instance of this class if made, any attempts to instantiate another will assert fail.
/// Any attempts to access this singleton before a instance has been made will also cause an assert fail.
template <typename T>
class FirstComeSingleton
{
private:
    /// Hide Copy and assignment constructors
    FirstComeSingleton(const FirstComeSingleton&){}

	/// Hide Copy and assignment constructors
	FirstComeSingleton& operator=(const FirstComeSingleton&){}

protected:
    static T * s_pInstance;

    FirstComeSingleton()
    {
		xassert_msg ( s_pInstance == NULL, "%s Singleton: Attempted to Create Multiple Singletons of the same time", typeid(s_pInstance).name() );
        s_pInstance = static_cast<T*>( this );
    }

    virtual ~FirstComeSingleton()
    {
        s_pInstance = 0;
    }

public:
	/// \brief Get the only instance of this object
    static T & GetInstance();
};

template< typename T>
T & FirstComeSingleton<T>::GetInstance()
{
    xassert_msg( s_pInstance != NULL , "%s Singleton: Attempted to get NULL Singleton", typeid(s_pInstance).name() );
    return *FirstComeSingleton<T>::s_pInstance;
}

}

/// Shortcut for making a static instance member
#define FIRST_COME_SINGLETON_INSTANCE(T) template<> T * FirstComeSingleton<T>::s_pInstance = 0

// class Test : public FirstComeSingleton< Test >
// {
// public:
    // int value1;
    // float value2;
// };

// FIRST_COME_SINGLETON_INSTANCE( Test );

#endif
