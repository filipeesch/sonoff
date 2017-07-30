#include "HtmlGenericElement.h"

HtmlGenericElement::HtmlGenericElement(
    const String tagName,
    bool selfClosing) : _tagName(tagName), _selfClosing(selfClosing)
{
}

HtmlGenericElement::~HtmlGenericElement()
{
    for (forward_list<HtmlAttribute *>::const_iterator it = _attrs.begin(); it != _attrs.end(); ++it)
        delete *it;

    for (forward_list<HtmlContent *>::const_iterator it = _children.begin(); it != _children.end(); ++it)
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
    _attrs.push_front(attr);
    return this;
}

HtmlGenericElement *HtmlGenericElement::append(HtmlContent *element)
{
    _children.push_front(element);
    return this;
}

int HtmlGenericElement::contentSize()
{
    int size = _tagName.length() + 1;

    for (forward_list<HtmlAttribute *>::const_iterator it = _attrs.begin(); it != _attrs.end(); ++it)
        size += (*it)->contentSize() + 1;

    if (_selfClosing)
        return size + 2;

    for (forward_list<HtmlContent *>::const_iterator it = _children.begin(); it != _children.end(); ++it)
        size += (*it)->contentSize();

    return size + _tagName.length() + 4;
}

void HtmlGenericElement::build(String &html)
{
    html.concat("<");
    html.concat(_tagName);

    for (forward_list<HtmlAttribute *>::const_iterator it = _attrs.begin(); it != _attrs.end(); ++it)
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

    for (forward_list<HtmlContent *>::const_iterator it = _children.begin(); it != _children.end(); ++it)
        (*it)->build(html);

    html.concat("</");
    html.concat(_tagName);
    html.concat(">");
}