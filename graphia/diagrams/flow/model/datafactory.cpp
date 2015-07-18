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

#include "datafactory.h"

CDataFactory::CDataFactory()
{
  m_nextID = 0;
}

CData* CDataFactory::createNewData(const QString& name, CClass* cl, CData::dataType type)
{
  CData* data;
  data = NULL;
  
  if (m_strList.contains(name) == false)
  {
    data = new CData(name);
    data->setClass(cl);
    data->setDataType(type);
    insertData(data);
  }
  
  return data;
}


void CDataFactory::insertData(CData* d, int id)
{
  if (id == -1)
  {
    d->setID(m_nextID);
  }
  
  m_data.append(d);
  
  if (id == -1)
  {
    m_nextID++;
  }
  else
  {
    if (id >= m_nextID) 
      m_nextID = id + 1;
  }
  
  m_strList << d->getName();
}

QString CDataFactory::getDataNameAt(int index)
{
 CData* data;
 data = getDataAt(index);
 Q_ASSERT(data != NULL);
 return data->getName();
}


QString CDataFactory::getTypeOfDataAt(int index)
{
  CData* data;
  data = m_data.at(index);
  return data->getClass()->getName();
}


QString CDataFactory::getFlowTypeofDataAt(int index)
{
  CData* data;
  data = m_data.at(index);
  return CData::dataFlowTypeoString(data->getDataType());
}


CData* CDataFactory::search(int index)
{
  CData* found = NULL;
  
  foreach(CData* d, m_data)
  {
    if (d->getID() == index)
    {
      found = d;
      break;
    }
  }
  
  return found;
}


CDataFactory::~CDataFactory()
{

}

