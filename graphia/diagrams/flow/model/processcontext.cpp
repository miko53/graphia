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
#include "processcontext.h"
#include "terminator.h"
#include "flow.h"
#include "store.h"
#include "processinstance.h"


CProcessContext::CProcessContext(CItemContainer* parent): CItemContainer(parent)
{
  m_nameChanged = false;
  qDebug("creation of CProcessContext '%p'", this);
}

CProcessContext::~CProcessContext()
{
  qDebug("deletion of CProcessContext '%p'", this);
}


QString CProcessContext::getName()
{
  if (m_nameChanged == false)
    return QString("process_context_%1").arg(m_id);
  else
    return CItem::getName();
}

void CProcessContext::setName ( const QString& newName )
{
  CItem::setName ( newName );
  m_nameChanged = true;
}


bool CProcessContext::dissociate(CItem* item, CGraphicsItem::LinkOrder order)
{
  Q_UNUSED(item);
  Q_UNUSED(order);

  //not possible graphically speaking
  Q_ASSERT(false);
  return false;
}

bool CProcessContext::associateWith(CItem* item, CGraphicsItem::LinkOrder order)
{
  Q_UNUSED(item);
  Q_UNUSED(order);

  //not possible graphically speaking
  Q_ASSERT(false);
  return false;
}


CItem* CProcessContext::createItem(int type) 
{
  CItem* item = NULL;
  
  switch (type)
  {
    case CTerminator::CTERMINATOR:
      item = m_terminatorFactory.createNew();
      break;
      
    case CFlow::CFLOW:
      item = m_flowFactory.createNew();
      break;
      
    case CStore::CSTORE:
      item = m_storeFactory.createNew();
      break;
      
    default:
      Q_ASSERT(false);
      break;
  }
  
  item->setParent(this);
  return item;
}

void CProcessContext::removeItem(CItem* item)
{
  bool bOK;
  
  //item->setParent(NULL);
  
  switch (item->type())
  {
    case CTerminator::CTERMINATOR:
      bOK = m_terminatorFactory.remove((CTerminator*)item);
      break;
      
    case CStore::CSTORE:
      bOK = m_storeFactory.remove((CStore*) item);
      break;
      
    case CFlow::CFLOW:
      bOK = m_flowFactory.remove((CFlow*) item);
      break;

    default:
      Q_ASSERT(false);
      break;    
  }

  Q_ASSERT(bOK == true);
}

bool CProcessContext::renameObject ( CItem* m_item, const QString& name )
{
  bool bRenameOK;
  bRenameOK = false;
  
  switch(m_item->type())
  {
    case CTerminator::CTERMINATOR:
      bRenameOK = m_terminatorFactory.renameItem(m_item, name);
      break;
      
    case CStore::CSTORE:
      bRenameOK = m_storeFactory.renameItem(m_item, name);
      break;
      
    default:
      Q_ASSERT(false);
      break;
  }
  
  return bRenameOK;
}


bool CProcessContext::insertItem ( CItem* item )
{
  bool bInsertionOk;
  bInsertionOk = true;
  
  switch(item->type())
  {
    case CFlow::CFLOW:
      m_flowFactory.insertItem((CFlow*) item);
      break;
            
    case CTerminator::CTERMINATOR:
      m_terminatorFactory.insertItem((CTerminator*) item);
      break;
      
    case CStore::CSTORE:
      m_storeFactory.insertItem((CStore*) item);
      break;
      
    default:
      qDebug("unknow type to insert %d", item->type());
      bInsertionOk = false;
      break;
  }
  
  if (bInsertionOk)
    item->setParent(this);
  
  return bInsertionOk;
}

CItem* CProcessContext::searchItem ( int type, int ID )
{
  CItem* item = NULL;
  
  switch(type)
  {
    case CFlow::CFLOW:
      item = m_flowFactory.search(ID);
      break;
      
    case CTerminator::CTERMINATOR:
      item = m_terminatorFactory.search(ID);
      break;
      
    case CStore::CSTORE:
      item = m_storeFactory.search(ID);
      break;
      
    case CProcessInstance::CPROCESS_INSTANCE:
      if (ID == -1)
        item = m_instanceOfContext;
      else
        Q_ASSERT(false); //not possible, only one instance possible in a contexte 
      break;
      
    default:
      Q_ASSERT(false);
      break;
  }
  
  return item;
}


