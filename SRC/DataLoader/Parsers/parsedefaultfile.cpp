#include "parsedefaultfile.h"
#include <fstream>
#include <stdexcept>

#include <cstring>

using namespace DataLoader;
using namespace EAnalyzer;
using namespace std;

ParseDefaultFile::ParseDefaultFile()
{
}

void ParseDefaultFile::LoadFromFile(const std::string &filename)
{
    ifstream file;
    file.open(filename.c_str());

    if (!file.is_open())
    {
        throw runtime_error("Unknown file or destination");
    }

    ElectionInfo info;

    int type;
    file >> type;

    switch (type)
    {
    case 1:
        info.type = ElectionInfo::Country;
        break;
    case 2:
        info.type = ElectionInfo::City;
        break;
    default:
        throw runtime_error("Unknown election type");
    }

    int day,month,year;

    file >> day >> month >> year;
    info.date->tm_year = year - 1900;
    info.date->tm_mon  = month - 1;
    info.date->tm_mday  = day;
    mktime ( info.date );

    if(info.type == ElectionInfo::City)
    {
        std::string tmp;
        file >> tmp;
        wchar_t* tttt = new wchar_t[9];
        memcpy(tttt, tmp.data(),16);
        tttt[8] = 0;
        std::wstring correct( (wchar_t*)tmp.data());
        info.city = correct;
    }

    int n;
    file >> n;
    info.candidates.clear();
    for(int i = 0; i<n; ++i)
    {
        Person p;
        //file >> p.surname >> p.name >> p.fname;
        info.candidates.push_back(p);
    }

    election.info = info;
    election.result.clear();

    file >> n;
    for(int i = 0; i<n; ++i)
    {
        ElectionResult r;
        //file >> r.N_LEC >> r.TEC >> r.VotersAtLEC >> r.VotingBallots;
        if(info.type == ElectionInfo::City)
        {
           file >> r.PeopleVotedInAdvance;
        }
        file >> r.PeopleVotedIn >> r.PeopleVotedOut >> r.CanceledBallots >> r.BallotsFoundOut >> r.BallotsFoundIn
                >> r.InvalidBallots >> r.ValidBallots;

        if(info.type == ElectionInfo::Country)
        {
           file >> r.AbsenteeBallots >> r.PeopleTookAbsenteeBallots >> r.PeopleVotedWithAbsenteeBallots
                >> r.CanceledAbsenteeBallots >> r.PeopleTookAbsenteeBallotsFromTEC >> r.AbsenteeBallotsLost;
        }

        file >> r.VotingBallotsLost >> r.VotingBallotsUnaccounted;

        r.candidates.resize(info.candidates.size(),0);
        for(size_t j = 0; j<info.candidates.size(); ++j)
        {
            file >> r.candidates[j];
        }

        election.result.push_back(r);
    }


    file.close();
}

void ParseDefaultFile::SaveToJson(const std::string &name)
{
    std::string str = Election::JsonCoder(this->election);
    ofstream file;
    file.open(name.c_str());
    file << str;
    file.close();
}

ParseDefaultFile::~ParseDefaultFile()
{
}
