#include "Tree.h"
#include "TrieNodeQueue.h"

using namespace Tree;

Trie::Trie() {
	root = new TrieNode();
}

Trie::~Trie() {
	delete root;
	root = NULL;
}

bool Trie::IsValidChar(const char alphabet) {
	return root->IsValidChar(alphabet);
}

TrieNode* Trie::FindNode(const string word) {
	int length = static_cast<int>(word.length());
	TrieNode* currentNode = root;
	for (int c = 0; c < length && currentNode; c++) {
		currentNode = currentNode->GetChild(word[c]);
	}
	return currentNode;
}

int Trie::GetWords(const string prefix, string*& words) { // return length and words
	//initialize
	words = NULL;
	//
	int returnLength = 0;
	int prefixLength = static_cast<int>(prefix.length());

	TrieNode* currentPrefixNode = FindNode(prefix);
	if (!currentPrefixNode)
		return returnLength;
	//BFS
	TrieNodeQueue queue;
	queue.Push(currentPrefixNode);
	TrieNode* currentNode;
	while (!queue.IsEmpty()) {
		currentNode = queue.Pop();
		if (currentNode->IsLeaf()) {
			string* oldWords = words;
			words = new string[returnLength + 1];
			if (oldWords) {
				for (int i = 0; i < returnLength; i++)
					words[i] = oldWords[i];
				delete[] oldWords;
			}
			words[returnLength] = currentNode->GetWord();
			returnLength++;
		}
		if (currentNode->HasChild()) {
			TrieChildNode* childNode = currentNode->GetFirstChildNode();
			while (childNode) {
				queue.Push(childNode->node);
				childNode = childNode->next;
			}
		}
	}
	return returnLength;
}

bool Trie::AddWord(const string word) { // return success
	int length = static_cast<int>(word.length());
	TrieNode* currentNode = root;
	TrieNode* lastNode = NULL;
	for (int c = 0; c < length; c++) {
		int mapIndex;
		currentNode->AddChildAndGetMapIndex(word[c], mapIndex);
		if (mapIndex > -1)
			lastNode = currentNode = currentNode->GetChildByMapIndex(mapIndex);
		else
			return false;
	}
	if (lastNode)
		lastNode->ChangeToLeaf();
	return true;
}

bool Trie::RemoveWord(const string word) { // return success
	TrieNode* currentNode = FindNode(word);
	TrieChildNode* currentChildNode = currentNode->GetThisChild();
	if (!currentNode->IsLeaf())
		return false;
	if (currentNode->HasChild()) {
		currentNode->ChangeToNoLeaf();
		return true;
	}
	//is leaf but no child
	if (currentNode = currentNode->GetParent()) {
		currentNode->DeleteChild(currentChildNode);
		currentChildNode = currentNode->GetThisChild();
		while (currentNode != root
			&& (!currentNode->IsLeaf() && !currentNode->HasChild())
			) {
			currentNode = currentNode->GetParent();
			currentNode->DeleteChild(currentChildNode);
			currentChildNode = currentNode->GetThisChild();
		}
		return true;
	}
	return false;
}

void Trie::Clear() {
	delete root;
	root = new TrieNode();
}

//reduced some useless loops, it can build faster
void Trie::BuildACTrie() {
	if (!root)
		return;
	//using pure BFS
	TrieNodeQueue queue;
	int rootChildCount = root->GetChildLength();
	if (rootChildCount < 1)
		return;
	TrieNode* currentNode;
	//push children first
	TrieChildNode* childNode = root->GetFirstChildNode();
	while (childNode) {
		queue.Push(currentNode = childNode->node);
		//set failto pointers of children point to root
		currentNode->SetFailToNode(root);
		childNode = childNode->next;
	}
	//go to children
	while (!queue.IsEmpty()) {
		currentNode = queue.Pop();
		//step 1
		//go to failTo Node and build transfer map
		TrieNode* failToNode = currentNode->GetFailToNode();
		if (failToNode && failToNode->HasTransferNode()) {
			TrieChildNode* transferChildNode = failToNode->GetFirstTransferNode();
			do {
				//get transfer Node
				currentNode->AddTransferNode(transferChildNode->node); // if the child has a same character with current node, it will return false and will not insert into map
			} while (transferChildNode = transferChildNode->next);
		}
		//step 2
		//go to child and search the failTo pointer
		if (currentNode->HasChild()) {
			TrieChildNode* childNode = currentNode->GetFirstChildNode();
			TrieNode* child;
			do {
				queue.Push(child = childNode->node);
				//find the failto pointers of children
				TrieNode* failToNode = currentNode->GetFailToNode();
				TrieNode* failToNodeTransfer;
				if (failToNodeTransfer = failToNode->GetTransferNode(child->GetCharacter()))
					child->SetFailToNode(failToNodeTransfer);
				else
					child->SetFailToNode(root);
			} while (childNode = childNode->next);
		}
		//step 3
		//skip no leaf failTo Node
		TrieNode* suffixNode = currentNode->GetFailToNode();
		//currentNode->SetSuffixNode(NULL);
		if (!suffixNode || suffixNode->IsLeaf()) // suffux node can only be a near Leaf or NULL
			currentNode->SetSuffixNode(suffixNode);
		else
			currentNode->SetSuffixNode(suffixNode->GetSuffixNode());
	}
}

////slower build
//void Trie::BuildACTrie() {
//	if (!root)
//		return;
//	//using pure BFS
//	TrieNodeQueue queue;
//	int rootChildCount = root->GetChildLength();
//	if (rootChildCount < 1)
//		return;
//	TrieNode* currentNode;
//	//push children first
//	TrieChildNode* childNode = root->GetFirstChildNode();
//	while (childNode) {
//		queue.Push(currentNode = childNode->node);
//		//set failto pointers of children point to root
//		currentNode->SetFailToNode(root);
//		childNode = childNode->next;
//	}
//	//go to children
//	while (!queue.IsEmpty()) {
//		currentNode = queue.Pop();
//		//step 1
//		//go to failTo Node and build transfer map
//		TrieNode* failToNode = currentNode->GetFailToNode();
//		while (failToNode) {
//			if (failToNode->HasChild()) {
//				childNode = failToNode->GetFirstChildNode();
//				TrieNode* child;
//				do {
//					//get transfer Node
//					currentNode->AddTransferNode(childNode->node); // if the child has a same character with current node, it will return false and will not insert into map
//					childNode = childNode->next;
//				} while (childNode);
//			}
//			failToNode = failToNode->GetFailToNode();
//		}
//		//step 2
//		//go to child and search the failTo pointer
//		if (currentNode->HasChild()) {
//			TrieChildNode* childNode = currentNode->GetFirstChildNode();
//			TrieNode* child;
//			do {
//				queue.Push(child = childNode->node);
//				//find the failto pointers of children
//				TrieNode* failToNode = currentNode->GetFailToNode();
//				TrieNode* failToNodeChild;
//				if (failToNodeChild = failToNode->GetChild(child->GetCharacter()))
//					child->SetFailToNode(failToNodeChild);
//				//go to root check again
//				else if (failToNodeChild = root->GetChild(child->GetCharacter()))
//					child->SetFailToNode(failToNodeChild);
//				else
//					child->SetFailToNode(root);
//			} while (childNode = childNode->next);
//		}
//		//step 3
//		//skip no leaf failTo Node
//		TrieNode* suffixNode = currentNode->GetFailToNode();
//		//currentNode->SetSuffixNode(NULL);
//		while (suffixNode && !suffixNode->IsLeaf())
//			suffixNode = suffixNode->GetFailToNode();
//		currentNode->SetSuffixNode(suffixNode); // suffux node can only be a near Leaf or NULL
//	}
//}

int Trie::MatchText(const string text, KeywordLocationCollection& outputLocationCollection) { //return Keyword Locations
	int locationCount = 0;
	int textLength = static_cast<int>(text.length());
	TrieNode* currentMatchingNode = root;
	for (int c = 0; c < textLength; c++) {
		char currentChar = text[c];
		if (currentMatchingNode = currentMatchingNode->GetTransferNode(currentChar)) {
			//match success
			if (currentMatchingNode->IsLeaf()) {
				//insert keyword location
				InsertKeywordLocationCollection(currentMatchingNode->GetWord(), c, outputLocationCollection);
				locationCount++;
			}
			//go to suffix node
			TrieNode* currentMatchingSuffixNode = currentMatchingNode->GetSuffixNode();
			while (currentMatchingSuffixNode) { // suffix node can only be a leaf or NULL
				//insert keyword location
				InsertKeywordLocationCollection(currentMatchingSuffixNode->GetWord(), c, outputLocationCollection);
				locationCount++;
				currentMatchingSuffixNode = currentMatchingSuffixNode->GetSuffixNode();
			}
			//
		}
		else {
			//no match, go to root directly
			currentMatchingNode = root;
		}
	}
	return locationCount;
}

void Trie::InsertKeywordLocationCollection(const string keyword, const int matchPosition, KeywordLocationCollection& outputLocationCollection) {
	KeywordLocation* location = new KeywordLocation();
	location->keyword = keyword;
	location->length = static_cast<int>(location->keyword.length());
	location->endPosition = matchPosition;
	location->startPosition = matchPosition - location->length + 1;
	outputLocationCollection.InsertLocation(location);
}
