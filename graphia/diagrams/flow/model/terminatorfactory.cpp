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


#include "terminatorfactory.h"

CTerminatorFactory::CTerminatorFactory()
{
  m_currentID = 0;
}

CTerminatorFactory::~CTerminatorFactory()
{
  removeAll();
}

CTerminator* CTerminatorFactory::createNew ()
{
  CTerminator* t;
  t = new CTerminator();
  t->setID(m_currentID);
  m_currentID++;
  m_data.append(t);
  return t;
}

void CTerminatorFactory::insertItem ( CTerminator* item )
{
  m_data.append(item);
  if (m_currentID < item->getID())
    m_currentID = item->getID() + 1;
}

CItem* CTerminatorFactory::search ( int ID )
{
  CTerminator* found = NULL;
  
  foreach(CTerminator* i, m_data)
  {
    if (i->getID() == ID)
    {
      found = i;
      break;
    }
  }
  
  return found;
}


CItem* CTerminatorFactory::ifAlreadyExists ( const QString& name )
{
  foreach(CTerminator* t, m_data)
  {
    if (name == t->getName())
    {
      return t;
    }
  }
  return NULL;
}

bool CTerminatorFactory::renameItem (CItem* item, const QString& name)
{
  bool renameOK = false;
  CItem* itemFound;
  
  itemFound = ifAlreadyExists(name);
  if (itemFound == item)
  {
    renameOK = true;
  }
  else if (itemFound == NULL)
  {
    item->setName(name);
    renameOK = true;
  }
  else
  {
    renameOK = false;
  }
  
  return renameOK;
}


bool CTerminatorFactory::remove ( CTerminator* item )
{
  Q_ASSERT(item != NULL);
  bool bOK;
  bOK = m_data.removeOne(item);
  
  delete item;
  return bOK;
}

void CTerminatorFactory::removeAll()
{
  foreach(CTerminator* t, m_data)
  {
    delete t;
  }
}
