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
#ifndef TYPEFACTORY_H
#define TYPEFACTORY_H

#include <QList>
#include <QCoreApplication>
#include "class.h"

class CTypeFactory
{
  Q_DECLARE_TR_FUNCTIONS(CTypeFactory);
  
public:
  CTypeFactory();
  virtual ~CTypeFactory();
  void deleteAll();
  void insertType(CClass* c, int id = -1);

  QList<CClass*>* getList(void) { return &m_dataList; }
  QStringList* getListOfName(void);
  CClass* getTypeAt(int index) { return m_dataList.at(index); }
  CClass* getTypeById (int id);
  int getNbOfType() { return m_dataList.size(); }
  
protected:
  QList<CClass*> m_dataList;
  QStringList m_dataNameList;
 // int m_currentID;
  int m_nextID;

private:
  void createBuildinType();
    
};

#endif // TYPEFACTORY_H
