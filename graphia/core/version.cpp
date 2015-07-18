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
#include "version.h"
#include <QStringList>

CVersion::CVersion()
{
  m_major = 0;
  m_minor = 0;
  m_patch = 0;
  m_isValid = false;
}


CVersion::CVersion ( unsigned int major, unsigned int minor, unsigned int patch )
{
  m_major = major;
  m_minor = minor;
  m_patch = patch;
  
  m_version = QString("%1.%2.%3").arg(m_major).arg(m_minor).arg(m_patch);
}

CVersion::CVersion ( QString version )
{
  m_version = version;
  bool ok0, ok1, ok2;
  QStringList versions;
  versions = m_version.split(".");
  
  if (versions.size() == 3)
  {
    m_major = versions[0].toUInt(&ok0);
    m_minor = versions[1].toUInt(&ok1);
    m_patch = versions[2].toUInt(&ok2);
    m_isValid = true;
    
    if ((ok0 != true) || 
        (ok1 != true) ||
        (ok2 != true))
    {
      m_major = 0;
      m_minor = 0;
      m_patch = 0;
      m_isValid =  false;
    }
  }
  else
  {
    m_major = 0;
    m_minor = 0;
    m_patch = 0;
    m_isValid = false;
  }
}

CVersion::~CVersion()
{

}

