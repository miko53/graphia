/*graphia - little tool for building designer diagram
Copyright (C) 2012  Mickael Sergent

This program is free software:
    you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
         but WITHOUT ANY WARRANTY;
without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "itemfactory.h"

CItemFactory::CItemFactory()
{

}

CItemFactory::~CItemFactory()
{
  qDebug("delete CItemFactory");
}


CProcessImplementation* CItemFactory::createNewImplementationProcess(const QString& name)
{
  return m_processFactory.createNewImplementationProcess(name);
}

CProcessContext* CItemFactory::createNewContextProcess ( QString processName )
{
  return m_processCtxFactory.createNewContextProcess(processName);
}



bool CItemFactory::renameImplementationProcess ( CProcessImplementation* item, const QString& newName )
{
  return m_processFactory.renameProcess(item, newName);
}

CProcessImplementation* CItemFactory::getImplementationProcess ( const QString& name )
{
  return m_processFactory.getImplementationProcess(name);
}

CItem* CItemFactory::search ( int type, int ID )
{
  CItem* found = NULL;
  switch(type)
  {
    case CProcessImplementation::CPROCESS_IMPL:
      found = m_processFactory.search(ID);
      break;
      
    case CProcessContext::CPROCESS_CONTEXT:
      found = m_processCtxFactory.search(ID);
      break;
      
    default:
      qDebug("search item for tyep=%d not possible", type);
      Q_ASSERT(false);
  }
  
  return found;
}

