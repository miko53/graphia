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
#include "graphicstextitem.h"
#include "graphicswriter.h"
#include "graphia.h"
#include <QFont>
#include <QStyleOptionGraphicsItem>
#include <QStyle>
#include <QTextDocument>
#include <QTextFrame>
#include <QPainter>
#include <QTextCursor>


CGraphicsTextItem::CGraphicsTextItem(QPointF& point, QGraphicsItem* parent):
 QGraphicsTextItem(parent), CGraphicsItem(NULL)
{
  setTextInteractionFlags(Qt::TextEditorInteraction);
  setFlags(flags() | ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
  setZValue(80.0);

  setFont(getApp()->getGraphicsConfig()->font());
  setDefaultTextColor(getApp()->getGraphicsConfig()->fontColor());
  m_alignmentH = getApp()->getGraphicsConfig()->alignmentH();
  
  QTextFrameFormat newFormat = document()->rootFrame()->frameFormat();
  newFormat.setMargin(7.0);
  newFormat.setPadding(0.);
  document()->rootFrame()->setFrameFormat(newFormat);

  setPos(point);
  
  setAcceptHoverEvents (true);

  m_itemProperties = IS_REMOVABLE; //TODO
  //when item is created and put into the scene, a click is transmitted to the item but in the edge,
  // so the focus is lost, the next boolean is used to avoid the behavior and to have the focus when
  // item is creating
  m_isFirstClick = true;

  d = new CGraphicsTextItemPrivate(this);
}

CGraphicsTextItem::~CGraphicsTextItem()
{
  if (d)
    delete d;
}


void CGraphicsTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  QStyleOptionGraphicsItem* opt = (QStyleOptionGraphicsItem*) option;


  if (opt->state & (QStyle::State_Selected | QStyle::State_HasFocus))
  {
    QPainterPath path;
    path.addRect(boundingRect());

    //QPainterPath docRect;
    QTextFrameFormat docFormat = document()->rootFrame()->frameFormat();

    path.addRect(boundingRect().adjusted(
      docFormat.leftMargin(),
      docFormat.topMargin(),
      -docFormat.rightMargin(),
      -docFormat.bottomMargin()));
    painter->fillPath(path,QBrush(Qt::Dense4Pattern));

  }
  
  opt->state &= ~(QStyle::State_Selected | QStyle::State_HasFocus);
  QGraphicsTextItem::paint(painter, opt, widget);

}



bool CGraphicsTextItem::associate(CGraphicsItem* item, CGraphicsItem::LinkOrder order)
{
  Q_UNUSED(item);
  Q_UNUSED(order);
  qDebug("CGraphicsTextItem::associate");
  Q_ASSERT(false);
  return false;
}

bool CGraphicsTextItem::getIntersectionPoint(const QPointF& refPoint, QPointF& interPoint)
{
  Q_UNUSED(refPoint);
  Q_UNUSED(interPoint);
  return false;
}

void CGraphicsTextItem::linkTo(CGraphicsItem* item)
{
  Q_UNUSED(item);
}

bool CGraphicsTextItem::onLeftClickEvent(const QPointF& point)
{
  Q_UNUSED(point);
  return false;
}

void CGraphicsTextItem::onMouseEventOnAnchor(QGraphicsSceneMouseEvent* sceneEvent, CGraphicsAnchor* anchor)
{
  Q_UNUSED(sceneEvent);
  Q_UNUSED(anchor);
}

void CGraphicsTextItem::updateCurrentPoint(const QPointF& point)
{
  Q_UNUSED(point);
}

void CGraphicsTextItem::onPositionHasChanged(void)
{

}

void CGraphicsTextItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
  //qDebug("CGraphicsTextItem::hoverEnterEvent");
  QGraphicsTextItem::hoverEnterEvent(event);
}

void CGraphicsTextItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
  //qDebug("CGraphicsTextItem::hoverMoveEvent");
  //qDebug("CGraphicsTextItem::hoverMoveEvent isselected %i, hasFocus %i", isSelected(), hasFocus());
  
  if ((isSelected() == true) || (hasFocus() == true))
  {
    bool bIsEdge;
    bIsEdge = isInEdge(event->pos());
    if (bIsEdge == true)
    {
      //qDebug("setCursor(Qt::SizeAllCursor);");
      setCursor(Qt::SizeAllCursor);
    }
    else
    {
      //qDebug("setCursor(Qt::ArrowCursor);");
      setCursor(Qt::ArrowCursor);
    }
  }
  //qDebug("CGraphicsTextItem::hoverMoveEvent");
  //QGraphicsTextItem::hoverMoveEvent(event);
}

bool CGraphicsTextItem::isInEdge(const QPointF& pos)
{
  bool bIsEdge;
  QPainterPath path;
  QPainterPath docPath;
  QTextFrameFormat docFormat = document()->rootFrame()->frameFormat();

  path.addRect(boundingRect());
  path.addRect(
               boundingRect().adjusted(docFormat.leftMargin(),
                                       docFormat.topMargin(),
                                       -docFormat.rightMargin(),
                                       -docFormat.bottomMargin())
              );
  bIsEdge = path.contains(pos);
  return bIsEdge;
}


void CGraphicsTextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
  //qDebug("CGraphicsTextItem::hoverLeaveEvent");
  setCursor(Qt::ArrowCursor);
  QGraphicsTextItem::hoverLeaveEvent(event);
}


void CGraphicsTextItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  QGraphicsTextItem::mousePressEvent(event);

  if (m_isFirstClick == true)
  {
    m_isFirstClick = false;
  }
  else
  {
    if (isInEdge(event->pos()))
    {
      //qDebug("click on Edge, focus is loose");
      clearFocus();
    }
  }

}

#include <QDebug>

QVariant CGraphicsTextItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
  qDebug() << change;
  if (change == ItemPositionHasChanged)
  {
    emit itemChanged(this, CPropertyDefinition::POS, pos());
  }

  return QGraphicsItem::itemChange(change, value);
}

void CGraphicsTextItem::updateAlignment()
{
    //alignment_ = alignment;
    QTextBlockFormat format;
    format.setAlignment((Qt::Alignment) m_alignmentH);
    QTextCursor cursor = textCursor();
    int position = textCursor().position();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    cursor.setPosition(position);
    setTextCursor(cursor);
}

QList<CPropertyDefinition::Type> CGraphicsTextItem::getListOfGraphicalProperties(void)
{
  QList<CPropertyDefinition::Type> list;

  list << CPropertyDefinition::POS;
  list << CPropertyDefinition::FONT;
  list << CPropertyDefinition::FONT_COLOR;
  list << CPropertyDefinition::ALIGNMENT_H;

  return list;
}

QVariant CGraphicsTextItem::getGraphicalProperties(CPropertyDefinition::Type type)
{
  QVariant property;

  switch(type)
  {
    case CPropertyDefinition::POS:
    property.setValue(pos());
    break;

    case CPropertyDefinition::FONT:
    property.setValue(font());
    break;

    case CPropertyDefinition::FONT_COLOR:
    property.setValue(defaultTextColor());
    break;

    case CPropertyDefinition::ALIGNMENT_H:
    property.setValue((int)CPropertyDefinition::intToAlignmentH(m_alignmentH));
    break;

    default:
    Q_ASSERT(false);
    break;
  }

  return property;
}

void CGraphicsTextItem::setGraphicalProperties(CPropertyDefinition::Type type, QVariant& value)
{
  switch (type)
  {
    case CPropertyDefinition::POS:
    setPos(value.value<QPointF>());
    break;

    case CPropertyDefinition::FONT:
    setFont(value.value<QFont>());
    break;

    case CPropertyDefinition::FONT_COLOR:
    setDefaultTextColor(value.value<QColor>());
    break;

    case CPropertyDefinition::ALIGNMENT_H:
    m_alignmentH = CPropertyDefinition::alignementHToInt(
              (CPropertyDefinition::alignHorizontal) value.value<int>());
    updateAlignment();
    break;

    default:
    Q_ASSERT(false);
    break;
  }
}


void CGraphicsTextItem::insertItemIntoDomDocument(QDomDocument& doc, QDomElement& element)
{
  CGraphicsWriter writer(this);
  writer.buildDomDocument(doc,element);
}

void CGraphicsTextItem::setAndUpdateGraphicalData ( void )
{
  //After set plain text, the frame is modified and must be updated again
  m_isFirstClick = false;
  
  QTextFrameFormat newFormat = document()->rootFrame()->frameFormat();
  newFormat.setMargin(7.0);
  newFormat.setPadding(0.);
  document()->rootFrame()->setFrameFormat(newFormat);
}

void CGraphicsTextItem::mouseMoveEvent ( QGraphicsSceneMouseEvent* event )
{
  QGraphicsItem::mouseMoveEvent ( event );
  CGraphicsScene* theScene = (CGraphicsScene*) scene();
  
  theScene->checkItemLocation(this);
}

