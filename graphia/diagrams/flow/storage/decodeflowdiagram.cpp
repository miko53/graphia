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

#include "decodeflowdiagram.h"
#include "diagrams/flow/model/basictypes.h"


bool CDecodeXmlFile::searchInXmlList(const QDomElement& list, const QString& itemName, CDecodeXmlFile* pDecode)
{
  bool bDecodingStatus;
  QDomNode currentItem = list.firstChild();
  
  bDecodingStatus = true;
  while (!currentItem.isNull() &&
         (bDecodingStatus == true) &&
         currentItem.isElement() &&
         currentItem.toElement().tagName() == itemName)
  {
    bDecodingStatus = pDecode->decode(currentItem.toElement());
    currentItem = currentItem.nextSibling();
  }
  
  return bDecodingStatus;
}


CDecodeFlowType::CDecodeFlowType ( CTypeFactory* typeFactory )
{
  m_typeFactory = typeFactory;
}

bool CDecodeFlowType::decode ( const QDomElement& item )
{
  CClass* cl = readClassType(item);
  bool bCorrectlyDecoded;
  
  if (cl != NULL)
  {
    m_typeFactory->insertType(cl, cl->getId());
    bCorrectlyDecoded = true;
  }
  else
    bCorrectlyDecoded = false;
  
  return bCorrectlyDecoded; 
}


CClass* CDecodeFlowType::readClassType ( const QDomElement& element)
{
  bool bOk = false;
  CClass* cl = NULL;
  
  QDomElement id = element.firstChildElement("id");
  QDomElement name = element.firstChildElement("name");
  QDomElement definition = element.firstChildElement("definition");
  QDomElement readOnly = element.firstChildElement("read-only");
  QDomElement values = element.firstChildElement("values");
  QDomElement type = element.firstChildElement("type");

  switch (type.text().toInt())
  {
    case CClass::VOID:
    {
      CVoid* v = new CVoid(name.text(), definition.text());
      v->setId(id.text().toInt(&bOk));
      v->setReadOnly((readOnly.text().toInt(&bOk)));
      cl = v;
      bOk = true;
    }
      break;
      
    case CClass::ENUMERATION:
    {
      CEnumeration* e = new CEnumeration(name.text(), definition.text());
      e->setId(id.text().toInt(&bOk));
      e->setReadOnly((readOnly.text().toInt(&bOk)));
      QDomElement data = values.firstChildElement("string");
      while (!data.isNull())
      {
        e->append(data.text());
        data = data.nextSiblingElement("string");
      }
      bOk = true;
      cl = e;
    }
      break;
    
    case CClass::NUMBER:
    {
      CNumber* n = new CNumber(name.text(), definition.text());
      n->setId(id.text().toInt(&bOk));
      n->setReadOnly((readOnly.text().toInt(&bOk)));
      QDomElement range = values.firstChildElement("range");
      if (!range.isNull())
      {
        Range r;
        r.begin = range.attribute("begin").toLongLong(&bOk);
        if (!bOk)
        {
          // try with uLongLong
          r.begin = range.attribute("begin").toULongLong(&bOk);
        }
        r.end = range.attribute("end").toLongLong(&bOk);
        if (!bOk)
        {
          // try with uLongLong
          r.end = range.attribute("end").toULongLong(&bOk);
        }
        r.beginName = range.attribute("begin-name");
        r.endName = range.attribute("end-name");
        n->insertRange(r);
        bOk = true;
      }
      else 
        bOk = false;
      
      cl = n;
    }
      break;
    
    case CClass::FLOAT:
    {
      CFloat* f = new CFloat(name.text(), definition.text());
      f->setId(id.text().toInt(&bOk));
      f->setReadOnly((readOnly.text().toInt(&bOk)));
      QDomElement range = values.firstChildElement("range");
      if (!range.isNull())
      {
        Range r;
        r.begin = range.attribute("begin").toDouble(&bOk);
        r.end = range.attribute("end").toDouble(&bOk);
        r.beginName = range.attribute("begin-name");
        r.endName = range.attribute("end-name");
        f->insertRange(r);
        bOk = true;
      }
      else 
        bOk = false;
      
      cl = f;
    }
      break;
      
    case CClass::CHAR:
    {
      CChar* c = new CChar(name.text(), definition.text());
      c->setId(id.text().toInt(&bOk));
      c->setReadOnly((readOnly.text().toInt(&bOk)));
      QDomElement range = values.firstChildElement("range");
      if (!range.isNull())
      {
        Range r;
        r.begin = range.attribute("begin").toInt(&bOk);
        r.end = range.attribute("end").toInt(&bOk);
        r.beginName = range.attribute("begin-name");
        r.endName = range.attribute("end-name");
        c->insertRange(r);
        bOk = true;
      }
      else 
        bOk = false;
      
      cl = c;
    }
      break;
      
    case CClass::STRING:
    {
      CString* s = new CString(name.text(), definition.text());
      s->setId(id.text().toInt(&bOk));
      s->setReadOnly((readOnly.text().toInt(&bOk)));
      QDomElement size = values.firstChildElement("size");
      if (!size.isNull())
      {
        s->setSize(size.text().toInt(&bOk));
        bOk = true;
      }
      else 
        bOk = false;
      
      cl = s;
    }
      break;
      
    case CClass::AGGREGATION:
    {
      CStructure* s = new CStructure(name.text(), definition.text());
      s->setId(id.text().toInt(&bOk));
      s->setReadOnly((readOnly.text().toInt(&bOk)));
      QDomElement structure = values.firstChildElement("structure");
      while (!structure.isNull() && !bOk)
      {
        Structure st;
        st.name = structure.attribute("name");
        st.classId = structure.attribute("type").toInt(&bOk);
        st.pClass = NULL;
        if (bOk == true)
        {
          s->insertField(st);
        }
        structure = structure.nextSiblingElement("structure");
      }      
      cl = s;
    }
      break;
      
    default:
      bOk = false;
      break;
  }
  
  if ((bOk == false) & (cl != NULL))
  {
    delete cl;
    cl = NULL;
  }
  
  return cl;
}


CDecodeFlowData::CDecodeFlowData ( CTypeFactory* typeFactory, CDataFactory* datafactory )
{
  m_dataFactory = datafactory;
  m_typeFactory = typeFactory;
}



bool CDecodeFlowData::decode ( const QDomElement& item )
{
  CData* d = readData(item);
  bool bCorrectlyDecoded;
  
  if (d != NULL)
  {
    m_dataFactory->insertData(d, d->getID());
    bCorrectlyDecoded = true;
  }
  else
    bCorrectlyDecoded = false;
  
  return bCorrectlyDecoded; 
}


CData* CDecodeFlowData::readData ( const QDomElement& data)
{
  bool bOk = false;
  CData* d = NULL;
  
  QDomElement id = data.firstChildElement("id");
  QDomElement name = data.firstChildElement("name");
  QDomElement cl = data.firstChildElement("class");
  QDomElement flowType = data.firstChildElement("flow-type");
  
  if (!id.isNull() && !name.isNull() &&
      !cl.isNull() && !flowType.isNull())
  {
    bOk = true;
    d = new CData(name.text());
    d->setID(id.text().toInt(&bOk));
    d->setDataType((CData::dataType) (flowType.text().toInt(&bOk)) );
    
    CClass* c = m_typeFactory->getTypeById(d->getID());
    if (c != NULL)
      d->setClass(c);
    else
      bOk = false;   
  }
  
  if ((d != NULL) && (bOk == false))
  {
    delete d;
    d = NULL;
  }
  
  return d;
}

