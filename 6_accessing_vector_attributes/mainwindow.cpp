/***************************************************************************
 *   Copyright (C) 2006 by Tim Sutton   *
 *   tim@linfiniti.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "mainwindow.h"
//
// QGIS Includes
//
#include <qgsapplication.h>
#include <qgsfeature.h>
#include <qgsfield.h>
#include <qgsproviderregistry.h>
#include <qgsvectordataprovider.h>
#include <qgsvectorlayer.h>
#include <qgsmapcanvas.h>
#include <qgsproject.h>
#include <qgsproviderregistry.h>

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags fl)
    : QMainWindow(parent,fl)
{
  //required by Qt4 to initialise the ui
  setupUi(this);

  // Instantiate Provider Registry
#if defined(Q_WS_MAC)
  QString myPluginsDir        = "/Users/timsutton/apps/qgis.app/Contents/MacOS/lib/qgis";
#else
  QString myPluginsDir        = "/usr/lib/qgis/plugins";
#endif
  QgsProviderRegistry::instance(myPluginsDir);

  QString myLayerPath         = "/home/thomasg/ne_10m_admin_0_countries.shp";
  QString myLayerBaseName     = "Countries";
  QString myProviderName      = "ogr";

  QgsMapCanvas *mpMapCanvas= new QgsMapCanvas();
  mpMapCanvas->enableAntiAliasing(true);
  mpMapCanvas->setCanvasColor(QColor(255, 255, 255));
  mpMapCanvas->freeze(false);

  QgsVectorLayer * mypLayer = new QgsVectorLayer(myLayerPath, myLayerBaseName, myProviderName);

  if (mypLayer->isValid())
  {
    qDebug("Layer is valid");
  }
  else
  {
    qDebug("Layer is NOT valid");
    return;
  }
  QString csvPath = "/home/thomasg/git/QGIS-Code-Examples/6_accessing_vector_attributes/data/countries.csv";
  QString csvProviderString = QString("file://") + csvPath + QString("?type=csv&encoding=UTF-8&useHeader=Yes&xField=longitude&yField=latitude&geomType=point&subsetIndex=no&watchFile=no");
  QgsVectorLayer * csvLayer = new QgsVectorLayer( csvProviderString, "vl", "delimitedtext" );

  qDebug(QgsProviderRegistry::instance()->providerList().join(",").toUtf8().constData());

  if (csvLayer->isValid())
  {
    qDebug("CSV Layer is valid");
  }
  else
  {
    qDebug("CSV Layer is NOT valid");
    return;
  }

  //create a layerset
  QList <QgsMapLayer *> layers;

  layers.append(csvLayer);
  QgsProject::instance()->addMapLayer(csvLayer, true);

  // Add the layers to the Layer Set
  layers.append(mypLayer);//bool visibility
  // set the canvas to the extent of our layer
  mpMapCanvas->setExtent(mypLayer->extent());
  // Add the Vector Layer to the Layer Registry
  QgsProject::instance()->addMapLayer(mypLayer, true);



  // Set the Map Canvas Layer Set
  mpMapCanvas->setLayers(layers);
  /*mpMapCanvas->show();*/
  this->setCentralWidget(mpMapCanvas);
}

MainWindow::~MainWindow()
{
}
