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


#ifndef STOREFACTORY_H
#define STOREFACTORY_H
#include <QList>
#include "store.h"

class CStoreFactory
{
public:
  CStoreFactory();
  virtual ~CStoreFactory();
  CStore* createNew();
  bool remove(CStore* item);
  bool renameItem ( CItem* item, QString name );
  int getCount() { return m_data.count(); }
  CStore* getAt(int index) { return m_data.at(index); }
  CItem* search ( int ID );
  void insertItem ( CStore* item );
  
protected:
  CItem* ifAlreadyExists ( const QString& name );

protected:
  QList<CStore*> m_data;
  int m_currentID;
  
private:
  void removeAll();
};

#endif // STOREFACTORY_H
