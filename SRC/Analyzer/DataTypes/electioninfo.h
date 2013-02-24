#ifndef ELECTIONINFO_H
#define ELECTIONINFO_H

#include <vector>
#include <time.h>
#include "SRC/Analyzer/DataTypes/person.h"
namespace EAnalyzer
{
    class ElectionInfo
    {
    public:
        ElectionInfo();
        enum ElectionType
        {
            Country,
            City
        };
        ElectionType type;
        std::vector<Person> candidates;
        std::wstring city;
        time_t raw_date;
        struct tm * date;

    };
}

#endif // ELECTIONINFO_H
