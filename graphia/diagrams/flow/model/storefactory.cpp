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


#include "storefactory.h"

CStoreFactory::CStoreFactory()
{
  m_currentID = 0;
}

CStoreFactory::~CStoreFactory()
{
  removeAll();
}


CStore* CStoreFactory::createNew()
{
  CStore* s;
  s = new CStore();
  s->setID(m_currentID);
  m_currentID++;
  m_data.append(s);
  return s;
}


bool CStoreFactory::remove ( CStore* item )
{
  bool bOK;
  Q_ASSERT(item != NULL);
  bOK = m_data.removeOne(item);
  delete item;
  return bOK;
}

CItem* CStoreFactory::ifAlreadyExists ( const QString& name )
{
  foreach(CStore* s, m_data)
  {
    if (name == s->getName())
    {
      return s;
    }
  }
  return NULL;
}

bool CStoreFactory::renameItem ( CItem* item, QString name )
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


void CStoreFactory::removeAll()
{
  foreach(CStore* s, m_data)
  {
    delete s;
  }
}

void CStoreFactory::insertItem ( CStore* item )
{
  m_data.append(item);
  if (m_currentID < item->getID())
    m_currentID = item->getID() + 1;
}



CItem* CStoreFactory::search ( int ID )
{
  CStore* found = NULL;
  
  foreach(CStore* i, m_data)
  {
    if (i->getID() == ID)
    {
      found = i;
      break;
    }
  }
  
  return found;
}
