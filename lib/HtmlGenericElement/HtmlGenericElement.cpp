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

    for (forward_list<HtmlGenericElement *>::const_iterator it = _children.begin(); it != _children.end(); ++it)
        delete *it;
}

void HtmlGenericElement::id(String value)
{
    appendAttr(new HtmlAttribute("id", value));
}

void HtmlGenericElement::name(String value)
{
    appendAttr(new HtmlAttribute("name", value));
}

void HtmlGenericElement::text(String value)
{
    _content = value;
}

void HtmlGenericElement::appendAttr(HtmlAttribute *attr)
{
    _attrs.push_front(attr);
}

void HtmlGenericElement::append(HtmlGenericElement *element)
{
    _children.push_front(element);
}

int HtmlGenericElement::contentSize()
{
    int size = _tagName.length() + 1;

    for (forward_list<HtmlAttribute *>::const_iterator it = _attrs.begin(); it != _attrs.end(); ++it)
        size += (*it)->contentSize() + 1;

    if (_selfClosing)
        return size + 2;

    for (forward_list<HtmlGenericElement *>::const_iterator it = _children.begin(); it != _children.end(); ++it)
        size += (*it)->contentSize();

    return size + _tagName.length() + _content.length() + 4;
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

    html.concat(_content);

    for (forward_list<HtmlGenericElement *>::const_iterator it = _children.begin(); it != _children.end(); ++it)
        (*it)->build(html);

    html.concat("</");
    html.concat(_tagName);
    html.concat(">");
}