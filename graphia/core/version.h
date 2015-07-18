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


#ifndef VERSION_H
#define VERSION_H
#include <QString>

class CVersion
{
public:
  CVersion();
  CVersion(unsigned int major, unsigned int minor, unsigned int patch);
  CVersion(QString version);
  virtual ~CVersion();
  QString getVersionString() const { return m_version; }
  
  int vmajor() const { return m_major;}
  int vminor() const { return m_minor;}
  int vpatch() const { return m_minor;}
  bool isValid() const { return m_isValid; }
  
protected:
  unsigned int m_major;
  unsigned int m_minor;
  unsigned int m_patch;
  QString m_version;
  bool m_isValid;
};

#endif // VERSION_H
