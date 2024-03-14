#include "Synonym.h"

Synonym::Synonym(const string& var_name, const string& type) : varName(var_name),
type(type)
{
}

string Synonym::get_var_name() const
{
    return varName;
}

string Synonym::get_type() const
{
    return type;
}
