#include "electionresult.h"

EAnalyzer::ElectionResult::ElectionResult(int NCandidates)
    :candidates(NCandidates),
        PeopleVotedInAdvance(0),
        PeopleTookAbsenteeBallots(0),
        PeopleTookAbsenteeBallotsFromTEC(0),
        PeopleVotedWithAbsenteeBallots(0),
        AbsenteeBallots(0),
        AbsenteeBallotsLost(0),
        CanceledAbsenteeBallots(0)
{
}
