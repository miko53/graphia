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

#include "decodegraphics.h"
#include "diagrams/generic/xmlbasicitem.h"
#include "diagrams/flow/graphics/graphicscircle.h"
#include "diagrams/flow/graphics/graphicsrect.h"
#include "diagrams/flow/graphics/graphicsdoubleline.h"
#include "diagrams/flow/graphics/graphicsinput.h"
#include "diagrams/flow/graphics/graphicsoutput.h"
#include "diagrams/flow/graphics/graphicstextitem.h"
#include "diagrams/flow/graphics/graphicsarrow.h"
#include "graphia.h"

CDecodeFlowGraphicsList::CDecodeFlowGraphicsList ( CItemFactory* itemFactory, QList< CGraphicsScene* >* list )
{
  m_itemFactory = itemFactory;
  m_listDiagram = list;
}

bool CDecodeFlowGraphicsList::decode ( const QDomElement& item )
{
  bool bOk = true;
  CGraphicsScene* scene;
  scene = readGraphicsScene(item);
  if (scene != NULL)
  {
    m_listDiagram->append(scene);
  }
  else 
    bOk = false;
  
  
  return bOk;
}

CGraphicsScene* CDecodeFlowGraphicsList::readGraphicsScene ( const QDomElement& diagram )
{
  CGraphicsScene* scene = NULL;
  QDomElement title = diagram.firstChildElement("title");
  QDomElement type = diagram.firstChildElement("type");
  QDomElement item = diagram.firstChildElement("item");
  QDomElement isDisplayed = diagram.firstChildElement("displayed");
  QDomElement rectScene = diagram.firstChildElement("scene");
  
  
  if (!title.isNull() && !type.isNull() && !item.isNull() && !rectScene.isNull())
  {
    scene = new CGraphicsScene();
    scene->setTitle(title.text());
    scene->setType(type.text().toInt());
    
    QRectF sceneRect;
    int typeOfItem;
    int IdOfItem;
    bool bOk;
    typeOfItem = item.attribute("type").toInt();
    IdOfItem = item.attribute("id").toInt();  
    
    sceneRect = CXmlBasicItem::decodeQRectF(rectScene, &bOk);
    if (bOk)
      scene->setSceneRect(sceneRect);
    else
    {
      qDebug("scene Rect unable to decode");
      delete scene;
      scene = NULL;
    }
    
    CItem* gitem;
    gitem = m_itemFactory->search(typeOfItem, IdOfItem);
    if ((gitem != NULL) && (scene != NULL))
    {
      scene->setItem(gitem);
    }
    else
    {
      qDebug("scene not found");
      delete scene;
      scene = NULL;
    }
    getApp()->getDocument()->buildAuthorizationList(scene);
    
    if (!isDisplayed.isNull() && (scene != NULL))
    {
      bool displayed;
      displayed = CXmlBasicItem::decodeInt(isDisplayed);
      scene->setDisplayState(displayed);
    }
    
    if (scene != NULL)
    {
      bool bCorrectlyDecoded;
      
      bCorrectlyDecoded = lookIntoDiagramElement(diagram, scene, dynamic_cast<CItemContainer*>(gitem), FIRST_PASS);
      if (bCorrectlyDecoded == true)
      {
        bCorrectlyDecoded = lookIntoDiagramElement(diagram, scene, dynamic_cast<CItemContainer*>(gitem), SECOND_PASS);
      }
      
      if (bCorrectlyDecoded == false)
      {
        delete scene;
        scene = NULL;
      }
    }
  }
  else
  {
    qDebug("unable to read scene");
  }

  return scene;
}

bool CDecodeFlowGraphicsList::lookIntoDiagramElement ( const QDomElement& diagram, CGraphicsScene* scene, CItemContainer* gitem, CDecodeFlowGraphicsList::ePassType passNo )
{
  //insert now the graphical component
  CGraphicsReader decodeGItem(gitem, scene);
  bool bCorrectlyDecoded = true;
  QDomElement gobjectItem = diagram.firstChildElement("gobject");
  while (!gobjectItem.isNull() &&
          (bCorrectlyDecoded == true))
  {
    switch (passNo)
    {
      case FIRST_PASS:
        bCorrectlyDecoded = decodeGItem.decodeFirstPass(gobjectItem);
        break;
      case SECOND_PASS:
        bCorrectlyDecoded = decodeGItem.decodeSecondPass(gobjectItem);
        break;
      default:
        Q_ASSERT(false);
        bCorrectlyDecoded = false;
        break;
    }
    gobjectItem = gobjectItem.nextSiblingElement("gobject");
  }  
  
  return bCorrectlyDecoded;
}


CGraphicsReader::CGraphicsReader( CItemContainer* item, CGraphicsScene* scene )
{
  //qDebug("creation");
  m_item = item;
  m_scene = scene;
}


bool CGraphicsReader::decodeFirstPass ( const QDomElement& item )
{
  QString sType = item.attribute("type");
  QString sId = item.attribute("id");
  int id = sId.toInt();
  CItem* currentItem = NULL;
  CGraphicsItem* graphicalItem = NULL;
  bool bCorrectlyDecoded = false;
  bool bToBeIgnored = false;

  //qDebug("gitem founded --> sType = %s, sId = %s", sType.toLocal8Bit().data(), sId.toAscii().data());
  
  if (sType == "graphicscircle")
  {
    if (m_item->type() == CProcessContext::CPROCESS_CONTEXT)
    {
      if (id == -1)
      {
        currentItem = (dynamic_cast<CProcessContext*>(m_item))->processInstance();
      } // not else currentItem already to NULL
    }
    else
    {
     currentItem = m_item->searchItem(CProcessInstance::CPROCESS_INSTANCE, id);
    }
    graphicalItem = decodeGraphicalCircle(item, currentItem);
  }
  else if (sType == "graphicsarrow")
  {
    bToBeIgnored = true;
  }
  else if (sType == "graphicsrect")
  {
     currentItem = m_item->searchItem(CTerminator::CTERMINATOR, id);
     graphicalItem = decodeGraphicalTerminator(item, currentItem);
  }
  else if (sType == "graphicsdoubleline")
  {
     currentItem = m_item->searchItem(CStore::CSTORE, id);   
     graphicalItem = decodeGraphicalStore(item, currentItem);
  }
  else if (sType == "graphicsinput")
  {
     currentItem = m_item->searchItem(CInput::CINPUT, id);
     graphicalItem = decodeGraphicalInput(item, currentItem);
  }
  else if (sType == "graphicsoutput")
  {
     currentItem = m_item->searchItem(COutput::COUTPUT, id);     
     graphicalItem = decodeGraphicalOutput(item, currentItem);
  }
  else if (sType == "textitem")
  {
    qDebug("textitem");
    graphicalItem = decodeGraphicalText(item);
  }
  else
  {
    qDebug("graphical item of type %s doesn't exist ", sType.toLocal8Bit().data());
  }
  
  if (bToBeIgnored == false)
  {
    if (graphicalItem != NULL)
    {
      m_scene->addItem(dynamic_cast<QGraphicsItem*>(graphicalItem));
      bCorrectlyDecoded = true;
    }
    else
    {
      qDebug("wrong decoding sType (1st pass) = %s, currentItem = %p", sType.toLocal8Bit().data(), currentItem);
      bCorrectlyDecoded = false;
    }
  }
  else
    bCorrectlyDecoded = true;

  return bCorrectlyDecoded;
}

bool CGraphicsReader::decodeSecondPass ( const QDomElement& item )
{
  QString sType = item.attribute("type");
  QString sId = item.attribute("id");
  int id = sId.toInt();
  CItem* currentItem = NULL;
  CGraphicsItem* graphicalItem = NULL;
  bool bCorrectlyDecoded = false;
  bool bToBeIgnored = false;
  
  if (sType == "graphicsarrow")
  {
     currentItem = m_item->searchItem(CFlow::CFLOW, id);
     graphicalItem = decodeGraphicalArrow(item, currentItem);
  }
  else
    bToBeIgnored = true;//in second pass, only arrow are added to be able to link another component
  
  if (bToBeIgnored == false)
  {
    if (graphicalItem != NULL)
    {
      m_scene->addItem(dynamic_cast<QGraphicsItem*>(graphicalItem));
      bCorrectlyDecoded = true;
    }
    else
    {
      qDebug("wrong decoding (2nd pass) sType = %s, currentItem = %p", sType.toLocal8Bit().data(), currentItem);
      bCorrectlyDecoded = false;
    }
  }
  else
    bCorrectlyDecoded = true;
  
  
  return bCorrectlyDecoded;
}

CGraphicsItem* CGraphicsReader::searchGraphicalItem ( CItem* item )
{
  CGraphicsItem* cgItem = NULL;
  QList<QGraphicsItem *> itemList;
  
  itemList = m_scene->items();
  
  foreach(QGraphicsItem* i, itemList)
  {
    cgItem = dynamic_cast<CGraphicsItem*>(i);
    if ((cgItem != NULL) &&
        (cgItem->getAssociatedItem() == item))
    {
      qDebug("graphical item founded");
      break;
    }
  }

  return cgItem;
}


CGraphicsItem* CGraphicsReader::decodeGraphicalCircle ( const QDomElement& item, CItem* circleItem )
{
  CGraphicsItem* graphicalItem = NULL;
  bool bOk = true;
  if (circleItem == NULL)
    return NULL;
  
  QDomElement eRadius = item.firstChildElement("radius");
  QDomElement ePosition = item.firstChildElement("position");
  QDomElement eFont = item.firstChildElement("font");
  QDomElement eFontColor = item.firstChildElement("fontColor");
  QDomElement eLineColor = item.firstChildElement("lineColor");
  QDomElement eBackgroundColor = item.firstChildElement("backgroundColor");
  QDomElement eAlignmentH = item.firstChildElement("alignmentH");
  QDomElement eAlignmentV = item.firstChildElement("alignmentV");
  
  qreal radius;
  QPointF pos;
  QFont font;
  QColor fontColor, lineColor, backgroundColor;
  int alignmentH;
  int alignmentV;
  
  
  if (!eRadius.isNull())
  {
    QString value = eRadius.attribute("value");
    radius = value.toDouble(&bOk);
  }
  
  if (bOk)
  { 
    pos = CXmlBasicItem::decodeQPointF(ePosition, &bOk);
  }
  
  if (bOk)
  {
    font = CXmlBasicItem::decodeQFont(eFont, &bOk);
  }
  
  if (bOk)
  {
    fontColor = CXmlBasicItem::decodeQColor(eFontColor, &bOk);
  }
  
  if (bOk)
  {
    lineColor = CXmlBasicItem::decodeQColor(eLineColor, &bOk);
  }
  
  if (bOk)
  {
    backgroundColor = CXmlBasicItem::decodeQColor(eBackgroundColor, &bOk);
  }
  
  if (bOk)
  {
    alignmentH = CXmlBasicItem::decodeInt(eAlignmentH, 16, &bOk);
  }
  
  if (bOk)
  {
    alignmentV = CXmlBasicItem::decodeInt(eAlignmentV, 16, &bOk);
  }

  
  if (bOk)
  {
    qDebug("decodage Graphical circle OK");
    CGraphicsCircle* gCircle = new CGraphicsCircle(pos, circleItem);
    gCircle->m_radius = radius;
    gCircle->m_font = font;
    gCircle->m_fontColor = fontColor;
    gCircle->m_lineColor = lineColor;    
    gCircle->m_lineColor = lineColor;
    gCircle->m_brush.setColor(backgroundColor);
    gCircle->m_alignmentH = alignmentH;
    gCircle->m_alignmentV = alignmentV;
    gCircle->setPos(pos);
    graphicalItem = gCircle;
    gCircle->setAndUpdateGraphicalData();
    //QSet<CGraphicsItem*> m_listAttachedItem;
  }
  return graphicalItem;
}

CGraphicsItem* CGraphicsReader::decodeGraphicalTerminator ( const QDomElement& item, CItem* terminatorItem )
{
  CGraphicsItem* graphicalItem = NULL;
  bool bOk = true;
  if (terminatorItem == NULL)
    return NULL;
  
  QDomElement ePosition = item.firstChildElement("position");
  QDomElement eRect = item.firstChildElement("rect");
  QDomElement eFont = item.firstChildElement("font");
  QDomElement eFontColor = item.firstChildElement("fontColor");
  QDomElement eLineColor = item.firstChildElement("lineColor");
  QDomElement eBackgroundColor = item.firstChildElement("backgroundColor");
  QDomElement eAlignmentH = item.firstChildElement("alignmentH");
  QDomElement eAlignmentV = item.firstChildElement("alignmentV");
    
  QPointF pos;
  QRectF rect;
  QFont font;
  QColor fontColor, lineColor, backgroundColor;
  int alignmentH;
  int alignmentV;

  rect = CXmlBasicItem::decodeQRectF(eRect, &bOk);  
  if (bOk)
  { 
    pos = CXmlBasicItem::decodeQPointF(ePosition, &bOk);
  }
  
  if (bOk)
  {
    font = CXmlBasicItem::decodeQFont(eFont, &bOk);
  }
  
  if (bOk)
  {
    fontColor = CXmlBasicItem::decodeQColor(eFontColor, &bOk);
  }
  
  if (bOk)
  {
    lineColor = CXmlBasicItem::decodeQColor(eLineColor, &bOk);
  }
  
  if (bOk)
  {
    backgroundColor = CXmlBasicItem::decodeQColor(eBackgroundColor, &bOk);
  }
  
  if (bOk)
  {
    alignmentH = CXmlBasicItem::decodeInt(eAlignmentH, 16, &bOk);
  }
  
  if (bOk)
  {
    alignmentV = CXmlBasicItem::decodeInt(eAlignmentV, 16, &bOk);
  }

  
  if (bOk)
  {
    qDebug("decodage terminatorItem OK");
    CGraphicsRect* gTerm = new CGraphicsRect(pos, terminatorItem);
    gTerm->m_rect = rect;
    gTerm->m_font = font;
    gTerm->m_fontColor = fontColor;
    gTerm->m_lineColor = lineColor;    
    gTerm->m_lineColor = lineColor;
    gTerm->m_brush.setColor(backgroundColor);
    gTerm->m_alignmentH = alignmentH;
    gTerm->m_alignmentV = alignmentV;
    gTerm->setPos(pos);
    gTerm->setAndUpdateGraphicalData();
    graphicalItem = gTerm;
    //QSet<CGraphicsItem*> m_listAttachedItem;
  }
  return graphicalItem;
}


CGraphicsItem* CGraphicsReader::decodeGraphicalStore ( const QDomElement& item, CItem* storeItem )
{
  if (storeItem == NULL)
    return NULL;
  
  CGraphicsItem* graphicalItem = NULL;
  bool bOk = true;
  QDomElement ePosition = item.firstChildElement("position");
  QDomElement eRect = item.firstChildElement("rect");
  QDomElement eFont = item.firstChildElement("font");
  QDomElement eFontColor = item.firstChildElement("fontColor");
  QDomElement eLineColor = item.firstChildElement("lineColor");
  QDomElement eBackgroundColor = item.firstChildElement("backgroundColor");
  QDomElement eAlignmentH = item.firstChildElement("alignmentH");
  QDomElement eAlignmentV = item.firstChildElement("alignmentV");
  
  QPointF pos;
  QRectF rect;
  QFont font;
  QColor fontColor, lineColor, backgroundColor;
  int alignmentH;
  int alignmentV;
  
  rect = CXmlBasicItem::decodeQRectF(eRect, &bOk);  
  if (bOk)
  { 
    pos = CXmlBasicItem::decodeQPointF(ePosition, &bOk);
  }
  
  if (bOk)
  {
    font = CXmlBasicItem::decodeQFont(eFont, &bOk);
  }
  
  if (bOk)
  {
    fontColor = CXmlBasicItem::decodeQColor(eFontColor, &bOk);
  }
  
  if (bOk)
  {
    lineColor = CXmlBasicItem::decodeQColor(eLineColor, &bOk);
  }
  
  if (bOk)
  {
    backgroundColor = CXmlBasicItem::decodeQColor(eBackgroundColor, &bOk);
  }
  
  if (bOk)
  {
    alignmentH = CXmlBasicItem::decodeInt(eAlignmentH, 16, &bOk);
  }
  
  if (bOk)
  {
    alignmentV = CXmlBasicItem::decodeInt(eAlignmentV, 16, &bOk);
  }
  
  if (bOk)
  {
    qDebug("decodage store OK");
    CGraphicsDoubleLine* gTerm = new CGraphicsDoubleLine(pos, storeItem);
    gTerm->m_rect = rect;
    gTerm->m_font = font;
    gTerm->m_fontColor = fontColor;
    gTerm->m_lineColor = lineColor;    
    gTerm->m_lineColor = lineColor;
    gTerm->m_brush.setColor(backgroundColor);
    gTerm->m_alignmentH = alignmentH;
    gTerm->m_alignmentV = alignmentV;
    gTerm->setPos(pos);
    graphicalItem = gTerm;
    gTerm->setAndUpdateGraphicalData();
    //QSet<CGraphicsItem*> m_listAttachedItem;
  }
  return graphicalItem;
}


CGraphicsItem* CGraphicsReader::decodeGraphicalInput ( const QDomElement& item, CItem* inputItem )
{
  if (inputItem == NULL)
    return NULL;

  CGraphicsItem* graphicalItem = NULL;
  bool bOk = true;
  QDomElement ePosition = item.firstChildElement("position");
  QDomElement eFont = item.firstChildElement("font");
  QDomElement eFontColor = item.firstChildElement("fontColor");
  QDomElement eLineColor = item.firstChildElement("lineColor");
  QDomElement eOffsetX = item.firstChildElement("offsetX");
  QDomElement eOffsetY = item.firstChildElement("offsetY");

  QPointF pos;
  QFont font;
  QColor fontColor, lineColor;
  qreal offsetX, offsetY;
  
  pos = CXmlBasicItem::decodeQPointF(ePosition, &bOk);
  if (bOk)
  {
    font = CXmlBasicItem::decodeQFont(eFont, &bOk);
  }
  
  if (bOk)
  {
    fontColor = CXmlBasicItem::decodeQColor(eFontColor, &bOk);
  }
  
  if (bOk)
  {
    lineColor = CXmlBasicItem::decodeQColor(eLineColor, &bOk);
  }
  
  if (bOk)
  {
    offsetX = CXmlBasicItem::decodeQreal(eOffsetX, &bOk);
  }
  
  if (bOk)
  {
    offsetY = CXmlBasicItem::decodeQreal(eOffsetY, &bOk);
  }
  
  if (bOk)
  {
    qDebug("decodage input OK");
    CGraphicsInput* gTerm = new CGraphicsInput(pos, inputItem);
    gTerm->m_font = font;
    gTerm->m_fontColor = fontColor;
    gTerm->m_lineColor = lineColor;    
    gTerm->m_lineColor = lineColor;
    gTerm->m_offsetX = offsetX;
    gTerm->m_offsetY = offsetY;
    gTerm->setPos(pos);
    graphicalItem = gTerm;
    gTerm->setAndUpdateGraphicalData();
    //QSet<CGraphicsItem*> m_listAttachedItem;
  }
  return graphicalItem;
}


CGraphicsItem* CGraphicsReader::decodeGraphicalOutput ( const QDomElement& item, CItem* outputItem )
{
  if (outputItem == NULL)
    return NULL;

  CGraphicsItem* graphicalItem = NULL;
  bool bOk = true;
  QDomElement ePosition = item.firstChildElement("position");
  QDomElement eFont = item.firstChildElement("font");
  QDomElement eFontColor = item.firstChildElement("fontColor");
  QDomElement eLineColor = item.firstChildElement("lineColor");
  QDomElement eOffsetX1 = item.firstChildElement("offsetX1");
  QDomElement eOffsetY = item.firstChildElement("offsetY");
  QDomElement eOffsetX2 = item.firstChildElement("offsetX2");
  QDomElement eOffsetYmin = item.firstChildElement("offsetYmin");

  QDomElement eOffsetArrowLenMin = item.firstChildElement("offsetArrowLenMin");
  QDomElement eArrowAngle = item.firstChildElement("arrowAngle");
  QDomElement eArrowLength = item.firstChildElement("arrowLength");
  
  
  QPointF pos;
  QFont font;
  QColor fontColor, lineColor;
  qreal offsetX1, offsetX2, offsetY, offsetYmin, offsetArrowLenMin, arrowLength;
  qreal arrowAngle;
  
  pos = CXmlBasicItem::decodeQPointF(ePosition, &bOk);
  if (bOk)
  {
    font = CXmlBasicItem::decodeQFont(eFont, &bOk);
  }
  
  if (bOk)
  {
    fontColor = CXmlBasicItem::decodeQColor(eFontColor, &bOk);
  }
  
  if (bOk)
  {
    lineColor = CXmlBasicItem::decodeQColor(eLineColor, &bOk);
  }
  
  if (bOk)
  {
    offsetX1 = CXmlBasicItem::decodeQreal(eOffsetX1, &bOk);
  }
  
  if (bOk)
  {
    offsetY = CXmlBasicItem::decodeQreal(eOffsetY, &bOk);
  }
  
  if (bOk)
  {
    offsetX2 = CXmlBasicItem::decodeQreal(eOffsetX2, &bOk);
  }
  
  if (bOk)
  {
    offsetYmin = CXmlBasicItem::decodeQreal(eOffsetYmin, &bOk);
  }
  
  if (bOk)
  {
    offsetArrowLenMin = CXmlBasicItem::decodeQreal(eOffsetArrowLenMin, &bOk);
  }

  if (bOk)
  {
    arrowLength = CXmlBasicItem::decodeQreal(eArrowLength, &bOk);
  }
  
  if (bOk)
  {
    arrowAngle = (qreal) ((CXmlBasicItem::decodeInt(eArrowAngle, 10, &bOk) * 3.141569) / 180.0);
  }
  
  if (bOk)
  {
    qDebug("decodage output OK");
    CGraphicsOutput* gTerm = new CGraphicsOutput(pos, outputItem);
    gTerm->m_font = font;
    gTerm->m_fontColor = fontColor;
    gTerm->m_lineColor = lineColor;    
    gTerm->m_offsetX1 = offsetX1;
    gTerm->m_offsetX2 = offsetX2;
    gTerm->m_offsetY = offsetY;
    gTerm->m_offsetYmin = offsetYmin;
    gTerm->m_offsetArrowLenMin = offsetArrowLenMin;
    gTerm->m_arrowLength = arrowLength;
    gTerm->m_arrowAngle = arrowAngle;
    gTerm->setPos(pos);
    gTerm->setAndUpdateGraphicalData();
    graphicalItem = gTerm;
    //QSet<CGraphicsItem*> m_listAttachedItem;
  }
  return graphicalItem;
}


CGraphicsItem* CGraphicsReader::decodeGraphicalArrow ( const QDomElement& item, CItem* arrowItem )
{
  if (arrowItem == NULL)
    return NULL;
  
  CGraphicsItem* gItem = NULL;
  bool bOk = true;
  QDomElement eArrowAngle = item.firstChildElement("arrowAngle");
  QDomElement eArrowLength = item.firstChildElement("arrowLength");
  QDomElement eLabelDisplayed = item.firstChildElement("labelDisplayed");
  QDomElement eFont = item.firstChildElement("font");
  QDomElement eFontColor = item.firstChildElement("fontColor");
  QDomElement eLineColor = item.firstChildElement("lineColor");
  QDomElement eBackgroundColor = item.firstChildElement("backgroundColor");
  QDomElement eLabelRect = item.firstChildElement("labelRect");
  QDomElement ePointList = item.firstChildElement("point-list");
  
  qreal arrowAngle, arrowLength;
  bool labelDisplayed;
  QFont font;
  QColor fontColor, lineColor, backgroundColor;
  QRectF labelRect;
  QPointF point;
  QList<QPointF> listPoints;

  arrowLength = CXmlBasicItem::decodeQreal(eArrowLength, &bOk);
  if (bOk)
  {
    arrowAngle = (qreal) ((CXmlBasicItem::decodeInt(eArrowAngle, 10, &bOk) * 3.141569) / 180.0);
  }
  
  if (bOk)
  {
    labelDisplayed = (bool) CXmlBasicItem::decodeInt(eLabelDisplayed, 10, &bOk);
  }
  
  if (bOk)
  {
    font = CXmlBasicItem::decodeQFont(eFont, &bOk);
  }
  
  if (bOk)
  {
    fontColor = CXmlBasicItem::decodeQColor(eFontColor, &bOk);
  }
  
  if (bOk)
  {
    lineColor = CXmlBasicItem::decodeQColor(eLineColor, &bOk);
  }
  
  if (bOk)
  {
    backgroundColor = CXmlBasicItem::decodeQColor(eBackgroundColor, &bOk);
  }
  
  
  if (bOk)
  {
    labelRect = CXmlBasicItem::decodeQRectF(eLabelRect, &bOk);
  }
 
  QDomElement eCurrentPoint = ePointList.firstChildElement("point");
  while (!eCurrentPoint.isNull() && (bOk == true))
  {
    point = CXmlBasicItem::decodeQPointF(eCurrentPoint, &bOk);
    if (bOk)
    {
      listPoints.append(point);
      eCurrentPoint = eCurrentPoint.nextSiblingElement("point");
    }
  }
  
  CFlow* flow = dynamic_cast<CFlow*>(arrowItem);
  CItem* in = flow->getInput();
  CItem* out = flow->getOutput();
  CGraphicsItem* gItemIn = NULL;
  CGraphicsItem* gItemOut = NULL;
  
  if (in != NULL)
  {
    //qDebug("in name = %s", in->getName().toLocal8Bit().data());
    gItemIn = searchGraphicalItem(in);
    if (gItemIn == NULL)
    {
      qDebug("IN item of arrow not found");
      bOk = false;
    }
  }
  
  if (out != NULL)
  {
    //qDebug("out name = %s", out->getName().toLocal8Bit().data());
    gItemOut = searchGraphicalItem(out);
    if (gItemOut == NULL)
    {
      qDebug("OUT item of arrow not found");
      bOk = false;      
    }
  }
 
  if (bOk && (listPoints.size() >= 2))
  {
    //qDebug("decoding Arrow OK");
    
    point = listPoints.at(0);
    CGraphicsArrow* arrow = new CGraphicsArrow(point, arrowItem);
    
    arrow->m_points = listPoints;
    arrow->m_arrowLength = arrowLength;
    arrow->m_arrowAngle = arrowAngle;
    arrow->m_labelRect = labelRect;
    arrow->m_font = font;
    arrow->m_fontColor = fontColor;
    arrow->m_color = lineColor;
    arrow->m_brush.setColor(backgroundColor);
    arrow->m_isLabelDisplayed = labelDisplayed;
    arrow->m_linkInGoing = false;
    
    arrow->setAndUpdateGraphicalData();
    
    /*for(int i = 0; i< listPoints.size(); i++)
    {
      arrow->updateAnchor(i);
    }*/
    
    if (gItemIn != NULL)
    {
      arrow->m_beginItem = gItemIn;
      gItemIn->linkTo(arrow);
    }
    if (gItemOut != NULL)
    {
      arrow->m_endItem = gItemOut; 
      gItemOut->linkTo(arrow);
    }
    
    gItem = arrow;
  }
  return gItem;
}


CGraphicsItem* CGraphicsReader::decodeGraphicalText ( const QDomElement& item )
{
  QDomElement ePosition = item.firstChildElement("position");
  QDomElement eData = item.firstChildElement("data");
  QDomElement eAlignmentH = item.firstChildElement("alignmentH");
  QDomElement eFont = item.firstChildElement("font");
  QDomElement eFontColor = item.firstChildElement("fontColor");
  bool bOk = true;
  QPointF pos;
  QString type;
  QString contentInPlainText;
  QFont font;
  QColor fontColor;
  CGraphicsItem* graphicalItem = NULL;  

  pos = CXmlBasicItem::decodeQPointF(ePosition, &bOk);
  if (bOk)
  {
    type = eData.attribute("type");
    //qDebug("type = %s", type.toLocal8Bit().data());
    QDomCDATASection cData = eData.firstChild().toCDATASection();
    //qDebug("isCDATASection %i", cData.isCDATASection());
    
    if ((type == "string") && (cData.isCDATASection()))
    {
      //QDomCDATASection CDataSection= cData.toCDATASection();
      contentInPlainText = cData.toText().data();
      //qDebug("content = %s", contentInPlainText.toLocal8Bit().data());
    }
    else
    {
      qDebug("KO decoding GraphicalText");
      bOk = false;
    }
  }
  

  
  if (bOk)
  {
    font = CXmlBasicItem::decodeQFont(eFont, &bOk);
  }
  
  if (bOk)
  {
    fontColor = CXmlBasicItem::decodeQColor(eFontColor, &bOk);
  }
    
  
  if (bOk)
  {
    CGraphicsTextItem* textItem = new CGraphicsTextItem(pos);
    textItem->setFont(font);
    textItem->setDefaultTextColor(fontColor);
    textItem->setPlainText(contentInPlainText);
    textItem->setAndUpdateGraphicalData();
    graphicalItem = textItem;
  }
  
  return graphicalItem;
}


