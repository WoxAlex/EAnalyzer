#include <cstdio>

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <locale>

using namespace std;

int main(int , char **)
{
    wstring w;

    string s;

    ifstream file;
    char* data = new char[100];



    file.open("../../DATA/tmp",ios_base::in | ios_base::binary);

    //file.read(data,100);
    file >> s;
    memcpy(data,s.data(),s.length());

    w = wstring((wchar_t*)data);

    cout << strcmp(data,"Кемерово") << endl;

    file.close();

    cout << data << endl;

    return 0;
}
