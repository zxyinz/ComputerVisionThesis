#include"cLeastSquareFittingAlgorithm.h"
using namespace std;
using namespace San;
using namespace San::Mathematics;
using namespace San::MachineLearning;
cLeastSquareFittingAlgorithm::cLeastSquareFittingAlgorithm(uint32 Dimension)
{
	Dimension = Dimension == 0 ? 1 : Dimension;

	this->m_WeightArray.resize(Dimension);
	this->m_OutputArray.clear();
}
cLeastSquareFittingAlgorithm::~cLeastSquareFittingAlgorithm()
{
	this->m_WeightArray.clear();
	this->m_OutputArray.clear();
}
bool cLeastSquareFittingAlgorithm::iSetWeightArrayDimension(const uint32 Dimension)
{
	if (Dimension == 0){ return false; }
	if (this->m_WeightArray.size() == Dimension){ return true; }

	this->m_WeightArray.resize(Dimension);
	return true;
}
uint32 cLeastSquareFittingAlgorithm::iGetWeightArrayDimension() const
{
	return this->m_WeightArray.size();
}
void cLeastSquareFittingAlgorithm::iTraining(const vector<vector<sfloat>> &TrainingSet)
{

}
void cLeastSquareFittingAlgorithm::iPredict(const vector<vector<sfloat>> &InputArray)
{
	const int32 Size = InputArray.size();
	const int32 WeightSize = this->m_WeightArray.size();

	this->m_OutputArray.resize(Size);

	for (int32 seek = 0; seek < Size; seek = seek + 1)
	{
		if (WeightSize != (InputArray.size() + 1))
		{
			this->m_OutputArray[seek] = 0.0;
			continue;
		}

		sfloat Predict = this->m_WeightArray[0];
		for (int32 seek_element = 1; seek_element <= WeightSize; seek_element = seek_element + 1)
		{
			Predict = Predict + this->m_WeightArray[seek_element] * InputArray[seek][seek_element - 1];
		}

		this->m_OutputArray[seek] = Predict;
	}
}
vector<sfloat> cLeastSquareFittingAlgorithm::iGetOutputArray() const
{
	return this->m_OutputArray;
}
vector<sfloat> cLeastSquareFittingAlgorithm::iGetWeightArray() const
{
	return this->m_WeightArray;
}