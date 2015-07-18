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

#include "processimplfactory.h"

CProcessImplFactory::CProcessImplFactory()
{
  m_nextID = 0;
}

CProcessImplementation* CProcessImplFactory::createNewImplementationProcess(const QString& name)
{
  CProcessImplementation* process;  
  process = new CProcessImplementation();
  
  m_listProcess.append(process);
  process->setID(m_nextID);
  m_nextID++;
  process->setName(name);
  
  return process;
}


void CProcessImplFactory::insertImplementationProcess ( CProcessImplementation* p, int id )
{
  if (id == -1)
  {
    p->setID(m_nextID);
  }
  
  m_listProcess.append(p);
  
  if (id == -1)
  {
    m_nextID++;
  }
  else
  {
    if (id >= m_nextID) 
      m_nextID = id + 1;
  }
}


bool CProcessImplFactory::renameProcess ( CProcessImplementation* item, QString newName )
{
  bool renameOK = false;
  CItem* itemFound;
  
  itemFound = ifAlreadyExists(newName);
  if (itemFound == item)
  {
    renameOK = true;
  }
  else if (itemFound == NULL)
  {
    item->setName(newName);
    renameOK = true;
  }
  else
  {
    renameOK = false;
  }
  
  return renameOK;  
}

CProcessImplementation* CProcessImplFactory::getImplementationProcess ( QString name )
{
  foreach(CProcessImplementation* p, m_listProcess)
  {
    if (name == p->getName())
    {
      return p;
    }
  }
  return NULL;
}

CProcessImplementation* CProcessImplFactory::getImplementationProcess ( int ID )
{
  foreach(CProcessImplementation* p, m_listProcess)
  {
    if (ID == p->getID())
    {
      return p;
    }
  }
  return NULL;
}


CItem* CProcessImplFactory::ifAlreadyExists ( const QString& name )
{
  foreach(CProcessImplementation* p, m_listProcess)
  {
    if (name == p->getName())
    {
      return p;
    }
  }
  return NULL;
}

CItem* CProcessImplFactory::search ( int ID )
{
  foreach(CProcessImplementation* p, m_listProcess)
  {
    if (p->getID() == ID)
      return p;
  }
  
  return NULL;
}


CProcessImplFactory::~CProcessImplFactory()
{
  qDebug("Delete CProcessImplFactory");
}


