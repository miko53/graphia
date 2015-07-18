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


#include "processinstancefactory.h"

CProcessInstanceFactory::CProcessInstanceFactory()
{
  m_currentID = 0;
  m_container = NULL;
}

CProcessInstanceFactory::~CProcessInstanceFactory()
{

}

CProcessInstance* CProcessInstanceFactory::createNew()
{
  CProcessInstance* p;
  p = new CProcessInstance();
  p->setID(m_currentID);
  p->setParent(m_container);
  m_currentID++;
  m_data.append(p);
  return p;
}


bool CProcessInstanceFactory::remove ( CProcessInstance* item )
{
  bool bOK;
  Q_ASSERT(item != NULL);
  bOK = m_data.removeOne(item);
  delete item;
  return bOK;
}

void CProcessInstanceFactory::insertItem ( CProcessInstance* item )
{
  m_data.append(item);
  if (m_currentID < item->getID())
    m_currentID = item->getID() + 1;
}


CItem* CProcessInstanceFactory::search ( int ID )
{
  CProcessInstance* found = NULL;
  
  foreach(CProcessInstance* i, m_data)
  {
    if (i->getID() == ID)
    {
      found = i;
      break;
    }
  }
  
  return found;
}


