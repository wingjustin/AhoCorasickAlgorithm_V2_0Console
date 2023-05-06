#pragma once

#include<stdlib.h>

namespace Tree {
	class TrieNode;
	class TrieChildNodeContainer;
	struct TrieChildNode {
		TrieChildNodeContainer* container = NULL;
		int mapIndex = INT_MIN;
		TrieNode* node = NULL;
		TrieChildNode* prev = NULL;
		TrieChildNode* next = NULL;
	};
	class TrieChildNodeContainer {
		int length;
		TrieChildNode* startNode;
		TrieChildNode* endNode;
	public:
		TrieChildNodeContainer();
		~TrieChildNodeContainer();

		bool IsEmpty();
		int GetLength();

		TrieChildNode* GetFirstChildNode();
		TrieChildNode* GetLastChildNode();

		TrieChildNode* InsertChildNode(TrieNode* newNode, int newMapIndex);
		TrieChildNode* InsertChildNodeToPrev(TrieChildNode* targetChildNode, TrieNode* newNode, int newMapIndex);
		TrieChildNode* InsertChildNodeToNext(TrieChildNode* targetChildNode, TrieNode* newNode, int newMapIndex);

		bool RemoveChildNode(TrieChildNode* targetChildNode);
	};
}
