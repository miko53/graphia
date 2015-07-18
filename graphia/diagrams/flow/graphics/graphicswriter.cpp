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


#include "graphicswriter.h"
#include "diagrams/generic/xmlbasicitem.h"

CGraphicsWriter::CGraphicsWriter(CGraphicsCircle* item)
{
  m_item.circleItem = item;
  m_type = CIRCLE;
}

CGraphicsWriter::CGraphicsWriter ( CGraphicsArrow* item )
{
  m_item.arrowItem = item;
  m_type = ARROW;
}

CGraphicsWriter::CGraphicsWriter (CGraphicsRect *item)
{
    m_item.terminatorItem = item;
    m_type = TERMINATOR;
}

CGraphicsWriter::CGraphicsWriter (CGraphicsDoubleLine *item)
{
    m_item.storeItem = item;
    m_type = STORE;
}

CGraphicsWriter::CGraphicsWriter(CGraphicsInput* item)
{
  m_item.inputItem = item;
  m_type = INPUT;
}

CGraphicsWriter::CGraphicsWriter(CGraphicsOutput* item)
{
  m_item.outputItem = item;
  m_type = OUTPUT;
}

CGraphicsWriter::CGraphicsWriter(CGraphicsTextItem* item)
{
  m_type = TEXTITEM;
  m_item.textItem = item;
}


CGraphicsWriter::~CGraphicsWriter()
{

}

void CGraphicsWriter::buildDomDocument ( QDomDocument& doc, QDomElement& element )
{
  QDomElement gobj = doc.createElement("gobject");
  element.appendChild(gobj);

  switch (m_type)
  {
    case CIRCLE:
      gobj.setAttribute("type", "graphicscircle");
      gobj.setAttribute("id", m_item.circleItem->getID());
      buildDomDocumentOfCircle(doc, gobj);
      break;
      
    case ARROW:
      gobj.setAttribute("type", "graphicsarrow");
      gobj.setAttribute("id", m_item.arrowItem->getID());
      buildDomDocumentOfArrow(doc, gobj);
      break;

    case TERMINATOR:
      gobj.setAttribute("type", "graphicsrect");
      gobj.setAttribute("id", m_item.terminatorItem->getID());
      buildDomDocumentOfTerminator(doc, gobj);
      break;

    case STORE:
      gobj.setAttribute("type", "graphicsdoubleline");
      gobj.setAttribute("id", m_item.storeItem->getID());
      buildDomDocumentOfStore(doc, gobj);
      break;
      
    case INPUT:
      gobj.setAttribute("type", "graphicsinput");
      gobj.setAttribute("id", m_item.storeItem->getID());
      buildDomDocumentOfInput(doc, gobj);
      break;
      
    case OUTPUT:
      gobj.setAttribute("type", "graphicsoutput");
      gobj.setAttribute("id", m_item.storeItem->getID());
      buildDomDocumentOfOutput(doc, gobj);
      break;
      
    case TEXTITEM:
      gobj.setAttribute("type", "textitem");
      gobj.setAttribute("id", m_item.textItem->getID());
      buildDomDocumentOfTextItem(doc, gobj);
      break;
      
    default:
      qDebug("TODO");
      break;
  }  
}

void CGraphicsWriter::buildDomDocumentOfCircle ( QDomDocument& doc, QDomElement& gobj )
{
  
  QDomElement radius = doc.createElement("radius");
  radius.setAttribute("value", m_item.circleItem->m_radius);
  gobj.appendChild(radius);
  
  
  QDomElement position = doc.createElement("position");
  CXmlBasicItem::insertQPointF(doc, position, m_item.circleItem->pos());
  gobj.appendChild(position);
  
  QDomElement font = doc.createElement("font");
  gobj.appendChild(font);
  CXmlBasicItem::insertQFont(doc, font, m_item.circleItem->m_font);
  
  QDomElement fontColor = doc.createElement("fontColor");
  gobj.appendChild(fontColor);
  CXmlBasicItem::insertQColor(doc, fontColor, m_item.circleItem->m_fontColor);
  
  QDomElement lineColor = doc.createElement("lineColor");
  gobj.appendChild(lineColor);
  CXmlBasicItem::insertQColor(doc, lineColor, m_item.circleItem->m_lineColor);
  
  QDomElement backgroundColor = doc.createElement("backgroundColor");
  gobj.appendChild(backgroundColor);
  CXmlBasicItem::insertQColor(doc, backgroundColor, m_item.circleItem->m_brush.color());
  
  QDomElement alignmentH = doc.createElement("alignmentH");
  gobj.appendChild(alignmentH);
  CXmlBasicItem::insertHexaNumber(doc, alignmentH, m_item.circleItem->m_alignmentH);
  
  QDomElement alignmentV = doc.createElement("alignmentV");
  gobj.appendChild(alignmentV);
  CXmlBasicItem::insertHexaNumber(doc, alignmentV, m_item.circleItem->m_alignmentV);  
}


void CGraphicsWriter::buildDomDocumentOfArrow ( QDomDocument& doc, QDomElement& gobj )
{
  QDomElement arrowAngle = doc.createElement("arrowAngle");
  CXmlBasicItem::insertInt(doc, arrowAngle, (int)(m_item.arrowItem->m_arrowAngle *180/3.141569));  
  gobj.appendChild(arrowAngle);

  QDomElement arrowLength = doc.createElement("arrowLength");
  CXmlBasicItem::insertQReal(doc, arrowLength, m_item.arrowItem->m_arrowLength);  
  gobj.appendChild(arrowLength);

  QDomElement labelDisplayed = doc.createElement("labelDisplayed");
  CXmlBasicItem::insertInt(doc, labelDisplayed, m_item.arrowItem->m_isLabelDisplayed);  
  gobj.appendChild(labelDisplayed);

  QDomElement font = doc.createElement("font");
  gobj.appendChild(font);
  CXmlBasicItem::insertQFont(doc, font, m_item.arrowItem->m_font);
  
  QDomElement fontColor = doc.createElement("fontColor");
  gobj.appendChild(fontColor);
  CXmlBasicItem::insertQColor(doc, fontColor, m_item.arrowItem->m_fontColor);
  
  QDomElement lineColor = doc.createElement("lineColor");
  gobj.appendChild(lineColor);
  CXmlBasicItem::insertQColor(doc, lineColor, m_item.arrowItem->m_color);
  
  QDomElement backgroundColor = doc.createElement("backgroundColor");
  gobj.appendChild(backgroundColor);
  CXmlBasicItem::insertQColor(doc, backgroundColor, m_item.arrowItem->m_brush.color());
  
  QDomElement labelRect = doc.createElement("labelRect");
  gobj.appendChild(labelRect);
  CXmlBasicItem::insertQRectF(doc, labelRect, m_item.arrowItem->m_labelRect);

  QDomElement listPoints = doc.createElement("point-list");
  gobj.appendChild(listPoints);
  for(int i = 0; i < m_item.arrowItem->m_points.count(); i++)
  {
      QDomElement pointElement = doc.createElement("point");
      listPoints.appendChild(pointElement);
      CXmlBasicItem::insertQPointF(doc, pointElement, m_item.arrowItem->m_points.at(i));
  }
}


void CGraphicsWriter::buildDomDocumentOfTerminator(QDomDocument& doc, QDomElement& gobj)
{
  QDomElement position = doc.createElement("position");
  CXmlBasicItem::insertQPointF(doc, position, m_item.terminatorItem->pos());
  gobj.appendChild(position);

  QDomElement rect = doc.createElement("rect");
  CXmlBasicItem::insertQRectF(doc, rect, m_item.terminatorItem->m_rect);
  gobj.appendChild(rect);

  QDomElement font = doc.createElement("font");
  gobj.appendChild(font);
  CXmlBasicItem::insertQFont(doc, font, m_item.terminatorItem->m_font);

  QDomElement fontColor = doc.createElement("fontColor");
  gobj.appendChild(fontColor);
  CXmlBasicItem::insertQColor(doc, fontColor, m_item.terminatorItem->m_fontColor);

  QDomElement lineColor = doc.createElement("lineColor");
  gobj.appendChild(lineColor);
  CXmlBasicItem::insertQColor(doc, lineColor, m_item.terminatorItem->m_lineColor);

  QDomElement backgroundColor = doc.createElement("backgroundColor");
  gobj.appendChild(backgroundColor);
  CXmlBasicItem::insertQColor(doc, backgroundColor, m_item.terminatorItem->m_brush.color());

  QDomElement alignmentH = doc.createElement("alignmentH");
  gobj.appendChild(alignmentH);
  CXmlBasicItem::insertHexaNumber(doc, alignmentH, m_item.terminatorItem->m_alignmentH);

  QDomElement alignmentV = doc.createElement("alignmentV");
  gobj.appendChild(alignmentV);
  CXmlBasicItem::insertHexaNumber(doc, alignmentV, m_item.terminatorItem->m_alignmentV);
}


void CGraphicsWriter::buildDomDocumentOfStore(QDomDocument &doc, QDomElement &gobj)
{
  QDomElement position = doc.createElement("position");
  CXmlBasicItem::insertQPointF(doc, position, m_item.storeItem->pos());
  gobj.appendChild(position);

  QDomElement rect = doc.createElement("rect");
  CXmlBasicItem::insertQRectF(doc, rect, m_item.storeItem->m_rect);
  gobj.appendChild(rect);

  QDomElement font = doc.createElement("font");
  gobj.appendChild(font);
  CXmlBasicItem::insertQFont(doc, font, m_item.storeItem->m_font);

  QDomElement fontColor = doc.createElement("fontColor");
  gobj.appendChild(fontColor);
  CXmlBasicItem::insertQColor(doc, fontColor, m_item.storeItem->m_fontColor);

  QDomElement lineColor = doc.createElement("lineColor");
  gobj.appendChild(lineColor);
  CXmlBasicItem::insertQColor(doc, lineColor, m_item.storeItem->m_lineColor);

  QDomElement backgroundColor = doc.createElement("backgroundColor");
  gobj.appendChild(backgroundColor);
  CXmlBasicItem::insertQColor(doc, backgroundColor, m_item.storeItem->m_brush.color());

  QDomElement alignmentH = doc.createElement("alignmentH");
  gobj.appendChild(alignmentH);
  CXmlBasicItem::insertHexaNumber(doc, alignmentH, m_item.storeItem->m_alignmentH);

  QDomElement alignmentV = doc.createElement("alignmentV");
  gobj.appendChild(alignmentV);
  CXmlBasicItem::insertHexaNumber(doc, alignmentV, m_item.storeItem->m_alignmentV);
}


void CGraphicsWriter::buildDomDocumentOfInput(QDomDocument& doc, QDomElement& gobj)
{
  QDomElement position = doc.createElement("position");
  CXmlBasicItem::insertQPointF(doc, position, m_item.inputItem->pos());
  gobj.appendChild(position);

  QDomElement font = doc.createElement("font");
  gobj.appendChild(font);
  CXmlBasicItem::insertQFont(doc, font, m_item.inputItem->m_font);

  QDomElement fontColor = doc.createElement("fontColor");
  gobj.appendChild(fontColor);
  CXmlBasicItem::insertQColor(doc, fontColor, m_item.inputItem->m_fontColor);

  QDomElement lineColor = doc.createElement("lineColor");
  gobj.appendChild(lineColor);
  CXmlBasicItem::insertQColor(doc, lineColor, m_item.inputItem->m_lineColor);

  QDomElement offsetX = doc.createElement("offsetX");
  gobj.appendChild(offsetX);
  CXmlBasicItem::insertQReal(doc, offsetX, m_item.inputItem->m_offsetX);

  QDomElement offsetY = doc.createElement("offsetY");
  gobj.appendChild(offsetY);
  CXmlBasicItem::insertQReal(doc, offsetY, m_item.inputItem->m_offsetY);
}

void CGraphicsWriter::buildDomDocumentOfOutput(QDomDocument& doc, QDomElement& gobj)
{
  QDomElement position = doc.createElement("position");
  CXmlBasicItem::insertQPointF(doc, position, m_item.outputItem->pos());
  gobj.appendChild(position);

  QDomElement font = doc.createElement("font");
  gobj.appendChild(font);
  CXmlBasicItem::insertQFont(doc, font, m_item.outputItem->m_font);

  QDomElement fontColor = doc.createElement("fontColor");
  gobj.appendChild(fontColor);
  CXmlBasicItem::insertQColor(doc, fontColor, m_item.outputItem->m_fontColor);

  QDomElement lineColor = doc.createElement("lineColor");
  gobj.appendChild(lineColor);
  CXmlBasicItem::insertQColor(doc, lineColor, m_item.outputItem->m_lineColor);

  QDomElement offsetX = doc.createElement("offsetX1");
  gobj.appendChild(offsetX);
  CXmlBasicItem::insertQReal(doc, offsetX, m_item.outputItem->m_offsetX1);

  QDomElement offsetY = doc.createElement("offsetY");
  gobj.appendChild(offsetY);
  CXmlBasicItem::insertQReal(doc, offsetY, m_item.outputItem->m_offsetY);
  
  QDomElement offsetX2 = doc.createElement("offsetX2");
  gobj.appendChild(offsetX2);
  CXmlBasicItem::insertQReal(doc, offsetX2, m_item.outputItem->m_offsetX2);
  
  QDomElement offsetYmin = doc.createElement("offsetYmin");
  gobj.appendChild(offsetYmin);
  CXmlBasicItem::insertQReal(doc, offsetYmin, m_item.outputItem->m_offsetYmin);
  
  QDomElement offsetArrowLenMin = doc.createElement("offsetArrowLenMin");
  gobj.appendChild(offsetArrowLenMin);
  CXmlBasicItem::insertQReal(doc, offsetArrowLenMin, m_item.outputItem->m_offsetArrowLenMin);

  QDomElement arrowAngle = doc.createElement("arrowAngle");
  CXmlBasicItem::insertInt(doc, arrowAngle, (int)(m_item.outputItem->m_arrowAngle * 180/3.141569));  
  gobj.appendChild(arrowAngle);

  QDomElement arrowLength = doc.createElement("arrowLength");
  CXmlBasicItem::insertQReal(doc, arrowLength, m_item.outputItem->m_arrowLength);  
  gobj.appendChild(arrowLength);
 
}


void CGraphicsWriter::buildDomDocumentOfTextItem(QDomDocument& doc, QDomElement& gobj)
{
  QDomElement position = doc.createElement("position");
  CXmlBasicItem::insertQPointF(doc, position, m_item.textItem->pos());
  gobj.appendChild(position);
  
  QDomElement data = doc.createElement("data");
  data.setAttribute("type", "string");

  QString content;
  content = m_item.textItem->toPlainText(); //m_item.textItem->toHtml();TODO
  QDomCDATASection dataSection = doc.createCDATASection(content);

  data.appendChild(dataSection);
  gobj.appendChild(data);
  
  QDomElement alignmentH = doc.createElement("alignmentH");
  gobj.appendChild(alignmentH);
  CXmlBasicItem::insertHexaNumber(doc, alignmentH, m_item.textItem->m_alignmentH);

  QDomElement font = doc.createElement("font");
  gobj.appendChild(font);
  CXmlBasicItem::insertQFont(doc, font, m_item.textItem->font());

  QDomElement fontColor = doc.createElement("fontColor");
  gobj.appendChild(fontColor);
  CXmlBasicItem::insertQColor(doc, fontColor, m_item.textItem->defaultTextColor());  
}



