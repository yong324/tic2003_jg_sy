#include "Synonym.h"

Synonym::Synonym(const string& var_name, const string& type) : varName(var_name),
type(type)
{
}

// get_var_name is a const method that returns the variable name of the Synonym.
string Synonym::get_var_name() const
{
    return varName;
}

// get_type is a const method that returns the type of the Synonym.
string Synonym::get_type() const
{
    return type;
}
