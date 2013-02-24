#include "electioninfo.h"

EAnalyzer::ElectionInfo::ElectionInfo()
{
    date = localtime ( &raw_date );
}
