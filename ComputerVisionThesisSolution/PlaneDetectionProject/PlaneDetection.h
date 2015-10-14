#include"Core\SanTypes.h"
#include"Device\cSanTerminalDeviceWin.h"
#include"Graphics\PixelBasedRenderFunc.h"
#include"ML\cKMeansAlgorithm.h"
using namespace std;
using namespace San;
using namespace San::Device;
using namespace San::Graphics;
using namespace San::MachineLearning;
#pragma once
#ifndef __PLANEDETECTION_H__
#define __PLANEDETECTION_H__
//Plane detection core fucntion
void PlaneDetectionFunc(_sstream<uint8> &RenderBuffer, const uint32 RenderBufferWidth, const uint32 RenderBufferHeight, _sstream<uint8> &DebugBuffer, const uint32 DebugBufferWidth, const uint32 DebugBufferHeight);

//Convert 2d RGB image pixel to 3D point
SVECTOR3 ProjectionPixelTo3DSpace(const SVECTOR3 &Coord, const SVECTOR3 &Pixel);

//Generate normalized point cloud from image
list<_spair<SVECTOR3, SVECTOR3>> GenerateDepthDataPoints(_sstream<SVECTOR3> &DataStream, const SVECTOR3 &StreamSize, _sstream<sfloat> &SubBuffer, const SVECTOR3 &SubBufferSize);

//Generate normalized normal vector cloud from image

//Use neighbour to calculate normal vector
list<_spair<SVECTOR3, SVECTOR3>> GenerateNormalDataPointsV1(_sstream<SVECTOR3> &DataStream, const SVECTOR3 &StreamSize, _sstream<sfloat> &SubBuffer, const SVECTOR3 &SubBufferSize);
//Use linear regression to calculate normal vector
list<_spair<SVECTOR3, SVECTOR3>> GenerateNormalDataPointsV2(_sstream<SVECTOR3> &DataStream, const SVECTOR3 &StreamSize, _sstream<sfloat> &SubBuffer, const SVECTOR3 &SubBufferSize);
#endif