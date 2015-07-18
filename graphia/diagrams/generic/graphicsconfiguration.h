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


#ifndef GRAPHICSCONFIGURATION_H
#define GRAPHICSCONFIGURATION_H

#include <QRect>
#include <QFont>
#include <QColor>
#include "global.h"

typedef struct
{
  qreal offsetX;
  qreal offsetY;
} connectorOffset;

class CGraphicsConfiguration
{
public:
  CGraphicsConfiguration();
  virtual ~CGraphicsConfiguration();

  QRectF sceneRect() { return m_sceneSize; }
  void setSceneRect ( const QRectF& sceneRect ) { m_sceneSize = sceneRect; }
  
  qreal circleRadius() { return m_circleRadius; }
  void setCircleRadius( qreal r ) { m_circleRadius = r; }
  
  qreal arrowAngle() { return ((m_arrowAngle * PI)/180); } // return in rad
  void setArrowAngle ( int i ) { m_arrowAngle = i; } // store in deg
  
  qreal arrowLength() { return m_arrowLength; }
  void setArrowLength ( qreal decodeQreal ) { m_arrowLength = decodeQreal; }
  
  qreal anchorSize() { return m_anchorSize; }
  void setAnchorSize ( qreal decodeQreal ) { m_anchorSize = decodeQreal; }
  
  qreal storeHeight() { return m_storeHeight; }
  void setStoreHeight ( qreal decodeQreal ) { m_storeHeight = decodeQreal; }
  
  qreal storeLength() { return m_storeLength; }
  void setStoreLength ( qreal decodeQreal ) { m_storeLength = decodeQreal; } 

  int alignmentH() { return m_alignmentH; }
  void setAlignmentH ( int decodeInt ) { m_alignmentH = decodeInt; }
  
  int alignmentV() { return m_alignmentV; }
  void setAlignmentV ( int decodeInt ) { m_alignmentV = decodeInt; }
  
  QFont font() { return m_font; }
  void setFont(const QFont& font) { m_font = font; }
  
  QColor fontColor() { return m_fontColor; }
  void setFontColor ( QColor decodeQColor ) { m_fontColor = decodeQColor; }
  
  QColor lineColor() { return m_lineColor; }
  void setLineColor ( QColor decodeQColor ) { m_lineColor = decodeQColor; }
  
  QColor background() { return m_background; }
  void setBackground ( QColor decodeQColor ) { m_background = decodeQColor; }
  
  connectorOffset offsets();
  bool displayLabel() { return m_displayLabel; }
  void setDisplayLabel ( int decodeInt ) { m_displayLabel = decodeInt; }

protected:
  QRectF m_sceneSize;
  qreal  m_circleRadius;
  int    m_arrowAngle;
  qreal  m_arrowLength;
  qreal  m_anchorSize;
  qreal  m_storeHeight;
  qreal  m_storeLength;
  int    m_alignmentH;
  int    m_alignmentV;
  bool   m_displayLabel;
  QFont  m_font;
  QColor m_fontColor;
  QColor m_lineColor;
  QColor m_background;
};

#endif // GRAPHICSCONFIGURATION_H
