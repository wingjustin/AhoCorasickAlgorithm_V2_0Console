#pragma once

#include "TrieNode.h"

namespace Tree {
	struct TrieNodeQueueElem {
		TrieNode* elem = NULL;
		TrieNodeQueueElem* next = NULL;
	};
	class TrieNodeQueue {
		//int length;
		TrieNodeQueueElem* startElem;
		TrieNodeQueueElem* endElem;
	public:
		TrieNodeQueue();
		~TrieNodeQueue();

		bool IsEmpty();
		//int GetLength();

		void Push(TrieNode* elem);
		TrieNode* Pop();
		TrieNode* Front();
		TrieNode* Back();
	};
}
