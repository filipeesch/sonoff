#ifndef HTML_ELEMENTS_H
#define HTML_ELEMENTS_H

#include "HtmlGenericElement.h"

class HtmlTag : public HtmlGenericElement
{
  public:
    inline HtmlTag() : HtmlGenericElement("html", false) {}
};

class HtmlHeadElement : public HtmlGenericElement
{
  public:
    inline HtmlHeadElement() : HtmlGenericElement("head", false) {}
};

class HtmlBodyElement : public HtmlGenericElement
{
  public:
    inline HtmlBodyElement() : HtmlGenericElement("body", false) {}
};

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
        this->value(value);
        this->name(name);
    }

    inline const HtmlInputElement &type(String v)
    {
        appendAttr(new HtmlAttribute("type", v));
        return *this;
    }

    inline const HtmlInputElement &value(String v)
    {
        appendAttr(new HtmlAttribute("value", v));
        return *this;
    }
};

class HtmlLinkElement : public HtmlGenericElement
{
  public:
    inline HtmlLinkElement() : HtmlGenericElement("a", false) {}

    inline HtmlLinkElement(String href) : HtmlGenericElement("a", false)
    {
        this->href(href);
    }

    inline const HtmlLinkElement &href(String v)
    {
        appendAttr(new HtmlAttribute("href", v));
        return *this;
    }
};

#endif