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
#include "class.h"
#include <QStringList>

class CEnumeration: public CClass
{
public:
  CEnumeration(const QString& name, const QString& definition);
  virtual ~CEnumeration();

  virtual classProperties getProperties();
  virtual void append(QString str) { m_enumList.append(str); }

protected:
  QStringList m_enumList;
  
private:
  
};

class CVoid : public CClass
{
public:
  CVoid(const QString& name, const QString& definition);
  virtual ~CVoid();
  
  virtual classProperties getProperties();

protected:

};


class CNumber: public CClass
{
public:
  CNumber(const QString& name, const QString& definition);
  virtual ~CNumber();
  
  virtual classProperties getProperties();
  virtual void insertRange(const Range& range);

protected:
  QList<Range> m_range;

private:
};

class CFloat: public CNumber
{
public:
  CFloat(const QString& name, const QString& definition);
  virtual ~CFloat();
};

class CChar : public CClass
{
public:
  CChar(const QString& name, const QString& definition);
  virtual ~CChar();
  virtual classProperties getProperties();
  virtual void insertRange(const Range& range);
  
protected:
  QList<Range> m_range;

};


class CString : public CClass
{
public:
  CString(const QString& name, const QString& definition);
  virtual ~CString();
  
  virtual classProperties getProperties(); 
  virtual void setSize(int s) { m_stringSize = s;}

protected:
  int m_stringSize;
};


class CStructure : public CClass
{
public:
  CStructure(const QString& name, const QString& definition);
  virtual ~CStructure();
  
  virtual classProperties getProperties();
  virtual void insertField ( const Structure& st ) { m_attributes.append(st); }
  
protected:
  QList<Structure> m_attributes;
};


