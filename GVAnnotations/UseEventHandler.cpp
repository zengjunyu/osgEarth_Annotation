#include "UseEventHandler.h"

UseEventHandler::UseEventHandler(osgEarth::MapNode* _mapNode, osg::Group* _annoGroup, osg::Group* _editGroup, osgViewer::Viewer* _viewer)
{
	myMapNode = _mapNode;
	myAnnoGroup = _annoGroup;
	myEditGroup = _editGroup;
	myViewer = _viewer;
	childNumOfAnnoGroup = 0;
	isStartAnno = false;
	isNew = false;
	lockDoubleClick = true;
	isSelect = false;
	indexOfLine = 0;
	indexOfPoint = 0;
	isSelectPoint = false;

	_gvLineCreator = new GVLineCreator();
	myLineStyle = new LineStyle();
	myDraggerPositionChanged = new DraggerPositionChanged();
}

bool UseEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* view = static_cast<osgViewer::View*>(aa.asView());
	if(isStartAnno && ea.getButtonMask())
	{

		if(ea.getButtonMask() & osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
		{
			if(_gvLineCreator->handle(ea, view, myMapNode))
			{
				myEditGroup->removeChild(0, myEditGroup->getNumChildren());
				isStartAnno = false;
			}
		}
		else if(isNew && ea.getButton() == 1) //第一次画标直接添加
		{
			if(_gvLineCreator->handle(ea, view, myMapNode))
			{
				isNew = false;
				myLine->setControlPoints(_gvLineCreator->_curCoords);
				myAnnoGroup->addChild(myLineStyle->drawLine(myMapNode, myLine));
				myEditGroup->addChild(_gvLineCreator->_controlPointShow.get());//新图标

				GVGeometry* _line = myLine;
				lineAddress.push_back(_line);
			}

			//childNumOfAnnoGroup++;

		}
		else if(!isNew && ea.getButton() == 1)
		{
			if(_gvLineCreator->handle(ea, view, myMapNode))
			{
				myLine->setControlPoints(_gvLineCreator->_curCoords);
				myAnnoGroup->setChild(myAnnoGroup->getNumChildren()-1,myLineStyle->drawLine(myMapNode, myLine));
				myEditGroup->setChild(0,_gvLineCreator->_controlPointShow.get());
			}

		}
		return true;

	}

	/*********鼠标选中某个标绘*****************/
	if(ea.getButton() == 2 && lockDoubleClick)
	{
		//osg::ref_ptr<osgGA::GUIEventAdapter> event = new osgGA::GUIEventAdapter(ea);
		if(pickLine(ea.getX(),ea.getY()) != -1)
		{
			indexOfLine = pickLine(ea.getX(),ea.getY());
			lockDoubleClick = false;
			isSelect = true;
			GVGeometry* line = lineAddress[indexOfLine];
			std::vector<GVCoord> controlPoints;
			line->getControlPoints(controlPoints);
			myDraggerPositionChanged->CreatControlPonitsShow(controlPoints,myMapNode,myEditGroup);
		}
		return true;
	}

	//删除选中线标
	if( (ea.getKey() == 'd' || ea.getKey() == 'D') && isSelect)
	{
		isSelect = false;
		lockDoubleClick = true;
		myAnnoGroup->removeChild(indexOfLine);
		myEditGroup->removeChild(0,myEditGroup->getNumChildren());
		std::vector<GVGeometry*>::iterator it = lineAddress.begin()+indexOfLine;
		lineAddress.erase(it);
		return true;
	}

	if(osgGA::GUIEventAdapter::DRAG == ea.getEventType() && isSelect )
	{
		if(pickPoint(ea.getX(),ea.getY()) != -1)
		{
			isSelectPoint = true;
			indexOfPoint = pickPoint(ea.getX(),ea.getY());
			GVGeometry* line = lineAddress[indexOfLine];
			if(indexOfPoint < line->getControlPointCount()) //拖动控制点
			{
				GVCoord coord;
				osgUtil::LineSegmentIntersector::Intersections intersection;
				view->computeIntersections(ea.getX(),ea.getY(),intersection);
				osgUtil::LineSegmentIntersector::Intersections::iterator iter = intersection.begin();
				if(iter != intersection.end())
				{
					osg::Vec3d geoPt;
					const osgEarth::SpatialReference *srs = myMapNode->getMapSRS();
					srs->transformFromWorld(iter->getWorldIntersectPoint(),geoPt);
					coord.lon = geoPt.x();
					coord.lat = geoPt.y();
					coord.alt = geoPt.z();
				}

				line->setControlPoint(indexOfPoint,coord);
				myAnnoGroup->setChild(indexOfLine,myLineStyle->drawLine(myMapNode,line));
				std::vector<GVCoord> controlPoints;
				line->getControlPoints(controlPoints);
				myEditGroup->removeChild(0,myEditGroup->getNumChildren());
				myDraggerPositionChanged->CreatControlPonitsShow(controlPoints,myMapNode,myEditGroup);
			}
			else if(indexOfPoint == line->getControlPointCount()) //拖动平移点
			{
				GVCoord coord;
				osgUtil::LineSegmentIntersector::Intersections intersection;
				view->computeIntersections(ea.getX(),ea.getY(),intersection);
				osgUtil::LineSegmentIntersector::Intersections::iterator iter = intersection.begin();
				if(iter != intersection.end())
				{
					osg::Vec3d geoPt;
					const osgEarth::SpatialReference *srs = myMapNode->getMapSRS();
					srs->transformFromWorld(iter->getWorldIntersectPoint(),geoPt);
					coord.lon = geoPt.x();
					coord.lat = geoPt.y();
					coord.alt = geoPt.z();
				}
				std::vector<GVCoord> controlPoints;
				line->getControlPoints(controlPoints);
				GVCoord centerCoord;
				if(myDraggerPositionChanged->getCenterControlPoint(controlPoints,centerCoord))
				{
					for(int i = 0; i < controlPoints.size(); i++)
					{
						controlPoints[i].lon += (coord.lon - centerCoord.lon);
						controlPoints[i].lat += (coord.lat - centerCoord.lat);
						//controlPoints[i].alt += (coord.alt - centerCoord.alt);
					}
				}
				line->setControlPointsEx(controlPoints);
				myEditGroup->removeChild(0,myEditGroup->getNumChildren());
				myAnnoGroup->setChild(indexOfLine,myLineStyle->drawLine(myMapNode,line));
				myDraggerPositionChanged->CreatControlPonitsShow(controlPoints,myMapNode,myEditGroup);

			}

		}
		else if(isSelectPoint)
		{

			//indexOfPoint = pickPoint(ea.getX(),ea.getY());
			GVGeometry* line = lineAddress[indexOfLine];
			if(indexOfPoint < line->getControlPointCount()) //拖动控制点
			{
				GVCoord coord;
				osgUtil::LineSegmentIntersector::Intersections intersection;
				view->computeIntersections(ea.getX(),ea.getY(),intersection);
				osgUtil::LineSegmentIntersector::Intersections::iterator iter = intersection.begin();
				if(iter != intersection.end())
				{
					osg::Vec3d geoPt;
					const osgEarth::SpatialReference *srs = myMapNode->getMapSRS();
					srs->transformFromWorld(iter->getWorldIntersectPoint(),geoPt);
					coord.lon = geoPt.x();
					coord.lat = geoPt.y();
					coord.alt = geoPt.z();
				}

				line->setControlPoint(indexOfPoint,coord);
				myAnnoGroup->setChild(indexOfLine,myLineStyle->drawLine(myMapNode,line));
				std::vector<GVCoord> controlPoints;
				line->getControlPoints(controlPoints);
				myEditGroup->removeChild(0,myEditGroup->getNumChildren());
				myDraggerPositionChanged->CreatControlPonitsShow(controlPoints,myMapNode,myEditGroup);
			}
			else if(indexOfPoint == line->getControlPointCount()) //拖动平移点
			{
				GVCoord coord;
				osgUtil::LineSegmentIntersector::Intersections intersection;
				view->computeIntersections(ea.getX(),ea.getY(),intersection);
				osgUtil::LineSegmentIntersector::Intersections::iterator iter = intersection.begin();
				if(iter != intersection.end())
				{
					osg::Vec3d geoPt;
					const osgEarth::SpatialReference *srs = myMapNode->getMapSRS();
					srs->transformFromWorld(iter->getWorldIntersectPoint(),geoPt);
					coord.lon = geoPt.x();
					coord.lat = geoPt.y();
					coord.alt = geoPt.z();
				}
				std::vector<GVCoord> controlPoints;
				line->getControlPoints(controlPoints);
				GVCoord centerCoord;
				if(myDraggerPositionChanged->getCenterControlPoint(controlPoints,centerCoord))
				{
					for(int i = 0; i < controlPoints.size(); i++)
					{
						controlPoints[i].lon += (coord.lon - centerCoord.lon);
						controlPoints[i].lat += (coord.lat - centerCoord.lat);
						//controlPoints[i].alt += (coord.alt - centerCoord.alt);
					}
				}
				line->setControlPointsEx(controlPoints);
				myEditGroup->removeChild(0,myEditGroup->getNumChildren());
				myAnnoGroup->setChild(indexOfLine,myLineStyle->drawLine(myMapNode,line));
				myDraggerPositionChanged->CreatControlPonitsShow(controlPoints,myMapNode,myEditGroup);

			}
		}
		return true;
	}

	else if(osgGA::GUIEventAdapter::RELEASE == ea.getEventType())
	{
		isSelectPoint = false;
		return true;
	}
	/********鼠标单击取消选中************/
	else if(ea.getButton() == 4 && (!lockDoubleClick))
	{

		lockDoubleClick = true;
		isSelect = false;
		myEditGroup->removeChild(0,myEditGroup->getNumChildren());
		return true;
	}

	return false;

}

void UseEventHandler::setIsStartAnno(bool _isStartAnno)
{
	isStartAnno = _isStartAnno;
}

void UseEventHandler::setIsNew(bool _isNew)
{
	isNew = _isNew;
}

void UseEventHandler::setLineGeometry(GVGeometry* line)
{
	myLine = line;
}

int UseEventHandler::pickLine(float x, float y)
{
	osgEarth::IntersectionPicker picker(myViewer, myAnnoGroup, ~0, 5, osgEarth::IntersectionPicker::LIMIT_ONE_PER_DRAWABLE);
	osgEarth::IntersectionPicker::Hits hits;


	if (picker.pick(x, y, hits))
	{
		osgEarth::IntersectionPicker::Hits::const_iterator h = hits.begin();
		if(h != hits.end())
		{
			const osgEarth::IntersectionPicker::Hit& hit = *h;
			//osgEarth::Util::AnnotationNode* anno = picker.getNode<osgEarth::Util::AnnotationNode>(hit);
			//osgEarth::Annotation::FeatureNode* p = dynamic_cast<osgEarth::Annotation::FeatureNode*>(anno);
			osgEarth::Annotation::FeatureNode* p= picker.getNode<osgEarth::Annotation::FeatureNode>(hit);
			int i = myAnnoGroup->getChildIndex(p->getParent(0));
			return i;
		}
	}

	return -1;
}

int UseEventHandler::pickPoint(float x, float y)
{
	osgEarth::IntersectionPicker picker(myViewer,myEditGroup, ~0, 5, osgEarth::IntersectionPicker::LIMIT_ONE_PER_DRAWABLE);
	osgEarth::IntersectionPicker::Hits hits;
	if (picker.pick(x, y, hits))
	{
		osgEarth::IntersectionPicker::Hits::const_iterator h = hits.begin();
		if(h != hits.end())
		{
			const osgEarth::IntersectionPicker::Hit& hit = *h;
			osg::MatrixTransform* anno = picker.getNode<osg::MatrixTransform>(hit);
			return myEditGroup->getChildIndex(anno);

		}
	}
	return -1;
}