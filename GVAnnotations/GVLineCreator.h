#ifndef __GV_LINE_CREATOR_H__
#define __GV_LINE_CREATOR_H__

#include <osgGA/GUIEventAdapter>
#include <osgEarth/MapNode>
#include <osg/ShapeDrawable>
#include <osg/AutoTransform>
#include <osgViewer/View>
#include <assert.h>
#include "GVCoord.h"

class GVLineCreator
{
public:
	GVLineCreator();
	~GVLineCreator();

	bool getGeoPosition(const osgGA::GUIEventAdapter& ea, osgViewer::View* view, GVCoord &coord, osgEarth::MapNode* mapNode) const;
	bool getGeoPosition(osgViewer::View *view, double mouseX, double mouseY, GVCoord &coord, osgEarth::MapNode* mapNode)const;

	bool handle(const osgGA::GUIEventAdapter& ea, osgViewer::View* view, osgEarth::MapNode* mapNode);
	bool stepCreate(const GVCoord &coord, bool finish, osgEarth::MapNode* mapNode);
	bool addGeometryPoint(const GVCoord &coord, osgEarth::MapNode* mapNode);
	void completeCreate();

	std::vector<GVCoord> _curCoords;	
	osg::ref_ptr<osg::Group> _controlPointShow;

};

#endif //__GV_LINE_CREATOR_H__
