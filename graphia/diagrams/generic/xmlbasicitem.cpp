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
#include "xmlbasicitem.h"

void CXmlBasicItem::insertQColor ( QDomDocument& doc, QDomElement& fontColor, const QColor& color )
{
  QDomElement red = doc.createElement("red");
  fontColor.setAttribute("type", "QColor");
  fontColor.appendChild(red);
  QDomText redValue = doc.createTextNode(QString::number(color.red()));
  red.appendChild(redValue);
  
  QDomElement green = doc.createElement("green");
  fontColor.appendChild(green);
  QDomText greenValue = doc.createTextNode(QString::number(color.green()));
  green.appendChild(greenValue);
  
  QDomElement blue = doc.createElement("blue");
  fontColor.appendChild(blue);
  QDomText blueValue = doc.createTextNode(QString::number(color.blue()));
  blue.appendChild(blueValue);
  
  QDomElement alpha = doc.createElement("alpha");
  fontColor.appendChild(alpha);
  QDomText alphaValue = doc.createTextNode(QString::number(color.alpha()));
  alpha.appendChild(alphaValue);
}

void CXmlBasicItem::insertInt ( QDomDocument& doc, QDomElement& element, const int integer )
{
  element.setAttribute("type", "integer");
  QDomText data = doc.createTextNode(QString::number(integer));
  element.appendChild(data);
}

void CXmlBasicItem::insertQReal ( QDomDocument& doc, QDomElement& element, const qreal number )
{
  element.setAttribute("type", "qreal");
  QDomText data = doc.createTextNode(QString::number(number));
  element.appendChild(data);
}

void CXmlBasicItem::insertHexaNumber ( QDomDocument& doc, QDomElement& element, const int hexaNumber )
{
  element.setAttribute("type", "hex");
  QDomText data = doc.createTextNode(QString::number(hexaNumber, 16));
  element.appendChild(data);
}


void CXmlBasicItem::insertQFont ( QDomDocument& doc, QDomElement& element, const QFont& font )
{
  element.setAttribute("type", "QFont");
  QDomElement fontFamily = doc.createElement("family");
  element.appendChild(fontFamily);
    QDomText fontFamilyValue = doc.createTextNode(font.family());
    fontFamily.appendChild(fontFamilyValue);
  QDomElement fontPointSize = doc.createElement("pointSize");
  element.appendChild(fontPointSize);
    QDomText fontPointSizeValue = doc.createTextNode(QString::number(font.pointSize()));
    fontPointSize.appendChild(fontPointSizeValue);
  QDomElement fontWeight = doc.createElement("weight");
  element.appendChild(fontWeight);
    QDomText fontWeightValue = doc.createTextNode(QString::number(font.weight()));
    fontWeight.appendChild(fontWeightValue);
  QDomElement fontItalic = doc.createElement("italic");
  element.appendChild(fontItalic);
    QDomText fontItalicValue = doc.createTextNode(QString::number(font.italic()));
    fontItalic.appendChild(fontItalicValue);
  QDomElement fontUnderline = doc.createElement("underline");
  element.appendChild(fontUnderline);
    QDomText fontUnderlineValue = doc.createTextNode(QString::number(font.underline()));
    fontUnderline.appendChild(fontUnderlineValue);
  QDomElement fontStrikeout = doc.createElement("strikeout");
  element.appendChild(fontStrikeout);
    QDomText fontStrikeoutValue = doc.createTextNode(QString::number(font.strikeOut()));
    fontStrikeout.appendChild(fontStrikeoutValue);
  QDomElement fontKerning = doc.createElement("kerning");
  element.appendChild(fontKerning);
    QDomText fontKerningValue = doc.createTextNode(QString::number(font.kerning()));
    fontKerning.appendChild(fontKerningValue);
}


void CXmlBasicItem::insertQRectF ( QDomDocument& doc, QDomElement& element, const QRectF& rect )
{
  element.setAttribute("type", "QRectF");
  
  QDomElement posx = doc.createElement("posx");
  element.appendChild(posx);
  QDomText posxValue = doc.createTextNode(QString::number(rect.x()));
  posx.appendChild(posxValue);
  QDomElement posy = doc.createElement("posy");
  element.appendChild(posy);
  QDomText posyValue = doc.createTextNode(QString::number(rect.y()));
  posy.appendChild(posyValue);
  
  QDomElement width = doc.createElement("width");
  element.appendChild(width);
  QDomText widthValue = doc.createTextNode(QString::number(rect.width()));
  width.appendChild(widthValue);
  QDomElement height = doc.createElement("height");
  element.appendChild(height);
  QDomText heightValue = doc.createTextNode(QString::number(rect.height()));
  height.appendChild(heightValue);  
}


void CXmlBasicItem::insertQPointF ( QDomDocument& doc, QDomElement& element, const QPointF& point )
{
  element.setAttribute("type", "QPointF");
  QDomElement posx = doc.createElement("posx");
  element.appendChild(posx);
  QDomText posxValue = doc.createTextNode(QString::number(point.x()));
  posx.appendChild(posxValue);
  
  QDomElement posy = doc.createElement("posy");
  element.appendChild(posy);
  QDomText posyValue = doc.createTextNode(QString::number(point.y()));
  posy.appendChild(posyValue); 
}



QRectF CXmlBasicItem::decodeQRectF ( QDomElement& element, bool* bOk )
{
  QRectF rect;
  *bOk = false;
  
  QString type =  element.attribute("type");
  if (type == "QRectF")
  {
    QDomElement posX = element.firstChildElement("posx");
    QDomElement posY = element.firstChildElement("posy");
    QDomElement width = element.firstChildElement("width");
    QDomElement height = element.firstChildElement("height");
    
    if (!posX.isNull() && !posY.isNull() && !width.isNull() && !height.isNull())
    {
      //qDebug("scene found");
      bool bPosX, bPosY, bWidth, bHeight;
      rect.setX(posX.text().toDouble(&bPosX));
      rect.setY(posY.text().toDouble(&bPosY));
      rect.setWidth(width.text().toDouble(&bWidth));
      rect.setHeight(height.text().toDouble(&bHeight));
      
      if ((bPosX == true) && (bPosY == true) && (bWidth == true) && (bHeight == true))
      {
          *bOk = true;
      }
      
    }
  }
  return rect;
}

QPointF CXmlBasicItem::decodeQPointF(QDomElement& element, bool* bOk)
{
  QPointF point;
  QString type = element.attribute("type");
  *bOk = false;
  
  if (type == "QPointF")
  {
    QDomElement posX = element.firstChildElement("posx");
    QDomElement posY = element.firstChildElement("posy");
    bool bPosX, bPosY;
    if (!posX.isNull() && !posY.isNull())
    {
      point.setX(posX.text().toDouble(&bPosX));
      point.setY(posY.text().toDouble(&bPosY));
      
      if ((bPosX == true) && (bPosY == true))
        *bOk = true;
      
    }
  }
  
  return point;
}


qreal CXmlBasicItem::decodeQreal ( QDomElement& element, bool* bOk )
{
  *bOk = false;
  qreal value = 0.0;
  
  QString type = element.attribute("type");
  if (type == "qreal")
  {
    //qDebug("qreal found");
    value = element.text().toDouble(bOk);
  }
  
  return value;
}

int CXmlBasicItem::decodeInt ( QDomElement& element, int base, bool* bOk )
{
  int value;
  
  if (bOk != NULL)
    *bOk = false;
  
  QString type = element.attribute("type");
  if (((base == 10) && (type == "integer")) ||
      ((base == 16) && (type == "hex")))
  {
    //qDebug("qreal found");
    value = element.text().toInt(bOk, base);
  }
  
  return value;  
}


QFont CXmlBasicItem::decodeQFont ( QDomElement& element, bool* bOk )
{
  *bOk = false;
  QFont qFontRead;
  QString type;
  type = element.attribute("type");
  if (type == "QFont")
  {
    QDomElement family = element.firstChildElement("family");
    QDomElement pointSize = element.firstChildElement("pointSize");
    QDomElement weight = element.firstChildElement("weight");
    QDomElement italic = element.firstChildElement("italic");
    QDomElement underline = element.firstChildElement("underline");
    QDomElement strikeout = element.firstChildElement("strikeout");
    QDomElement kerning = element.firstChildElement("kerning");
    
    if (!family.isNull() && !pointSize.isNull() &&
        !weight.isNull() && !italic.isNull() && 
        !underline.isNull() && !strikeout.isNull() &&
        !kerning.isNull())
    {
      bool bPointSize, bWeight, bItalic, bUnderline, bStrikeOut, bKerning;

      qFontRead.setFamily(family.text());
      qFontRead.setPointSize(pointSize.text().toInt(&bPointSize));
      qFontRead.setWeight(weight.text().toInt(&bWeight));
      qFontRead.setItalic(italic.text().toInt(&bItalic));
      qFontRead.setUnderline(underline.text().toInt(&bUnderline));
      qFontRead.setStrikeOut(strikeout.text().toInt(&bStrikeOut));
      qFontRead.setKerning(kerning.text().toInt(&bKerning));
      
      if ((bPointSize == true) && (bWeight == true) &&
          (bItalic == true) && (bUnderline == true) &&
          (bStrikeOut == true) && (bKerning == true))
      {
        *bOk = true;
      }
    }
  }
  
  return qFontRead;
}


QColor CXmlBasicItem::decodeQColor ( QDomElement& element, bool* bOk )
{
  *bOk = false;
  QColor qColorRead;
  QString type;
  
  type = element.attribute("type");
  if (type == "QColor")
  {
    QDomElement red = element.firstChildElement("red");
    QDomElement green = element.firstChildElement("green");
    QDomElement blue = element.firstChildElement("blue");
    QDomElement alpha = element.firstChildElement("alpha");
    
    if (!red.isNull() && !green.isNull() &&
        !blue.isNull() && !alpha.isNull())
    {
      bool bRed, bGreen, bBlue, bAlpha;
      
      qColorRead.setRed(red.text().toInt(&bRed));
      qColorRead.setGreen(green.text().toInt(&bGreen));
      qColorRead.setBlue(blue.text().toInt(&bBlue));
      qColorRead.setAlpha(alpha.text().toInt(&bAlpha));
      
      if ((bRed == true) && (bGreen == true) &&
          (bBlue == true) && (bAlpha == true))
        *bOk = true;
    }
  }
  
  return qColorRead;
}
