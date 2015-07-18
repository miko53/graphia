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
#include "diagrams/generic/toolitemmodel.h"
#include "toolbox/toolgroup.h"
#include "toolbox/toolitem.h"

#include "../flow/model/processinstance.h"
#include "../flow/model/processimplementation.h"
#include "../flow/model/flow.h"
#include "../flow/model/input.h"
#include "../flow/model/output.h"
#include "../flow/model/store.h"
#include "../flow/model/terminator.h"
#include "../flow/model/processcontext.h"

#include "../flow/graphics/graphicscircle.h"
#include "../flow/graphics/graphicsarrow.h"
#include "../flow/graphics/graphicstextitem.h"
#include "../flow/graphics/graphicsinput.h"
#include "../flow/graphics/graphicsoutput.h"
#include "../flow/graphics/graphicsdoubleline.h"
#include "../flow/graphics/graphicsrect.h"
#include "diagrams/flow/graphics/graphicsscene.h"
#include "graphia.h"

typedef enum
{
   ITEM_GRP_COMMON
  ,ITEM_GRP_FLOW
} ItemGroup;


CToolItemModel::CToolItemModel()
{
  buildListItem();
}

void CToolItemModel::buildListItem()
{
  buildCommonList();
  buildFlowList();
}

void CToolItemModel::buildCommonList()
{
  CToolGroup* grpItem;
  grpItem = new CToolGroup(tr("Common Item"));
  
  m_groupItems.insert(ITEM_GRP_COMMON, grpItem);
  CToolItem* item;
  
  item = new CToolItem(tr("select"),
                       ITEM_SELECT,
                       QIcon(":/select.png"),
                       QIcon(":/select.png"),
                       NULL);
  item->setDefault(true);
  grpItem->insertToolItem(item);
  m_listItems.insert(ITEM_SELECT, item);

  item = new CToolItem(tr("add note"),
                       ITEM_TEXT,
                       QIcon(":/text_tool.png"),
                       QIcon(":/text_tool.png"),
                       NULL);
  grpItem->insertToolItem(item);
  m_listItems.insert(ITEM_TEXT, item);
}

void CToolItemModel::buildFlowList()
{
  CToolGroup* grpItem;
  grpItem = new CToolGroup(tr("Flow Diagram"));

  m_groupItems.insert(ITEM_GRP_FLOW, grpItem);
  CToolItem* item;
  
  item = new CToolItem(tr("process"),
                       ITEM_PROCESS,
                       QIcon(":/circle.png"),
                       QIcon(":/circle_lock.png"),
                       NULL);
  grpItem->insertToolItem(item);
  m_listItems.insert(ITEM_PROCESS, item);

  item = new CToolItem(tr("flow"),
                       ITEM_ARROW,
                       QIcon(":/arrow.png"),
                       QIcon(":/arrow_lock.png"),
                       NULL);
  grpItem->insertToolItem(item);
  m_listItems.insert(ITEM_ARROW, item);

  item = new CToolItem(tr("input"),
                       ITEM_INPUT,
                       QIcon(":/input.png"),
                       QIcon(":/input_lock.png"),
                       NULL);
  grpItem->insertToolItem(item);
  m_listItems.insert(ITEM_INPUT, item);
  
  item = new CToolItem(tr("output"),
                       ITEM_OUTPUT,
                       QIcon(":/output.png"),
                       QIcon(":/output_lock.png"),
                       NULL);
  grpItem->insertToolItem(item);
  m_listItems.insert(ITEM_OUTPUT, item);

  item = new CToolItem(tr("bus"),
                       ITEM_BUS,
                       QIcon(":/bus.png"),
                       QIcon(":/bus_lock.png"),
                       NULL);
  grpItem->insertToolItem(item);
  m_listItems.insert(ITEM_BUS, item);

  item = new CToolItem(tr("link_bus"),
                       ITEM_BUS_LINK,
                       QIcon(":/bus_link.png"),
                       QIcon(":/bus_link_lock.png"),
                       NULL);
  grpItem->insertToolItem(item);
  m_listItems.insert(ITEM_BUS_LINK, item);

  item = new CToolItem(tr("store"),
                       ITEM_STORE,
                       QIcon(":/store.png"),
                       QIcon(":/store_lock.png"),
                       NULL);
  grpItem->insertToolItem(item);
  m_listItems.insert(ITEM_STORE, item);

  item = new CToolItem(tr("terminator"),
                       ITEM_TERMINATOR,
                       QIcon(":/rectangle.png"),
                       QIcon(":/rectangle_lock.png"),
                       NULL);
  grpItem->insertToolItem(item);
  m_listItems.insert(ITEM_TERMINATOR, item);
}

CToolGroup* CToolItemModel::getAt(int i)
{
  Q_ASSERT(i < m_groupItems.size());
  return m_groupItems[i];
}

int CToolItemModel::getNbOfGroups()
{
  return m_groupItems.size();
}

QGraphicsItem* CToolItemModel::createItem(int id, QPointF& point, CItem* p)
{
  QGraphicsItem* createdItem = NULL;
  int type;
  type = p->type();
  
  switch (type)
  {
    case CProcessImplementation::CPROCESS_IMPL:
      createdItem = createInImplementationDiagram(id, point, dynamic_cast<CProcessImplementation*> (p));
      break;
      
    case CProcessContext::CPROCESS_CONTEXT:
      createdItem = createInContextDiagram(id, point, dynamic_cast<CProcessContext*> (p));
      break;
    
    default:
      Q_ASSERT(false);
      break;
  }
  
  return createdItem;
}

QGraphicsItem* CToolItemModel::createInImplementationDiagram(int id, QPointF& point, CProcessImplementation* parent)
{
  QGraphicsItem* createdItem = NULL;
  CItem* newItem = NULL;

  Q_ASSERT(parent != NULL);
  
  switch (id)
  {
    case ITEM_SELECT:
      Q_ASSERT(false); //not possible
      break;
      
    case ITEM_TEXT:
      createdItem = new CGraphicsTextItem(point);
      break;
      
    case ITEM_PROCESS:
      newItem = parent->createItem(CProcessInstance::CPROCESS_INSTANCE);
      if (newItem != NULL)
      {
        createdItem = new CGraphicsCircle(point, newItem);
      }
      break;
      
    case ITEM_ARROW:
      newItem = parent->createItem(CFlow::CFLOW);
      if (newItem != NULL)
      {
        createdItem = new CGraphicsArrow(point, newItem);
      }
      break;
      
    case ITEM_INPUT:
      newItem = parent->createItem(CInput::CINPUT);
      if (newItem != NULL)
      {
        createdItem = new CGraphicsInput(point, newItem);
      }
      break;
      
    case ITEM_OUTPUT:
      newItem = parent->createItem(COutput::COUTPUT);
      if (newItem != NULL)
      {
        createdItem = new CGraphicsOutput(point, newItem);
      }
      break;
      
    case ITEM_BUS:
      //TODO
      break;
      
    case ITEM_BUS_LINK:
      //TODO
      break;
      
    case ITEM_STORE:
      newItem = parent->createItem(CStore::CSTORE);
      if (newItem != NULL)
      {
        createdItem = new CGraphicsDoubleLine(point, newItem);
      }
      break;
      
    
    default:
      Q_ASSERT(false);
      break;
  }

  return createdItem;
}

QGraphicsItem* CToolItemModel::createInContextDiagram(int id, QPointF& point, CProcessContext* parent)
{
  QGraphicsItem* createdItem = NULL;
  CItem* newItem = NULL;
//  CTypeFactory* dico;
//  dico = getApp()->getDocument()->getItemFactory()->getTypeFactory();
  
  Q_ASSERT(parent != NULL);

  switch (id)
  {
    case ITEM_TEXT:
      createdItem = new CGraphicsTextItem(point);
      break;
    
    case ITEM_TERMINATOR:
      newItem = parent->createItem(CTerminator::CTERMINATOR);
      if (newItem != NULL)
      {
        createdItem = new CGraphicsRect(point, newItem);
      }
      break;
      
    case ITEM_ARROW:
      newItem = parent->createItem(CFlow::CFLOW);
      if (newItem != NULL)
      {
        createdItem = new CGraphicsArrow(point, newItem);
      }
      break;
      
    case ITEM_STORE:
      newItem = parent->createItem(CStore::CSTORE);
      if (newItem != NULL)
      {
        createdItem = new CGraphicsDoubleLine(point, newItem);
      }
      break;
      
    default:
      Q_ASSERT(false);
      break;
  }
  
  return createdItem;
}


void CToolItemModel::removeItem(CGraphicsItem* item)
{
  Q_ASSERT(item != NULL);  
  item->unlink();

  CItem* mItem;
  mItem = item->getAssociatedItem();
  
  if (mItem != NULL)
  {
    qDebug("remove item '%s'", mItem->getName().toLocal8Bit().data());
    
    switch(mItem->type())
    {
      case CProcessInstance::CPROCESS_INSTANCE:
        removeProcessItem(dynamic_cast<CProcessInstance*> (mItem));
        break;

      case CFlow::CFLOW:
        removeFlowItem(dynamic_cast<CFlow*> (mItem) );
        break;
        
      case CStore::CSTORE:
        removeStoreItem(dynamic_cast<CStore*> (mItem) );
        break;

      case CInput::CINPUT:
        removeInputItem(dynamic_cast<CInput*> (mItem) );
        break;
        
      case COutput::COUTPUT:
        removeOutputItem(dynamic_cast<COutput*> (mItem) );
        break;
        
      case CTerminator::CTERMINATOR:
        removeTerminatorItem(dynamic_cast<CTerminator*> (mItem) );
        break;
        
      default:
        Q_ASSERT(false);
        break;
    }
  }
  else
  {
    qDebug("delete graphical item without associated item !\n");
  }

  delete item;
}

void CToolItemModel::removeFlowItem(CFlow* flow)
{
  CItemContainer* parent;
  CItem* in;
  CItem* out;

  parent = /*dynamic_cast<CProcessImplementation*> */(flow->getParent());
  Q_ASSERT(parent != NULL);
  
  in = flow->getInput();
  out = flow->getOutput();
  
  if (in != NULL)
  {
    //indiquer à l'item de retirer le lien avec le flow
    switch (in->type())
    {
      case CProcessInstance::CPROCESS_INSTANCE:
        (dynamic_cast<CProcessInstance*>(in))->removeOutput(flow);
        break;
        
      case CStore::CSTORE:
        (dynamic_cast<CStore*>(in))->removeOutput(flow);
        break;
        
      case CInput::CINPUT:
        (dynamic_cast<CInput*>(in))->setFlow(NULL);
        break;
        
      default:
        Q_ASSERT(false);
        break;
    }
  }
  
  if (out != NULL)
  {
    //indiquer à l'item de retirer le lien avec le flow
    switch (out->type())
    {
      case CProcessInstance::CPROCESS_INSTANCE:
        (dynamic_cast<CProcessInstance*>(out))->removeInput(flow);
        break;
        
      case CStore::CSTORE:
        (dynamic_cast<CStore*>(out))->removeInput(flow);
        break;
        
      case COutput::COUTPUT:
        (dynamic_cast<COutput*>(out))->setFlow(NULL);
        break;
        
      default:
        Q_ASSERT(false);
        break;
    }    
  }
  parent->removeItem(flow);
}

void CToolItemModel::removeProcessItem(CProcessInstance* process)
{
  //request to remove all inputs and all outputs
  //that's all, the delete on item remove all another items
  //not need specifiq behaviour when a scene exist on this process --> need to delete all scene items --> warn the document about this
  CProcessImplementation* parent;
  
  //dans le cas d'un process intance le parent ne peut être qu'un CProcessImplementation
  parent = dynamic_cast<CProcessImplementation*> (process->getParent());
  Q_ASSERT(parent != NULL);

  process->removeAllInputs();
  process->removeAllOutputs();

  parent->removeItem(process);  
}

void CToolItemModel::removeStoreItem(CStore* store)
{
  CItemContainer* parent;
  parent = store->getParent();
  Q_ASSERT(parent != NULL);
  
  store->removeAllInputs();
  store->removeAllOutputs();

  parent->removeItem(store);
}

void CToolItemModel::removeInputItem(CInput* input)
{
  CProcessImplementation* parent;
  CFlow* f;
  
  //dans le cas d'une input le parent ne peut être qu'un CProcessImplementation
  parent = dynamic_cast<CProcessImplementation*> (input->getParent());
  Q_ASSERT(parent != NULL);
  
  f = input->getFlow();
  
  if (f != NULL)
  {
    Q_ASSERT(f->getInput() == input);
    f->setInput(NULL);
  }
  
  parent->removeItem(input);
}

void CToolItemModel::removeOutputItem(COutput* output)
{
  CProcessImplementation* parent;
  CFlow* f;

  //dans le cas d'une output le parent ne peut être qu'un CProcessImplementation
  parent = dynamic_cast<CProcessImplementation*> (output->getParent());
  Q_ASSERT(parent != NULL);
  
  f = output->getFlow();
  
  if (f != NULL)
  {
    Q_ASSERT(f->getOutput() == output);
    f->setOutput(NULL);
  }
  
  parent->removeItem(output);
}

void CToolItemModel::removeTerminatorItem(CTerminator* terminator)
{
  CProcessContext* parent;
  
  // le parent d'un terminator est uniquement un process context
  parent = dynamic_cast<CProcessContext*> (terminator->getParent());
  Q_ASSERT(parent != NULL);
  
  terminator->removeAttachedFlow();

  parent->removeItem(terminator);
}

void CToolItemModel::removeDiagramItem(CGraphicsScene* diagram)
{
  CItem* item = diagram->getItem();
  CItemContainer* p = dynamic_cast<CItemContainer*> (item);
  Q_ASSERT(p != NULL);
   
  delete p;
  delete diagram;
}


CToolItemModel::~CToolItemModel()
{
  qDebug("~CToolItemModel");
}
