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


#include "documentreader.h"
#include <QDomElement>
#include "diagrams/generic/xmlbasicitem.h"
#include "diagrams/generic/graphicsconfiguration.h"
#include "decodeflowmodel.h"
#include "decodeflowcontext.h"
#include "decodegraphics.h"


CDocumentReader::CDocumentReader(CDocument* doc)
{
  m_doc = doc;
  m_itembuilder = NULL;
}

CDocumentReader::~CDocumentReader()
{

}

bool CDocumentReader::readFile ( const QString& filename )
{
  QDomDocument domDoc("graphia-document");
  bool bDecodeOk = false;
  QFile file(filename);
  
  if (!file.open(QIODevice::ReadOnly))
    return false;
  
  bDecodeOk = domDoc.setContent(&file);
  if (bDecodeOk)
  {
    bDecodeOk = decodeFile(domDoc);
  }
  
  if (bDecodeOk == false)
  {
    if (m_itembuilder != NULL)
      delete m_itembuilder;
    foreach(CGraphicsScene* scene, m_diagramList)
      delete scene;
    m_diagramList.clear();
  }
  
  file.close();
  return bDecodeOk;
}

bool CDocumentReader::decodeFile ( const QDomDocument& domDoc )
{  
  bool bDecodeOk = false;
  QDomElement docElem = domDoc.documentElement();
  QDomNode n = docElem.firstChild();
  QDomElement metaInf = docElem.firstChildElement("meta-inf");
  
  if (!metaInf.isNull())
  {
    bDecodeOk = readMetaInf(metaInf);
    qDebug("meta-inf found");
    
    
    if (bDecodeOk == true)
    {
      QDomElement defaultConfig = docElem.firstChildElement("default-config");
      if (!defaultConfig.isNull())
      {
        bDecodeOk = readDefaultConfig(defaultConfig, &m_readConfig);
      }
      else
        bDecodeOk = false;
    }
    
    if (bDecodeOk == true)
    {
      QDomElement document = docElem.firstChildElement("document");
      if (!document.isNull())
      {
        bDecodeOk = readDocument(document);
      }
      else 
        bDecodeOk = false;
    }
  }
  else
  {
    qDebug("meta-inf not found");
  }
  
  qDebug("decoding result = %i", bDecodeOk);
  return bDecodeOk;
}

bool CDocumentReader::readMetaInf ( const QDomElement& metaInf )
{
  bool bDecodeOk = false;
  
  //first the version
  QDomElement docNoVersion = metaInf.firstChildElement("doc-version");
  if (!docNoVersion.isNull())
  {
    QString version = docNoVersion.text();
    CVersion v(version);
    //qDebug(version.toLocal8Bit());
    qDebug("valid = %i; %i;%i;%i", v.isValid(),
           v.vmajor(),
           v.vminor(),
           v.vpatch());
    if ((v.vmajor() == 1) &&  //TODO
        (v.vminor() == 0) &&
        (v.vpatch() == 0))
      bDecodeOk = true;
    else 
      bDecodeOk = false;
  }
  
  if (bDecodeOk == true)
  {
    QDomElement components = metaInf.firstChildElement("components");
    if (!docNoVersion.isNull())
      bDecodeOk = readComponentsList(components);
    else
      bDecodeOk = false;
  }
   
  return bDecodeOk;
}

//TODO improvement management of type of component later when more diagram type will exist...
bool CDocumentReader::readComponentsList ( const QDomElement& components )
{
  bool bDecodeOk = false;
  QDomNode component;
  component = components.firstChild();
  while (!component.isNull())
  {
    //TODO check tag name "component"
    QDomElement name = component.firstChildElement("name");
    QDomElement version = component.firstChildElement("version");
    if (!name.isNull() && !version.isNull())
    {
      //qDebug("component valid %s, %s\n", name.text().toLocal8Bit().data(), version.text().toLocal8Bit().data());
      bDecodeOk = checkIfComponentExist(name.text(), CVersion(version.text()));
      if (!bDecodeOk)
        return bDecodeOk;
    }
    component = component.nextSibling();
  }
  
  return bDecodeOk;
}

bool CDocumentReader::checkIfComponentExist ( const QString& name, const CVersion& version )
{
  bool bExist = false;
  
  if (version.isValid() && 
      (name == "flow-diagram") &&
      ((version.vmajor() == 1) && (version.vminor() == 0) && (version.vpatch() == 0))
  )
  {
    bExist = true;
  }
  
  return bExist;
}



bool CDocumentReader::readDefaultConfig ( const QDomElement& defaultConfig,  CGraphicsConfiguration* pReadConfig)
{
  bool bDecodeOk = false;
  
  QDomElement scene;
  
  scene = defaultConfig.firstChildElement("scene");
  if (!scene.isNull())
  {
    QRectF sceneRect;
    sceneRect = CXmlBasicItem::decodeQRectF(scene, &bDecodeOk);
    pReadConfig->setSceneRect(sceneRect);
  }
  
  if (bDecodeOk)
  {
    QDomElement circleRadius = defaultConfig.firstChildElement("circleRadius");
    if (!circleRadius.isNull())
      pReadConfig->setCircleRadius(CXmlBasicItem::decodeQreal(circleRadius, &bDecodeOk));
    else
      bDecodeOk = false;
  }
  
  if (bDecodeOk)
  {
    QDomElement arrowAngle = defaultConfig.firstChildElement("arrowAngle");
    if (!arrowAngle.isNull())
      pReadConfig->setArrowAngle(CXmlBasicItem::decodeInt(arrowAngle, 10, &bDecodeOk));    
    else
      bDecodeOk = false;
  }
  
  if (bDecodeOk)
  {
    QDomElement arrowLength = defaultConfig.firstChildElement("arrowLength");
    if (!arrowLength.isNull())
      pReadConfig->setArrowLength(CXmlBasicItem::decodeQreal(arrowLength, &bDecodeOk));    
    else
      bDecodeOk = false;
  }
  
  if (bDecodeOk)
  {
    QDomElement anchorSize = defaultConfig.firstChildElement("anchorSize");
    if (!anchorSize.isNull())
      pReadConfig->setAnchorSize(CXmlBasicItem::decodeQreal(anchorSize, &bDecodeOk));    
    else
      bDecodeOk = false;
  }

  if (bDecodeOk)
  {
    QDomElement storeHeight = defaultConfig.firstChildElement("storeHeight");
    if (!storeHeight.isNull())
      pReadConfig->setStoreHeight(CXmlBasicItem::decodeQreal(storeHeight, &bDecodeOk));    
    else
      bDecodeOk = false;
  }
  
  if (bDecodeOk)
  {
    QDomElement storeLength = defaultConfig.firstChildElement("storeLength");
    if (!storeLength.isNull())
      pReadConfig->setStoreLength(CXmlBasicItem::decodeQreal(storeLength, &bDecodeOk));    
    else
      bDecodeOk = false;
  }
  
  if (bDecodeOk)
  {
    QDomElement storeLength = defaultConfig.firstChildElement("storeLength");
    if (!storeLength.isNull())
      pReadConfig->setStoreLength(CXmlBasicItem::decodeQreal(storeLength, &bDecodeOk));    
    else
      bDecodeOk = false;
  }
  
  if (bDecodeOk)
  {
    QDomElement alignmentH = defaultConfig.firstChildElement("alignmentH");
    if (!alignmentH.isNull())
      pReadConfig->setAlignmentH(CXmlBasicItem::decodeInt(alignmentH , 16, &bDecodeOk));    
    else
      bDecodeOk = false;
  }

  if (bDecodeOk)
  {
    QDomElement alignmentV = defaultConfig.firstChildElement("alignmentV");
    if (!alignmentV.isNull())
      pReadConfig->setAlignmentV(CXmlBasicItem::decodeInt(alignmentV, 16, &bDecodeOk));    
    else
      bDecodeOk = false;
  }

  if (bDecodeOk)
  {
    QDomElement displayLabel = defaultConfig.firstChildElement("displayLabel");
    if (!displayLabel.isNull())
      pReadConfig->setDisplayLabel(CXmlBasicItem::decodeInt(displayLabel, 10, &bDecodeOk));    
    else
      bDecodeOk = false;
  }
  
  if (bDecodeOk)
  {
    QDomElement font = defaultConfig.firstChildElement("font");
    if (!font.isNull())
      pReadConfig->setFont(CXmlBasicItem::decodeQFont(font, &bDecodeOk));
    else
      bDecodeOk = false;
  }
  
  if (bDecodeOk)
  {
    QDomElement fontColor = defaultConfig.firstChildElement("fontColor");
    if (!fontColor.isNull())
      pReadConfig->setFontColor(CXmlBasicItem::decodeQColor(fontColor, &bDecodeOk));
    else
      bDecodeOk = false;
  }

  if (bDecodeOk)
  {
    QDomElement lineColor = defaultConfig.firstChildElement("lineColor");
    if (!lineColor.isNull())
      pReadConfig->setLineColor(CXmlBasicItem::decodeQColor(lineColor, &bDecodeOk));
    else
      bDecodeOk = false;
  }

  if (bDecodeOk)
  {
    QDomElement backgroundColor = defaultConfig.firstChildElement("backgroundColor");
    if (!backgroundColor.isNull())
      pReadConfig->setBackground(CXmlBasicItem::decodeQColor(backgroundColor, &bDecodeOk));
    else
      bDecodeOk = false;
  }

  qDebug("configuration decoding status = %i", bDecodeOk);
  return bDecodeOk;
}



bool CDocumentReader::readDocument ( const QDomElement& document )
{
  bool bOk = false;
  QString type;
  type = document.attribute("type");
  if (type == "flow-process")
  {
    m_itembuilder = new CItemFactory();
    m_itembuilder->getTypeFactory()->deleteAll();
    bOk = readFlowProcessDocument(document);
  }
  
  return bOk;
}

bool CDocumentReader::readFlowProcessDocument ( const QDomElement& document )
{
  bool bOk = true;
  CDecodeFlowType decodeType(m_itembuilder->getTypeFactory());
  
  //start with types
  QDomElement types = document.firstChildElement("flow-types");
  if (!types.isNull())
  {
    bOk = CDecodeXmlFile::searchInXmlList(types, "flow-type", &decodeType);
    if (bOk)
    {
      QDomElement datas = document.firstChildElement("flow-datas");
      CDecodeFlowData decodeData(m_itembuilder->getTypeFactory(), m_itembuilder->getDataFactory());
      bOk = CDecodeXmlFile::searchInXmlList(datas, "flow-data", &decodeData);
    }

    if (bOk)
    {
      QDomElement models = document.firstChildElement("flow-model");
      if (!models.isNull())
      {
        bOk = readFlowModel(models);
      }
      else
        bOk = false;
    }
    
    if (bOk == true)
    {
      //diagram
      qDebug("start list of diagram");
      QDomElement diagramList = document.firstChildElement("diagrams-list");
      qDebug("isnull = %d", diagramList.isNull());
      CDecodeFlowGraphicsList xmlDecodeDiagramList(m_itembuilder, &m_diagramList);
      bOk = xmlDecodeDiagramList.searchInXmlList(diagramList, "diagram", &xmlDecodeDiagramList);
    }
  }
  
  return bOk;
}


bool CDocumentReader::readFlowModel( const QDomElement& datas )
{
  //implementation-process-list
  QDomNode data;
  bool bOk;
  CDecodeFlowModel xmlDecodeFlowModel(m_itembuilder);
  QDomElement processImplementationList = datas.firstChildElement("implementation-process-list");
  bOk = xmlDecodeFlowModel.searchInXmlList(processImplementationList, "implementation-process", &xmlDecodeFlowModel);

  if (bOk == true)
  {
    //context-process-list
    QDomElement processContextList = datas.firstChildElement("context-process-list");
    CDecodeFlowContext xmlDecodeProcessContext(m_itembuilder);
    bOk = xmlDecodeProcessContext.searchInXmlList(processContextList, "context-process", &xmlDecodeProcessContext);
  }
  
  return bOk;
}
