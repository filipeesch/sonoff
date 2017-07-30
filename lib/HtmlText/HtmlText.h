#ifndef HTML_TEXT_H
#define HTML_TEXT_H

#include <WString.h>

#include "HtmlContent.h"

class HtmlText : public HtmlContent
{
  private:
    String _content;

  public:
    HtmlText(String content) : _content(content) {}

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