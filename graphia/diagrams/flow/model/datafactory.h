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

#ifndef DATAFACTORY_H
#define DATAFACTORY_H

#include <QStringList>
#include "data.h"
#include "class.h"

class CDataFactory
{
public:
  CDataFactory();
  virtual ~CDataFactory();

  CData* createNewData(const QString& name, CClass* cl, CData::dataType type);  
  QStringList* getListOfName(void) { return &m_strList; }
  
  int getCount() { return m_data.count(); }
  CData* getDataAt(int index) { return m_data.at(index); }
  
  QString getDataNameAt(int index);
  QString getTypeOfDataAt(int index);
  QString getFlowTypeofDataAt(int index);
  
  void insertData(CData* d, int id = -1);
  CData* search(int index);
  
protected:

protected:
  QStringList m_strList;
  QList<CData*> m_data;
  int m_nextID;

};


#endif // DATAFACTORY_H
