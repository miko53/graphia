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

#include "graphicsoutput.h"
#include "graphia.h"
#include "global.h"
#include "diagrams/flow/model/output.h"
#include <QPainter>
#include <QAction>
#include <QMenu>
#include "diagrams/flow/dialog/dialog.h"
#include "graphicswriter.h"


CGraphicsOutput::CGraphicsOutput(QPointF& center, CItem* obj, QGraphicsItem* parent):
  QGraphicsItem(parent), CGraphicsItem(obj)
{
  Q_ASSERT(m_item != NULL);
  
  m_font = getApp()->getGraphicsConfig()->font();
  m_fontColor = getApp()->getGraphicsConfig()->fontColor();
  m_lineColor = getApp()->getGraphicsConfig()->lineColor();
  m_arrowBrush = QBrush(m_lineColor, Qt::SolidPattern);

  setPos(center);
  m_offsetX1 = 0.0;
  m_offsetY = 0.0;
  m_offsetX2 = 0.0;
  
  m_arrowAngle = getApp()->getGraphicsConfig()->arrowAngle();
  m_arrowLength = getApp()->getGraphicsConfig()->arrowLength();
  
  m_offsetArrowLenMin = getApp()->getGraphicsConfig()->offsets().offsetX;
  m_offsetYmin = getApp()->getGraphicsConfig()->offsets().offsetY;

  m_anchors[0] = new CGraphicsAnchor(this);
  m_anchors[1] = new CGraphicsAnchor(this);

  updateAnchorPosition();
  for(int i = 0; i < 2; i++)
  {
    m_anchors[i]->setCursor(Qt::SizeAllCursor);
    m_anchors[i]->setVisible(false);
    m_anchors[i]->setPrivData(i);
  }
  
  m_itemProperties = (CAN_BE_LINKED | IS_REMOVABLE);
  setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
  setZValue(50.0);
}

void CGraphicsOutput::updateAnchorPosition(void)
{
  QRectF rectString;
  QPointF topLeftAnchor1;  
  QPointF topLeftAnchor2;
  qreal anchorsize = getApp()->getGraphicsConfig()->anchorSize();
  
  rectString = boundingRectString();

  topLeftAnchor1 = rectString.bottomLeft();

  topLeftAnchor1.setX(topLeftAnchor1.x() - m_offsetX2 - anchorsize);
  topLeftAnchor1.setY(topLeftAnchor1.y() + m_offsetYmin + m_offsetY - anchorsize/2);
  
  topLeftAnchor2 = rectString.bottomRight();
  topLeftAnchor2.setX(topLeftAnchor2.x() + m_offsetX1 + m_offsetArrowLenMin - anchorsize/2);
  topLeftAnchor2.setY(topLeftAnchor2.y() + m_offsetYmin + m_offsetY - anchorsize/2);

  m_anchors[0]->setTopLeft(topLeftAnchor1);
  m_anchors[1]->setTopLeft(topLeftAnchor2); 
}

CGraphicsOutput::~CGraphicsOutput()
{
  for(int i=0; i<2; i++)
  {
    if (m_anchors[i] != NULL)
      delete m_anchors[i];
    m_anchors[i] = NULL;
  }
}

void CGraphicsOutput::setAndUpdateGraphicalData ( void )
{
  //prepareGeometryChange();
  updateAnchorPosition();
  //update();
}



QVariant CGraphicsOutput::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
  if (change == ItemPositionHasChanged)
  {
    warmThatPosHasChanged();
    emit itemChanged(this, CPropertyDefinition::POS, pos());
  }
  else if (change == ItemSelectedChange)
  {
    for(int i =0; i<2; i++)
    {
      m_anchors[i]->setVisible(value.toBool());
    }
    
    if (value.toBool() == true)
      setZValue(95.0);
    else
      setZValue(50.0);
  }
  return QGraphicsItem::itemChange(change, value);
}

void CGraphicsOutput::warmThatPosHasChanged(void)
{
  foreach(CGraphicsItem* item, m_listAttachedItem)
  {
    item->onPositionHasChanged();
  }
}


void CGraphicsOutput::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  qreal anchorLenght = (getApp()->getGraphicsConfig()->anchorSize())/2;
  QRectF rectStr = boundingRectString();

  painter->setRenderHint(QPainter::Antialiasing);
  painter->setRenderHint(QPainter::TextAntialiasing);
  painter->setFont(m_font);

  QPointF bottomLeft(rectStr.bottomLeft());
  QPointF bottomRight(rectStr.bottomRight());

  bottomLeft.setX(bottomLeft.x() - m_offsetX2 - anchorLenght);
  bottomLeft.setY(bottomLeft.y() + m_offsetY + m_offsetYmin);

  //no need to set X already OK.
  bottomRight.setX(bottomRight.x() + m_offsetX1);
  bottomRight.setY(bottomRight.y() + m_offsetY + m_offsetYmin);

  painter->setPen(m_fontColor);
  painter->drawText(rectStr, Qt::AlignLeft|Qt::TextWrapAnywhere/*|Qt::TextWordWrap*/, getFlowName());

  painter->setPen(m_lineColor);
  painter->drawLine(bottomLeft, bottomRight);

  //draw arrow
  QPolygonF arrow;
  QPointF arrowPoints[2];
  QPointF endPoint;

  endPoint = bottomRight;
  endPoint.setX(endPoint.x() + m_offsetArrowLenMin);
  
  arrowPoints[0].setX(endPoint.x() - m_arrowLength * cos(m_arrowAngle));
  arrowPoints[0].setY(endPoint.y() - m_arrowLength * sin(m_arrowAngle));
  arrowPoints[1].setX(endPoint.x() - m_arrowLength * cos(m_arrowAngle));
  arrowPoints[1].setY(endPoint.y() + m_arrowLength * sin(m_arrowAngle));

  //qDebug("ArrowPoint[0], %f, %f", arrowPoints[0].x(), arrowPoints[0].y());
  //qDebug("ArrowPoint[1], %f, %f", arrowPoints[1].x(), arrowPoints[1].y());
  //qDebug("endPoint, %f, %f", endPoint.x(), endPoint.y());
  arrow << endPoint << arrowPoints[0] << arrowPoints[1];
  painter->setBrush(m_arrowBrush);
  painter->drawPolygon(arrow);
  painter->setBrush(Qt::NoBrush);

  //Debug, display of boundingRect
  //QRectF rect = boundingRect();
  //painter->drawRect(rect);

  //rect = boundingRectString();
  //painter->drawRect(rect);
}

QRectF CGraphicsOutput::boundingRect() const
{
  qreal anchorSize = getApp()->getGraphicsConfig()->anchorSize();
  qreal PADDING = 1.0;
  QRectF rect;

  rect = boundingRectString();
  //qreal x1,x2,x3,x4;
  //rect.getCoords(&x1, &x2, &x3, &x4);
  //qDebug("rectString :%f, %f, %f, %f", x1, x2, x3, x4);

  rect.adjust(
              -(m_offsetX2 + anchorSize + PADDING),
              0,
              m_offsetX1 + getApp()->getGraphicsConfig()->offsets().offsetX + anchorSize/2 + PADDING,
              m_offsetY + 2*m_offsetYmin + PADDING);

  //qreal x1,x2,x3,x4;
  //rect.getCoords(&x1, &x2, &x3, &x4);
  //qDebug("rect :%f, %f, %f, %f", x1, x2, x3, x4);
  return rect;
}

QRectF CGraphicsOutput::boundingRectString() const
{
  qreal PADDING = 1.0;
  QRectF rect;
  QFontMetricsF fontMetrics(m_font);
 
  rect.setWidth(fontMetrics.width(getFlowName()));
  rect.setHeight(fontMetrics.height());
  rect.adjust(0, 0, PADDING, PADDING);
  return rect;
}

void CGraphicsOutput::onMouseEventOnAnchor(QGraphicsSceneMouseEvent* sceneEvent, CGraphicsAnchor* anchor)
{
  QPointF lastPoint;
  QPointF point;
  qreal dx;
  qreal dy;

  lastPoint = sceneEvent->lastScenePos();
  point = sceneEvent->scenePos();
  dx = point.x() - lastPoint.x();
  dy = point.y() - lastPoint.y();

  Q_ASSERT(anchor != NULL);
  switch (anchor->privData())
  {
    case 0:
      m_offsetX2 -= dx;
      m_offsetY += dy;
      break;
      
    case 1:
      m_offsetX1 += dx;
      m_offsetY += dy;      
      break;
      
    default:
      Q_ASSERT(false);
      break;
  }
  
  if (m_offsetX1 < 0)
    m_offsetX1 = 0.0;
  
  if (m_offsetX2 < 0)
    m_offsetX2 = 0.0;

  if (m_offsetY < 0)
    m_offsetY = 0.0;

  prepareGeometryChange();
  updateAnchorPosition();
  update();
  warmThatPosHasChanged();

  if (dy != 0.0)
    emit itemChanged(this, CPropertyDefinition::OFFSET_Y, m_offsetY);

  if (dx != 0.0)
  {
    switch (anchor->privData())
    {
      case 0:
        emit itemChanged(this, CPropertyDefinition::OFFSET_Xb, m_offsetX2);
        break;

      case 1:
        emit itemChanged(this, CPropertyDefinition::OFFSET_X, m_offsetX1);
        break;

      default:
        Q_ASSERT(false);
        break;
    }
  }
}

bool CGraphicsOutput::associate(CGraphicsItem* item, CGraphicsItem::LinkOrder order)
{
  Q_UNUSED(item);
  Q_UNUSED(order);
  qDebug("CGraphicsOutput::associate");
  Q_ASSERT(false);
  return false;
}


bool CGraphicsOutput::getIntersectionPoint(const QPointF& refPoint, QPointF& interPoint)
{
  Q_UNUSED(refPoint);
  QRectF rectStr = boundingRectString();
  qreal anchorLenght = getApp()->getGraphicsConfig()->anchorSize()/2;

  interPoint = rectStr.bottomLeft();

  interPoint.setX( interPoint.x() - m_offsetX2 - anchorLenght );
  interPoint.setY( interPoint.y() + m_offsetYmin + m_offsetY );
  
  interPoint = mapToScene(interPoint);
  
  return true;
}

void CGraphicsOutput::linkTo(CGraphicsItem* item)
{
  m_listAttachedItem.insert(item);
}


void CGraphicsOutput::unlink(void)
{
  foreach(CGraphicsItem* item, m_listAttachedItem)
  {
    item->removeLinkTo(this);
  }
}

void CGraphicsOutput::removeLinkTo(CGraphicsItem* item)
{
  bool b;
  b = m_listAttachedItem.remove(item);
  Q_ASSERT(b == true);
}




QList<CPropertyDefinition::Type> CGraphicsOutput::getListOfGraphicalProperties(void)
{
  QList<CPropertyDefinition::Type> list;

  list << CPropertyDefinition::NAME;
  list << CPropertyDefinition::POS;
  list << CPropertyDefinition::FONT;
  list << CPropertyDefinition::FONT_COLOR;
  list << CPropertyDefinition::LINE_COLOR;
  list << CPropertyDefinition::OFFSET_X;
  list << CPropertyDefinition::OFFSET_Y;
  list << CPropertyDefinition::OFFSET_Xb;
  list << CPropertyDefinition::FLOW_NAME;

  return list;
}

QVariant CGraphicsOutput::getGraphicalProperties(CPropertyDefinition::Type type)
{
  QVariant property;

  switch (type)
  {
    case CPropertyDefinition::FLOW_NAME:
      property.setValue(getFlowName());
    break;
    
    case CPropertyDefinition::NAME:
    property.setValue(m_item->getName());
    break;

    case CPropertyDefinition::POS:
    property.setValue(pos());
    break;

    case CPropertyDefinition::FONT:
    property.setValue(m_font);
    break;

    case CPropertyDefinition::FONT_COLOR:
    property.setValue(m_fontColor);
    break;

    case CPropertyDefinition::LINE_COLOR:
    property.setValue(m_lineColor);
    break;

    case CPropertyDefinition::OFFSET_X:
    property.setValue(m_offsetX1);
    break;

    case CPropertyDefinition::OFFSET_Xb:
    property.setValue(m_offsetX2);
    break;

    case CPropertyDefinition::OFFSET_Y:
    property.setValue(m_offsetY);
    break;
    
    default:
    Q_ASSERT(false);
    break;
  }

  return property;
}

void CGraphicsOutput::setGraphicalProperties(CPropertyDefinition::Type type, QVariant& value)
{
  prepareGeometryChange();

  switch (type)
  {
    case CPropertyDefinition::NAME:
    m_item->setName(value.toString());
    break;

    case CPropertyDefinition::POS:
    setPos(value.toPointF());
    break;

    case CPropertyDefinition::FONT:
    m_font = value.value<QFont>();
    break;

    case CPropertyDefinition::FONT_COLOR:
    m_fontColor = value.value<QColor>();
    break;

    case CPropertyDefinition::LINE_COLOR:
    m_lineColor = value.value<QColor>();
    m_arrowBrush.setColor(m_lineColor);
    break;

    case CPropertyDefinition::OFFSET_X:
    m_offsetX1 = value.value<qreal>();
    break;

    case CPropertyDefinition::OFFSET_Xb:
    m_offsetX2 = value.value<qreal>();
    break;

    case CPropertyDefinition::OFFSET_Y:
    m_offsetY = value.value<qreal>();
    break;
    
    case CPropertyDefinition::FLOW_NAME:
      //no possible ?
    break;
    
    default:
    Q_ASSERT(false);
    break;
  }

  updateAnchorPosition();
  update();
  warmThatPosHasChanged();
}

QString CGraphicsOutput::getFlowName() const
{
  Q_ASSERT(m_item != NULL);
  
  CData* dataFlow = dynamic_cast<COutput*>(m_item)->getDataFlow();
  if (dataFlow == NULL)
  {
    return QString(tr("none"));
  }

  return dataFlow->getName();
}


void CGraphicsOutput::updateDataFlow(CData* data)
{
  COutput* flow = dynamic_cast<COutput*>(m_item);
  flow->setDataFlow(data);
  emit itemChanged(this, CPropertyDefinition::FLOW_NAME, getFlowName());
}

void CGraphicsOutput::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
  CDocument* doc = getApp()->getDocument();
  QMenu menu;
  int result;
  QAction *selectData = menu.addAction(tr("select a data..."));
  QAction *addData = menu.addAction(tr("create and select a new data..."));
  
  QAction *selectedAction = menu.exec(event->screenPos());
  if (selectedAction == addData)
  {
    CDialogCreateData dialog(doc->getItemFactory()->getDataFactory(),
                             doc->getItemFactory()->getTypeFactory());
    result = dialog.exec();
    if (result == QDialog::Accepted)
    {
      CData* data = dialog.getDataFlowCreated();
      updateDataFlow(data);
    }
  }
  else if (selectedAction == selectData)
  {
    CDialogChooseData dialog(doc->getItemFactory()->getDataFactory(),
                             doc->getItemFactory()->getTypeFactory());
    result = dialog.exec();
    if (result == QDialog::Accepted)
    {
      //qDebug("accepted");
      CData* data = dialog.getChosenData();
      updateDataFlow(data);
    }
    
  }
  
  QGraphicsItem::contextMenuEvent(event);
}

void CGraphicsOutput::mouseMoveEvent ( QGraphicsSceneMouseEvent* event )
{
  QGraphicsItem::mouseMoveEvent ( event );
  CGraphicsScene* theScene = (CGraphicsScene*) scene();
  
  theScene->checkItemLocation(this);
}


void CGraphicsOutput::insertItemIntoDomDocument(QDomDocument& doc, QDomElement& element)
{
  CGraphicsWriter writer(this);
  writer.buildDomDocument(doc, element);
}
