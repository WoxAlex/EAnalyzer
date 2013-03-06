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

        static Election JsonParserFromFile(const std::string& file);
        static Election JsonParser(const std::wstring& str);
        static std::wstring JsonCoder(const Election& election);
        static void JsonCoderToFile(const Election& election, const std::string& file);
    public:
        ElectionInfo info;
        std::vector<ElectionResult> result;
        bool operator == (const Election& other);
    };
}

#endif // ELECTION_H
