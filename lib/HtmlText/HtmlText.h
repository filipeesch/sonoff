#ifndef HTML_CONTENT_H
#define HTML_CONTENT_H

#include <WString.h>

#include "HtmlContent.h"

class HtmlText : public HtmlContent
{
  private:
    String _content;

  public:
    HtmlText(String content) : _content(content) {}

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