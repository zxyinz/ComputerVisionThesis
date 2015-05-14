#include"cRestrictedBoltzmannMachineAlgorithm.h"
using namespace std;
using namespace San;
using namespace San::MachineLearning;
cRestrictedBoltzmannMachineAlgorithm::cRestrictedBoltzmannMachineAlgorithm(const uint32 VisibleLayerSize, const uint32 HiddenLayerSize, const sfloat MinBoundary, const sfloat MaxBoundary)
{
	this->iUpdateDBMState(VisibleLayerSize, HiddenLayerSize, MinBoundary, MaxBoundary);
}
cRestrictedBoltzmannMachineAlgorithm::~cRestrictedBoltzmannMachineAlgorithm()
{
}
bool cRestrictedBoltzmannMachineAlgorithm::_InitializeRBM(const vector<vector<sfloat>> &TrainingSet)
{
	if (TrainingSet.empty())
	{
		return false;
	}

	uint32 TrainingSetSize = TrainingSet.size();
	uint32 VisibleLayerSize = this->m_VisibleLayerBias.size();
	uint32 HiddenLayerSize = this->m_HiddenLayerBias.size();

	///*Calculate attribute probability for whole set*/
	//vector<sfloat> ProbabilityArray(InputArraySize);
	//for (uint32 seek = 0; seek < InputArraySize; seek = seek + 1)
	//{
	//	ProbabilityArray[seek] = 0.0;
	//}
	//for (uint32 seek = 0; seek < TrainingSetSize; seek = seek + 1)
	//{
	//	for (uint32 seek_input = 0; seek_input < InputArraySize; seek_input = seek_input + 1)
	//	{
	//		ProbabilityArray[seek_input] = ProbabilityArray[seek_input] + this->_CalcSigmoidValue(TrainingSet[seek][seek_input]);
	//	}
	//}
	//for (uint32 seek = 0; seek < InputArraySize; seek = seek + 1)
	//{
	//	ProbabilityArray[seek] = ProbabilityArray[seek] / (sfloat) TrainingSetSize;
	//}

	///*For Visible Bias, Initialize with log[Pi / (1 - Pi)]*/
	//uint32 VisibleLayerSize = InputArraySize;
	//for (uint32 seek = 0; seek < VisibleLayerSize; seek = seek + 1)
	//{
	//	sfloat Base = (1.0 - ProbabilityArray[seek]);
	//	if (Base == 0)
	//	{
	//		Base = 0.0001;
	//	}
	//	pVisibleLayer->NodeArray[seek].VisibleBias = ::log(ProbabilityArray[seek] / Base);
	//}


	/*For Visible Bias, Initialize with 0*/
	for (uint32 seek = 0; seek < VisibleLayerSize; seek = seek + 1)
	{
		this->m_VisibleLayerBias[seek] = 0.0;
	}

	/*For Hidden Bias, Initialize with 0*/
	for (uint32 seek = 0; seek < HiddenLayerSize; seek = seek + 1)
	{
		this->m_HiddenLayerBias[seek] = 0.0;
	}

	sfloat Base = 1.0 / (sfloat)this->m_VisibleLayerBias.size();

	/*Random Initialize Weight Array*/
	for (uint32 seek = 0; seek < HiddenLayerSize; seek = seek + 1)
	{
		for (uint32 seek_vis = 0; seek_vis < VisibleLayerSize; seek_vis = seek_vis + 1)
		{
			this->m_WeightArray[seek][seek_vis] = this->_CalcUniformDistribution(-Base, Base);
		}
	}
	return true;
}
void cRestrictedBoltzmannMachineAlgorithm::iPrintRBM()
{
	::cout << "W\n";
	for (int i = 0; i < this->m_HiddenLayerBias.size(); i++)
	{
		for (int j = 0; j < this->m_VisibleLayerBias.size(); j++)
		{
			::cout << this->m_WeightArray[i][j] << " ";
		}
		::cout << "\n";
	}

	::cout << "H\n";
	for (int i = 0; i < this->m_HiddenLayerBias.size(); i++)
	{
		::cout << this->m_HiddenLayerBias[i] << " ";
	}
	::cout << "\n";

	::cout << "V\n";
	for (int j = 0; j < this->m_VisibleLayerBias.size(); j++)
	{
		::cout << this->m_VisibleLayerBias[j] << " ";
	}
	::cout << "\n";
}
void cRestrictedBoltzmannMachineAlgorithm::iTrainingRBM(vector<vector<sfloat>> &TrainingSet, vector<vector<sfloat>> &OutputSet, const uint32 TrainingTimes, const uint32 KTimes, const sfloat LearningRate)
{
	uint32 TrainingSetSize = TrainingSet.size();
	uint32 VisibleLayerSize = this->m_VisibleLayerBias.size();
	uint32 HiddenLayerSize = this->m_HiddenLayerBias.size();

	this->_InitializeRBM(TrainingSet);

	vector<sfloat> HiddenProbability(HiddenLayerSize);
	vector<sfloat> HiddenSample(HiddenLayerSize);

	vector<sfloat> NewVisibleProbability(VisibleLayerSize);
	vector<sfloat> NewVisibleSample(VisibleLayerSize);

	vector<sfloat> NewHiddenProbability(HiddenLayerSize);
	vector<sfloat> NewHiddenSample(HiddenLayerSize);

	for (uint32 seek_itaration = 0; seek_itaration < TrainingTimes; seek_itaration = seek_itaration + 1)
	{
		for (uint32 seek_instance = 0; seek_instance < TrainingSetSize; seek_instance = seek_instance + 1)
		{
			/*Contrastive Divergence*/

			this->_SampleHV(TrainingSet[seek_instance], HiddenProbability, HiddenSample);

			/*Gibbs Sample*/
			for (uint32 seek = 0; seek < KTimes; seek = seek + 1)
			{
				if (seek == 0)
				{
					this->_SampleVH(HiddenSample, NewVisibleProbability, NewVisibleSample);
					this->_SampleHV(NewVisibleSample, NewHiddenProbability, NewHiddenSample);
				}
				else
				{
					this->_SampleVH(NewHiddenSample, NewVisibleProbability, NewVisibleSample);
					this->_SampleHV(NewVisibleSample, NewHiddenProbability, NewHiddenSample);
				}
			}

			/*Update Weight*/
			for (uint32 seek = 0; seek < HiddenLayerSize; seek = seek + 1)
			{
				for (uint32 seek_visible = 0; seek_visible < VisibleLayerSize; seek_visible = seek_visible + 1)
				{
					sfloat DeltaWeight = (HiddenProbability[seek] * TrainingSet[seek_instance][seek_visible] - NewHiddenProbability[seek] * NewVisibleSample[seek_visible]) / (sfloat) TrainingSetSize;
					this->m_WeightArray[seek][seek_visible] = this->m_WeightArray[seek][seek_visible] + DeltaWeight*LearningRate;
				}
				this->m_HiddenLayerBias[seek] = this->m_HiddenLayerBias[seek] + LearningRate*(HiddenSample[seek] - NewHiddenProbability[seek]) / (sfloat) TrainingSetSize;
			}

			for (uint32 seek = 0; seek < VisibleLayerSize; seek = seek + 1)
			{
				this->m_VisibleLayerBias[seek] = this->m_VisibleLayerBias[seek] + LearningRate*(TrainingSet[seek_instance][seek] - NewVisibleSample[seek]) / (sfloat) TrainingSetSize;
			}
		}
	}

	//this->iPrintRBM();

	/*Encode Output Set For Next RBM Training*/
	for (uint32 seek_instance = 0; seek_instance < TrainingSetSize; seek_instance = seek_instance + 1)
	{
		this->_SampleHV(TrainingSet[seek_instance], OutputSet[seek_instance], HiddenSample);
	}
}
void cRestrictedBoltzmannMachineAlgorithm::_SampleHV(vector<sfloat> &Sample, vector<sfloat> &HiddenNodeProbability, vector<sfloat> &HiddenNodeSample)
{
	uint32 HiddenLayerSize = this->m_HiddenLayerBias.size();

	for (uint32 seek = 0; seek < HiddenLayerSize; seek = seek + 1)
	{
		HiddenNodeProbability[seek] = this->_CalcProbabilityUp(seek, Sample);
		HiddenNodeSample[seek] = this->_CalcBinomialValue(1, HiddenNodeProbability[seek]);
	}
}
void cRestrictedBoltzmannMachineAlgorithm::_SampleVH(vector<sfloat> &Sample, vector<sfloat> &VisibleNodeProbability, vector<sfloat> &VisibleNodeSample)
{
	uint32 VisibleLayerSize = this->m_VisibleLayerBias.size();

	for (uint32 seek = 0; seek < VisibleLayerSize; seek = seek + 1)
	{
		VisibleNodeProbability[seek] = this->_CalcProbabilityDown(seek, Sample);
		VisibleNodeSample[seek] = this->_CalcBinomialValue(1, VisibleNodeProbability[seek]);
	}
}
sfloat cRestrictedBoltzmannMachineAlgorithm::_CalcProbabilityUp(const uint32 HiddenIndex, vector<sfloat> &Sample)
{
	/*P(Hi | v£© = sig(sum(Vj * Wij) + Bi)*/
	sfloat Probability = 0.0;
	uint32 VisibleLayerSize = this->m_VisibleLayerBias.size();

	for (uint32 seek = 0; seek < VisibleLayerSize; seek = seek + 1)
	{
		Probability = Probability + Sample[seek] * this->m_WeightArray[HiddenIndex][seek];
	}
	Probability = Probability + this->m_HiddenLayerBias[HiddenIndex];
	return this->_CalcSigmoidValue(Probability);
}
sfloat cRestrictedBoltzmannMachineAlgorithm::_CalcProbabilityDown(const uint32 VisibleIndex, vector<sfloat> &Sample)
{
	/*P(Vi | h) = sig(sum(Hj*Wij) + Ai)*/
	sfloat Probability = 0.0;
	uint32 HiddenLayerSize = this->m_HiddenLayerBias.size();

	for (uint32 seek = 0; seek < HiddenLayerSize; seek = seek + 1)
	{
		Probability = Probability + Sample[seek] * this->m_WeightArray[seek][VisibleIndex];
	}
	Probability = Probability + this->m_VisibleLayerBias[VisibleIndex];
	return this->_CalcSigmoidValue(Probability);
}
sfloat cRestrictedBoltzmannMachineAlgorithm::_CalcBinomialValue(const sfloat N, const sfloat Probability) const
{
	if ((Probability<0.0) || (Probability>1.0))
	{
		return 0;
	}
	sfloat Res = 0;
	sfloat RandVal = 0.0;
	for (uint32 seek = 0; seek < N; seek = seek + 1)
	{
		RandVal = (sfloat)::rand() / ((sfloat)RAND_MAX + 1.0);
		if (RandVal < Probability)
		{
			Res = Res + 1.0;
		}
	}
	return Res;
}
sfloat cRestrictedBoltzmannMachineAlgorithm::_CalcSigmoidValue(const sfloat Net) const
{
	return 1.0 / (1.0 + ::exp(-Net));
}
sfloat cRestrictedBoltzmannMachineAlgorithm::_CalcUniformDistribution(const sfloat MinBoundary, const sfloat MaxBoundary) const
{
	return rand() / (RAND_MAX + 1.0) * (MaxBoundary - MinBoundary) + MinBoundary;
	const sfloat Base = 1000.0;
	sfloat RandValue = 0.0;
	while (true)
	{
		RandValue = ::rand() % (uint32)Base;
		RandValue = RandValue / Base;
		RandValue = this->m_RandomBoundary[0] + (this->m_RandomBoundary[1] - this->m_RandomBoundary[0])*RandValue;
		if (!::gloIsFloatEqual(RandValue, 0.0, 0.0001))
		{
			break;
		}
	}
	return RandValue;
}
bool cRestrictedBoltzmannMachineAlgorithm::iUpdateDBMState(const uint32 VisibleLayerSize, const uint32 HiddenLayerSize, const sfloat MinBoundary, const sfloat MaxBoundary)
{
	if ((VisibleLayerSize==0)||(HiddenLayerSize==0))
	{
		return false;
	}
	if (MinBoundary >= MaxBoundary)
	{
		return false;
	}
	this->m_VisibleLayerBias.resize(VisibleLayerSize);
	this->m_HiddenLayerBias.resize(HiddenLayerSize);
	this->m_WeightArray.resize(HiddenLayerSize);
	for (uint32 seek = 0; seek < HiddenLayerSize; seek = seek + 1)
	{
		this->m_WeightArray[seek].resize(VisibleLayerSize);
	}
	this->m_RandomBoundary[0] = MinBoundary;
	this->m_RandomBoundary[1] = MaxBoundary;
	return true;
}
vector<vector<sfloat>> cRestrictedBoltzmannMachineAlgorithm::iGetWeightArray() const
{
	return this->m_WeightArray;
}
void cRestrictedBoltzmannMachineAlgorithm::iReconstruct(vector<sfloat> &VisibleSample, vector<sfloat> &ReconstructResult)
{
	uint32 VisibleLayerSize = this->m_VisibleLayerBias.size();
	uint32 HiddenLayerSize = this->m_HiddenLayerBias.size();

	if (ReconstructResult.size() != VisibleLayerSize)
	{
		ReconstructResult.resize(VisibleLayerSize);
	}

	vector<sfloat> HiddenProbability(HiddenLayerSize);
	/*Calculate Hidden Layer Probability*/
	for (uint32 seek = 0; seek < HiddenLayerSize; seek = seek + 1)
	{
		HiddenProbability[seek] = this->_CalcProbabilityUp(seek, VisibleSample);
	}

	for (uint32 seek = 0; seek < VisibleLayerSize; seek = seek + 1)
	{
		sfloat SigVal = 0.0;
		for (uint32 seek_hidden = 0; seek_hidden < HiddenLayerSize; seek_hidden = seek_hidden + 1)
		{
			SigVal = SigVal + this->m_WeightArray[seek_hidden][seek] * HiddenProbability[seek_hidden];
		}
		SigVal = SigVal + this->m_VisibleLayerBias[seek];
		ReconstructResult[seek] = this->_CalcSigmoidValue(SigVal);
	}
}