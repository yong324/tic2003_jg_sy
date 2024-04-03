#pragma once

#include <string>

using namespace std;

class Synonym
{
private:
    string varName;
    string type;
public:
    Synonym(const string& var_name, const string& type);
    string get_var_name() const;
    string get_type() const;
};
