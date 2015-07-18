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

#ifndef DECODEFLOWDIAGRAM_H
#define DECODEFLOWDIAGRAM_H

#include <QDomElement>
#include "diagrams/flow/model/class.h"
#include "diagrams/flow/model/typefactory.h"
#include "diagrams/flow/model/datafactory.h"

class CDecodeXmlFile
{
public:
  virtual bool decode(const QDomElement& item) = 0;
  static bool searchInXmlList( const QDomElement& list, const QString& itemName, CDecodeXmlFile* pDecode); //TODO remove pDecode et le fait que la fonction soit statci cela doit etre possible
};

class CDecodeFlowType: public CDecodeXmlFile
{
public:
  CDecodeFlowType(CTypeFactory* typeFactory);
  virtual bool decode ( const QDomElement& item );
  
protected:
  CClass* readClassType (const QDomElement& element);

protected:
  CTypeFactory* m_typeFactory;
};

class CDecodeFlowData: public CDecodeXmlFile
{
public:
  CDecodeFlowData(CTypeFactory* typeFactory, CDataFactory* datafactory);
  virtual bool decode ( const QDomElement& item );

protected:
  CData* readData (const QDomElement& element);

protected:
  CTypeFactory* m_typeFactory;
  CDataFactory* m_dataFactory;
};


#endif /* DECODEFLOWDIAGRAM_H */


