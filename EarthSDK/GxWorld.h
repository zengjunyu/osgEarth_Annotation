#ifndef GXWORLD_H
#define GXWORLD_H

#include "I_GxWorld.h"
#include "GVAnnotations.h"
#include <process.h>

// OSG
#include <OpenThreads/Thread>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/Win32/GraphicsWindowWin32>
#include <osg/Group>
#include <osgGA/StateSetManipulator>
#include <osg/ref_ptr>

// EARTH
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarthUtil/EarthManipulator>

class GxWorld : public I_GxWorld
{
public:
	GxWorld(HWND hwnd);
	~GxWorld();

	void Display();
	void InitAnno();
	void sendKey(int key);

private:
	struct Hidden;
	friend struct Hidden;

	Hidden* h;
	GVAnnotations* anno; 
};
#endif //GXWORLD_H