#ifndef HTML_GENERIC_ELEMENT_H
#define HTML_GENERIC_ELEMENT_H

#include <list>
#include <WString.h>

#include "HtmlContent.h"
#include "HtmlAttribute.h"

using namespace std;

class HtmlGenericElement : public HtmlContent
{
private:
  const String _tagName;
  list<HtmlAttribute *> _attrs;
  list<HtmlContent *> _children;
  bool _selfClosing;

public:
  HtmlGenericElement(const String tagName, bool selfClosing);
  ~HtmlGenericElement();

  int contentSize();
  void build(String &);

  HtmlGenericElement *id(String);
  HtmlGenericElement *name(String);

  HtmlGenericElement *appendAttr(HtmlAttribute *);
  HtmlGenericElement *append(HtmlContent *);
};

#endif