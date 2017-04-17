#ifndef __LINE_STYLE_H__
#define __LINE_STYLE_H__

#include <osgEarth/MapNode>
#include <osgEarthAnnotation/FeatureNode>
#include "GVCoord.h"
#include "GVGeometry.h"

using namespace osgEarth::Features;

class LineStyle
{
public:
	osg::Group* drawLine(osgEarth::MapNode* mapNode, GVGeometry* line);
};

#endif //__LINE_STYLE_H__