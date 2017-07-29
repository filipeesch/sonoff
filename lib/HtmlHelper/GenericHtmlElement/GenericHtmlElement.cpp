#include "GenericHtmlElement.h"

GenericHtmlElement::GenericHtmlElement(
    const String &tagName,
    bool selfClosing,
    forward_list<HtmlAttribute> &attrs,
    forward_list<GenericHtmlElement> &children) : _tagName(tagNae), _selfClosing(selfClosing), _attrs(attrs), _children(children)
{
}

int GenericHtmlElement::contentSize()
{
    int size = _tagName.size() + 1;

    if (_attrs != nullptr)
    {
        for (forward_list<HtmlAttribute>::const_iterator it = _attrs.begin(); it != _attrs.end(); ++it)
            size += it->contentSize() + 1;
    }

    if (_selfClosing)
        return size + 2;

    if (_children != nullptr)
    {
        for (forward_list<GenericHtmlElement>::const_iterator it = _children.begin(); it != _children.end(); ++it)
            size += it->contentSize();
    }

    return size + _tagName.size() + 4;
}

void GenericHtmlElement::build(String &html)
{
    html.concat("<");
    html.concat(_tagName);

    if (_attrs != nullptr)
    {
        for (forward_list<HtmlAttribute>::const_iterator it = _attrs.begin(); it != _attrs.end(); ++it)
        {
            html.concat(" ");
            it->build(html);
        }
    }

    if (_selfClosing)
    {
        html.concat("/>");
        return;
    }

    html.concat(">");

    if (_children != nullptr)
    {
        for (forward_list<GenericHtmlElement>::const_iterator it = _children.begin(); it != _children.end(); ++it)
            it->build(html);
    }

    html.concat("</");
    html.concat(_tagName);
    html.concat(">");
}