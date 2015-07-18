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

#ifndef DECODEFLOWCONTEXT_H
#define DECODEFLOWCONTEXT_H

#include "decodeflowmodel.h"
#include "diagrams/flow/model/itemfactory.h"

class CDecodeFlowContext: public CDecodeXmlFile
{
public:
  CDecodeFlowContext(CItemFactory* factory);
  virtual bool decode ( const QDomElement& item );

protected:
  CProcessContext* readCProcessContext ( const QDomElement& element );
  CProcessInstance* readInstanceOfContextProcess ( const QDomElement& data, CProcessContext* processContext );
  
protected:
  CItemFactory* m_pItemFactory;
};


class CDecodeTerminatorList : public CDecodeXmlFile
{
public:
  CDecodeTerminatorList(CProcessContext* p, CDataFactory* dataFactory);
  virtual bool decode ( const QDomElement& item );
  
protected:
   CTerminator* readTerminator ( QDomElement item );

protected:
  CProcessContext* m_ctxProcess;
  CDataFactory* m_dataFactory;
};



#endif /* DECODEFLOWCONTEXT_H */
