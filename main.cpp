#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    vector<string> msg{"Hello", "C++", "World"};
    for (const auto& str: msg)
    {
        cout<< str << endl;
        /* code */
    }
    
    cout<< endl;
    return 0;
}