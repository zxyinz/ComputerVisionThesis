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
inline SVECTOR3 ProjectionPixelTo3DSpace(const SVECTOR3 &Coord, const SVECTOR3 &Pixel)
{
	return SVECTOR3(Coord.y, Coord.z, Pixel.x);
}
inline list<_spair<SVECTOR3, SVECTOR3>> GenerateDepthDataPoints(_sstream<SVECTOR3> &DataStream, const SVECTOR3 &StreamSize, _sstream<sfloat> &SubBuffer, const SVECTOR3 &SubBufferSize)
{
	//Generate point data set, projection and nomrallize
	list<_spair<SVECTOR3, SVECTOR3>> PointDataSet;

	SVECTOR3 MinVector(MAX_FLOAT32_VALUE, MAX_FLOAT32_VALUE, MAX_FLOAT32_VALUE);
	SVECTOR3 MaxVector(-MAX_FLOAT32_VALUE, -MAX_FLOAT32_VALUE, -MAX_FLOAT32_VALUE);

	//Delete noise data points and projection points
	const int32 DataSetSize = DataStream.Size;

	int32 ColSize = StreamSize.x;
	int32 RowSize = StreamSize.y;

#pragma omp parallel for
	for (int32 seek_row = 0; seek_row < RowSize; seek_row = seek_row + 1)
	{
		int32 Index = seek_row * StreamSize.x;
		SVECTOR3 Coord(Index, 0, seek_row);

		for (int32 seek_col = 0; seek_col<ColSize; seek_col = seek_col + 1)
		{
			DataStream[Index] = ProjectionPixelTo3DSpace(Coord, DataStream[Index]);

			MinVector.x = MinVector.x > DataStream[Index].x ? DataStream[Index].x : MinVector.x;
			MinVector.y = MinVector.y > DataStream[Index].y ? DataStream[Index].y : MinVector.y;
			MinVector.z = MinVector.z > DataStream[Index].z ? DataStream[Index].z : MinVector.z;

			MaxVector.x = MaxVector.x < DataStream[Index].x ? DataStream[Index].x : MaxVector.x;
			MaxVector.y = MaxVector.y < DataStream[Index].y ? DataStream[Index].y : MaxVector.y;
			MaxVector.z = MaxVector.z < DataStream[Index].z ? DataStream[Index].z : MaxVector.z;

			Index = Index + 1;
			Coord.x = Coord.x + 1;
			Coord.y = Coord.y + 1;
		}
	}

	SVECTOR3 OffsetVectorInv = MaxVector - MinVector;

	OffsetVectorInv.x = 1.0 / OffsetVectorInv.x;
	OffsetVectorInv.y = 1.0 / OffsetVectorInv.y;
	OffsetVectorInv.z = 1.0 / OffsetVectorInv.z;

//#pragma omp parallel for
	for (int32 seek_row = 0; seek_row < RowSize; seek_row = seek_row + 1)
	{
		int32 Index = seek_row * StreamSize.x;
		SVECTOR3 Coord(Index, 0, seek_row);

		for (int32 seek_col = 0; seek_col < ColSize; seek_col = seek_col + 1)
		{
			//Normalize
			DataStream[Index] = (DataStream[Index] - MinVector) * OffsetVectorInv;

			if (DataStream[Index].z > 0.1)
			{ 
				PointDataSet.push_back(_spair<SVECTOR3, SVECTOR3>(Coord, DataStream[Index]));
			}

			Index = Index + 1;
			Coord.x = Coord.x + 1;
			Coord.y = Coord.y + 1;
		}
	}

	return PointDataSet;
}
inline list<_spair<SVECTOR3, SVECTOR3>> GenerateNormalDataPoints(_sstream<SVECTOR3> &DataStream, const SVECTOR3 &StreamSize, _sstream<sfloat> &SubBuffer, const SVECTOR3 &SubBufferSize)
{
	const int32 ColSize = StreamSize.x;
	const int32 RowSize = StreamSize.y;

	const int32 IndexArrayX[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	const int32 IndexArrayY[] = { 1, 1, 1, 0, 0, -1, -1, -1 };

	list<_spair<SVECTOR3, SVECTOR3>> NormalPointDataSet;

	vector<SVECTOR3*> DataMatrix(RowSize);

	for (int32 seek = 0; seek < RowSize; seek = seek + 1)
	{
		DataMatrix[seek] = DataStream.pStream + (seek * ColSize);
	}

	uint32 PointIndex = 0;

	for (int32 seek_row = 1; seek_row < (RowSize - 1); seek_row = seek_row + 1)
	{
		for (int32 seek_col = 1; seek_col < (ColSize - 1); seek_col = seek_col + 1)
		{
			//Calculate local normal
			SVECTOR3* pCurrentPos = DataMatrix[seek_row] + seek_col;
			SVECTOR3 NormalArray[8];

			for (int8 seek_id = 0; seek_id < 8; seek_id = seek_id + 1)
			{
				int32 RowIndex = seek_row + IndexArrayY[seek_id];
				int32 ColIndex = seek_col + IndexArrayX[seek_id];

				NormalArray[seek_id] = *pCurrentPos - DataMatrix[RowIndex][ColIndex];
				NormalArray[seek_id] = ::gloNormalize(NormalArray[seek_id]);
			}

			SVECTOR3 Normal(0.0, 0.0, 0.0);

			for (uint8 seek_id = 0; seek_id < 8; seek_id = seek_id + 1)
			{
				uint8 dest_id = (seek_id + 1) % 8;

				Normal = Normal + ::gloNormalize(::gloCross(NormalArray[seek_id], NormalArray[dest_id]));
			}

			Normal = ::gloNormalize(Normal);

			//Write to render buffer
			DataStream[PointIndex] = Normal;

			NormalPointDataSet.push_back(_spair<SVECTOR3, SVECTOR3>(SVECTOR3(PointIndex, seek_col, seek_row), Normal));
		}
	}

	return NormalPointDataSet;
}
inline void PlaneDetectionFunc(_sstream<uint8> &RenderBuffer, const uint32 RenderBufferWidth, const uint32 RenderBufferHeight, _sstream<uint8> &DebugBuffer, const uint32 DebugBufferWidth, const uint32 DebugBufferHeight)
{
	_sstream<SVECTOR3> DataStream(RenderBuffer.Size);

	_sstream<sfloat> DebugBufferF(DebugBuffer.Size);
	_sstream<sfloat> SubBuffer(DebugBuffer.Size / 4);

	//Copy render buffer data
	uint32 StreamIndex = 0;
	for (uint32 seek = 0; seek < RenderBuffer.Size; seek = seek + 4)
	{ 
		DataStream[StreamIndex].x = RenderBuffer[seek];
		DataStream[StreamIndex].y = RenderBuffer[seek + 1];
		DataStream[StreamIndex].z = RenderBuffer[seek + 2];

		StreamIndex = StreamIndex + 1;
	}

	const SVECTOR3 RenderBufferSize(RenderBufferWidth, RenderBufferHeight, 0.0);
	const SVECTOR3 SubBufferSize(DebugBufferWidth / 2, DebugBufferHeight / 2, 0.0);

	//Generate depth space based data point
	list<_spair<SVECTOR3, SVECTOR3>> PointDataSet = GenerateDepthDataPoints(DataStream, RenderBufferSize, SubBuffer, SubBufferSize);

	//Draw on debug buffer
	SubBuffer.iClear(0);

	for (auto seek : PointDataSet)
	{
		const uint32 ColIndex = seek.first.y / 2.0;
		const uint32 RowIndex = seek.first.z / 2.0;

		const uint32 Index = RowIndex * (DebugBufferWidth << 1) + (ColIndex << 2);

		SubBuffer[Index] = SubBuffer[Index] + seek.second.x * 64.0;
		SubBuffer[Index + 1] = SubBuffer[Index + 1] + seek.second.y * 64.0;
		SubBuffer[Index + 2] = SubBuffer[Index + 2] + seek.second.z * 64.0;

		SubBuffer[Index + 3] = 255.0;
	}

	::DrawImageRGBA(DebugBufferF.pStream, DebugBufferWidth, DebugBufferHeight, SubBuffer.pStream, SubBufferSize.x, SubBufferSize.y, SVECTOR3(0.0, 0.0, 0.0));
	for (uint32 seek = 0; seek < DebugBuffer.Size; seek = seek + 1){ DebugBuffer[seek] = DebugBufferF[seek]; }

	//Generate normal space data point
	list<_spair<SVECTOR3, SVECTOR3>> NormalPointDataSet = GenerateNormalDataPoints(DataStream, RenderBufferSize, SubBuffer, SubBufferSize);

	//Draw on debug buffer
	SubBuffer.iClear(0);

	for (auto seek : NormalPointDataSet)
	{
		const uint32 ColIndex = seek.first.y / 2.0;
		const uint32 RowIndex = seek.first.z / 2.0;

		const uint32 Index = RowIndex * (DebugBufferWidth << 1) + (ColIndex << 2);

		SubBuffer[Index] = SubBuffer[Index] + seek.second.x * 64.0;
		SubBuffer[Index + 1] = SubBuffer[Index + 1] + seek.second.y * 64.0;
		SubBuffer[Index + 2] = SubBuffer[Index + 2] + seek.second.z * 64.0;

		SubBuffer[Index + 3] = 255.0;
	}

	::DrawImageRGBA(DebugBufferF.pStream, DebugBufferWidth, DebugBufferHeight, SubBuffer.pStream, SubBufferSize.x, SubBufferSize.y, SVECTOR3(SubBufferSize.x, 0.0, 0.0));
	for (uint32 seek = 0; seek < DebugBuffer.Size; seek = seek + 1){ DebugBuffer[seek] = DebugBufferF[seek]; }
}
#endif