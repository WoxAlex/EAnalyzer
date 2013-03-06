#ifndef ELECTIONINFO_H
#define ELECTIONINFO_H

#include <vector>
#include <QDate>
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
        QDate date;
        bool operator == (const ElectionInfo& other);

    };
}

#endif // ELECTIONINFO_H
