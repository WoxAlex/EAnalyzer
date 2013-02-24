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
    election.info.raw_date = info.get<time_t>(L"Election date");

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

    return election;
}


std::wstring Election::JsonCoder(const Election &election)
{
    basic_ptree< std::wstring, std::wstring > json;

    basic_ptree< std::wstring, std::wstring > info;
    info.put(L"Election type", election.info.type);
    info.put(L"Election date", election.info.raw_date);
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
    json.add_child(L"Result",result);

    wstringstream ss;
    json_parser::write_json(ss,json);
    return ss.str();
}
