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
#include "basictypes.h"


CVoid::CVoid(const QString& name, const QString& definition): CClass(name, definition)
{
  m_typeClass = VOID;
}

CVoid::~CVoid()
{

}

classProperties CVoid::getProperties()
{
  classProperties ret;
  ret.voidtype = 0;
  return ret;
}


CEnumeration::CEnumeration(const QString& name, const QString& definition) : CClass(name, definition)
{
  m_typeClass = ENUMERATION;
}

CEnumeration::~CEnumeration()
{

}

classProperties CEnumeration::getProperties()
{
  classProperties ret;
  ret.enumerationList = &m_enumList;
  return ret;
}

void CChar::insertRange(const Range& range)
{
  m_range.append(range);
}


CNumber::CNumber(const QString& name, const QString& definition): CClass(name, definition)
{
  m_typeClass = NUMBER;
}

CNumber::~CNumber()
{

}

classProperties CNumber::getProperties()
{
  classProperties ret;
  ret.rangeList = &m_range;
  
  return ret;
}

void CNumber::insertRange(const Range& range)
{
  m_range.append(range);
}

CFloat::CFloat ( const QString& name, const QString& definition ) : CNumber ( name, definition )
{
  m_typeClass = FLOAT;
}

CFloat::~CFloat()
{

}



CChar::CChar(const QString& name, const QString& definition): CClass(name, definition)
{
  m_typeClass = CHAR;
}

CChar::~CChar()
{

}

classProperties CChar::getProperties()
{
  classProperties ret;
  ret.rangeList = &m_range;
  return ret;
}

CString::CString(const QString& name, const QString& definition): CClass(name, definition)
{
  m_typeClass = STRING;
}

CString::~CString()
{

}

classProperties CString::getProperties()
{
  classProperties ret;
  ret.stringSize = m_stringSize;
  return ret;
}



CStructure::CStructure(const QString& name, const QString& definition): CClass(name, definition)
{
  m_typeClass = AGGREGATION;
}

CStructure::~CStructure()
{

}

classProperties CStructure::getProperties()
{
  classProperties ret;
  ret.aggregationList = &m_attributes;
  return ret;
}







