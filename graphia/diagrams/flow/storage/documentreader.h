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
#ifndef DOCUMENTREADER_H
#define DOCUMENTREADER_H
#include <QString>
#include <QDomElement>
#include "core/document.h"
#include "core/version.h"
#include "diagrams/generic/graphicsconfiguration.h"
#include "decodeflowdiagram.h"

class CDocumentReader
{
public:
  CDocumentReader(CDocument* doc);
  virtual ~CDocumentReader();

  bool readFile(const QString& filename);

  CGraphicsConfiguration* getReadConfig() { return &m_readConfig; }
  CItemFactory* getItemBuilder() { return m_itembuilder;}
  QList<CGraphicsScene*>* getDiagramList() { return &m_diagramList;}  
  
protected:
  CDocument* m_doc;
  //variable utilisé pour le decodage a recopier ensuite dans le document
  CGraphicsConfiguration m_readConfig;
  CItemFactory* m_itembuilder;
  QList<CGraphicsScene*> m_diagramList;
  
  
private:
  bool decodeFile (const QDomDocument& domDoc );
  bool readMetaInf(const QDomElement& metaInf);
  bool readComponentsList (const QDomElement& components );
  bool checkIfComponentExist (const QString& name, const CVersion& version);
  
  bool readDefaultConfig(const QDomElement& defaultConfig, CGraphicsConfiguration* pReadConfig);
  bool readDocument ( const QDomElement& document );
  bool readFlowProcessDocument ( const QDomElement& document );
  CTypeFactory* readFlowTypeList ( const QDomElement& types, bool* bOk );
  CClass* readClassType ( const QDomElement& element, bool* bOk );
  CDataFactory* readFlowDataList ( const QDomElement& datas, bool* bOk, CTypeFactory* pTypeFactory );
  CData* readData ( const QDomElement& data, bool* bOk, CTypeFactory* pTypeFactory );
  
  bool readFlowModel(const QDomElement& datas);

};

#endif // DOCUMENTREADER_H
