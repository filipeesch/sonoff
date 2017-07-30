#ifndef HTML_CONTENT_H
#define HTML_CONTENT_H

#include <Arduino.h>
#include <WString.h>

class HtmlContent
{
  private:
    String _content;

  public:
    virtual ~HtmlContent() {}
    virtual int contentSize() {}
    virtual void build(String &) {}
};

#endif