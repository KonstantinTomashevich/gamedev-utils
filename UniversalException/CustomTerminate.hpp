#pragma once
#include <iostream>
#include "UniversalException.hpp"

void CustomTerminate ()
{
    try
    {
        std::rethrow_exception (std::current_exception ());
    }

    catch (AnyUniversalException &exception)
    {
        std::cerr << exception.GetException () << std::endl;
    }
    abort ();
}
