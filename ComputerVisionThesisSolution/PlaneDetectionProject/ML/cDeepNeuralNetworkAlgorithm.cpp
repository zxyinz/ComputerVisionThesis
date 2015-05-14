#include"cDeepNeuralNetworkAlgorithm.h"
using namespace std;
using namespace San;
using namespace San::MachineLearning;
cDeepNeuralNetworkAlgorithm::cDeepNeuralNetworkAlgorithm(const sfloat LearningRate, const sfloat MomentumValue, const sfloat RandomMinBoundary, const sfloat RandomMaxBoundary)
	:cArtificialNeuralNetworkAlgorithm(LearningRate,MomentumValue,RandomMinBoundary,RandomMaxBoundary)
{
}
cDeepNeuralNetworkAlgorithm::~cDeepNeuralNetworkAlgorithm()
{

}
void cDeepNeuralNetworkAlgorithm::iPreProcessing(const ANNTRAININGSET &TrainingSet, const sfloat RBMLearningRate, const uint32 TrainingTimes)
{
	uint32 TrainingSetSize = TrainingSet.size();
	uint32 RBMVisibleLayerSize = this->m_LayerArray[0].NodeArray.size() + 1;
	uint32 RBMHiddenLayerSize = this->m_LayerArray[1].NodeArray.size();

	cRestrictedBoltzmannMachineAlgorithm RBM(RBMVisibleLayerSize, RBMHiddenLayerSize, this->m_RandomBoundary[0], this->m_RandomBoundary[1]);

	vector<vector<sfloat>> RBMInputTrainingSet(TrainingSetSize);
	vector<vector<sfloat>> RBMOutputTrainingSet(TrainingSetSize);

	for (uint32 seek = 0; seek < TrainingSetSize; seek = seek + 1)
	{
		RBMInputTrainingSet[seek].resize(RBMVisibleLayerSize);

		RBMInputTrainingSet[seek][0] = 1.0;
		for (uint32 seek_attrib = 1; seek_attrib < RBMVisibleLayerSize; seek_attrib = seek_attrib + 1)
		{
			RBMInputTrainingSet[seek][seek_attrib] = TrainingSet[seek].first[seek_attrib - 1];
		}

		RBMOutputTrainingSet[seek].resize(this->m_LayerArray[1].NodeArray.size());
	}

	RBM.iTrainingRBM(RBMInputTrainingSet, RBMOutputTrainingSet, TrainingTimes, 1, RBMLearningRate);

	/*Update Weight*/
	vector<vector<sfloat>> WeightArray = RBM.iGetWeightArray();
	for (uint32 seek_hidden = 0; seek_hidden < RBMHiddenLayerSize; seek_hidden = seek_hidden + 1)
	{
		for (uint32 seek_visible = 0; seek_visible < RBMVisibleLayerSize; seek_visible = seek_visible + 1)
		{
			this->m_LayerArray[1][seek_hidden][seek_visible].w = WeightArray[seek_hidden][seek_visible];
		}
	}

	uint32 HiddenLayerSize = this->m_LayerArray.size() - 1;
	for (uint32 seek = 2; seek < HiddenLayerSize; seek = seek + 1)
	{
		RBMVisibleLayerSize = this->m_LayerArray[seek - 1].NodeArray.size() + 1;
		RBMHiddenLayerSize = this->m_LayerArray[seek].NodeArray.size();

		for (uint32 seek_instance = 0; seek_instance < TrainingSetSize; seek_instance = seek_instance + 1)
		{
			RBMInputTrainingSet[seek_instance].resize(RBMVisibleLayerSize);

			/*W0*/
			RBMInputTrainingSet[seek_instance][0] = 1.0;
			/*Other*/
			for (uint32 seek_attrib = 1; seek_attrib <= RBMVisibleLayerSize; seek_attrib = seek_attrib + 1)
			{
				RBMInputTrainingSet[seek_instance][seek_attrib] = RBMOutputTrainingSet[seek_instance][seek_attrib - 1];
			}

			RBMOutputTrainingSet[seek_instance].resize(RBMHiddenLayerSize);
		}

		RBM.iUpdateDBMState(RBMVisibleLayerSize, RBMHiddenLayerSize, this->m_RandomBoundary[0], this->m_RandomBoundary[1]);
		RBM.iTrainingRBM(RBMInputTrainingSet, RBMOutputTrainingSet, TrainingTimes, 1, RBMLearningRate);

		/*Update Weight*/
		vector<vector<sfloat>> WeightArray = RBM.iGetWeightArray();
		for (uint32 seek_hidden = 0; seek_hidden < RBMHiddenLayerSize; seek_hidden = seek_hidden + 1)
		{
			for (uint32 seek_visible = 0; seek_visible < RBMVisibleLayerSize; seek_visible = seek_visible + 1)
			{
				this->m_LayerArray[seek][seek_hidden][seek_visible].w = WeightArray[seek_hidden][seek_visible];
			}
		}
	}
}