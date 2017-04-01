#pragma once

#include "GVAnnotations.h"

using namespace osgEarth;
using namespace osgEarth::Annotation;
using namespace osgEarth::Features;
using namespace osgEarth::Util;
using namespace osgEarth::Util::Controls;

GVAnnotations::GVAnnotations(osg::Group* root, osgEarth::MapNode* mapNode)
{
	mRoot = root;
	mMapNode = mapNode;
}

GVAnnotations::~GVAnnotations()
{

}

void GVAnnotations::draw()
{
	osg::ref_ptr<osg::Group> node = new osg::Group();

	const SpatialReference* geoSRS = mMapNode->getMapSRS()->getGeographicSRS();

	Geometry* geom = new Polygon();
	geom->push_back( osg::Vec3d(0,   40, 0) );
	geom->push_back( osg::Vec3d(-60, 40, 0) );
	geom->push_back( osg::Vec3d(-60, 60, 0) );
	geom->push_back( osg::Vec3d(0,   60, 0) );
	Style geomStyle;
	geomStyle.getOrCreate<LineSymbol>()->stroke()->color() = Color::Cyan;
	geomStyle.getOrCreate<LineSymbol>()->stroke()->width() = 5.0f;
	geomStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
	geomStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_GPU;
	FeatureNode* gnode = new FeatureNode(mMapNode, new Feature(geom, geoSRS), geomStyle);
	node->addChild( gnode );

	mRoot->addChild(node.get());

}