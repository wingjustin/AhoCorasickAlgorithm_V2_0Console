#include "Tree.h"

using namespace Tree;

TrieNode::TrieNode()
	: TrieNode(NULL, '\0', false) { // for root
}

TrieNode::TrieNode(TrieNode* const parent, const char character)
	: TrieNode(parent, character, false) { // default no leaf
}

TrieNode::TrieNode(TrieNode* const parent, const char character, const bool isLeaf) {
	TrieNode::parent = parent;
	TrieNode::thisChild = NULL;
	TrieNode::character = character;
	//TrieNode::childLength = 0;
	TrieNode::isLeaf = isLeaf;

	for (int i = 0; i < CHILDMAP_SIZE; i++)
		childMap[i] = NULL;

	wordLength = 0;

	failToNode = NULL;
}

TrieNode::~TrieNode() {
	parent = NULL;
	TrieChildNode* childNode = GetFirstChildNode();
	while (childNode) {
		childMap[childNode->mapIndex] = NULL;
		delete childNode->node;
		childNode = childNode->next;
	}
}

bool TrieNode::TryConvertToMapIndex(const char character, int& mapIndexOutput) { // return success and index
	//Digit
	if (character >= DIGIT_ASCII_MIN
		&& character <= DIGIT_ASCII_MAX) {
		//mapIndexOutput = character - DIGIT_ASCII_MIN;
		mapIndexOutput = character ^ 0x30;
		return true;
	}
	//Uppercase
	else if (character >= UPPER_ASCII_MIN
		&& character <= UPPER_ASCII_MAX) {
		//mapIndexOutput = DIGIT_SIZE + character - UPPER_ASCII_MIN;
		mapIndexOutput = 9 + (character ^ 0x40);
		return true;
	}
	//Lowercase
	else if (character >= LOWER_ASCII_MIN
		&& character <= LOWER_ASCII_MAX) {
		//mapIndexOutput = DIGIT_SIZE + ALPHABET_SIZE + character - LOWER_ASCII_MIN;
		mapIndexOutput = 35 + (character ^ 0x60);
		return true;
	}
	//invalid character
	else {
		mapIndexOutput = INT_MIN;
		return false;
	}
}

bool TrieNode::IsValidChar(const char character) {
	int temp;
	return TryConvertToMapIndex(character, temp);
}

bool TrieNode::IsLeaf() {
	return isLeaf;
}

void TrieNode::ChangeToLeaf() {
	isLeaf = true;
}

void TrieNode::ChangeToNoLeaf() {
	isLeaf = false;
}

bool TrieNode::HasChild() {
	//return childLength > 0;
	return childContainer.GetLength() > 0;
}

int TrieNode::GetChildLength() {
	//return childLength;
	return childContainer.GetLength();
}

char TrieNode::GetCharacter() {
	return character;
}

void TrieNode::SetCharacter(const char character) {
	TrieNode::character = character;
}

TrieNode* TrieNode::GetFailToNode() {
	return failToNode;
}

void TrieNode::SetFailToNode(TrieNode* failToNode) {
	TrieNode::failToNode = failToNode;
}

int TrieNode::GetWordLength() {
	return wordLength;
}

string TrieNode::GetWord() {
	if (!IsLeaf())
		return "";
	string reverse;
	TrieNode* currentNode = this;
	while (currentNode->character != '\0') {
		reverse.append(1, currentNode->character);
		currentNode = currentNode->parent;
	}
	//reverse
	int length = static_cast<int>(reverse.length());
	int left = 0;
	int right = length - 1;
	do {
		//swap
		char temp = reverse[left];
		reverse[left] = reverse[right];
		reverse[right] = temp;
	} while (++left < --right);
	return reverse;
}

TrieNode* TrieNode::GetParent() {
	return parent;
}

TrieChildNode* TrieNode::GetThisChild() {
	return thisChild;
}

TrieNode* TrieNode::GetChild(const char character) {
	int mapIndex;
	return TryConvertToMapIndex(character, mapIndex)
		? childMap[mapIndex]
		: NULL;
}

TrieNode* TrieNode::GetChildByMapIndex(const int mapIndex) {
	return childMap[mapIndex];
}

TrieChildNode* TrieNode::GetFirstChildNode() {
	return childContainer.GetFirstChildNode();
}

TrieChildNode* TrieNode::GetLastChildNode() {
	return childContainer.GetLastChildNode();
}

bool TrieNode::AddChild(const char character) { // return success
	int mapIndex;
	return AddChildAndGetMapIndex(character, mapIndex);
}

bool TrieNode::AddChildAndGetMapIndex(const char character, int& mapIndexOutput) { // return child index and index
	if (TryConvertToMapIndex(character, mapIndexOutput)
		&& !GetChildByMapIndex(mapIndexOutput)) { // if it has match child, return false;
		TrieNode* newNode = new TrieNode(this, character);
		TrieChildNode* newChild = childContainer.InsertChildNode(newNode, mapIndexOutput);
		childMap[newChild->mapIndex] = newNode;
		newNode->thisChild = newChild;
		newNode->wordLength = this->wordLength + 1;
		return true;
	}
	return false;
}

bool TrieNode::DeleteChild(const char character) { // return success
	int mapIndex;
	TrieNode* target;
	if (TryConvertToMapIndex(character, mapIndex)
		&& (target = GetChildByMapIndex(mapIndex)))
		return DeleteChild(target->thisChild);
	return false;
}

bool TrieNode::DeleteChild(TrieChildNode* const child) { // return success
	if (childContainer.RemoveChildNode(child)) {
		childMap[child->mapIndex] = NULL;
		delete child->node;
		delete child;
		return true;
	}
	return false;
}
