#include "TrieChildNodeContainer.h"

using namespace Tree;

TrieChildNodeContainer::TrieChildNodeContainer() {
	length = 0;
	startNode = NULL;
	endNode = NULL;
}

TrieChildNodeContainer::~TrieChildNodeContainer() {
	TrieChildNode* current = startNode;
	if (current) {
		TrieChildNode* temp = current;
		do {
			temp = current;
			//temp->node = NULL;
			current = temp->next;
			temp->container = NULL;
			temp->prev = NULL;
			temp->next = NULL;
			delete temp;
		} while (current);
		temp = NULL;
	}
	startNode = NULL;
	endNode = NULL;
}

bool TrieChildNodeContainer::IsEmpty() {
	return length == 0;
}

int TrieChildNodeContainer::GetLength() {
	return length;
}

TrieChildNode* TrieChildNodeContainer::GetFirstChildNode() {
	return startNode;
}

TrieChildNode* TrieChildNodeContainer::GetLastChildNode() {
	return endNode;
}

TrieChildNode* TrieChildNodeContainer::InsertChildNode(TrieNode* const newNode, const int newMapIndex) {
	if (endNode)
		return InsertChildNodeToNext(endNode, newNode, newMapIndex);
	else {
		endNode = startNode = new TrieChildNode();
		endNode->container = this;
		endNode->node = newNode;
		endNode->mapIndex = newMapIndex;

		length++;

		return endNode;
	}
}

TrieChildNode* TrieChildNodeContainer::InsertChildNodeToPrev(TrieChildNode* const targetChildNode, TrieNode* const newNode, const int newMapIndex) {
	if (targetChildNode->container != this)
		return NULL;
	TrieChildNode* middle = new TrieChildNode();
	middle->container = this;
	middle->node = newNode;
	middle->mapIndex = newMapIndex;
	TrieChildNode* left = targetChildNode->prev;
	TrieChildNode* right = targetChildNode;
	if (left) {
		left->next = middle;
		middle->prev = left;
		right->prev = middle;
		middle->next = right;
	}
	else {
		right->prev = middle;
		middle->next = right;
		if (startNode == right)
			startNode = middle;
	}
	length++;

	return middle;
}

TrieChildNode* TrieChildNodeContainer::InsertChildNodeToNext(TrieChildNode* const targetChildNode, TrieNode* const newNode, const int newMapIndex) {
	if (targetChildNode->container != this)
		return NULL;
	TrieChildNode* middle = new TrieChildNode();
	middle->container = this;
	middle->node = newNode;
	middle->mapIndex = newMapIndex;
	TrieChildNode* left = targetChildNode;
	TrieChildNode* right = targetChildNode->next;
	if (right) {
		left->next = middle;
		middle->prev = left;
		right->prev = middle;
		middle->next = right;
	}
	else {
		left->next = middle;
		middle->prev = left;
		if (endNode == left)
			endNode = middle;
	}
	length++;

	return middle;
}

bool TrieChildNodeContainer::RemoveChildNode(TrieChildNode* const targetChildNode) {
	if (targetChildNode->container != this)
		return false;
	TrieChildNode* left = targetChildNode->prev;
	TrieChildNode* right = targetChildNode->next;
	if (left && right) {
		left->next = right;
		right->prev = left;
	}
	else if (left) {
		left->next = NULL;
		if (endNode == targetChildNode)
			endNode = left;
	}
	else if (right) {
		right->prev = NULL;
		if (startNode == targetChildNode)
			startNode = right;
	}
	else {
		if (startNode == targetChildNode)
			startNode = NULL;
		if (endNode == targetChildNode)
			endNode = NULL;
	}
	targetChildNode->container = NULL;
	//targetChildNode->node = NULL;
	targetChildNode->prev = NULL;
	targetChildNode->next = NULL;
	//delete targetChildNode;

	length--;
	return true;
}
