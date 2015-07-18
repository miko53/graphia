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

#ifndef DATA_H
#define DATA_H

#include <QCoreApplication>
#include "class.h"

class CData
{
  Q_DECLARE_TR_FUNCTIONS(CData);

public:
  typedef enum
  {
    DATA,
    CONTROL,
    CONTINUEOUS
  } dataType;
  
  CData(const QString& name);
  virtual ~CData();
  
  const QString& getName() { return m_name;}
  
  void setClass(CClass* cl) { m_class = cl; }
  CClass* getClass() { return m_class; }
  
  void setDataType(dataType t) { m_type = t; }
  dataType getDataType() { return m_type; }
  
  void setID(int id) { m_id = id; }
  int getID(void) { return m_id; }
  
  static QString dataFlowTypeoString(dataType type);
  
protected:
  QString m_name;
  CClass* m_class;
  dataType m_type;
  int m_id;
};

#endif // DATA_H
