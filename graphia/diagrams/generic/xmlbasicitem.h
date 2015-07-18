/*
    graphia - little tool for building designer diagram
    Copyright (C) 2012  Mickael Sergent

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef XMLBASICITEM_H
#define XMLBASICITEM_H

#include <QDomElement>
#include <QColor>
#include <QFont>
#include <QRect>

class CXmlBasicItem
{
public:
  static void insertQColor(QDomDocument& doc, QDomElement& fontColor, const QColor& color);
  static void insertQFont(QDomDocument& doc, QDomElement& element, const QFont& font);
  static void insertQRectF(QDomDocument& doc, QDomElement& element, const QRectF& rect);
  static void insertQReal(QDomDocument& doc, QDomElement& element, const qreal number);
  static void insertQPointF(QDomDocument& doc, QDomElement& element, const QPointF& point);
  static void insertHexaNumber ( QDomDocument& doc, QDomElement& element, const int hexaNumber );
  static void insertInt(QDomDocument& doc, QDomElement& element, const int integer);
  
  
  static QRectF decodeQRectF(QDomElement& element, bool* bOk);
  static qreal decodeQreal(QDomElement& element, bool* bOk);
  static QPointF decodeQPointF(QDomElement& element, bool* bOk);
  static int decodeInt(QDomElement& element, int base = 10, bool* bOk = NULL);
  static QFont decodeQFont(QDomElement& element, bool* bOk);
  static QColor decodeQColor(QDomElement& element, bool* bOk);
  
};

#endif // XMLBASICITEM_H
