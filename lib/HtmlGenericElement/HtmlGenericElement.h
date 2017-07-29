#ifndef HTML_GENERIC_ELEMENT_H
#define HTML_GENERIC_ELEMENT_H

#include <forward_list>
#include <WString.h>

#include "HtmlAttribute.h"

using namespace std;

class HtmlGenericElement
{
  private:
    const String _tagName;
    String _content;
    forward_list<HtmlAttribute *> _attrs;
    forward_list<HtmlGenericElement *> _children;
    bool _selfClosing;

  public:
    HtmlGenericElement(const String tagName, bool selfClosing);
    ~HtmlGenericElement();

    void id(String);
    void name(String);
    void text(String);

    void appendAttr(HtmlAttribute *);
    void append(HtmlGenericElement *);

    int contentSize();
    void build(String &);
};

#endif