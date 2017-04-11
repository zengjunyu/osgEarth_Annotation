#ifndef __USE_EVENT_HANDLER_H__
#define __USE_EVENT_HANDLER_H__

#include "GVLineCreator.h"
#include <osgViewer/Viewer>

class UseEventHandler : public osgGA::GUIEventHandler
{
public:
	UseEventHandler(osgEarth::MapNode* mapNode, osg::Group* annoGroup, osgViewer::Viewer* viewer, bool _isStartAnno, bool _isNew);

	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

public:
	unsigned int childNumOfAnnoGroup;

private:
	osg::ref_ptr<osgEarth::MapNode> myMapNode;
	osg::ref_ptr<osg::Group> myAnnoGroup;
	osg::ref_ptr<osgViewer::Viewer> myViewer;
	bool isStartAnno; 
	bool isNew;

	GVLineCreator* _gvLineCreator;
};

#endif //__USE_EVENT_HANDLER_H__