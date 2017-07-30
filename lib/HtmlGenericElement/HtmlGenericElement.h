#ifndef HTML_GENERIC_ELEMENT_H
#define HTML_GENERIC_ELEMENT_H

#include <forward_list>
#include <WString.h>

#include "HtmlContent.h"
#include "HtmlAttribute.h"

using namespace std;

class HtmlGenericElement : public HtmlContent
{
  private:
    const String _tagName;
    forward_list<HtmlAttribute *> _attrs;
    forward_list<HtmlContent *> _children;
    bool _selfClosing;

  public:
    HtmlGenericElement(const String tagName, bool selfClosing);
    ~HtmlGenericElement();

    const HtmlGenericElement &id(String);
    const HtmlGenericElement &name(String);

    const HtmlGenericElement &appendAttr(HtmlAttribute *);
    const HtmlGenericElement &append(HtmlContent *);

    int contentSize();
    void build(String &);
};

#endif