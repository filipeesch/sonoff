#ifndef HTML_ELEMENTS_H
#define HTML_ELEMENTS_H

#include "HtmlGenericElement.h"
#include "HtmlText.h"
#include "HtmlRaw.h"

class HtmlTag : public HtmlGenericElement
{
  public:
    HtmlTag() : HtmlGenericElement("html", false) {}
    ~HtmlTag() {}
};

class HtmlHeadElement : public HtmlGenericElement
{
  public:
    HtmlHeadElement() : HtmlGenericElement("head", false) {}
    ~HtmlHeadElement() {}
};

class HtmlBodyElement : public HtmlGenericElement
{
  public:
    HtmlBodyElement() : HtmlGenericElement("body", false) {}
    ~HtmlBodyElement() {}
};

class HtmlBrElement : public HtmlGenericElement
{
  public:
    HtmlBrElement() : HtmlGenericElement("br", true) {}
    ~HtmlBrElement() {}
};

class HtmlDivElement : public HtmlGenericElement
{
  public:
    HtmlDivElement() : HtmlGenericElement("div", false) {}
    ~HtmlDivElement() {}
};

class HtmlUlElement : public HtmlGenericElement
{
  public:
    HtmlUlElement() : HtmlGenericElement("ul", false) {}
    ~HtmlUlElement() {}
};

class HtmlLiElement : public HtmlGenericElement
{
  public:
    HtmlLiElement() : HtmlGenericElement("li", false) {}
    ~HtmlLiElement() {}
};

class HtmlInputElement : public HtmlGenericElement
{
  public:
    HtmlInputElement() : HtmlGenericElement("input", true) {}
    ~HtmlInputElement() {}

    HtmlInputElement(String type, String name, String value = "") : HtmlGenericElement("input", true)
    {
        this->type(type);
        this->value(value);
        this->name(name);
    }

    const HtmlInputElement &type(String v)
    {
        appendAttr(new HtmlAttribute("type", v));
        return *this;
    }

    const HtmlInputElement &value(String v)
    {
        appendAttr(new HtmlAttribute("value", v));
        return *this;
    }
};

class HtmlLinkElement : public HtmlGenericElement
{
  public:
    HtmlLinkElement() : HtmlGenericElement("a", false) {}
    ~HtmlLinkElement() {}

    HtmlLinkElement(String href, String text = "") : HtmlGenericElement("a", false)
    {
        this->href(href);

        if (text != "")
            append(new HtmlText(text));
    }

    const HtmlLinkElement &href(String v)
    {
        appendAttr(new HtmlAttribute("href", v));
        return *this;
    }
};

#endif