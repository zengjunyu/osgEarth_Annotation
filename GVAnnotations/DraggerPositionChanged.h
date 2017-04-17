#ifndef __DRAGGER_POSITION_CHANGED_H__
#define __DRAGGER_POSITION_CHANGED_H__

#include "GVCoord.h"
#include <osg/Node>
#include <osg/StateSet>
#include <osg/ShapeDrawable>
#include <osg/AutoTransform>
#include <osgEarth/MapNode>


class DraggerPositionChanged
	: public osg::Group
{
public:
	DraggerPositionChanged();
	~DraggerPositionChanged();
	void CreatControlPonitsShow(std::vector<GVCoord> vtxBuffer,osgEarth::MapNode* mapNode,osg::Group* editGroup);
	void updateDraggerPosition(int idx,const GVCoord coord,osgEarth::MapNode* mapNode,osg::Group* editGroup);
	bool getCenterControlPoint(const std::vector<GVCoord> vtxBuffer,GVCoord &coord);
};

#endif //__DRAGGER_POSITION_CHANGED_H__