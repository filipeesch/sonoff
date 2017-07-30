#include "HtmlGenericElement.h"

HtmlGenericElement::HtmlGenericElement(
    const String tagName,
    bool selfClosing) : _tagName(tagName), _selfClosing(selfClosing)
{
}

HtmlGenericElement::~HtmlGenericElement()
{
    for (auto it = _attrs.begin(); it != _attrs.end(); ++it)
        delete *it;

    for (auto it = _children.begin(); it != _children.end(); ++it)
        delete *it;
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

    for (auto it = _attrs.begin(); it != _attrs.end(); ++it)
        size += (*it)->contentSize() + 1;

    if (_selfClosing)
        return size + 2;

    for (auto it = _children.begin(); it != _children.end(); ++it)
        size += (*it)->contentSize();

    return size + _tagName.length() + 4;
}

void HtmlGenericElement::build(String &html)
{
    html.concat("<");
    html.concat(_tagName);

    for (auto it = _attrs.begin(); it != _attrs.end(); ++it)
    {
        html.concat(" ");
        (*it)->build(html);
    }

    if (_selfClosing)
    {
        html.concat("/>");
        return;
    }

    html.concat(">");

    for (auto it = _children.begin(); it != _children.end(); ++it)
        (*it)->build(html);

    html.concat("</");
    html.concat(_tagName);
    html.concat(">");
}