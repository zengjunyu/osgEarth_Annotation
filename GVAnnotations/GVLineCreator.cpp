#include "GVLineCreator.h"

class CreateControlPointShow
	: public osg::Group
{
public:
	CreateControlPointShow()
	{
		osg::StateSet* stateSet = getOrCreateStateSet();
		stateSet->setMode(0x0B71, 0);
		stateSet->setRenderBinDetails(99, "RenderBin");
		stateSet->setMode(0x0B50, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	}

	void clear()
	{
		removeChild(0, getNumChildren());
	}

	void addControlPoint(const GVCoord &coord, osgEarth::MapNode* mapNode)
	{
		osg::Sphere* shape = new osg::Sphere(osg::Vec3(0, 0, 0), 4.0f);
		osg::ShapeDrawable *_centerSphere = new osg::ShapeDrawable(shape);
		_centerSphere->setDataVariance(osg::Object::DYNAMIC);
		_centerSphere->setColor(osg::Vec4(1.0, 0.0, 0.0, 1.0));

		osg::Geode* geode = new osg::Geode();
		geode->addDrawable(_centerSphere);

		osg::AutoTransform *at = new osg::AutoTransform;
		at->setAutoScaleToScreen(true);
		at->addChild(geode);

		/*******************************************/
		{

			const osgEarth::SpatialReference *srs = mapNode->getMapSRS();
			osg::Vec3d worldPt;
			srs->transformFromWorld(osg::Vec3d(coord.lon, coord.lat, coord.alt),worldPt);

			osgEarth::GeoPoint abs(srs,coord.lon,coord.lat,coord.alt,osgEarth::ALTMODE_ABSOLUTE);
			osg::Matrixd local2world;
			abs.createLocalToWorld(local2world);

			osg::MatrixTransform *mt = new osg::MatrixTransform;
			mt->setMatrix(local2world);
			mt->addChild(at);

			addChild(mt);
		}
	}

};

GVLineCreator::GVLineCreator()
{
	if(!_controlPointShow.valid())
		_controlPointShow = new CreateControlPointShow();
}

GVLineCreator::~GVLineCreator()
{

}

bool GVLineCreator::getGeoPosition(const osgGA::GUIEventAdapter& ea, osgViewer::View* view, GVCoord &coord, osgEarth::MapNode* mapNode) const
{
	if(view == NULL)
		return false;

	return getGeoPosition(view, ea.getX(), ea.getY(), coord, mapNode);
}

bool GVLineCreator::getGeoPosition(osgViewer::View* view, double mouseX, double mouseY, GVCoord &coord, osgEarth::MapNode *mapNode) const
{
	if(view == NULL)
	{
		return false;
	}

	if(mapNode == NULL)
	{
		return false;
	}

	//获取当前点
	osgUtil::LineSegmentIntersector::Intersections intersection;
	view->computeIntersections(mouseX, mouseY, intersection);
	osgUtil::LineSegmentIntersector::Intersections::iterator iter = intersection.begin();
	if(iter != intersection.end())
	{
		osg::Vec3d geoPt;
		const osgEarth::SpatialReference *srs = mapNode->getMapSRS();
		srs->transformFromWorld(iter->getWorldIntersectPoint(), geoPt);
		coord.lon = geoPt.x();
		coord.lat = geoPt.y();
		coord.alt = geoPt.z();

		return true;
	}

	return false;
}

bool GVLineCreator::handle(const osgGA::GUIEventAdapter& ea, osgViewer::View* view, osgEarth::MapNode *mapNode)
{
	switch (ea.getEventType())
	{
	case osgGA::GUIEventAdapter::PUSH:
		{
			GVCoord coord;
			if(getGeoPosition(ea, view, coord, mapNode))
			{
				bool finish = false;
				if(ea.getButtonMask() & osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
					finish = true;
				bool ret = stepCreate(coord, finish, mapNode);
				return ret;
			}
			break;
		}
	case osgGA::GUIEventAdapter::DRAG:
		{
			return true;
		}
	case osgGA::GUIEventAdapter::RELEASE:
		{
			break;
		}
	}
	return false;
}

bool GVLineCreator::stepCreate(const GVCoord &coord, bool finish, osgEarth::MapNode* mapNode)
{
	addGeometryPoint(coord, mapNode);
	if(finish)
		completeCreate();
	return true;
}

bool GVLineCreator::addGeometryPoint(const GVCoord &coord, osgEarth::MapNode* mapNode)
{
	_curCoords.push_back(coord);
	static_cast<CreateControlPointShow*>(_controlPointShow.get())->addControlPoint(coord, mapNode);

	return true;
}

void GVLineCreator::completeCreate()
{
	_curCoords.clear();
	static_cast<CreateControlPointShow*>(_controlPointShow.get())->clear();
}