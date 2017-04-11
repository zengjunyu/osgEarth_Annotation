#pragma once

#include "GVAnnotations.h"

using namespace osgEarth;
using namespace osgEarth::Annotation;
using namespace osgEarth::Features;
using namespace osgEarth::Util;
using namespace osgEarth::Util::Controls;

GVAnnotations::GVAnnotations(osgViewer::Viewer* viewer, osg::Group* root, osgEarth::MapNode* mapNode, osg::Group* annoGroup, osg::Group* editGroup)
{
	mViewer = viewer;
	mRoot = root;
	mMapNode = mapNode;
	mAnnoGroup = annoGroup;
	mEitGroup = editGroup;
}

GVAnnotations::~GVAnnotations()
{

}

void GVAnnotations::draw()
{
	/*Style geomStyle;
	geomStyle.getOrCreate<LineSymbol>()->stroke()->color() = Color::Red;
	geomStyle.getOrCreate<LineSymbol>()->stroke()->width() = 2.0f;
	geomStyle.getOrCreate<LineSymbol>()->stroke()->widthUnits() = Units::PIXELS;
	geomStyle.getOrCreate<LineSymbol>()->tessellation() = 200.0f;
	geomStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
	geomStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_GPU;

	std::vector<GVCoord> coords;
	std::vector<GVCoord> vtxBuffer;
	std::vector<int> vtxBuffer2;

	coords.push_back(GVCoord(0, 0, 0));
	coords.push_back(GVCoord(70, 0, 0));
	geom->setControlPoints(coords);

	if(geom->toVertice(vtxBuffer, &vtxBuffer2))
	{
		osg::ref_ptr<osgEarth::Features::MultiGeometry> geoms = new osgEarth::Features::MultiGeometry;
		unsigned int count = 0;
		for(int i =0; i < vtxBuffer2.size(); i++)
		{
			osg::ref_ptr<osgEarth::Symbology::LineString> ls = new osgEarth::Symbology::LineString;
			for(int j =0; j < vtxBuffer2[i]; j++)
			{
				ls->push_back(osg::Vec3d(vtxBuffer[count].lon, vtxBuffer[count].lat, vtxBuffer[count].alt));
				count++;
			}
			geoms->add(ls.get());

			osg::ref_ptr<osgEarth::Features::Feature> feature = new osgEarth::Features::Feature(geoms, mMapNode->getMapSRS(), geomStyle);
			osg::ref_ptr<osgEarth::Annotation::FeatureNode> gnode = new osgEarth::Annotation::FeatureNode(mMapNode, feature);
			gnode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
			mAnnoGroup->addChild( gnode.get() );
		}
	}*/

	mRoot->addChild(mAnnoGroup.get());

}

void GVAnnotations::initGeom(int key)
{
	switch(key)
	{
	case 1:
		geom = new GV00001();
		mViewer->addEventHandler(new UseEventHandler(mMapNode, mAnnoGroup, mViewer, true, true));
		break;
	}
}