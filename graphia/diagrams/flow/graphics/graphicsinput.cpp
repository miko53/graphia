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

#include "graphicsinput.h"
#include "graphia.h"
#include "diagrams/flow/model/input.h"
#include <QPainter>
#include <QAction>
#include <QMenu>
#include <QDialog>
#include "diagrams/flow/dialog/dialog.h"
#include "graphicswriter.h"


CGraphicsInput::CGraphicsInput(QPointF& center, CItem* obj, QGraphicsItem* parent) :
  QGraphicsItem(parent), CGraphicsItem(obj)
{
  Q_ASSERT(m_item != NULL);

  m_font = getApp()->getGraphicsConfig()->font();
  m_fontColor = getApp()->getGraphicsConfig()->fontColor();
  m_lineColor = getApp()->getGraphicsConfig()->lineColor();

  setPos(center);

  m_offsetX = 0.0;//getApp()->getGraphicsConfig()->offsets().offsetX;
  m_offsetY = 0.0;//getApp()->getGraphicsConfig()->offsets().offsetY;
  m_anchor = new CGraphicsAnchor(this);

  updateAnchorPosition();
  m_anchor->setCursor(Qt::SizeAllCursor);
  m_anchor->setVisible(false);
   
  m_itemProperties = (CAN_BE_LINKED | IS_REMOVABLE);
  setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
  setZValue(50.0);
}


void CGraphicsInput::setAndUpdateGraphicalData ( void )
{
  //prepareGeometryChange();
  updateAnchorPosition();
  //update();
}


CGraphicsInput::~CGraphicsInput()
{
  if (m_anchor)
  {
    delete m_anchor;
    m_anchor = NULL;
  }
}

void CGraphicsInput::updateAnchorPosition(void)
{
  QRectF strRect;
  strRect = boundingRectString();

  QPointF anchorTop = strRect.bottomRight();
  anchorTop.setX(anchorTop.x() + m_offsetX);
  anchorTop.setY(anchorTop.y() + m_offsetY);

  m_anchor->setTopLeft(anchorTop);
}


void CGraphicsInput::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
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

  bottomLeft.setY(bottomLeft.y() + m_offsetY + anchorLenght);
  bottomRight.setX(bottomRight.x() + m_offsetX + anchorLenght);
  bottomRight.setY(bottomRight.y() + m_offsetY + anchorLenght);

  painter->setPen(m_lineColor);
  painter->drawLine(bottomLeft, bottomRight);

  painter->setPen(m_fontColor);
  painter->drawText(rectStr, Qt::AlignLeft|Qt::TextWrapAnywhere/*|Qt::TextWordWrap*/, getFlowName());

  //Debug, display of boundingRect
/*  QRectF rect = boundingRect();
  painter->drawRect(rect);
  rect = boundingRectString();
  painter->drawRect(rect);*/
  
}

QRectF CGraphicsInput::boundingRect() const
{
  qreal anchorSize = getApp()->getGraphicsConfig()->anchorSize();
  qreal PADDING = 1.0;
  QRectF rect;
  
  rect =boundingRectString();
  rect.adjust(0, 0,
              m_offsetX + anchorSize + PADDING,
              m_offsetY + anchorSize + PADDING);
  
  return rect;
}

QRectF CGraphicsInput::boundingRectString() const
{
  qreal PADDING = 1.0;
  QRectF rect;
  QFontMetricsF fontMetrics(m_font);
 
  rect.setWidth(fontMetrics.width(getFlowName()));
  rect.setHeight(fontMetrics.height());
  rect.adjust(0, 0, PADDING, PADDING);
  return rect;
}

QVariant CGraphicsInput::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
  if (change == ItemPositionHasChanged)
  {
    warmThatPosHasChanged();
    emit itemChanged(this, CPropertyDefinition::POS, pos());
  }
  else if (change == ItemSelectedChange)
  {
    m_anchor->setVisible(value.toBool());
    if (value.toBool() == true)
      setZValue(95.0);
    else
      setZValue(50.0);    
  }
  return QGraphicsItem::itemChange(change, value);
}

void CGraphicsInput::warmThatPosHasChanged(void)
{
  foreach(CGraphicsItem* item, m_listAttachedItem)
  {
    item->onPositionHasChanged();
  }
}

void CGraphicsInput::onMouseEventOnAnchor(QGraphicsSceneMouseEvent* sceneEvent, CGraphicsAnchor* anchor)
{
  Q_UNUSED(anchor);
  QPointF lastPoint;
  QPointF point;
  qreal dx;
  qreal dy;

  lastPoint = sceneEvent->lastScenePos();
  point = sceneEvent->scenePos();
  dx = point.x() - lastPoint.x();
  dy = point.y() - lastPoint.y();

  //et new offset.
  m_offsetX += dx;
  m_offsetY += dy;

  if (m_offsetX < 0)
    m_offsetX = 0.0;
  
  if (m_offsetY < 0)
    m_offsetY = 0.0;

  prepareGeometryChange();
  updateAnchorPosition();  
  update();
  warmThatPosHasChanged();  

  if (dx != 0)
  emit itemChanged(this, CPropertyDefinition::OFFSET_X, m_offsetX);

  if (dy != 0)
  emit itemChanged(this, CPropertyDefinition::OFFSET_Y, m_offsetY);
}

bool CGraphicsInput::associate(CGraphicsItem* item, CGraphicsItem::LinkOrder order)
{
  Q_UNUSED(item);
  Q_UNUSED(order);
  qDebug("CGraphicsInput::associate");

  Q_ASSERT(false);
  return false;
}

void CGraphicsInput::linkTo(CGraphicsItem* item)
{
  m_listAttachedItem.insert(item);
}

bool CGraphicsInput::getIntersectionPoint(const QPointF& refPoint, QPointF& interPoint)
{
  Q_UNUSED(refPoint);

  qreal anchorLenght = (getApp()->getGraphicsConfig()->anchorSize())/2;
  QRectF rectStr = boundingRectString();

  interPoint = rectStr.bottomRight();
  interPoint.setX(interPoint.x() + m_offsetX + anchorLenght);
  interPoint.setY(interPoint.y() + m_offsetY + anchorLenght);

  interPoint = mapToScene(interPoint);

  return true;
}

void CGraphicsInput::unlink(void)
{
  foreach(CGraphicsItem* item, m_listAttachedItem)
  {
    item->removeLinkTo(this);
  }    
}


void CGraphicsInput::removeLinkTo(CGraphicsItem* item)
{
  bool b;
  b = m_listAttachedItem.remove(item);
  Q_ASSERT(b == true);
}


QList<CPropertyDefinition::Type> CGraphicsInput::getListOfGraphicalProperties(void)
{
  QList<CPropertyDefinition::Type> list;

  list << CPropertyDefinition::NAME;
  list << CPropertyDefinition::POS;
  list << CPropertyDefinition::FONT;
  list << CPropertyDefinition::FONT_COLOR;
  list << CPropertyDefinition::LINE_COLOR;
  list << CPropertyDefinition::OFFSET_X;
  list << CPropertyDefinition::OFFSET_Y;
  list << CPropertyDefinition::FLOW_NAME;

  return list;
}

QVariant CGraphicsInput::getGraphicalProperties(CPropertyDefinition::Type type)
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
    property.setValue(m_offsetX);
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

void CGraphicsInput::setGraphicalProperties(CPropertyDefinition::Type type, QVariant& value)
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
    break;

    case CPropertyDefinition::OFFSET_X:
    m_offsetX = value.value<qreal>();
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

QString CGraphicsInput::getFlowName() const
{
  Q_ASSERT(m_item != NULL);
  
  CData* dataFlow = dynamic_cast<CInput*>(m_item)->getDataFlow();
  if (dataFlow == NULL)
  {
    return QString(tr("none"));
  }

  return dataFlow->getName();
}


void CGraphicsInput::updateDataFlow(CData* data)
{
  CInput* flow = dynamic_cast<CInput*>(m_item);
  flow->setDataFlow(data);
  emit itemChanged(this, CPropertyDefinition::FLOW_NAME, getFlowName());
}

void CGraphicsInput::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
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

void CGraphicsInput::mouseMoveEvent ( QGraphicsSceneMouseEvent* event )
{
  QGraphicsItem::mouseMoveEvent ( event );
  CGraphicsScene* theScene = (CGraphicsScene*) scene();
  
  theScene->checkItemLocation(this);
}


void CGraphicsInput::insertItemIntoDomDocument(QDomDocument& doc, QDomElement& element)
{
  CGraphicsWriter writer(this);
  writer.buildDomDocument(doc, element);  
}
