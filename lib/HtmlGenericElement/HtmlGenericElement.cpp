#include "HtmlGenericElement.h"

HtmlGenericElement::HtmlGenericElement(
    const String tagName,
    bool selfClosing) : _tagName(tagName), _selfClosing(selfClosing)
{
}

HtmlGenericElement::~HtmlGenericElement()
{
    for (auto i : _attrs)
        delete i;

    for (auto i : _children)
        delete i;
}

HtmlGenericElement *HtmlGenericElement::id(String value)
{
    appendAttr(new HtmlAttribute("id", value));
    return this;
}

HtmlGenericElement *HtmlGenericElement::name(String value)
{
    appendAttr(new HtmlAttribute("name", value));
    return this;
}

HtmlGenericElement *HtmlGenericElement::appendAttr(HtmlAttribute *attr)
{
    _attrs.push_back(attr);
    return this;
}

HtmlGenericElement *HtmlGenericElement::append(HtmlContent *element)
{
    _children.push_back(element);
    return this;
}

int HtmlGenericElement::contentSize()
{
    int size = _tagName.length() + 1;

    for (auto attr : _attrs)
        size += attr->contentSize() + 1;

    if (_selfClosing)
        return size + 2;

    for (auto child : _children)
        size += child->contentSize();

    return size + _tagName.length() + 4;
}

void HtmlGenericElement::build(String &html)
{
    html.concat("<");
    html.concat(_tagName);

    for (auto attr : _attrs)
    {
        html.concat(" ");
        attr->build(html);
    }

    if (_selfClosing)
    {
        html.concat("/>");
        return;
    }

    html.concat(">");

    for (auto child : _children)
        child->build(html);

    html.concat("</");
    html.concat(_tagName);
    html.concat(">");
}