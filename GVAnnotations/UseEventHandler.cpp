#include "UseEventHandler.h"

UseEventHandler::UseEventHandler(osgEarth::MapNode* _mapNode, osg::Group* _annoGroup, osgViewer::Viewer* _viewer, bool _isStartAnno, bool _isNew)
{
	myMapNode = _mapNode;
	myAnnoGroup = _annoGroup;
	myViewer = _viewer;
	childNumOfAnnoGroup = 0;
	isStartAnno = _isStartAnno;
	isNew = _isNew;

	_gvLineCreator = new GVLineCreator();
}

bool UseEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* view = static_cast<osgViewer::View*>(aa.asView());
	if(isStartAnno && ea.getButtonMask())
	{

		if(ea.getButtonMask() & osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
		{
			_gvLineCreator->handle(ea, view, myMapNode);
			isStartAnno = false;
		}
		else if(isNew && ea.getButton() == 1) //第一次画标直接添加
		{
			_gvLineCreator->handle(ea, view, myMapNode);
			isNew = false;

			myAnnoGroup->addChild(_gvLineCreator->_controlPointShow.get());//新图标
			//myViewer->getSceneData()->asGroup()->setChild(1,myAnnoGroup);

			childNumOfAnnoGroup++;

		}
		else if(!isNew && ea.getButton() == 1)
		{
			_gvLineCreator->handle(ea, view, myMapNode);
			myAnnoGroup->setChild(childNumOfAnnoGroup-1, _gvLineCreator->_controlPointShow.get());
			myViewer->getSceneData()->asGroup()->setChild(1, myAnnoGroup.get());

		}
		return true;

	}
	if ( ea.getKey() == 'c' || ea.getKey() == 'C' )
	{
		isStartAnno = true;
		isNew = true;

	}

	return false;

}