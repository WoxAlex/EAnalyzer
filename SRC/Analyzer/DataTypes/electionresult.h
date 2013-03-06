#ifndef ELECTIONRESULT_H
#define ELECTIONRESULT_H
#include<string>
#include<vector>

namespace EAnalyzer
{
    class ElectionResult
    {
    public:
        ElectionResult(int NCandidates = 0);
        int N_LEC;
        std::wstring TEC;
        int VotersAtLEC;
        int VotingBallots;
        int PeopleVotedInAdvance;
        int PeopleVotedIn;
        int PeopleVotedOut;
        int CanceledBallots;
        int BallotsFoundOut;
        int BallotsFoundIn;
        int InvalidBallots;
        int ValidBallots;
        int AbsenteeBallots;
        int PeopleTookAbsenteeBallots;
        int PeopleVotedWithAbsenteeBallots;
        int CanceledAbsenteeBallots;
        int PeopleTookAbsenteeBallotsFromTEC;
        int AbsenteeBallotsLost;
        int VotingBallotsLost;
        int VotingBallotsUnaccounted;
        std::vector<int> candidates;
        bool operator == (const ElectionResult& other);
    };
}

#endif // ELECTIONRESULT_H
