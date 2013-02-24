#include "parsedefaultfile.h"
#include <fstream>
#include <stdexcept>
#include <cstdlib>
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
    info.date->tm_year = year;
    info.date->tm_mon  = month;
    info.date->tm_mday  = day;
    mktime ( info.date );

    if(info.type == ElectionInfo::City)
    {
        std::string t1;
        file >> t1;
        info.city = Convert(t1.c_str());
    }

    int n;
    file >> n;
    info.candidates.clear();
    for(int i = 0; i<n; ++i)
    {
        Person p;
        std::string t1,t2,t3;
        file >> t1 >> t2 >> t3;
        p.surname = Convert(t1.c_str());
        p.name = Convert(t2.c_str());
        p.fname = Convert(t3.c_str());
        info.candidates.push_back(p);
    }

    election.info = info;
    election.result.clear();

    file >> n;
    for(int i = 0; i<n; ++i)
    {
        ElectionResult r;
        std::string t1;
        file >> r.N_LEC >> t1 >> r.VotersAtLEC >> r.VotingBallots;
        r.TEC = Convert(t1.c_str());
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
    std::wstring str = Election::JsonCoder(this->election);
    wofstream file;
    file.open(name.c_str());
    file << str;
    file.close();
}

wstring ParseDefaultFile::Convert(const char *p)
{
    size_t size = strlen(p) + 1;
    wchar_t* str = new wchar_t[size];
    mbstowcs(str, p, size);
    std::wstring retStr(str);
    delete[] str;
    return retStr;
}

ParseDefaultFile::~ParseDefaultFile()
{
}
