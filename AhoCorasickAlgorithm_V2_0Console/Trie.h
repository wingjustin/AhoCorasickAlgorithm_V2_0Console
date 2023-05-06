#pragma once

#include <string>
#include "TrieNodeQueue.h"
#include "KeywordLocationCollection.h"

using namespace std;
using namespace ACAlgorithm;

namespace Tree {
	class Trie {
	protected:
		TrieNode* root;

		TrieNode* FindNode(string word);

		void InsertKeywordLocationCollection(string keyword, int matchPosition, KeywordLocationCollection& outputLocationCollection);
	public:
		Trie();
		~Trie();

		bool IsValidChar(char alphabet);

		int GetWords(string prefix, string*& words); // return length and words
		bool AddWord(string word); // return success
		bool RemoveWord(string word); // return success
		void Clear();

		//using pure BFS
		void BuildACTrie();

		int MatchText(string text, KeywordLocationCollection& outputLocationCollection); //return KeywordLocationCollection
	};
}
