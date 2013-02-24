#include "election.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace EAnalyzer;
using namespace boost::property_tree;
using namespace std;

Election::Election()
{
}

Election Election::JsonParser(const std::string &)
{
    return Election();
}


std::string Election::JsonCoder(const Election &election)
{
    ptree json;

    ptree info;
    info.put("Election type", election.info.type);
    info.put("Election date", election.info.raw_date);
    info.put("Election city", election.info.city);

    ptree candidates;
    for(vector<Person>::const_iterator it = election.info.candidates.begin();
        it != election.info.candidates.end(); ++it)
    {
        ptree candidate;

        candidate.put("Surname",it->surname);
        candidate.put("Name ",it->name);
        candidate.put("Father's name",it->fname);


        candidates.push_back(std::make_pair("",candidate));
    }
    info.add_child("Candidates",candidates);

    ptree result;
    json.add_child("Info",info);
    json.add_child("Result",result);

    stringstream ss;
    json_parser::write_json(ss,json);
    return ss.str();
}
