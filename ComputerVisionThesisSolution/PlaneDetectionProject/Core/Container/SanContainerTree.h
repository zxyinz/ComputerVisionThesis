//Project: San Lab Game Engine
//Version: 2.1.1
//Debug State: Add tree container [_stree, _swtree, dfs_iterator, bfs_iterator]
#include"SanContainerDef.h"
#pragma once
using namespace std;
namespace San
{
#ifndef __STDCONTAINERTREE_H__
#define __STDCONTAINERTREE_H__

#pragma region San octree structure defination
	template<class ST>
	struct _stSANOCTREE
	{
	private:
		_stSANOCTREE<ST>*	p_trunk;
		_stSANOCTREE<ST>*	p_offshootarray[8];
		ST		data;
		uint32	weight;
		uint32	identify;
	public:

	};
	template<class ST>
	using _soctree = _stSANOCTREE<ST>;
#pragma endregion

#pragma region San general tree structure defination
	template<class ST>
	struct _stSANTREEEX
	{
	private:
		uint32						ID;
		ST							Data;
		uint32						Depth;
		_stSANTREEEX<ST>*			pTrunk;
		vector<_stSANTREEEX<ST>*>	pOffshootArray;
		//uint32	weight;
		//uint32	identify;
		//uint32	listsize;
	public:

	};
	template<class ST>
	using _stree = _stSANTREEEX<ST>;
#pragma endregion

#pragma region San weighted tree structure defination
	template<class DType>
	struct TREENODE
	{
	public:
		TREENODE(){};
		~TREENODE(){};

		virtual SString iPrintNode(const TREENODE &Node) const { return SString(); };
	};

	/*Tree defination*/
	template<class DType, class WType = sfloat>
	struct TREE
	{
	private:
		uint32						ID;						// Local ID
		DType						Data;					// Data, for AI stand for current node state
		uint32						Depth;					// Current depth
		TREE<DType, WType>*			pTrunk;					// Trunk pointer, if null, root node
		vector<TREE<DType, WType>*>	OffshootPtrArray;		// Offshoot pointer array, if empty, leaf node
		vector<WType>				OffshootWeightArray;	// Offshoot weight array
		WType						GlobalWeight;			// Glboal weight, for build the heap
	protected:
		TREE<DType, WType>& operator=(const TREE<DType, WType> Tree);
		TREE(const TREE<DType, WType> &Tree);

		void _Reshape(const uint32 ID, const uint32 Depth)
		{
			this->ID = ID;
			this->Depth = Depth;

			vector<TREE<DType, WType>*>::iterator pOffshoot = this->OffshootPtrArray.begin();
			vector<WType>::iterator pWeight = this->OffshootWeightArray.begin();
			while (pOffshoot != this->OffshootPtrArray.end())
			{
				if ((*pOffshoot) == nullptr)
				{
					pOffshoot = this->OffshootPtrArray.erase(pOffshoot);
					pWeight = this->OffshootWeightArray.erase(pWeight);
				}
				else
				{
					pOffshoot++;
					pWeight++;
				}
			}
			for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
			{
				this->OffshootPtrArray[seek]->_Reshape(seek, this->Depth + 1);
			}
		}
	public:
		TREE(const uint32 ID = 0, TREE<DType, WType>* pTrunk = nullptr)
			:ID(ID), Depth(0), pTrunk(pTrunk), GlobalWeight(0)
		{
		};

		TREE(const uint32 ID, const DType &Data, const uint32 Depth, TREE<DType, WType>* pTrunk)
			:ID(ID), Data(Data), Depth(Depth), pTrunk(pTrunk), GlobalWeight(0)
		{
		};

		~TREE()
		{
			if (this->pTrunk != nullptr)
			{
				this->pTrunk->OffshootPtrArray[this->ID] = nullptr;
			}

			for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
			{
				if (this->OffshootPtrArray[seek] != nullptr)
				{
					delete this->OffshootPtrArray[seek];
					this->OffshootPtrArray[seek] = nullptr;
				}
			}
			this->ID = 0;
			this->OffshootPtrArray.clear();
			this->OffshootWeightArray.clear();

			this->pTrunk = nullptr;
		};

		/*Operator Ovarload*/
		const TREE<DType, WType>& operator[](const  size_t Position) const
		{
			if (this->OffshootPtrArray[Position] == nullptr){ ::cout << "Error: Tree access invalid node\n"; }
			return *this->OffshootPtrArray[Position];
		};

		TREE<DType, WType>& operator[](const  size_t Position)
		{
			if (this->OffshootPtrArray[Position] == nullptr){ ::cout << "Error: Tree access invalid node\n"; }
			return *this->OffshootPtrArray[Position];
		};

		/*For heap use*/
		bool operator()(const TREE<DType, WType> &Tree1, const TREE<DType, WType> &Tree2)
		{
			return Tree1.GlobalWeight < Tree2.GlobalWeight;
		};

		bool operator<(const TREE<DType, WType> &Tree)
		{
			return this->GlobalWeight < Tree.GlobalWeight;
		};

		bool operator==(const TREE<DType, WType> &Tree)
		{
			if (this->Data != Tree.Data){ return false; }
			if (this->GlobalWeight != Tree.GlobalWeight){ return false; }
			if (this->OffshootPtrArray.size() != Tree.OffshootPtrArray.size()){ return false; }
			for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
			{
				if (this->OffshootWeightArray[seek] != Tree.OffshootWeightArray[seek]){ return false; }
				if ((*this->OffshootPtrArray[seek]) != (*Tree.OffshootPtrArray[seek])){ return false; }
			}
			return true;
		};

		bool operator!=(const TREE<DType, WType> &Tree)
		{
			return !(*this == Tree);
		};

		/*Basic tree functions*/
		void iSetID(const uint32 ID)
		{
			this->ID = ID;
		};

		void iSetData(const DType &Data)
		{
			this->Data = Data;
		};

		bool iResize(const uint32 Size, const DType &DefaultData, const WType &DefaultWeight)
		{
			if (this->OffshootPtrArray.size() == Size)
			{
				return true;
			}

			if (this->OffshootPtrArray.size() < Size)
			{
				this->OffshootPtrArray.resize(Size);
				this->OffshootWeightArray.resize(Size);

				for (uint32 seek = this->OffshootPtrArray.size(); seek < Size; seek = seek + 1)
				{
					this->OffshootPtrArray[seek] = new TREE<DType, WType>(seek, DefaultData, this->Depth + 1, this);
					this->OffshootWeightArray[seek] = DefaultWeight;
				}
				return true;
			}
			else
			{
				for (uint32 seek = (Size - 1) > 0 ? (Size - 1) : 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
				{
					if (this->OffshootPtrArray[seek] != nullptr)
					{
						delete this->OffshootPtrArray[seek];
						//this->OffshootPtrArray[seek] = nullptr;
					}
				}
				this->OffshootPtrArray.resize(Size);
				this->OffshootWeightArray.resize(Size);
				return true;
			}
		};

		TREE<DType, WType>* iCreate(const DType &Data, const WType &Weight)
		{
			TREE<DType, WType>* pNode = new  TREE<DType, WType>(this->OffshootPtrArray.size(), Data, this->Depth + 1, this);
			this->OffshootPtrArray.push_back(pNode);
			this->OffshootWeightArray.push_back(Weight);
			return pNode;
		};

		TREE<DType, WType>* iCreate(const TREE<DType, WType> &Tree)
		{
			TREE<DType, WType>* pNode = Tree.iCopy();

			pNode->pTrunk = this;

			pNode->_Reshape(this->OffshootPtrArray.size(), this->Depth + 1);

			this->OffshootPtrArray.push_back(pNode);
			this->OffshootWeightArray.push_back(Weight);
			return pNode;
		};

		TREE<DType, WType>* iInsert(const uint32 ID, const DType &Data, const WType &Weight)
		{
			if (this->OffshootPtrArray.size() < ID)
			{
				return nullptr;
			}

			/*Search for the target node*/
			vector<TREE<DType, WType>*>::iterator pItem = this->OffshootPtrArray.begin();
			vector<WType>::iterator pWeightItem = this->OffshootWeightArray.begin();
			for (uint32 seek = 0; seek < ID; seek = seek + 1)
			{
				pItem++;
				pWeightItem++;
			}

			/*Insert*/
			TREE<DType, WType>* pTreeNode = new TREE<DType, WType>(ID, Data, this->Depth, this);

			this->OffshootPtrArray.insert(pItem, pTreeNode);
			this->OffshootWeightArray.insert(pWeightItem, Weight);

			/*Adjust the local ID for rest offshoot nodes*/
			for (uint32 seek = ID; seek < this->OffshootPtrArray.size(); seek = seek + 1)
			{
				this->OffshootPtrArray[seek]->ID = seek;
			}

			this->OffshootPtrArray[ID]->_Reshape(ID, this->Depth + 1);

			return this->OffshootPtrArray[ID];
		};

		TREE<DType, WType>* iInsert(const uint32 ID, const TREE<DType, WType> &TreeNode, const WType &Weight)
		{
			if (this->OffshootPtrArray.size() < ID)
			{
				return nullptr;
			}

			/*Search for the target node*/
			vector<TREE<DType, WType>*>::iterator pItem = this->OffshootPtrArray.begin();
			vector<WType>::iterator pWeightItem = this->OffshootWeightArray.begin();
			for (uint32 seek = 0; seek < ID; seek = seek + 1)
			{
				pItem++;
				pWeightItem++;
			}

			/*Insert*/
			TREE<DType, WType>* pTreeNode = TreeNode.iCopy();

			pTreeNode->pTrunk = this;

			this->OffshootPtrArray.insert(pItem, pTreeNode);
			this->OffshootWeightArray.insert(pWeightItem, Weight);

			/*Adjust the local ID for rest offshoot nodes*/
			for (uint32 seek = ID; seek < this->OffshootPtrArray.size(); seek = seek + 1)
			{
				this->OffshootPtrArray[seek]->ID = seek;
			}

			this->OffshootPtrArray[ID]->_Reshape(ID, this->Depth + 1);

			return this->OffshootPtrArray[ID];
		};

		bool iDelete(const uint32 ID)
		{

			if (this->OffshootPtrArray.size() <= ID)
			{
				return false;
			}

			/*If target node exist, delete the target node*/
			if (this->OffshootPtrArray[ID] != nullptr)
			{
				delete this->OffshootPtrArray[ID];
				this->OffshootPtrArray[ID] = nullptr;
			}

			/*Erase the target node from the array*/
			vector<TREE<DType, WType>*>::iterator pItem = this->OffshootPtrArray.begin();
			vector<WType>::iterator pWeightItem = this->OffshootWeightArray.begin();
			for (uint32 seek = 0; seek < ID; seek = seek + 1)
			{
				pItem++;
				pWeightItem++;
			}
			this->OffshootPtrArray.erase(pItem);
			this->OffshootWeightArray.erase(pWeightItem);

			/*Adjust the local ID for rest offshoot nodes*/
			for (uint32 seek = ID; seek < this->OffshootPtrArray.size(); seek = seek + 1)
			{
				this->OffshootPtrArray[seek]->ID = seek;
			}
		};

		void iCopy(TREE<DType, WType> &Tree) const
		{
			Tree.Data = this->Data;
			Tree.pTrunk = nullptr;// this->pTrunk;

			pTree->OffshootPtrArray.resize(this->OffshootPtrArray.size());
			for (uint32 seek = 0; seek < pTree->OffshootPtrArray.size(); seek = seek + 1)
			{
				pTree->OffshootPtrArray[seek] = this->OffshootPtrArray[seek]->iCopy();
			}
			pTree->OffshootWeightArray = this->OffshootWeightArray;
			pTree->GlobalWeight = this->GlobalWeight;

			Tree._Reshape(0, 0);
		};

		TREE<DType, WType>* iCopy() const
		{
			TREE<DType, WType>* pTree = new TREE<DType, WType>();

			pTree->Data = this->Data;
			pTree->Depth = this->Depth;

			pTree->OffshootPtrArray.resize(this->OffshootPtrArray.size());
			for (uint32 seek = 0; seek < pTree->OffshootPtrArray.size(); seek = seek + 1)
			{
				pTree->OffshootPtrArray[seek] = this->OffshootPtrArray[seek]->iCopy();
			}
			pTree->OffshootWeightArray = this->OffshootWeightArray;
			pTree->GlobalWeight = this->GlobalWeight;

			pTree->_Reshape(0, 0);
			return pTree;
		};

		void iClear(bool bDeleteObject = true)
		{
			if (bDeleteObject)
			{
				for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
				{
					if (this->OffshootPtrArray[seek] != nullptr)
					{
						delete this->OffshootPtrArray[seek];
						this->OffshootPtrArray[seek] = nullptr;
					}
				}
			}
			this->OffshootPtrArray.clear();
			this->OffshootWeightArray.clear();
		};

		bool iSetOffshootWeight(const uint32 ID, const WType Weight)
		{
			if (ID >= this->OffshootWeightArray.size())
			{
				return false;
			}
			this->OffshootWeightArray[ID] = Weight;
			return true;
		};

		WType iSetGlobalWeight(WType GlobalWeight)
		{
			this->GlobalWeight = GlobalWeight;
			return this->GlobalWeight;
		};

		const uint32 iGetID() const
		{
			return this->ID;
		};

		DType iGetData() const
		{
			return Data;
		};

		const DType* iGetDataPtrConst() const
		{
			return &Data;
		};

		DType* iGetDataPtr()
		{
			return &Data;
		};

		uint32 iGetDepth() const
		{
			return Depth;
		};

		const TREE<DType, WType>* iGetTrunkPtrConst() const
		{
			return this->pTrunk;
		};

		TREE<DType, WType>* iGetTrunkPtr() const
		{
			return this->pTrunk;
		};

		const uint32 iGetSize() const
		{
			return this->OffshootPtrArray.size();
		};

		const TREE<DType, WType>* iGetOffshootPtrConst(const uint32 ID) const
		{
			if (ID >= this->OffshootPtrArray.size())
			{
				return nullptr;
			}
			return this->OffshootPtrArray[ID];
		};

		TREE<DType, WType>* iGetOffshootPtr(const uint32 ID) const
		{
			if (ID >= this->OffshootPtrArray.size())
			{
				return nullptr;
			}
			return this->OffshootPtrArray[ID];
		};

		WType iGetOffshootWeight(const uint32 ID) const
		{
			if (ID >= this->OffshootWeightArray.size())
			{
				return 0.0;
			}
			return this->OffshootWeightArray[ID];
		};

		WType iGetGlobalWeight() const
		{
			return this->GlobalWeight;
		};

		uint32 iGetRelativeMaxDepth() const
		{
			//this->_Reshape(this->ID, this->Depth);

			uint32 MaxDepth = 0;

			for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
			{
				uint32 CurrentDepth = this->OffshootPtrArray[seek]->iGetRelativeMaxDepth() + 1;
				if (CurrentDepth>MaxDepth)
				{
					MaxDepth = CurrentDepth;
				}
			}

			return MaxDepth;// -this->Depth + 1;
		};

		SString iPrintTree(const SString &strBaseString = _SSTR("   ")) const
		{
			SString strOutput;

			for (uint32 seek = 0; seek < this->Depth; seek = seek + 1)
			{
				strOutput = strOutput + strBaseString;
			}

			strOutput = strOutput + this->Data.iPrintNode() + _SSTR("\r\n");

			for (uint32 seek = 0; seek < this->OffshootPtrArray.size(); seek = seek + 1)
			{
				strOutput = strOutput + this->OffshootPtrArray[seek]->iPrintTree();
			}

			return strOutput;
		}

		bool iIsRoot() const
		{
			return (this->pTrunk == nullptr);
		};

		bool iIsLeaf() const
		{
			return this->OffshootPtrArray.empty();
		};
	};
	template<class DType, class WType = sfloat>
	using _swtree = TREE<DType, WType>;
#pragma endregion
#endif
}