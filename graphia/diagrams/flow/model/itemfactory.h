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

#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include "processimplfactory.h"
#include "processcontextfactory.h"
#include "typefactory.h"
#include "datafactory.h"


class CItemFactory
{
public:
  CItemFactory();
  virtual ~CItemFactory();
  
  CProcessImplementation* createNewImplementationProcess(const QString& name);
  CProcessContext* createNewContextProcess ( QString processName );
  
  bool renameImplementationProcess(CProcessImplementation* item, const QString& newName);
  CProcessImplementation* getImplementationProcess(const QString& name);
  
  CTypeFactory* getTypeFactory() { return &m_typeFactory; }
  CDataFactory* getDataFactory() { return &m_dataFactory; }
  CProcessImplFactory* getProcessFactory() { return &m_processFactory; }
  CProcessContextFactory* getContexProcessFactory() { return &m_processCtxFactory; }
  
  CItem* search( int type, int ID );
  
protected:
  CProcessImplFactory m_processFactory;
  CTypeFactory        m_typeFactory;
  CDataFactory        m_dataFactory;
  CProcessContextFactory m_processCtxFactory;
};

#endif // ITEMFACTORY_H
