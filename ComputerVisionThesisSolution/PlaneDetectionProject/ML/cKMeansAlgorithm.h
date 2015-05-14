#include"..\Core\SanContainer.h"
#include"..\Core\SanMathematics.h"
using namespace std;
namespace San
{
	namespace MachineLearning
	{
#ifndef __CKMEANSALGORITHM_H__
#define __CKMEANSALGORITHM_H__

		template<class DataType>
		using CLUSTERSET = vector<vector<DataType>>;

		template<class DataType>
		class cKMeansRelyOnFuncSet
		{
		public:
			cKMeansRelyOnFuncSet();
			~cKMeansRelyOnFuncSet();

			sfloat iCalcInstanceSimilarity(const DataType &D1, const DataType D2) const = 0;
			DataType iCalcClusterCenter(const vector<uint32> &ClusterArray) const = 0;
			DataType iGenerateRandomInstance() const = 0;
		};

		template<class DataType, class SimilarFunc, class CenterFunc, class RandomFunc>
		class cKMeansRelyOnFuncSetT : public cKMeansRelyOnFuncSet<DataType>
		{
		public:
			cKMeansRelyOnFuncSetT(){};
			~cKMeansRelyOnFuncSetT(){};

			sfloat iCalcInstanceSimilarity(const DataType &D1, const DataType D2) const { return SimilarFunc(D1, D2); };
			DataType iCalcClusterCenter(const vector<uint32> &ClusterArray) const { return CenterFunc(ClusterArray); };
			DataType iGenerateRandomInstance() const { return RandomFunc(); };
		};

		template<class DataType, class FuncSet>
		class cKMeansAlgorithm
		{
		private:
			CLUSTERSET<uint32> m_ClusterIndex;
			vector<DataType> m_ClusterCenterArray;
		/*protected:
			sfloat _CalcInstanceSimilarity(const DataType &Data1, const DataType Data2) const = 0;
			DataType _CalcClusterCenter(const vector<uint32> &ClusterArray) const = 0;
			DataType _GenerateRandomInstance() const = 0;*/
		public:
			cKMeansAlgorithm()
			{
				this->m_ClusterIndex.clear();
				this->m_ClusterCenterArray.clear();
			};
			~cKMeansAlgorithm()
			{
				this->m_ClusterIndex.clear();
				this->m_ClusterCenterArray.clear();
			};

			bool iClusterIndex(const vector<DataType> &DataSet, const uint32 ClusterNum, const sfloat ConvergeBoundary = 0.0)
			{
				if (DataSet.empty()){ this->m_ClusterIndex.clear(); return false; }
				if (ClusterNum == 0){ this->m_ClusterIndex.clear(); return false; }

				this->m_ClusterIndex.resize(ClusterNum);
				this->m_ClusterCenterArray.resize(ClusterNum);

				//Random initialize all the center data point
				/*for (uint32 seek = 0; seek < ClusterNum; seek = seek + 1)
				{
					m_ClusterCenterArray[seek] = FuncSet.iGenerateRandomInstance();
				}*/

				//Select center data point from data set
				vector<uint32> CenterIndex;
				for (uint32 seek = 0; seek < ClusterNum; seek = seek + 1)
				{
					while (true)
					{
						uint32 Index = ::rand() % DataSet.size();

						for (uint32 seek_center = 0; seek < CenterIndex.size(); seek_center = seek_center + 1)
						{
							if (CenterIndex[seek_center] == Index){ Index = DataSet.size(); }
						}

						if (Index < DataSet.size()){ break; }
					}

					CenterIndex.push_back(Index);
					this->m_ClusterCenterArray[seek] = DataSet[Index];
				}

				sfloat MaxChange = MAX_FLOAT32_VALUE;
				uint32 DataSetSize = DataSet.size();

				while (MaxChange > ConvergeBoundary)
				{
					//Clear the cluster set
					for (uint32 seek = 0; seek < ClusterNum; seek = seek + 1)
					{
						this->m_ClusterIndex[seek].clear();
					}

					for (uint32 seek = 0; seek < DataSetSize; seek = seek + 1)
					{
						sfloat MinDis = MAX_FLOAT32_VALUE;
						int32 MinDisIndex = -1;

						//Assign each data point to the nearest cluster
						for (uint32 seek_cluster = 0; seek_cluster < ClusterNum; seek_cluster = seek_cluster + 1)
						{
							sfloat Distance = FuncSet.iCalcInstanceSimilarity(DataSet[seek], m_ClusterCenterArray[seek_cluster]);

							if (Distance < MinDis)
							{
								MinDis = Distance;
								MinDisIndex = seek_cluster;
							}
						}

						if (MinDisIndex == -1){ return CLUSTERSET<uint32>(); }

						this->m_ClusterIndex[MinDisIndex].push_back(seek);
					}

					//Recalculate each cluster center
					DataType NewClusterCenter;
					MaxChange = -MAX_FLOAT32_VALUE;

					for (uint32 seek = 0; seek < ClusterNum; seek = seek + 1)
					{
						NewClusterCenter = FuncSet.iCalcClusterCenter(ClusterSet[seek]);

						sfloat CurrentChange = FuncSet.iCalcInstanceSimilarity(m_ClusterCenterArray[seek], NewClusterCenter);

						m_ClusterCenterArray[seek] = NewClusterCenter;

						MaxChange = MaxChange>CurrentChange ? MaxChange : CurrentChange;
					}
				}

				return true;
			};
			CLUSTERSET<uint32> iGetClusterIndex() const
			{
				return this->m_ClusterIndex;
			};
			CLUSTERSET<DataType> iGetClusterSet(const vector<DataSet> &DataSet) const
			{
				if (this->m_ClusterIndex.empty()){ return CLUSTERSET<DataType>(); }

				CLUSTERSET<DataType> ClusterArray(this->m_ClusterIndex.size());

				for (uint32 seek = 0; seek < this->m_ClusterIndex.size(); seek = seek + 1)
				{
					uint32 SetSize = this->m_ClusterIndex[seek].size();

					ClusterArray[seek].resize(SetSize);

					for (uint32 seek_data = 0; seek_data < SetSize; seek = seek + 1)
					{
						ClusterArray[seek][seek_data] = DataSet[this->m_ClusterIndex[seek][seek_data]];
					}
				}

				return ClusterArray;
			};

			sfloat iCalcSumOfSquaredError(const vector<DataType> &DataSet) const
			{
				sfloat SSE = 0.0;

				const uint32 ClusterNumber = this->m_ClusterIndex.size();
				for (uint32 seek_cluster = 0; seek_cluster < ClusterNumber; seek_cluster = seek_cluster + 1)
				{
					const uint32 ClusterSize = this->m_ClusterIndex[seek_cluster].size();

					for (uint32 seek = 0; seek < ClusterSize; seek = seek + 1)
					{
						uint32 Index = this->m_ClusterIndex[seek_cluster][seek];

						sfloat CurrentDis = FuncSet.iCalcInstanceSimilarity(DataSet[Index], this->m_ClusterCenterArray[seek_cluster]);

						SSE = SSE + CurrentDis*CurrentDis;
					}
				}

				return SSE;
			};
		};

		template<class DataType, class SimilarFunc, class CenterFunc, class RandomFunc>
		CLUSTERSET<uint32> KMeansClusterAlgorithm(const vector<DataType> &DataSet, const uint32 ClusterNum, const sfloat ConvergeBoundary = 0.0)
		{
			if (DataSet.empty()){ return CLUSTERSET<uint32>(); }
			if (ClusterNum == 0){ return CLUSTERSET<uint32>(); }

			vector<DataType> ClusterCenter;
			CLUSTERSET<uint32> ClusterSet(ClusterNum);

			//Random Initialize all the center data point
			for (uint32 seek = 0; seek < ClusterNum; seek = seek + 1)
			{
				ClusterCenter.push_back(RandomFunc());
			}

			sfloat MaxChange = MAX_FLOAT32_VALUE;
			uint32 DataSetSize = DataSet.size();

			while (MaxChange > ConvergeBoundary)
			{
				//Clear the cluster set
				for (uint32 seek = 0; seek < ClusterNum; seek = seek + 1)
				{
					ClusterSet[seek].clear();
				}

				for (uint32 seek = 0; seek < DataSetSize; seek = seek + 1)
				{
					sfloat MinDis = MAX_FLOAT32_VALUE;
					int32 MinDisIndex = -1;

					//Assign each data point to the nearest cluster
					for (uint32 seek_cluster = 0; seek_cluster < ClusterNum; seek_cluster = seek_cluster + 1)
					{
						sfloat Distance = SimilarFunc(DataSet[seek], ClusterCenter[seek_cluster]);

						if (Distance < MinDis)
						{
							MinDis = Distance;
							MinDisIndex = seek_cluster;
						}
					}

					if (MinDisIndex == -1){ return CLUSTERSET<uint32>(); }

					ClusterSet[MinDisIndex].push_back(seek);
				}

				//Recalculate each cluster center
				DataType NewClusterCenter;
				MaxChange = -MAX_FLOAT32_VALUE;

				for (uint32 seek = 0; seek < ClusterNum; seek = seek + 1)
				{
					NewClusterCenter = CenterFunc(ClusterSet[seek]);

					sfloat CurrentChange = SimilarFunc(ClusterCenter[seek], NewClusterCenter);

					ClusterCenter[seek] = NewClusterCenter;

					MaxChange = MaxChange>CurrentChange ? MaxChange : CurrentChange;
				}
			}

			return ClusterSet;
		};
#endif
	}
}