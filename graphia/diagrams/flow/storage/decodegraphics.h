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

#include "decodeflowdiagram.h"
#include "diagrams/flow/model/itemfactory.h"
#include "diagrams/flow/graphics/graphicsscene.h"

class CDecodeFlowGraphicsList: public CDecodeXmlFile
{
public:
  CDecodeFlowGraphicsList(CItemFactory* itemFactory, QList<CGraphicsScene*>* list);
  virtual bool decode ( const QDomElement& item );

protected:
  typedef enum { FIRST_PASS, SECOND_PASS} ePassType;
  bool lookIntoDiagramElement( const QDomElement& diagram, CGraphicsScene* scene, CItemContainer* gitem, ePassType passNo );
  
protected:
  CGraphicsScene* readGraphicsScene ( const QDomElement& diagram );  
  
protected:
  CItemFactory* m_itemFactory;
  QList< CGraphicsScene*>* m_listDiagram;
  
private:
};


class CGraphicsReader
{
public:
  CGraphicsReader(CItemContainer* item, CGraphicsScene* scene);
  bool decodeFirstPass ( const QDomElement& item );
  bool decodeSecondPass ( const QDomElement& item );

protected:
  CGraphicsItem* searchGraphicalItem(CItem* item);
  
protected:
  CGraphicsItem* decodeGraphicalCircle ( const QDomElement& item, CItem* circleItem );
  CGraphicsItem* decodeGraphicalTerminator ( const QDomElement& item, CItem* terminatorItem );
  CGraphicsItem* decodeGraphicalStore ( const QDomElement& item, CItem* storeItem );
  CGraphicsItem* decodeGraphicalInput ( const QDomElement& item, CItem* inputItem );
  CGraphicsItem* decodeGraphicalOutput ( const QDomElement& item, CItem* outputItem );
  CGraphicsItem* decodeGraphicalText ( const QDomElement& item );
  CGraphicsItem* decodeGraphicalArrow ( const QDomElement& item, CItem* arrowItem );

protected:
  CItemContainer* m_item;
  CGraphicsScene* m_scene;
  
private:
  
};
