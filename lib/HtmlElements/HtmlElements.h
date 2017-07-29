#ifndef HTML_ELEMENTS_H
#define HTML_ELEMENTS_H

#include "HtmlGenericElement.h"

class HtmlDivElement : public HtmlGenericElement
{
  public:
    inline HtmlDivElement() : HtmlGenericElement("div", false) {}
};

class HtmlUlElement : public HtmlGenericElement
{
  public:
    inline HtmlUlElement() : HtmlGenericElement("ul", false) {}
};

class HtmlLiElement : public HtmlGenericElement
{
  public:
    inline HtmlLiElement() : HtmlGenericElement("li", false) {}
};

class HtmlInputElement : public HtmlGenericElement
{
  public:
    inline HtmlInputElement() : HtmlGenericElement("input", true) {}

    inline HtmlInputElement(String type, String name, String value = "") : HtmlGenericElement("input", true)
    {
        this->type(type);
        this->name(name);
        this->value(value);
    }

    inline void type(String v)
    {
        appendAttr(new HtmlAttribute("type", v));
    }

    inline void value(String v)
    {
        appendAttr(new HtmlAttribute("value", v));
    }
};

class HtmlHyperlinkElement : public HtmlGenericElement
{
  public:
    inline HtmlHyperlinkElement() : HtmlGenericElement("a", false) {}

    inline HtmlInputElement(String href, String text = "") : HtmlGenericElement("a", false)
    {
        this->href(href);
        this->text(text);
    }

    inline void href(String v)
    {
        appendAttr(new HtmlAttribute("href", v));
    }
};

#endif