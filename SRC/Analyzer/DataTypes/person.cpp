#include "person.h"

using namespace EAnalyzer;

Person::Person()
{
}

bool Person::operator ==(const Person &other)
{
    return name == other.name &&
            fname == other.fname &&
            surname == other.surname;
}
