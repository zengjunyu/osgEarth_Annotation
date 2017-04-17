#ifndef __USE_EVENT_HANDLER_H__
#define __USE_EVENT_HANDLER_H__

#include "GVLineCreator.h"
#include "LineStyle.h"
#include "GVGeometry.h"
#include "DraggerPositionChanged.h"
#include <osgEarth/IntersectionPicker>
#include <osgViewer/Viewer>

class UseEventHandler : public osgGA::GUIEventHandler
{
public:
	UseEventHandler(osgEarth::MapNode* mapNode, osg::Group* annoGroup, osg::Group* editGroup, osgViewer::Viewer* viewer);
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

	void setIsStartAnno(bool _isStartAnno);
	void setIsNew(bool _isNew);
	void setLineGeometry(GVGeometry* line);
	int pickLine(float x, float y); 
	int pickPoint(float x, float y);

public:
	unsigned int childNumOfAnnoGroup;

private:
	osg::ref_ptr<osgEarth::MapNode> myMapNode;
	osg::ref_ptr<osg::Group> myAnnoGroup;
	osg::ref_ptr<osg::Group> myEditGroup;
	osg::ref_ptr<osgViewer::Viewer> myViewer;
	bool isStartAnno; 
	bool isNew;
	bool lockDoubleClick;
	bool isSelect;
	bool isSelectPoint;
	int indexOfLine;
	int indexOfPoint;

	GVLineCreator* _gvLineCreator;
	GVGeometry* myLine;
	LineStyle* myLineStyle;
	DraggerPositionChanged * myDraggerPositionChanged;
	std::vector<GVGeometry*> lineAddress; //存放线表函数指针
};

#endif //__USE_EVENT_HANDLER_H__