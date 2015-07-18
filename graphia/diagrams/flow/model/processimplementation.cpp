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
#include "processimplementation.h"
#include "processinstance.h"
#include "flow.h"
#include "input.h"
#include "output.h"
#include "store.h"


CProcessImplementation::CProcessImplementation(CProcessImplementation* parent): CItemContainer(parent)
{
  m_name = QString("process implementation %1").arg(m_id);
  qDebug("creation of CProcessImplementation '%s'", getName().toLocal8Bit().data());
}

CProcessImplementation::~CProcessImplementation()
{
  removeAll();
  qDebug("deletion of CProcessImplementation '%s'", getName().toLocal8Bit().data());
}

void CProcessImplementation::removeAll(void )
{   
  //remove all reference to this implementation of process 
  foreach(CProcessInstance* p, m_InstanciatedProcess)
  {
    p->setImplementedProcess(NULL);
  }
}

bool CProcessImplementation::associateWith(CItem* item, CGraphicsItem::LinkOrder order)
{
  Q_UNUSED(item);
  Q_UNUSED(order);
  
  //not possible graphically speaking
  Q_ASSERT(false);
  return false;
}


bool CProcessImplementation::dissociate(CItem* item, CGraphicsItem::LinkOrder order)
{
  Q_UNUSED(item);
  Q_UNUSED(order);

  //not possible graphically speaking
  Q_ASSERT(false);
  return false;
}


CItem* CProcessImplementation::createItem(int type)
{
  CItem* item = NULL;

  switch (type)
  {
    case CProcessInstance::CPROCESS_INSTANCE:
      item = m_processInstFactory.createNew();
      break;

    case CFlow::CFLOW:
      item = m_flowFactory.createNew();
      break;

    case CInput::CINPUT:
      item = m_inputFactory.createNew();
      break;
      
    case COutput::COUTPUT:
      item = m_outputFactory.createNew();
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

void CProcessImplementation::removeItem(CItem* item)
{
  Q_ASSERT(item != NULL);
  bool bOK = false;

  switch (item->type())
  {
    case CProcessInstance::CPROCESS_INSTANCE:
      bOK = m_processInstFactory.remove((CProcessInstance*) item);
      break;

    case CFlow::CFLOW:
      bOK = m_flowFactory.remove((CFlow*) item);
      break;

    case CInput::CINPUT:
      bOK = m_inputFactory.remove((CInput*) item);
      break;
      
    case COutput::COUTPUT:
      bOK = m_outputFactory.remove((COutput*) item);
      break;

    case CStore::CSTORE:
      bOK = m_storeFactory.remove((CStore*) item);
      break;

    default:
      Q_ASSERT(false);
      break;    
  }

  Q_ASSERT(bOK == true);
  item->setParent(NULL);
}

bool CProcessImplementation::insertItem ( CItem* item )
{
  bool bInsertionOk;
  bInsertionOk = true;
  
  switch(item->type())
  {
    case CProcessInstance::CPROCESS_INSTANCE:
      m_processInstFactory.insertItem((CProcessInstance*) item);
      break;
      
    case CFlow::CFLOW:
      m_flowFactory.insertItem((CFlow*) item);
      break;
      
    case CInput::CINPUT:
      m_inputFactory.insertItem((CInput*) item);
      break;
      
    case COutput::COUTPUT:
      m_outputFactory.insertItem((COutput*) item);
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

CItem* CProcessImplementation::searchItem ( int type, int ID )
{
  CItem* item = NULL;
  
  switch(type)
  {
    case CProcessInstance::CPROCESS_INSTANCE:
      item = m_processInstFactory.search(ID);
      break;
      
    case CFlow::CFLOW:
      item = m_flowFactory.search(ID);
      break;
      
    case CInput::CINPUT:
      item = m_inputFactory.search(ID);
      break;
      
    case COutput::COUTPUT:
      item = m_outputFactory.search(ID);
      break;
      
    case CStore::CSTORE:
      item = m_storeFactory.search(ID);
      break;
    
    default:
      Q_ASSERT(false);
      break;
  }
  
  
  return item;
}



void CProcessImplementation::addInstance(CProcessInstance* process)
{
  Q_ASSERT(process != NULL);
  m_InstanciatedProcess.append(process);
  Q_ASSERT(process->getImplementedProcess() == this);
  //process->setImplementedProcess(this); //TODO ??
}

void CProcessImplementation::setName ( const QString& newName )
{
  foreach(CProcessInstance* p, m_InstanciatedProcess)
  {
    p->setName(newName);
  }
  
  CItem::setName ( newName );
}

bool CProcessImplementation::renameObject ( CItem* m_item, const QString& name )
{
  bool bRenameOK;
  bRenameOK = false;
  
  switch(m_item->type())
  {
    case CStore::CSTORE:
      bRenameOK = m_storeFactory.renameItem(m_item, name);
      break;
      
    default:
      Q_ASSERT(false);
      break;
  }
  
  return bRenameOK;
}
