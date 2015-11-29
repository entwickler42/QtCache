#include "qtcachexmlobject.h"

using namespace QtCacheXml;

Object::Object()
{
}

QString Object::fromType(Type t)
{
    switch (t)
    {
    case Object::GBL: return "GBL";
    case Object::CLS: return "CLS";
    case Object::MAC: return "MAC";
    case Object::INT: return "INT";
    case Object::INC: return "INC";
    default: return "UNKNOWN";
    }
}

Object::Type Object::fromString(QStringRef typeString)
{
    if (typeString.compare("GBL", Qt::CaseInsensitive) == 0) return Object::GBL;
    if (typeString.compare("CLS", Qt::CaseInsensitive) == 0) return Object::CLS;
    if (typeString.compare("MAC", Qt::CaseInsensitive) == 0) return Object::MAC;
    if (typeString.compare("INT", Qt::CaseInsensitive) == 0) return Object::INT;
    if (typeString.compare("INC", Qt::CaseInsensitive) == 0) return Object::INC;
    return Object::UNKNOWN;
}

Object::List Object::select(const List& source, Types types)
{
    List selected_objects;
    foreach(const Object& i, source){
        if ((i.m_type & types) == types){ selected_objects.append(i); }
    }
    return selected_objects;
}
