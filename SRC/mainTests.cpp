#include <cstdio>

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <locale>
#include <cstdlib>
using namespace std;

std::wstring Convert(const char* p)
{
    size_t size = strlen(p) + 1;
    wchar_t* str = new wchar_t[size];
    mbstowcs(str, p, size);
    std::wstring retStr(str);
    delete[] str;
    return retStr;
}

int main(int , char **)
{
    wstring w;

    string s;
    std::setlocale(LC_ALL, "");
    ifstream file;
    char data[] = "Как";
    wchar_t* utf8 = new wchar_t[10];



    file.open("../../DATA/tmp",ios_base::in | ios_base::binary);

    //file.read(data,100);
    file >> s;

    cout << mbstowcs(utf8,data,10) << endl;

    w = Convert(data);

    file.close();

    cout << data << endl;
    wcout << w << endl;

    return 0;
}
