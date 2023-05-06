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

//traverse, BFS
void Trie::BuildACTrie() {
	if (!root)
		return;
	//BFS
	TrieNodeQueue queue;
	int rootChildCount = root->GetChildLength();
	if (rootChildCount < 1)
		return;
	//push children first
	TrieChildNode* childNode = root->GetFirstChildNode();
	while (childNode) {
		queue.Push(childNode->node);
		childNode = childNode->next;
	}
	TrieNode* currentNode;
	//set childs' failTo point to root and push grandchildren first
	for (; rootChildCount > 0 && !queue.IsEmpty(); rootChildCount--) {
		currentNode = queue.Pop();
		currentNode->SetFailToNode(root); // set failTo point to root directly, like KMP set -1 to next[0]
		if (currentNode->HasChild()) {
			TrieChildNode* childNode = currentNode->GetFirstChildNode();
			while (childNode) {
				queue.Push(childNode->node);
				childNode = childNode->next;
			}
		}
	}
	//go to grand children
	while (!queue.IsEmpty()) {
		currentNode = queue.Pop();
		//matching and update fail pointers
		BuildACTrie(currentNode, &queue);
	}
}

//matching, DFS
void Trie::BuildACTrie(TrieNode* const matchingNode, TrieNodeQueue* const queue) {
	TrieNode* matchNode = matchingNode;
	TrieNode* currentChildNode;
	if (currentChildNode = root->GetChild(matchNode->GetCharacter())) { // no child will also return NULL;
		matchNode->SetFailToNode(currentChildNode);
		if (matchNode->HasChild()) {
			TrieChildNode* matchChild = matchNode->GetFirstChildNode();
			while (matchChild) {
				BuildACTrie(matchChild->node, currentChildNode, queue);
				matchChild = matchChild->next;
			}
		}
	}
	else {
		matchNode->SetFailToNode(root);
		if (matchNode->HasChild()) {
			TrieChildNode* matchChild = matchNode->GetFirstChildNode();
			//push children into queue and process later
			while (matchChild) {
				queue->Push(matchChild->node);
				matchChild = matchChild->next;
			}
		}
	}
}

void Trie::BuildACTrie(TrieNode* const matchingNode, TrieNode* const currentNode, TrieNodeQueue* const queue) {
	TrieNode* matchNode = matchingNode;
	TrieNode* currentChildNode;
	if (currentChildNode = currentNode->GetChild(matchNode->GetCharacter())) { // no child will also return NULL;
		matchNode->SetFailToNode(currentChildNode);
		if (matchNode->HasChild()) {
			TrieChildNode* matchChild = matchNode->GetFirstChildNode();
			while (matchChild) {
				BuildACTrie(matchChild->node, currentChildNode, queue);
				matchChild = matchChild->next;
			}
		}
	}
	else {
		//if no child match , push into queue and process later
		queue->Push(matchNode);
	}
}

int Trie::MatchText(const string text, KeywordLocationCollection& outputLocationCollection) { //return Keyword Locations
	int locationCount = 0;
	int textLength = static_cast<int>(text.length());
	TrieNode* currentMatchingNode = root;
	for (int c = 0; c < textLength; ) {//c++) {
		char currentChar = text[c];
		TrieNode* currentMatchingChildNode;
		if (currentMatchingChildNode = currentMatchingNode->GetChild(currentChar)) {
			currentMatchingNode = currentMatchingChildNode;
			//match success
			if (currentMatchingNode->IsLeaf()) {
				//insert keyword location
				InsertKeywordLocationCollection(currentMatchingNode->GetWord(), c, outputLocationCollection);
				locationCount++;
			}
			// these 2 parts will optimize on version 2.0
			//go to failto check if the suffix is also match
			// example : 3 keywords "she he e", when the text matchs "she", it also match "he" and "e"
			// example : 3 keywords "she her e", when the text matchs "she", it also match "e"
			TrieNode* currentMatchingFailToNode = currentMatchingNode->GetFailToNode();
			while (currentMatchingFailToNode != root) {
				if (currentMatchingFailToNode->IsLeaf()) {
					//insert keyword location
					InsertKeywordLocationCollection(currentMatchingFailToNode->GetWord(), c, outputLocationCollection);
					locationCount++;
				}
				currentMatchingFailToNode = currentMatchingFailToNode->GetFailToNode();
			}
			// if no child, go to failto pointer to match more suffix
			//example 1 : text = "shers", keywords = "she hers";
			// when finished matching the last character 'e' of word "she"
			// , jump to the second character 'e' of word "hers"
			// example 2 : text = "shersh", keywords = "hersh she";
			// when finished matching the last character 'h' of word "hersh"
			//, jump to the second character 'h' of word "she"
			if (!currentMatchingNode->HasChild()) {
				currentMatchingNode = currentMatchingNode->GetFailToNode();
			}
			//
			c++;
		}
		else {
			//no match, go to fail node first
			currentMatchingNode = currentMatchingNode->GetFailToNode();
			if (!currentMatchingNode) {
				currentMatchingNode = root;
				c++;
			}
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
