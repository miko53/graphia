/*
graphia - little tool for building designer diagram
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

#include "data.h"

CData::CData(const QString& name)
{
  m_name = name;
  m_class = NULL;
  m_id = -1;
  m_type = CData::DATA;
}

QString CData::dataFlowTypeoString(CData::dataType type)
{
  switch (type)
  {
    case CData::CONTROL:
      return QString(tr("Control"));
      break;
      
    case CData::DATA:
      return QString(tr("Data"));
      break;
      
    case CData::CONTINUEOUS:
      return QString(tr("Contineous"));
      break;
      
    default:
      Q_ASSERT(false);
  }
  
  return QString();
}


CData::~CData()
{

}

