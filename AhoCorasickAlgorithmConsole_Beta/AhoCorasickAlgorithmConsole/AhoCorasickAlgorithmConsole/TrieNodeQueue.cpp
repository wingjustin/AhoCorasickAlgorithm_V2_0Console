#include "TrieNodeQueue.h"

using namespace Tree;

TrieNodeQueue::TrieNodeQueue() {
	//length = 0;
	startElem = NULL;
	endElem = NULL;
}

TrieNodeQueue::~TrieNodeQueue() {
	TrieNodeQueueElem* current = startElem;
	if (current) {
		TrieNodeQueueElem* temp = current;
		do {
			temp = current;
			temp->elem = NULL;
			current = temp->next;
			temp->next = NULL;
			delete temp;
		} while (current);
		temp = NULL;
	}
	startElem = NULL;
	endElem = NULL;
}

bool TrieNodeQueue::IsEmpty() {
	//return length == 0;
	return startElem == NULL;
}

//int TrieNodeQueue::GetLength() {
//	return length;
//}

void TrieNodeQueue::Push(TrieNode* elem) {
	if (endElem) {
		endElem->next = new TrieNodeQueueElem();
		endElem->next->elem = elem;
		endElem = endElem->next;
	}
	else {
		endElem = startElem = new TrieNodeQueueElem();
		endElem->elem = elem;
	}
	//length++;
}

TrieNode* TrieNodeQueue::Pop() {
	if (startElem) {
		TrieNodeQueueElem* popElem = startElem;
		if (!(startElem = startElem->next))
			endElem = NULL;
		//length--;
		TrieNode* popNode = popElem->elem;
		delete popElem;
		return popNode;
	}
	else {
		return NULL;
	}
}

TrieNode* TrieNodeQueue::Front() {
	return startElem ? startElem->elem : NULL;
}

TrieNode* TrieNodeQueue::Back() {
	return endElem ? endElem->elem : NULL;
}