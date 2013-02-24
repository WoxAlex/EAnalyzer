#ifndef PERSON_H
#define PERSON_H

#include <string>

namespace EAnalyzer
{
    class Person
    {
    public:
        Person();
        std::wstring name;
        std::wstring surname;
        std::wstring fname;
    };
}

#endif // PERSON_H
