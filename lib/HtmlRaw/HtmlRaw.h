#ifndef HTML_CONTENT_H
#define HTML_CONTENT_H

#include <WString.h>

#include "HtmlContent.h"

class HtmlRaw : public HtmlContent
{
  private:
    String _content;

  public:
    HtmlRaw(String content) : _content(content) {}

    inline int contentSize()
    {
        return _content.length();
    }

    inline void build(String &html)
    {
        html.concat(_content);
    }
};

#endif