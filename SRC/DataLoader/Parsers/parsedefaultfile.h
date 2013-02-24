#ifndef PARSEDEFAULTFILE_H
#define PARSEDEFAULTFILE_H

#include <string>
#include "SRC/Analyzer/DataTypes/election.h"

namespace DataLoader
{
    class ParseDefaultFile
    {
    public:
        ParseDefaultFile();

        void LoadFromFile(const std::string& filename);

        void SaveToJson(const std::string& filename);

        ~ParseDefaultFile();
    public:
        EAnalyzer::Election election;
    };
}

#endif // PARSEDEFAULTFILE_H
