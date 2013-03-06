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

bool EAnalyzer::ElectionResult::operator ==(const EAnalyzer::ElectionResult &other)
{
    bool f =  N_LEC == other.N_LEC &&
           TEC == other.TEC &&
           VotersAtLEC == other.VotersAtLEC &&
           VotingBallots == other.VotingBallots &&
           PeopleVotedInAdvance == other.PeopleVotedInAdvance &&
           PeopleVotedIn == other.PeopleVotedIn &&
           PeopleVotedOut == other.PeopleVotedOut &&
           CanceledBallots == other.CanceledBallots &&
           BallotsFoundOut == other.BallotsFoundOut &&
           BallotsFoundIn == other.BallotsFoundIn &&
           InvalidBallots == other.InvalidBallots &&
           ValidBallots == other.ValidBallots &&
           AbsenteeBallots == other.AbsenteeBallots &&
           PeopleTookAbsenteeBallots == other.PeopleTookAbsenteeBallots &&
           PeopleVotedWithAbsenteeBallots == other.PeopleVotedWithAbsenteeBallots &&
           CanceledAbsenteeBallots == other.CanceledAbsenteeBallots &&
           PeopleTookAbsenteeBallotsFromTEC == other.PeopleTookAbsenteeBallotsFromTEC &&
           AbsenteeBallotsLost  == other.AbsenteeBallotsLost &&
           VotingBallotsLost  == other.VotingBallotsLost &&
           VotingBallotsUnaccounted  == other.VotingBallotsUnaccounted &&
           candidates.size()  == other.candidates.size();
    for(size_t i = 0; i < candidates.size(); ++i)
        f = f && candidates[i] == other.candidates[i];
    return f;
}
