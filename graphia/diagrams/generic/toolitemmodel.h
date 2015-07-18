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
#ifndef TOOLITEMMODEL_H
#define TOOLITEMMODEL_H

#include <QMultiMap>
#include <QVector>
#include <QString>
#include <QCoreApplication>
#include "toolbox/toolgroup.h"

class CGraphicsScene;
class CGraphicsItem;
class CProcessContext;
class CProcessImplementation;
class CItem;
class CFlow;
class CStore;
class CProcessInstance;
class CInput;
class COutput;
class CTerminator;
class QGraphicsItem;

class CToolItemModel
{
  Q_DECLARE_TR_FUNCTIONS(CToolItemModel)

public:
    
  typedef enum
  {
    ITEM_SELECT,
    ITEM_TEXT,
    ITEM_PROCESS,
    ITEM_ARROW,
    ITEM_INPUT,
    ITEM_OUTPUT,
    ITEM_BUS,
    ITEM_BUS_LINK,
    ITEM_STORE,
    ITEM_TERMINATOR,
    ITEM_NB
  } ItemListId;

  CToolItemModel();
  virtual ~CToolItemModel();

  //retrieve content of model
  CToolGroup* getAt(int i);
  int getNbOfGroups();

  //build the item
  QGraphicsItem* createItem(int id, QPointF& point, CItem* p);

  //remove a item
  //api to be working
  void removeItem(CGraphicsItem* item);
  void removeDiagramItem(CGraphicsScene* diagram);

protected:
  QGraphicsItem* createInImplementationDiagram(int id, QPointF& point, CProcessImplementation* parent);
  QGraphicsItem* createInContextDiagram(int id, QPointF& point, CProcessContext* parent);
  
  void buildListItem();
  QVector<CToolGroup*> m_groupItems;
  QVector<CToolItem*> m_listItems;

private:
  void removeFlowItem(CFlow* flow);
  void removeProcessItem(CProcessInstance* process);
  void removeInputItem(CInput* input);
  void removeOutputItem(COutput* output);
  void removeTerminatorItem(CTerminator* terminator);
  
  void buildCommonList();
  void buildFlowList();
  void removeStoreItem(CStore* store);

};

#endif /*TOOLITEMMODEL_H*/
