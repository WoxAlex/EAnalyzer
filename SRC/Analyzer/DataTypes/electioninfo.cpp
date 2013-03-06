#include "electioninfo.h"


EAnalyzer::ElectionInfo::ElectionInfo()
    :type(EAnalyzer::ElectionInfo::City)
{
}

bool EAnalyzer::ElectionInfo::operator ==(const EAnalyzer::ElectionInfo &other)
{
    bool f = type == other.type &&
            city == other.city &&
            date == other.date &&
            candidates.size() == other.candidates.size();
    for(unsigned int i = 0; i < candidates.size(); ++i)
        f = f && candidates[i] == other.candidates[i];
    return f;
}
