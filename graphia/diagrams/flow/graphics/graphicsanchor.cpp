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

#include "graphicsanchor.h"
#include "graphia.h"
#include <QBrush>

CGraphicsAnchor::CGraphicsAnchor(qreal x, qreal y, qreal w, qreal h, QGraphicsItem* parent)
  :QGraphicsRectItem(x, y, w, h, parent)
{
  setZValue(95.0); //TODO is it necessary since selection change the zvalue ?
  QBrush brush(Qt::green, Qt::SolidPattern);
  setBrush(brush);
  setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges); //TODO is necessary ?
  //setCursor(Qt::SizeFDiagCursor);
  //setAcceptHoverEvents(true);
}

CGraphicsAnchor::CGraphicsAnchor(const QPointF& topLeft, QGraphicsItem* parent) 
  :QGraphicsRectItem(parent)
{
  setZValue(95.0);
  QBrush brush(Qt::green, Qt::SolidPattern);
  setBrush(brush);
  setRect(topLeft.x(),topLeft.y(), 
          getApp()->getGraphicsConfig()->anchorSize(),
          getApp()->getGraphicsConfig()->anchorSize());
  //setCursor(Qt::SizeFDiagCursor);  
  //setAcceptHoverEvents(true);
  
}

CGraphicsAnchor::CGraphicsAnchor(QGraphicsItem* parent): QGraphicsRectItem(parent)
{
  setZValue(95.0);
  QBrush brush(Qt::green, Qt::SolidPattern);
  setBrush(brush);
  //setCursor(Qt::SizeFDiagCursor);
  //setAcceptHoverEvents(true);
}

void CGraphicsAnchor::setTopLeft(qreal x, qreal y)
{
  setRect(x, y, 
          getApp()->getGraphicsConfig()->anchorSize(),
          getApp()->getGraphicsConfig()->anchorSize());
}

void CGraphicsAnchor::setTopLeft(const QPointF& topLeft)
{
  setRect(topLeft.x(), topLeft.y(),
          getApp()->getGraphicsConfig()->anchorSize(),
          getApp()->getGraphicsConfig()->anchorSize());
}

CGraphicsAnchor::~CGraphicsAnchor()
{

}
