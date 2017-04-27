#include "LineStyle.h"

osg::Group* LineStyle::drawLine(osgEarth::MapNode* mapNode,GVGeometry* line)
{
	osg::Group* annoGroup = new osg::Group();

	Style geomStyle;
	geomStyle.getOrCreate<LineSymbol>()->stroke()->color() = Color::Red;
	geomStyle.getOrCreate<LineSymbol>()->stroke()->width() = 4.0f;
	/*geomStyle.getOrCreate<LineSymbol>()->stroke()->widthUnits() = osgEarth::Symbology::Units::PIXELS;
	geomStyle.getOrCreate<LineSymbol>()->tessellation() = 200.0f;*/
	geomStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
	//geomStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_MAP;
	geomStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_DRAPE;
	//geomStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::Red, 1.0);
	//geomStyle.getOrCreate<ExtrusionSymbol>()->height() = 250.0;

	const SpatialReference* geoSRS = mapNode->getMapSRS()->getGeographicSRS();
	std::vector<GVCoord> vtxBuffer;
	std::vector<int> vtxBuffer2;
	if(line->toVertice(vtxBuffer,&vtxBuffer2))
	{
		osg::ref_ptr<osgEarth::Features::MultiGeometry> geoms = new osgEarth::Features::MultiGeometry;
		unsigned int count = 0;
		for(int i =0; i < vtxBuffer2.size(); i++)
		{
			osg::ref_ptr<osgEarth::Symbology::LineString> geom = new osgEarth::Symbology::LineString;
			for(int j =0; j < vtxBuffer2[i]; j++)
			{
				geom->push_back(osg::Vec3d(vtxBuffer[count].lon,vtxBuffer[count].lat,vtxBuffer[count].alt));
				count++;
			}
			geoms->add(geom);

			osg::ref_ptr<osgEarth::Features::Feature> feature = new osgEarth::Features::Feature(geoms,mapNode->getMapSRS(),geomStyle);
			osg::ref_ptr<osgEarth::Annotation::FeatureNode> gnode = new osgEarth::Annotation::FeatureNode(mapNode,feature);
			gnode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF|osg::StateAttribute::PROTECTED);
			annoGroup->addChild( gnode );
		}
	}

	return annoGroup;
}