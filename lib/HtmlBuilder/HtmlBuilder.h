#ifndef HTML_BUILDER_H
#define HTML_BUILDER_H

#include <forward_list>
#include <functional>

#include "HtmlElements.h"

class HtmlBuilder;

typedef std::function<void(HtmlBuilder *builder)> HtmlBuilderHandler;

class HtmlBuilder
{
  private:
    HtmlGenericElement *_elem;
    String _html;

    forward_list<HtmlBuilder *> buildersToDealloc;

    HtmlBuilder *CreateBuilder(HtmlGenericElement *elem)
    {
        auto builder = new HtmlBuilder(elem);

        buildersToDealloc.push_front(builder);

        return builder;
    }

    HtmlBuilder *CommonElement(HtmlGenericElement *elem, HtmlBuilderHandler h = nullptr)
    {
        auto builder = CreateBuilder(elem);

        if (h != nullptr)
            h(builder);

        _elem->append(elem);

        return builder;
    }

  public:
    HtmlBuilder(HtmlGenericElement *elem) : _elem(elem) {}

    virtual ~HtmlBuilder()
    {
        for (auto builder : buildersToDealloc)
            delete builder;
    }

    const String &build()
    {
        auto size = _elem->contentSize() + 1;

        _html.reserve(size);
        _elem->build(_html);

        return _html;
    }

    void raw(String html)
    {
        _elem->append(new HtmlRaw(html));
    }

    void text(String text)
    {
        _elem->append(new HtmlText(text));
    }

    void attr(String name)
    {
        _elem->appendAttr(new HtmlAttribute(name));
    }

    void attr(String name, String value)
    {
        _elem->appendAttr(new HtmlAttribute(name, value));
    }

    HtmlBuilder *head(HtmlBuilderHandler h = nullptr)
    {
        return CommonElement(new HtmlHeadElement(), h);
    }

    HtmlBuilder *body(HtmlBuilderHandler h = nullptr)
    {
        return CommonElement(new HtmlBodyElement(), h);
    }

    HtmlBuilder *div(HtmlBuilderHandler h = nullptr)
    {
        return CommonElement(new HtmlDivElement(), h);
    }

    HtmlBuilder *ul(HtmlBuilderHandler h = nullptr)
    {
        return CommonElement(new HtmlUlElement(), h);
    }

    HtmlBuilder *li(HtmlBuilderHandler h = nullptr)
    {
        return CommonElement(new HtmlLiElement(), h);
    }

    HtmlBuilder *br()
    {
        return CommonElement(new HtmlBrElement());
    }

    HtmlBuilder *a(String href, String text)
    {
        auto elem = new HtmlLinkElement();
        elem->href(href);
        elem->append(new HtmlText(text));
        return CommonElement(elem);
    }

    HtmlBuilder *a(String href, HtmlBuilderHandler h = nullptr)
    {
        auto elem = new HtmlLinkElement();
        elem->href(href);
        return CommonElement(elem, h);
    }

    HtmlBuilder *label(HtmlBuilderHandler h = nullptr)
    {
        return CommonElement(new HtmlLabelElement(), h);
    }

    HtmlBuilder *label(String text)
    {
        auto elem = new HtmlLabelElement();
        elem->append(new HtmlText(text));
        return CommonElement(elem);
    }

    HtmlBuilder *form(String action, String method, HtmlBuilderHandler h = nullptr)
    {
        auto elem = new HtmlFormElement();
        elem->appendAttr(new HtmlAttribute("action", action));
        elem->appendAttr(new HtmlAttribute("method", method));
        return CommonElement(elem, h);
    }

    HtmlBuilder *input(String type, String name, String value = "")
    {
        auto elem = new HtmlInputElement();
        elem->appendAttr(new HtmlAttribute("type", type));
        elem->appendAttr(new HtmlAttribute("name", name));
        elem->appendAttr(new HtmlAttribute("value", value));
        return CommonElement(elem);
    }
};

#endif