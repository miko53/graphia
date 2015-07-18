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


#include "flowfactory.h"

CFlowFactory::CFlowFactory()
{
  m_currentID = 0;

}

CFlowFactory::~CFlowFactory()
{

}


CFlow* CFlowFactory::createNew()
{
  CFlow* f = new CFlow();
  f->setID(m_currentID);
  m_currentID++;
  m_data.append(f);
  return f;
}

void CFlowFactory::insertItem ( CFlow* flow )
{
  m_data.append(flow);
  if (m_currentID < flow->getID())
    m_currentID = flow->getID() + 1;
}

CFlow* CFlowFactory::search ( int ID )
{
  CFlow* found = NULL;
  
  foreach(CFlow* f, m_data)
  {
    if (f->getID() == ID)
    {
      found = f;
      break;
    }
  }
  
  return found;
}


bool CFlowFactory::remove ( CFlow* item )
{
  bool bOK;
  Q_ASSERT(item != NULL);
  bOK = m_data.removeOne(item);
  delete item;
  return bOK;
}

void CFlowFactory::removeAll()
{
  foreach(CFlow* f, m_data)
  {
    delete f;
  }
}

