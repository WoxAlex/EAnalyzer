#include "election.h"
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <sstream>
#include <cstring>

using namespace EAnalyzer;
using namespace boost::property_tree;
using namespace std;

Election::Election()
{
}

Election Election::JsonParser(const std::wstring &str)
{
    std::string strs;
    strs.assign(str.begin(), str.end());

    Election election;
    std::stringstream ss;
    ss.str(strs);
    ptree json;
    read_json(ss, json);

    ptree info;
    info = json.get_child("Info");

    election.info.city.assign(info.get<std::string>("Election city").begin(), info.get<std::string>("Election city").end());
    election.info.type = ElectionInfo::ElectionType(info.get<int>("Election type"));
    election.info.raw_date = info.get<time_t>("Election date");

    ptree candidates;
    candidates = info.get_child("Candidates");
//    for(ptree::iterator child = candidates.begin(); child != candidates.end(); ++child)
//    {
    BOOST_FOREACH(ptree::value_type &child, info.get_child("Candidates"))
    {
        Person p;

        p.fname.assign(child.second.get<std::string>("Father's name").begin(), child.second.get<std::string>("Father's name").end());
        p.name.assign(child.second.get<std::string>("Name").begin(), child.second.get<std::string>("Name").end());
        p.surname.assign(child.second.get<std::string>("Surname").begin(), child.second.get<std::string>("Surname").end());

        election.info.candidates.push_back(p);
    }

    ptree result;
    result = json.get_child("Result");

    //for(ptree::iterator child = result.begin(); child != result.end(); ++child)
    //{
    BOOST_FOREACH(ptree::value_type &child, json.get_child("Result"))
    {
        ElectionResult er;

        er.N_LEC = child.second.get<int>("LEC number");
        er.TEC.assign(child.second.get<std::string>("TEC name").begin(), child.second.get<std::string>("TEC name").end());
        er.VotersAtLEC = child.second.get<int>("Number of voters at LEC");
        er.VotingBallots = child.second.get<int>("Number of voting ballots given by TEC");
        er.PeopleVotedInAdvance = child.second.get<int>("Number of people that voted in advance");
        er.PeopleVotedIn = child.second.get<int>("Number of people that voted inside");
        er.PeopleVotedOut = child.second.get<int>("Number of people voted outside");
        er.CanceledBallots = child.second.get<int>("Number of canceled ballots");
        er.BallotsFoundOut = child.second.get<int>("Number of ballots that found out");
        er.BallotsFoundIn = child.second.get<int>("Number of ballots that found in");
        er.InvalidBallots = child.second.get<int>("Number of invalid ballots");
        er.ValidBallots = child.second.get<int>("Number of valid ballots");
        er.AbsenteeBallots = child.second.get<int>("Number of absentee ballots");
        er.PeopleTookAbsenteeBallots = child.second.get<int>("Number of people that took absentee ballots");
        er.PeopleVotedWithAbsenteeBallots = child.second.get<int>("Number of people that voted with absentee ballots");
        er.CanceledAbsenteeBallots = child.second.get<int>("Number of canceled absentee ballots");
        er.PeopleTookAbsenteeBallotsFromTEC = child.second.get<int>("Number of people that took absentee ballots from TEC");
        er.AbsenteeBallotsLost = child.second.get<int>("Number of lost absentee ballots");
        er.VotingBallotsLost = child.second.get<int>("Number of lost voting ballots");
        er.VotingBallotsUnaccounted = child.second.get<int>("Number of unaccounted voting ballots");

        er.N_LEC = child.second.get<int>("LEC number");

//        BOOST_FOREACH(ptree::value_type &v, child.second.get<std::string>("Candidate's votes"))
//        {
//            int temp = v.second.data();
//            er.candidates.push_back(temp);
//        }

        election.result.push_back(er);
    }

    return election;

//    Election election;
//    wstringstream ss;
//    ss.str(str);
//    basic_ptree< std::wstring, std::wstring > json;
//    json_parser::read_json(ss,json);


//    basic_ptree< std::wstring, std::wstring > info;
//    info = json.get_child(L"Info");

//    election.info.city = info.get<std::wstring>(L"Election city");
//    election.info.type = ElectionInfo::ElectionType(info.get<int>(L"Election type"));
//    election.info.raw_date = info.get<time_t>(L"Election date");

//    basic_ptree< std::wstring, std::wstring > candidates;
//    candidates = info.get_child(L"Candidates");
//    for(basic_ptree< std::wstring, std::wstring >::iterator child = candidates.begin();
//        child != candidates.end(); ++child)
//    {
//        Person p;
//        p.fname = child->second.get<std::wstring>(L"Father's name");
//        p.name = child->second.get<std::wstring>(L"Name");
//        p.surname = child->second.get<std::wstring>(L"Surname");
//        election.info.candidates.push_back(p);
//    }


//    basic_ptree< std::wstring, std::wstring > result;
//    result = json.get_child(L"Result");

//    //template typename basic_ptree< std::wstring, std::wstring >
//    //BOOST_FOREACH(basic_ptree< std::wstring, std::wstring >)

//    for(basic_ptree< std::wstring, std::wstring >::iterator child = result.begin();
//        child != result.end(); ++child)
//    {
//        ElectionResult er;
//        er.N_LEC = child->second.get<int>(L"LEC number");
//        er.TEC = child->second.get<std::wstring>(L"TEC name");
//        er.VotersAtLEC = child->second.get<int>(L"Number of voters at LEC");
//        er.VotingBallots = child->second.get<int>(L"Number of voting ballots given by TEC");
//        er.PeopleVotedInAdvance = child->second.get<int>(L"Number of people that voted in advance");
//        er.PeopleVotedIn = child->second.get<int>(L"Number of people that voted inside");
//        er.PeopleVotedOut = child->second.get<int>(L"Number of people voted outside");
//        er.CanceledBallots = child->second.get<int>(L"Number of canceled ballots");
//        er.BallotsFoundOut = child->second.get<int>(L"Number of ballots that found out");
//        er.BallotsFoundIn = child->second.get<int>(L"Number of ballots that found in");
//        er.InvalidBallots = child->second.get<int>(L"Number of invalid ballots");
//        er.ValidBallots = child->second.get<int>(L"Number of valid ballots");
//        er.AbsenteeBallots = child->second.get<int>(L"Number of absentee ballots");
//        er.PeopleTookAbsenteeBallots = child->second.get<int>(L"Number of people that took absentee ballots");
//        er.PeopleVotedWithAbsenteeBallots = child->second.get<int>(L"Number of people that voted with absentee ballots");
//        er.CanceledAbsenteeBallots = child->second.get<int>(L"Number of canceled absentee ballots");
//        er.PeopleTookAbsenteeBallotsFromTEC = child->second.get<int>(L"Number of people that took absentee ballots from TEC");
//        er.AbsenteeBallotsLost = child->second.get<int>(L"Number of lost absentee ballots");
//        er.VotingBallotsLost = child->second.get<int>(L"Number of lost voting ballots");
//        er.VotingBallotsUnaccounted = child->second.get<int>(L"Number of unaccounted voting ballots");

//        BOOST_FOREACH(typename basic_ptree<std::wstring,std::wstring>::value_type &v, child->get_child(L"Candidate's votes"))
//        {
//            er.candidates.push_back(v.second.data());
//        }

//        /*basic_ptree< std::wstring, std::wstring > candidates_votes;
//        candidates_votes = child->second.get(L"Candidate's votes");
//        for(basic_ptree< std::wstring, std::wstring >::iterator cchild = candidates_votes.begin();
//            cchild != candidates_votes.end(); ++cchild)
//        {

//        }*/

//        election.result.push_back(er);
//    }

//    return election;
}


std::wstring Election::JsonCoder(const Election &election)
{
    ptree json;
    ptree info;
    ptree result;

    std::string temp_str;
    temp_str.assign(election.info.city.begin(), election.info.city.end());
    info.put("Election type", election.info.type);
    info.put("Election date", election.info.raw_date);
    //info.put("Election city", temp_str);

//    ptree candidates;
//    for(vector<Person>::const_iterator it = election.info.candidates.begin();
//        it != election.info.candidates.end(); ++it)
//    {
//        ptree candidate;

////        temp_str.assign(it->surname.begin(), it->surname.end());
////        candidate.put("Surname", temp_str);
////        temp_str.assign(it->name.begin(), it->name.end());
////        candidate.put("Name", temp_str);
////        temp_str.assign(it->fname.begin(), it->fname.end());
////        candidate.put("Father's name", temp_str);

//        candidates.push_back(std::make_pair("", candidate));
//    }

//    info.add_child("Candidates", candidates);

//    for(vector<ElectionResult>::const_iterator it = election.result.begin();
//        it != election.result.end(); ++it)
//    {
//        ptree LEC;

//        LEC.put("LEC number", it->N_LEC);
//        temp_str.assign(it->TEC.begin(), it->TEC.end());
//        LEC.put("TEC name", temp_str);
//        LEC.put("Number of voters at LEC", it->VotersAtLEC);
//        LEC.put("Number of voting ballots given by TEC", it->VotingBallots);
//        LEC.put("Number of people that voted in advance", it->PeopleVotedInAdvance);
//        LEC.put("Number of people that voted inside", it->PeopleVotedIn);
//        LEC.put("Number of people voted outside", it->PeopleVotedOut);
//        LEC.put("Number of canceled ballots", it->CanceledBallots);
//        LEC.put("Number of ballots that found out", it->BallotsFoundOut);
//        LEC.put("Number of ballots that found in", it->BallotsFoundIn);
//        LEC.put("Number of invalid ballots", it->InvalidBallots);
//        LEC.put("Number of valid ballots", it->ValidBallots);
//        LEC.put("Number of absentee ballots", it->AbsenteeBallots);
//        LEC.put("Number of people that took absentee ballots", it->PeopleTookAbsenteeBallots);
//        LEC.put("Number of people that voted with absentee ballots", it->PeopleVotedWithAbsenteeBallots);
//        LEC.put("Number of canceled absentee ballots", it->CanceledAbsenteeBallots);
//        LEC.put("Number of people that took absentee ballots from TEC", it->PeopleTookAbsenteeBallotsFromTEC);
//        LEC.put("Number of lost absentee ballots", it->AbsenteeBallotsLost);
//        LEC.put("Number of lost voting ballots", it->VotingBallotsLost);
//        LEC.put("Number of unaccounted voting ballots", it->VotingBallotsUnaccounted);

//        basic_ptree< std::wstring, std::wstring > candidates_votes;
//        for(vector<int>::const_iterator iter = it->candidates.begin();
//            iter != it->candidates.end(); ++iter)
//        {
//            ptree candidate;

//            candidate.put("", *iter);

//            candidates_votes.push_back(std::make_pair("", candidate));
//        }

//        LEC.add_child("Candidate's votes", candidates_votes);
//        result.push_back(std::make_pair("", LEC));
//    }

    json.add_child("Info",info);
    json.add_child("Result",result);

    std::stringstream ss;
    json_parser::write_json(ss,json);

    std::wstring str;
    str.assign(ss.str().begin(), ss.str().end());

    return str;










//    basic_ptree< std::wstring, std::wstring > json;

//    basic_ptree< std::wstring, std::wstring > info;
//    basic_ptree< std::wstring, std::wstring > result;

//    info.put(L"Election type", election.info.type);
//    info.put(L"Election date", election.info.raw_date);
//    info.put(L"Election city", election.info.city);

//    basic_ptree< std::wstring, std::wstring > candidates;
//    for(vector<Person>::const_iterator it = election.info.candidates.begin();
//        it != election.info.candidates.end(); ++it)
//    {
//        basic_ptree< std::wstring, std::wstring > candidate;

//        candidate.put(L"Surname",it->surname);
//        candidate.put(L"Name",it->name);
//        candidate.put(L"Father's name",it->fname);
//        candidates.push_back(std::make_pair(L"",candidate));
//    }
//    info.add_child(L"Candidates",candidates);

//    for(vector<ElectionResult>::const_iterator it = election.result.begin();
//        it != election.result.end(); ++it)
//    {
//        basic_ptree< std::wstring, std::wstring > LEC;

//        LEC.put(L"LEC number", it->N_LEC);
//        LEC.put(L"TEC name", it->TEC);
//        LEC.put(L"Number of voters at LEC", it->VotersAtLEC);
//        LEC.put(L"Number of voting ballots given by TEC", it->VotingBallots);
//        LEC.put(L"Number of people that voted in advance", it->PeopleVotedInAdvance);
//        LEC.put(L"Number of people that voted inside", it->PeopleVotedIn);
//        LEC.put(L"Number of people voted outside", it->PeopleVotedOut);
//        LEC.put(L"Number of canceled ballots", it->CanceledBallots);
//        LEC.put(L"Number of ballots that found out", it->BallotsFoundOut);
//        LEC.put(L"Number of ballots that found in", it->BallotsFoundIn);
//        LEC.put(L"Number of invalid ballots", it->InvalidBallots);
//        LEC.put(L"Number of valid ballots", it->ValidBallots);
//        LEC.put(L"Number of absentee ballots", it->AbsenteeBallots);
//        LEC.put(L"Number of people that took absentee ballots", it->PeopleTookAbsenteeBallots);
//        LEC.put(L"Number of people that voted with absentee ballots", it->PeopleVotedWithAbsenteeBallots);
//        LEC.put(L"Number of canceled absentee ballots", it->CanceledAbsenteeBallots);
//        LEC.put(L"Number of people that took absentee ballots from TEC", it->PeopleTookAbsenteeBallotsFromTEC);
//        LEC.put(L"Number of lost absentee ballots", it->AbsenteeBallotsLost);
//        LEC.put(L"Number of lost voting ballots", it->VotingBallotsLost);
//        LEC.put(L"Number of unaccounted voting ballots", it->VotingBallotsUnaccounted);

//        LEC.put(L"Number of candidates", it->candidates.size());

//        std::string str = "Votes for candidate #1";
//        //!!basic_ptree< std::wstring, std::wstring > candidates_votes;
//        for(vector<int>::const_iterator iter = it->candidates.begin();
//            iter != it->candidates.end(); ++iter)
//        {
//            //!!basic_ptree< std::wstring, std::wstring > candidate;
//            std::wstring strw;
//            strw.assign(str.begin(), str.end());
//            str[21]++;

////!!            candidate.put(strw, *iter);

////!!            candidates_votes.push_back(std::make_pair(L"", candidate));
////!!        }
////!!        LEC.add_child(L"Candidate's votes", candidates_votes);
//            LEC.put(strw, *iter);
//        }

//        result.push_back(std::make_pair(L"", LEC));
//    }

//    json.add_child(L"Info",info);
//    json.add_child(L"Result",result);

//    wstringstream ss;
//    json_parser::write_json(ss,json);
//    return ss.str();
}
