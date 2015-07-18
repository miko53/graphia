/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

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

#include "graphicsrectselection.h"
#include <QPen>

CGraphicsRectSelection::CGraphicsRectSelection(QGraphicsItem* parent): QGraphicsRectItem(parent)
{
  QPen aPen;
  
  setZValue(100.0);
  aPen = pen();

  aPen.setColor(Qt::darkGreen);
  setPen(aPen);
}

void CGraphicsRectSelection::setFirstPoint(const QPointF& point)
{
  QRectF selectionRect;

  m_firstPoint = point;
  
  selectionRect = rect();
  selectionRect.setTopLeft(point);
  selectionRect.setSize(QSizeF(0,0));
  
  setRect(selectionRect);
}


void CGraphicsRectSelection::setMoveablePoint(const QPointF& point)
{
  QRectF selectionRect;

  selectionRect = rect();

  selectionRect.setTopLeft(m_firstPoint);
  selectionRect.setBottomRight(point);

  //qDebug("CGraphicsRectSelection- isValid = %i :", selectionRect.isValid());
  //qDebug("CGraphicsRectSelection height = %f :", selectionRect.height());
  //qDebug("CGraphicsRectSelection width= %f :", selectionRect.width());
 
  setRect(selectionRect.normalized());
}

QPainterPath CGraphicsRectSelection::path()
{
  QPainterPath path;
  path.addRect(rect());
  return path;
}


CGraphicsRectSelection::~CGraphicsRectSelection()
{

}
