#include "HtmlAttribute.h"

HtmlAttribute::HtmlAttribute(String name) : _name(name)
{
    _nameOnly = true;
}

HtmlAttribute::HtmlAttribute(String name, String value) : _name(name), _value(value)
{
    _nameOnly = false;
}

int HtmlAttribute::contentSize()
{
    if (_nameOnly)
        return _name.size();
    else
        return _name.size() + _value.size() + 3;
}

void HtmlAttribute::build(String &html)
{
    if (_nameOnly)
        html.concat(_name);
    else
    {
        html.concat(_name);
        html.concat("=\"");
        html.concat(_value);
        html.concat("\"");
    }
}