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
#ifndef CCLASS_H
#define CCLASS_H

#include <QString>
#include <QStringList>
#include <QVariant>

class CClass;

typedef struct
{
  QVariant begin;
  QVariant end;
  QString  beginName;
  QString  endName;
} Range;

typedef struct
{
  QString name;
  CClass* pClass;
  int classId;
} Structure;

typedef union
{
  QList<Range>* rangeList;
  QStringList* enumerationList;
  QList<Structure>* aggregationList;
  int stringSize;
  int voidtype;
} classProperties;
  
class CClass
{
public:
  CClass(const QString& name, const QString& definition);
  virtual ~CClass();

  QString getName() { return m_name; }
  QString getDefinition() { return m_definition; }
  int getId() { return m_id; }
  void setId(int id) { m_id = id; }
  bool isReadOnly() { return m_readOnly; }
  void setReadOnly(bool ro) { m_readOnly = ro; }
  
  virtual int getTypeOfClass() { return m_typeClass; }
  virtual classProperties getProperties() = 0;
  virtual void insertRange(const Range& range) { Q_UNUSED(range); }

  typedef enum
  {
    INVALID,
    VOID,
    ENUMERATION,
    NUMBER,
    FLOAT,
    CHAR,
    STRING,
    AGGREGATION,
  } classType;
  
protected:
  QString m_name;
  QString m_definition;
  int m_id;
  bool m_readOnly;
  classType m_typeClass;
    
private:
};

#endif /* CCLASS_H */
