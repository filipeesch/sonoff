#ifndef HTML_RAW_H
#define HTML_RAW_H

#include <WString.h>

#include "HtmlContent.h"

class HtmlRaw : public HtmlContent
{
  private:
    String _content;

  public:
    HtmlRaw(String content) : _content(content) {}

    int contentSize()
    {
        return _content.length();
    }

    void build(String &html)
    {
        html.concat(_content);
    }
};

#endif