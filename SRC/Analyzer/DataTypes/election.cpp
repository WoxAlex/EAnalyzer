#include "election.h"
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
using namespace EAnalyzer;
using namespace boost::property_tree;
using namespace std;

Election::Election()
{
}

Election Election::JsonParserFromFile(const string &filename)
{
    std::wifstream file;
    std::wstring str;
    file.open(filename.c_str());
    while(file.good())
    {
        std::wstring t;
        file >> t;
        str += t+ L" ";
    }
    file.close();

    return EAnalyzer::Election::JsonParser(str);
}

Election Election::JsonParser(const std::wstring &str)
{
    Election election;
    wstringstream ss;
    ss.str(str);
    basic_ptree< std::wstring, std::wstring > json;
    json_parser::read_json(ss,json);


    basic_ptree< std::wstring, std::wstring > info;
    basic_ptree< std::wstring, std::wstring > result;

    info = json.get_child(L"Info");
    result = json.get_child(L"Result");


    election.info.city = info.get<std::wstring>(L"Election city");
    election.info.type =ElectionInfo::ElectionType(info.get<int>(L"Election type"));
    std::wstring date_s = info.get<std::wstring>(L"Election date");
    election.info.date = QDate::fromString(QString::fromStdWString(date_s));

    basic_ptree< std::wstring, std::wstring > candidates;

    candidates = info.get_child(L"Candidates");

    for(basic_ptree< std::wstring, std::wstring >::iterator child = candidates.begin();
        child != candidates.end(); ++child)
    {
        Person p;
        p.fname = child->second.get<std::wstring>(L"Father's name");
        p.name = child->second.get<std::wstring>(L"Name");
        p.surname = child->second.get<std::wstring>(L"Surname");
        election.info.candidates.push_back(p);
    }

    election.result.clear();
    for(basic_ptree< std::wstring, std::wstring >::iterator child = result.begin(), end = result.end();
        child != end; ++child)
    {
        ElectionResult er;
        er.N_LEC = child->second.get<int>(L"LEC number");
        er.TEC = child->second.get<std::wstring>(L"TEC name");
        er.VotersAtLEC = child->second.get<int>(L"Number of voters at LEC");
        er.VotingBallots = child->second.get<int>(L"Number of voting ballots given by TEC");
        er.PeopleVotedInAdvance = child->second.get<int>(L"Number of people that voted in advance");
        er.PeopleVotedIn = child->second.get<int>(L"Number of people that voted inside");
        er.PeopleVotedOut = child->second.get<int>(L"Number of people voted outside");
        er.CanceledBallots = child->second.get<int>(L"Number of canceled ballots");
        er.BallotsFoundOut = child->second.get<int>(L"Number of ballots that found out");
        er.BallotsFoundIn = child->second.get<int>(L"Number of ballots that found in");
        er.InvalidBallots = child->second.get<int>(L"Number of invalid ballots");
        er.ValidBallots = child->second.get<int>(L"Number of valid ballots");
        er.AbsenteeBallots = child->second.get<int>(L"Number of absentee ballots");
        er.PeopleTookAbsenteeBallots = child->second.get<int>(L"Number of people that took absentee ballots");
        er.PeopleVotedWithAbsenteeBallots = child->second.get<int>(L"Number of people that voted with absentee ballots");
        er.CanceledAbsenteeBallots = child->second.get<int>(L"Number of canceled absentee ballots");
        er.PeopleTookAbsenteeBallotsFromTEC = child->second.get<int>(L"Number of people that took absentee ballots from TEC");
        er.AbsenteeBallotsLost = child->second.get<int>(L"Number of lost absentee ballots");
        er.VotingBallotsLost = child->second.get<int>(L"Number of lost voting ballots");
        er.VotingBallotsUnaccounted = child->second.get<int>(L"Number of unaccounted voting ballots");

        er.candidates.clear();
        basic_ptree< std::wstring, std::wstring > candidates_votes = child->second.get_child(L"Candidates' votes");
        for(basic_ptree< std::wstring, std::wstring >::iterator child = candidates_votes.begin(), end = candidates_votes.end();
            child != end; ++child)
        {
            int i = child->second.get<int>(L"");
            er.candidates.push_back(i);
        }

        election.result.push_back(er);
    }


    return election;
}


std::wstring Election::JsonCoder(const Election &election)
{
    basic_ptree< std::wstring, std::wstring > json;

    basic_ptree< std::wstring, std::wstring > info;
    info.put(L"Election type", election.info.type);
    info.put(L"Election date", election.info.date.toString().toStdWString());
    info.put(L"Election city", election.info.city);

    basic_ptree< std::wstring, std::wstring > candidates;
    for(vector<Person>::const_iterator it = election.info.candidates.begin();
        it != election.info.candidates.end(); ++it)
    {
        basic_ptree< std::wstring, std::wstring > candidate;

        candidate.put(L"Surname",it->surname);
        candidate.put(L"Name",it->name);
        candidate.put(L"Father's name",it->fname);
        candidates.push_back(std::make_pair(L"",candidate));
    }
    info.add_child(L"Candidates",candidates);

    basic_ptree< std::wstring, std::wstring > result;
    json.add_child(L"Info",info);



    for(vector<ElectionResult>::const_iterator it = election.result.begin(), end = election.result.end();
        it != end; ++it)
    {
        basic_ptree< std::wstring, std::wstring > LEC;

        LEC.put(L"LEC number", it->N_LEC);
        LEC.put(L"TEC name", it->TEC);
        LEC.put(L"Number of voters at LEC", it->VotersAtLEC);
        LEC.put(L"Number of voting ballots given by TEC", it->VotingBallots);
        LEC.put(L"Number of people that voted in advance", it->PeopleVotedInAdvance);
        LEC.put(L"Number of people that voted inside", it->PeopleVotedIn);
        LEC.put(L"Number of people voted outside", it->PeopleVotedOut);
        LEC.put(L"Number of canceled ballots", it->CanceledBallots);
        LEC.put(L"Number of ballots that found out", it->BallotsFoundOut);
        LEC.put(L"Number of ballots that found in", it->BallotsFoundIn);
        LEC.put(L"Number of invalid ballots", it->InvalidBallots);
        LEC.put(L"Number of valid ballots", it->ValidBallots);
        LEC.put(L"Number of absentee ballots", it->AbsenteeBallots);
        LEC.put(L"Number of people that took absentee ballots", it->PeopleTookAbsenteeBallots);
        LEC.put(L"Number of people that voted with absentee ballots", it->PeopleVotedWithAbsenteeBallots);
        LEC.put(L"Number of canceled absentee ballots", it->CanceledAbsenteeBallots);
        LEC.put(L"Number of people that took absentee ballots from TEC", it->PeopleTookAbsenteeBallotsFromTEC);
        LEC.put(L"Number of lost absentee ballots", it->AbsenteeBallotsLost);
        LEC.put(L"Number of lost voting ballots", it->VotingBallotsLost);
        LEC.put(L"Number of unaccounted voting ballots", it->VotingBallotsUnaccounted);

        basic_ptree< std::wstring, std::wstring > candidates_votes;
        for(vector<int>::const_iterator iter = it->candidates.begin(), i_end = it->candidates.end();
            iter != i_end; ++iter)
        {
            basic_ptree< std::wstring, std::wstring > candidate;
            candidate.put(L"",*iter);
            candidates_votes.push_back(std::make_pair(L"", candidate));

        }
        LEC.add_child(L"Candidates' votes",candidates_votes);
        result.push_back(std::make_pair(L"",LEC));
    }
    json.add_child(L"Result",result);
    wstringstream ss;
    json_parser::write_json(ss,json);
    return ss.str();
}

void Election::JsonCoderToFile(const Election &election, const string &name)
{
    std::wstring str = Election::JsonCoder(election);
    wofstream file;
    file.open(name.c_str());
    file << str;
    file.close();
}

bool Election::operator ==(const Election &other)
{
    bool f = (this->info == other.info);
    f = f && this->result.size() == other.result.size();
    for(uint i = 0; i< this->result.size(); ++i)
            f = f && this->result[i] == other.result[i];
    return f;
}
