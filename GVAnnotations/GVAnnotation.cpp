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
	mEditGroup = editGroup;

	ush = new UseEventHandler(mMapNode, mAnnoGroup, mEditGroup, mViewer);
	mViewer->addEventHandler(ush);
}

GVAnnotations::~GVAnnotations()
{

}

void GVAnnotations::initGeom(int key)
{
	switch(key)
	{
	case 1:
		line = new GV00001();
		ush->setIsStartAnno(true);
		ush->setIsNew(true);
		ush->setLineGeometry(line);
		break;
	case 2:
		line = new GV00002();
		ush->setIsStartAnno(true);
		ush->setIsNew(true);
		ush->setLineGeometry(line);
		break;
	case 3:
		line = new GV00003();
		ush->setIsStartAnno(true);
		ush->setIsNew(true);
		ush->setLineGeometry(line);
		break;
	case 4:
		line = new GV00004();
		ush->setIsStartAnno(true);
		ush->setIsNew(true);
		ush->setLineGeometry(line);
		break;
	case 5:
		line = new GV00005();
		ush->setIsStartAnno(true);
		ush->setIsNew(true);
		ush->setLineGeometry(line);
		break;
	case 6:
		line = new GV00006();
		ush->setIsStartAnno(true);
		ush->setIsNew(true);
		ush->setLineGeometry(line);
		break;
	case 7:
		line = new GV00007();
		ush->setIsStartAnno(true);
		ush->setIsNew(true);
		ush->setLineGeometry(line);
		break;
	}
}