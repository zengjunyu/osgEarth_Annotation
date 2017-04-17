#include "DraggerPositionChanged.h"

DraggerPositionChanged::DraggerPositionChanged()
{
	osg::StateSet* stateSet = getOrCreateStateSet();
	stateSet->setMode(0x0B71,0);
	stateSet->setRenderBinDetails(99,"RenderBin");
	stateSet->setMode(0x0B50,osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
}

DraggerPositionChanged::~DraggerPositionChanged()
{

}

void DraggerPositionChanged::CreatControlPonitsShow(std::vector<GVCoord> vtxBuffer,osgEarth::MapNode* mapNode,osg::Group* editGroup)
{
	osg::Sphere* shape = new osg::Sphere(osg::Vec3(0,0,0),4.0f);
	osg::ShapeDrawable *_centerSphere = new osg::ShapeDrawable(shape);
	_centerSphere->setDataVariance(osg::Object::DYNAMIC);
	_centerSphere->setColor(osg::Vec4(0.0, 1.0, 0.0, 1.0));

	osg::Geode* geode = new osg::Geode();
	geode->addDrawable(_centerSphere);

	osg::AutoTransform *at = new osg::AutoTransform;
	at->setAutoScaleToScreen(true);
	at->addChild(geode);
	int cpSize = vtxBuffer.size();
	for(int i = 0; i < cpSize; i++)
	{
		const osgEarth::SpatialReference *srs = mapNode->getMapSRS();
		osg::Vec3d worldPt;
		srs->transformFromWorld(osg::Vec3d(vtxBuffer[i].lon, vtxBuffer[i].lat, vtxBuffer[i].alt),worldPt);

		osgEarth::GeoPoint abs(srs,vtxBuffer[i].lon,vtxBuffer[i].lat,vtxBuffer[i].alt,osgEarth::ALTMODE_ABSOLUTE);
		osg::Matrixd local2world;
		abs.createLocalToWorld(local2world);

		osg::MatrixTransform *mt = new osg::MatrixTransform;
		mt->setMatrix(local2world);
		mt->addChild(at);
		editGroup->addChild(mt);
	}
	//加入中心点即平移点
	osg::Sphere* shape1 = new osg::Sphere(osg::Vec3(0,0,0),4.0f);
	osg::ShapeDrawable *_centerSphere1 = new osg::ShapeDrawable(shape1);
	_centerSphere1->setDataVariance(osg::Object::DYNAMIC);
	_centerSphere1->setColor(osg::Vec4(0.0, 0.0, 1.0, 1.0));

	osg::Geode* geode1 = new osg::Geode();
	geode1->addDrawable(_centerSphere1);

	osg::AutoTransform *at1 = new osg::AutoTransform;
	at1->setAutoScaleToScreen(true);
	at1->addChild(geode1);
	GVCoord centerCoord;
	if(getCenterControlPoint(vtxBuffer,centerCoord))
	{
		const osgEarth::SpatialReference *srs = mapNode->getMapSRS();
		osg::Vec3d worldPt;
		srs->transformFromWorld(osg::Vec3d(centerCoord.lon, centerCoord.lat, centerCoord.alt),worldPt);

		osgEarth::GeoPoint abs(srs,centerCoord.lon,centerCoord.lat,centerCoord.alt,osgEarth::ALTMODE_ABSOLUTE);
		osg::Matrixd local2world;
		abs.createLocalToWorld(local2world);

		osg::MatrixTransform *mt1 = new osg::MatrixTransform;
		mt1->setMatrix(local2world);
		mt1->addChild(at1);
		editGroup->addChild(mt1);
	}



	_centerSphere->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	_centerSphere1->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);

}

void DraggerPositionChanged::updateDraggerPosition(int idx,const GVCoord coord,osgEarth::MapNode* mapNode,osg::Group* editGroup)
{
	osg::Sphere* shape = new osg::Sphere(osg::Vec3(0,0,0),4.0f);
	osg::ShapeDrawable *_centerSphere = new osg::ShapeDrawable(shape);
	_centerSphere->setDataVariance(osg::Object::DYNAMIC);
	_centerSphere->setColor(osg::Vec4(0.0, 1.0, 0.0, 1.0));

	osg::Geode* geode = new osg::Geode();
	geode->addDrawable(_centerSphere);

	osg::AutoTransform *at = new osg::AutoTransform;
	at->setAutoScaleToScreen(true);
	at->addChild(geode);

	const osgEarth::SpatialReference *srs = mapNode->getMapSRS();
	osg::Vec3d worldPt;
	srs->transformFromWorld(osg::Vec3d(coord.lon, coord.lat, coord.alt),worldPt);

	osgEarth::GeoPoint abs(srs,coord.lon,coord.lat,coord.alt + 1,osgEarth::ALTMODE_ABSOLUTE);
	osg::Matrixd local2world;
	abs.createLocalToWorld(local2world);

	osg::MatrixTransform *mt = new osg::MatrixTransform;
	mt->setMatrix(local2world);
	mt->addChild(at);
	editGroup->setChild(idx,mt);
	_centerSphere->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
}

bool DraggerPositionChanged::getCenterControlPoint(const std::vector<GVCoord> vtxBuffer,GVCoord &coord)
{
	if(vtxBuffer.size() < 2) return false; 
	double lon = 0.0, lat = 0.0, alt=0.0;
	for(int i = 0; i < vtxBuffer.size(); i++)
	{

		lon += vtxBuffer[i].lon;
		lat += vtxBuffer[i].lat;
		alt += vtxBuffer[i].alt;
	}

	int cpCount = vtxBuffer.size();
	coord.lat = lat / cpCount;
	coord.lon = lon / cpCount;
	coord.alt = alt / cpCount;
	return true;
}