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

#include "typefactory.h"
#include <limits>
#include "basictypes.h"

CTypeFactory::CTypeFactory()
{
  m_nextID = 0;
  createBuildinType();
}

CTypeFactory::~CTypeFactory()
{
  deleteAll();
}

QStringList* CTypeFactory::getListOfName(void )
{
  return &m_dataNameList;
}


void CTypeFactory::insertType(CClass* c, int id)
{
  if (id == -1)
  {
    c->setId(m_nextID);
  }
  m_dataList.append(c);
  
  if (id == -1)
  {
    m_nextID++;
  }
  else
  {
    if (id >= m_nextID) 
      m_nextID = id + 1;
  }
  
  m_dataNameList << c->getName();
}

void CTypeFactory::createBuildinType()
{
  CNumber* nb;
  CChar* ch;
  Range r;
  CVoid* v;
  
  v = new CVoid(tr("void"), tr("no type"));
  v->setReadOnly(true);
  insertType(v);
  
  nb = new CNumber(tr("signed char"), tr("signed integer of 8bits"));
  r.begin = -128;
  r.beginName = QString("-128");
  r.end = 127;
  r.endName = QString("127");
  nb->insertRange(r);
  nb->setReadOnly(true);
  insertType(nb);
  
  nb = new CNumber(tr("unsigned char"), tr("unsigned integer of 8bits"));
  r.begin = 0;
  r.beginName = QString("0");
  r.end = 255;
  r.endName = QString("255");
  nb->insertRange(r);
  nb->setReadOnly(true);
  insertType(nb);
  
  nb = new CNumber(tr("signed short"), tr("signed integer of 16bits"));
  r.begin = -32768;
  r.beginName = QString("-32768");
  r.end = 32767;
  r.endName = QString("32767");
  nb->insertRange(r);
  nb->setReadOnly(true);
  insertType(nb);

  nb = new CNumber(tr("unsigned short"), tr("unsigned integer of 16bits"));
  r.begin = 0;
  r.beginName = QString("0");
  r.end = 65535;
  r.endName = QString("65535");
  nb->insertRange(r);
  nb->setReadOnly(true);
  insertType(nb);

  nb = new CNumber(tr("signed long"), tr("signed integer of 32bits"));
  r.begin = (int) -2147483648;
  r.beginName = QString("-2147483648");
  r.end = (int) 2147483647;
  r.endName = QString("2147483647");
  nb->insertRange(r);
  nb->setReadOnly(true);
  insertType(nb);
  
  nb = new CNumber(tr("unsigned long"), tr("unsigned integer of 32bits"));
  r.begin = 0;
  r.beginName = QString("0");
  r.end = (unsigned int) 4294967296;
  r.endName = QString("4294967296");
  nb->insertRange(r);
  nb->setReadOnly(true);
  insertType(nb);
  
  nb = new CNumber(tr("signed long long"), tr("signed integer of 64bits"));
  r.begin = (long long) 0x8000000000000000;
  r.beginName = QString("2^63");
  r.end = (long long) 0x7FFFFFFFFFFFFFFF;
  r.endName = QString("2^63-1");
  nb->insertRange(r);
  nb->setReadOnly(true);
  insertType(nb);
  
  nb = new CNumber(tr("unsigned long long"), tr("unsigned integer of 64bits"));
  r.begin = 0;
  r.beginName = QString("0");
  r.end = (unsigned long long) std::numeric_limits<unsigned long long>::max(); //0xFFFFFFFFFFFFFFFF;
  r.endName = QString("2^64");
  nb->insertRange(r);
  nb->setReadOnly(true);
  insertType(nb);
  
  ch = new CChar(tr("ascii character"), tr("ascii character"));
  r.begin = 0;
  r.beginName = QString("NUL (0)");
  r.end = 127;
  r.endName = QString("DEL (127)");
  ch->insertRange(r);
  ch->setReadOnly(true);
  insertType(ch);
  
  ch = new CChar(tr("ext. ascii character"), tr("extended ascii character"));
  r.begin = 0;
  r.beginName = QString("NUL (0)");
  r.end = 0xFF;
  r.endName = QString(" (255)");
  ch->insertRange(r);
  ch->setReadOnly(true);
  insertType(ch);
  
  CString* s = new CString(tr("string"), tr("string"));
  s->setSize(-1);
  s->setReadOnly(true);
  insertType(s);
  
  CEnumeration* b = new CEnumeration(tr("bool"), tr("boolean"));
  b->append(tr("FALSE"));
  b->append(tr("TRUE"));
  b->setReadOnly(true);
  insertType(b);

  nb = new CFloat(tr("float"), tr("float single precision"));
  r.begin = std::numeric_limits<qreal>::min();
  r.beginName = QString("-INF");
  r.end = std::numeric_limits<qreal>::max();;
  r.endName = QString("+INF");
  nb->insertRange(r);
  nb->setReadOnly(true);
  insertType(nb);

  nb = new CFloat(tr("double"), tr("float double precision"));
  r.begin = std::numeric_limits<qreal>::min();
  r.beginName = QString("-INF");
  r.end = std::numeric_limits<qreal>::max();;
  r.endName = QString("+INF");
  nb->insertRange(r);
  nb->setReadOnly(true);
  insertType(nb);
}


CClass* CTypeFactory::getTypeById ( int id )
{
  CClass* cFound = NULL;
  
  foreach(CClass* c, m_dataList)
  {
    if (c->getId() == id)
    {
      cFound = c;
      break;
    }
  }
  return cFound;
}

void CTypeFactory::deleteAll()
{
  foreach(CClass* c, m_dataList)
  {
    delete c;
  }
  m_dataList.clear();
  m_nextID = 0;
}


