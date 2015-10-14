#include"..\Core\SanTypes.h"
#include"..\Core\SanContainer.h"
#include"..\Core\SanMathematics.h"
using namespace std;
namespace San
{
	namespace MachineLearning
	{
#ifndef __CLEASTSQUAREFITTINGALGORITHM_H__
#define __CLEASTSQUAREFITTINGALGORITHM_H__
		class cLeastSquareFittingAlgorithm
		{
		private:
			vector<sfloat> m_WeightArray;
			vector<sfloat> m_OutputArray;
		public:
			cLeastSquareFittingAlgorithm(uint32 Dimension);
			~cLeastSquareFittingAlgorithm();

			bool iSetWeightArrayDimension(const uint32 Dimension);

			uint32 iGetWeightArrayDimension() const;

			void iTraining(const vector<vector<sfloat>> &TrainingSet);
			void iPredict(const vector<vector<sfloat>> &InputArray);

			vector<sfloat> iGetOutputArray() const;
			vector<sfloat> iGetWeightArray() const;
		};
#endif
	}
}