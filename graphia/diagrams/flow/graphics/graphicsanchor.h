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

#ifndef GRAPHICSANCHOR_H
#define GRAPHICSANCHOR_H

#include <QGraphicsItem>


class CGraphicsAnchor : public QGraphicsRectItem
{
public:
  CGraphicsAnchor(qreal x, qreal y, qreal w, qreal h, QGraphicsItem* parent = 0);
  CGraphicsAnchor(const QPointF& topLeft, QGraphicsItem* parent = 0);
  CGraphicsAnchor(QGraphicsItem* parent = 0);
  virtual ~CGraphicsAnchor();

  enum { GraphicsAnchor = UserType + 2 };
  virtual int type() const { return GraphicsAnchor; };
  
  void setTopLeft(qreal x, qreal y);
  void setTopLeft(const QPointF& topLeft);

  void setPrivData(int privData) { m_privData = privData; }
  int privData() { return m_privData; }
  
protected:
  int m_privData;

private:
};

#endif // GRAPHICSANCHOR_H
