#ifndef HTML_ATTRIBUTE_H
#define HTML_ATTRIBUTE_H

#include <WString.h>

#include "HtmlContent.h"

class HtmlAttribute : public HtmlContent
{
    String _name;
    String _value;
    bool _nameOnly;

  public:
    HtmlAttribute(String name);
    HtmlAttribute(String name, String value);

    int contentSize();
    void build(String &);
};

#endif