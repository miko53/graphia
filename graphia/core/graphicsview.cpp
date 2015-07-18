/*
 *
 *     graphia - little tool for building designer diagram
 *     Copyright (C) 2012  Mickael Sergent
 *
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
*/
#include "graphicsview.h"
#include <QWheelEvent>
#include <QPrinter>

CGraphicsView::CGraphicsView ( QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent)
{
  qDebug("CGraphicsView::CGraphicsView");
  setAlignment(Qt::AlignTop|Qt::AlignLeft);
  QPointF topleft = mapToScene(0, 0);
  QPointF bottomright = mapToScene(viewport()->width(), viewport()->height());
  
  QRectF viewportRect(topleft, bottomright);
  setResizeAnchor(QGraphicsView::AnchorViewCenter);
  ensureVisible(viewportRect);
}

void CGraphicsView::zoomNormal ( void )
{
  //qDebug("normal zoom");
  resetTransform();
  //print();
}


void CGraphicsView::zoomIn ( void )
{
  //qDebug("zoomIn");
  scale(1.25, 1.25);
}

void CGraphicsView::zoomOut ( void )
{
  //qDebug("zoomOut");
  scale(1 / 1.25, 1 / 1.25);
}



void CGraphicsView::wheelEvent ( QWheelEvent* event )
{
  //qDebug("CGraphicsView::wheelEvent");
  if (event->modifiers() == Qt::ControlModifier)
  {
    if (event->orientation() != Qt::Vertical)
    {
      event->ignore();
      return;
    }
    
    QGraphicsView::ViewportAnchor anchor = transformationAnchor();
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    
    if (event->delta() > 0)
    {
      zoomIn();
    }
    else
    {
      zoomOut();
    }
    
    setTransformationAnchor(anchor);    
    event->accept();
    return;
  }
  
  QGraphicsView::wheelEvent(event);
}


void CGraphicsView::print ( void )
{
  //TODO : example d'impression
  
  QPrinter printer(QPrinter::ScreenResolution);
  printer.setPageSize(QPrinter::A4);
  printer.setOutputFileName("/home/mickael/programmation/graphia/trunk/tests/essai.pdf");
  printer.setOutputFormat(QPrinter::PdfFormat);

  QPainter painter(&printer);
  painter.setRenderHint(QPainter::Antialiasing);

  render(&painter, QRectF(), sceneRect().toRect());
  
  //QPainterPath path = scene()->selectionArea();
  //render(&painter, QRectF(), path.boundingRect().toRect());
}



CGraphicsView::~CGraphicsView()
{
  qDebug("CGraphicsView::~CGraphicsView");
}
