#include <iostream>
#include <SharedPointer/SharedPointer.hpp>
#include <SharedPointer/ReferenceCounted.hpp>

bool TestFunction (SharedPointer <ReferenceCounted> ptr)
{
    std::cout << "While function call: " << ptr->GetReferencesCount () << std::endl;
    return ptr->GetReferencesCount () == 2;
}

int main ()
{
    ReferenceCounted *object = new ReferenceCounted ();
    SharedPointer <ReferenceCounted> pointer;
    pointer.Set (object);

    std::cout << "After first pointer creation: " << pointer->GetReferencesCount () << std::endl;
    if (pointer->GetReferencesCount () != 1)
    {
        std::cout << "Incorrect refs count after creation!" << std::endl;
        return 1;
    }

    {
        SharedPointer <ReferenceCounted> secondPointer (pointer);
        std::cout << "After second pointer creation: " << secondPointer->GetReferencesCount () << std::endl;
        if (pointer->GetReferencesCount () != 2)
        {
            std::cout << "Incorrect refs count after second pointer!" << std::endl;
            return 1;
        }
    }

    std::cout << "After second pointer deletion: " << pointer->GetReferencesCount () << std::endl;
    if (pointer->GetReferencesCount () != 1)
    {
        std::cout << "Incorrect refs count after second pointer deletion!" << std::endl;
        return 1;
    }

    if (!TestFunction (pointer))
    {
        std::cout << "Incorrect refs count while function call!" << std::endl;
        return 1;
    }

    std::cout << "After function call: " << pointer->GetReferencesCount () << std::endl;
    if (pointer->GetReferencesCount () != 1)
    {
        std::cout << "Incorrect refs count after function call!" << std::endl;
        return 1;
    }

    pointer.Detach ();
    if (pointer.Get () != nullptr)
    {
        std::cout << "Incorrect tracking object after detach!" << std::endl;
        return 1;
    }

    std::cout << "After detach: " << object->GetReferencesCount () << std::endl;
    if (object->GetReferencesCount () != 1)
    {
        std::cout << "Incorrect refs count after detach!" << std::endl;
        return 1;
    }

    return 0;
}
