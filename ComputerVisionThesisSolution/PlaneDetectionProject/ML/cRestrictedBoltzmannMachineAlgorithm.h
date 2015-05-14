#include"..\Core\SanContainer.h"
#include"..\Core\SanMathematics.h"
using namespace std;
using namespace San::Mathematics;
#pragma once;
namespace San
{
	namespace MachineLearning
	{
#ifndef __CRESTRICTEDBOLTZMANNMACHINEALGORITHM_H__
#define __CRESTRICTEDBOLTZMANNMACHINEALGORITHM_H__
		class cRestrictedBoltzmannMachineAlgorithm
		{
		private:
			vector<sfloat> m_HiddenLayerBias;
			vector<sfloat> m_VisibleLayerBias;
			vector<vector<sfloat>> m_WeightArray; // Size = HiddenLayerSize * VisibleLayerSize, 1D Array
			sfloat m_RandomBoundary[2];
		protected:
			bool _InitializeRBM(const vector<vector<sfloat>> &TrainingSet);

			void _SampleHV(vector<sfloat> &Sample, vector<sfloat> &HiddenNodeProbability, vector<sfloat> &HiddenNodeSample);
			void _SampleVH(vector<sfloat> &Sample, vector<sfloat> &VisibleNodeProbability, vector<sfloat> &VisibleNodeSample);

			sfloat _CalcProbabilityUp(const uint32 HiddenIndex, vector<sfloat> &Sample);
			sfloat _CalcProbabilityDown(const uint32 VisibleIndex, vector<sfloat> &Sample);

			sfloat _CalcBinomialValue(const sfloat N, const sfloat Probability) const;
			sfloat _CalcSigmoidValue(const sfloat Net) const;
			sfloat _CalcUniformDistribution(const sfloat MinBoundary, const sfloat MaxBoundary) const;
		public:
			cRestrictedBoltzmannMachineAlgorithm(const uint32 VisibleLayerSize = 1, const uint32 HiddenLayerSize = 1, const sfloat MinBoundary = -0.05, const sfloat MaxBoundary = 0.05);
			~cRestrictedBoltzmannMachineAlgorithm();

			bool iUpdateDBMState(const uint32 VisibleLayerSize, const uint32 HiddenLayerSize, const sfloat MinBoundary, const sfloat MaxBoundary);

			void iTrainingRBM(vector<vector<sfloat>> &TrainingSet, vector<vector<sfloat>> &OutputSet, const uint32 TrainingTimes, const uint32 KTimes, const sfloat LearningRate);

			void iReconstruct(vector<sfloat> &VisibleSample, vector<sfloat> &ReconstructResult);

			vector<vector<sfloat>> iGetWeightArray() const;

			void iPrintRBM();
		};
#endif
	}
}