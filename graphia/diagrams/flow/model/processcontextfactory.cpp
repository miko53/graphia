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
#include "processcontextfactory.h"

CProcessContextFactory::CProcessContextFactory()
{
  m_nextID = 0;
}

CProcessContextFactory::~CProcessContextFactory()
{
  qDebug("delete CProcessContextFactory");
}


CProcessContext* CProcessContextFactory::createNewContextProcess ( const QString& name )
{
  CProcessContext* process;
  process = new CProcessContext();
  process->setName(name);
  process->setID(m_nextID);
  m_nextID++;
  m_listProcess.append(process);
  return process;
}

void CProcessContextFactory::insertContextProcess ( CProcessContext* p, int id )
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

CItem* CProcessContextFactory::search ( int ID )
{
  foreach(CProcessContext* p, m_listProcess)
  {
    if (p->getID() == ID)
      return p;
  }
  
  return NULL;
}
