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
#ifndef DOCUMENT_H
#define DOCUMENT_H
#include "diagrams/flow/model/processimplementation.h"
#include "diagrams/flow/graphics/graphicsscene.h"
#include "diagrams/flow/model/itemfactory.h"
#include <QFile>
#include <QVector>

class CGraphicsScene;

enum
{
  FLOW_PROCESS
};

class CDocument : public QObject
{
  Q_OBJECT
  
  public:  
    CDocument();
    virtual ~CDocument();

    void createNewDocument();
    bool read(const QString& filename);
    bool save(const QString& filename);
    bool addNewDiagram(int type, const QString& diagramName, const QString& processName);
    void removeDiagram(CGraphicsScene* diagram);
    bool needToBeSaved() { return m_isModified; }
    
    //QString getDocumentName() { return m_name; }
    //bool hasAutomaticName() { return m_automaticName; }
    
    //bool isEmpty() { return (m_diagramList.count() == 0); }
    int getNbOfDiagram() { return m_diagramList.count(); }
    CGraphicsScene* getAt(int index) { return m_diagramList.at(index); }

    CItemFactory* getItemFactory() { return m_itembuilder; }
    int getDocumentType() { return m_type; }
    
    void buildAuthorizationList( CGraphicsScene* scene );
    
    CGraphicsScene* searchExistingDiagram( const QString& name);
    bool createNewImplementationProcessDiagram( const QString& name );
    bool displayDiagram(CGraphicsScene* scene);
    
signals:
    void newDocumentCreated();
    void displayNewDiagram(CGraphicsScene* scene);
    void diagramToBeRemoved(CGraphicsScene* scene);
    void documentModified(bool isModified);
    
  public slots:
    
protected:
  struct existingDiagram
  {
    //1 = context
    //2 = implementation process
    int diagram;
    CItem* item;
  };
  
  struct existingDiagram checkExistingDiagram(const QString& name);
  
  CGraphicsScene* createImplementationProcessDiagram(const QString& diagramName, const QString& processName, CItem* exitingItem);
  CGraphicsScene* createContextDiagram(const QString& diagramName, const QString& processName, CItem* exitingItem); 

private:
   void cleanDocument();
   void emitDocumentIsModified(bool modifiedValue);
   void displayTheDiagrams();

protected:
  int m_type;
  bool m_isModified;
  QList<CGraphicsScene*> m_diagramList;
  CItemFactory* m_itembuilder;
};

#endif // DOCUMENT_H
