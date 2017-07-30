#ifndef HTML_CONTENT_H
#define HTML_CONTENT_H

#include <WString.h>

class HtmlContent
{
  public:
    inline virtual int contentSize() {}
    inline virtual void build(String &) {}
};

#endif