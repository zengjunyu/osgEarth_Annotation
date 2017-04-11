#pragma once

#include <osgEarth/MapNode>
#include <osgEarth/Decluttering>
#include <osgEarth/ECEF>
#include <osgEarth/Registry>
#include <osgEarthUtil/ActivityMonitorTool>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/AnnotationEvents>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/ExampleResources>
#include <osgEarthAnnotation/AnnotationEditing>
#include <osgEarthAnnotation/AnnotationRegistry>
#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/ImageOverlayEditor>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/RectangleNode>
#include <osgEarthAnnotation/EllipseNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/LocalGeometryNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/HighlightDecoration>
#include <osgEarthAnnotation/ScaleDecoration>
#include <osgEarthSymbology/GeometryFactory>
#include <osgEarthSymbology/Style>
#include <osgEarthFeatures/Feature>
#include <osgEarthFeatures/TessellateOperator>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgGA/EventVisitor>
#include <osgDB/WriteFile>
#include <osg/Group>

#include "GVCoord.h"
#include "GVGeometry.h"
#include "GV00001.h"
#include "UseEventHandler.h"

#ifdef GVANNOTATIONS_EXPORTS
#define GVANNOTATIONS_API __declspec(dllexport)
#else
#define GVANNOTATIONS_API __declspec(dllimport)
#endif

class GVANNOTATIONS_API GVAnnotations
{
public:
	GVAnnotations(osgViewer::Viewer* viewer, osg::Group* root, osgEarth::MapNode* mapNode, osg::Group* annoGroup, osg::Group* editGroup);
	~GVAnnotations();

	void draw();
	void initGeom(int key);

private:
	osg::ref_ptr<osg::Group> mRoot;
	osg::ref_ptr<osg::Group> mAnnoGroup;
	osg::ref_ptr<osg::Group> mEitGroup;
	osg::ref_ptr<osgEarth::MapNode> mMapNode;
	osg::ref_ptr<osgViewer::Viewer> mViewer;

	GVGeometry* geom;
};