#include"cArtificialNeuralNetworkAlgorithm.h"
#include"cRestrictedBoltzmannMachineAlgorithm.h"
using namespace std;
#pragma once
namespace San
{
	namespace MachineLearning
	{
#ifndef __CDEEPNEURALNETWORKALGORITHM_H__
#define __CDEEPNEURALNETWORKALGORITHM_H__
		class cDeepNeuralNetworkAlgorithm : public cArtificialNeuralNetworkAlgorithm
		{
		private:
		public:
			cDeepNeuralNetworkAlgorithm(const sfloat LearningRate = 0.1, const sfloat MomentumValue = 0.0, const sfloat RandomMinBoundary = -0.05, const sfloat RandomMaxBoundary = 0.05);
			~cDeepNeuralNetworkAlgorithm();
			void iPreProcessing(const ANNTRAININGSET &TrainingSet, const sfloat RBMLearningRate, const uint32 TrainingTimes = 1);
		};
#endif
	}
}