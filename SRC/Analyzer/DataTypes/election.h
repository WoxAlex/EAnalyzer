#ifndef ELECTION_H
#define ELECTION_H
#include <vector>
#include "SRC/Analyzer/DataTypes/electioninfo.h"
#include "SRC/Analyzer/DataTypes/electionresult.h"

namespace EAnalyzer
{
    class Election
    {
    public:
        Election();

        static Election JsonParser(const std::wstring& str);
        static std::wstring JsonCoder(const Election& election);
    public:
        ElectionInfo info;
        std::vector<ElectionResult> result;
    };
}

#endif // ELECTION_H
