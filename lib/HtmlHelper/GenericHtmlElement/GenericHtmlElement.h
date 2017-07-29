#include <forward_list>

#include "HtmlAttribute.h"

using namespace std;

class GenericHtmlElement
{
  private:
    const String &_tagName;
    forward_list<HtmlAttribute> &_attrs;
    forward_list<GenericHtmlElement> &_children;
    bool _selfClosing;

  public:
    GenericHtmlElement(const String &tagName, bool selfClosing, list<HtmlAttribute> &attrs = nullptr, list<GenericHtmlElement> &children = nullptr);

    int contentSize();
    void build(String &);
};