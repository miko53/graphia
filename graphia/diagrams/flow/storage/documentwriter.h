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


#ifndef DOCUMENTWRITER_H
#define DOCUMENTWRITER_H

#include "core/document.h"
#include <QDomDocument>

class CDocumentWriter
{
public:
  CDocumentWriter(CDocument* doc);
  virtual ~CDocumentWriter();

  void saveDocument(const QString& filename);
  
protected:
  CDocument* m_doc;
  
private:
  void buildPreambule( QDomDocument& doc );
  void buildMetaInf( QDomDocument& doc, QDomElement& root );
  void insertVersion ( QDomDocument& doc, QDomElement& metaInf );
  void insertListOfComponents ( QDomDocument& doc, QDomElement& components );
  void buildDocument ( QDomDocument& doc, QDomElement& root );
  void buildDefaultConfiguration ( QDomDocument& doc, QDomElement& root );
  void insertFlowType( QDomDocument& doc, QDomElement& diagrams );
  void insertFlowData ( QDomDocument& doc, QDomElement& diagrams );
  void insertFlowModel ( QDomDocument& doc, QDomElement& diagrams );
  void insertFlowOfImplementedProcess ( QDomDocument& doc, QDomElement& process, CProcessImplementation* processCurrent );
  void insertProcessInstanceOfImplementedProcess ( QDomDocument& doc, QDomElement& process, CProcessImplementation* processCurrent );
  void insertStoreOfImplementedProcess ( QDomDocument& doc, QDomElement& process, CProcessImplementation* processCurrent );
  void insertInputOfImplementedProcess ( QDomDocument& doc, QDomElement& process, CProcessImplementation* processCurrent );
  void insertOutputOfImplementedProcess ( QDomDocument& doc, QDomElement& process, CProcessImplementation* processCurrent );
  void insertFlowOfContextProcess ( QDomDocument& doc, QDomElement& process, CProcessContext* processCtxCurrent );
  void insertStoreOfContextProcess ( QDomDocument& doc, QDomElement& process, CProcessContext* processCtxCurrent );
  void insertTerminatorOrContextProcess ( QDomDocument& doc, QDomElement& process, CProcessContext* processCtxCurrent );
  void insertProcessInstanceOfContextProcess ( QDomDocument& doc, QDomElement& process, CProcessContext* processCtxCurrent );
  void insertFlowDiagram(QDomDocument& doc, QDomElement& diagrams);
};

#endif // DOCUMENTWRITER_H
