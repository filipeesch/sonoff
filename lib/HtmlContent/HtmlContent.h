#ifndef HTML_CONTENT_H
#define HTML_CONTENT_H

#include <WString.h>

class HtmlContent
{
private:
  String _content;

public:
  inline virtual int contentSize() {}
  inline virtual void build(String &) {}

  inline const String &buildAll()
  {
    _content.reserve(contentSize());
    build(_content);

    return _content;
  }
};

#endif