#pragma once

#include <string>
#include "TrieChildNodeContainer.h"

#define CHILDMAP_SIZE 62 //10 digit + 26 Uppercase + 26 Lowercase
#define DIGIT_SIZE 10
#define ALPHABET_SIZE 26
#define DIGIT_ASCII_MIN 0x30 // '0'
#define DIGIT_ASCII_MAX 0x39 // '9'
#define UPPER_ASCII_MIN 0x41 // 'A'
#define UPPER_ASCII_MAX 0x5A // 'Z'
#define LOWER_ASCII_MIN 0x61 // 'a'
#define LOWER_ASCII_MAX 0x7A // 'z'

using namespace std;

namespace Tree {
	class TrieNode {
	private:
		TrieNode* parent;
		TrieChildNode* thisChild;

		char character;
		bool isLeaf;

		int wordLength;
	protected:
		TrieChildNodeContainer childContainer;

		TrieNode* childMap[CHILDMAP_SIZE]; // A-Z = 0-25, a-z = 26-51

		TrieNode* failToNode; //failto pointer, for the scenario of fail to match, like KMP Algorithm
	public:
		TrieNode(); // for root
		TrieNode(TrieNode* parent, char character); // default no leaf
		TrieNode(TrieNode* parent, char character, bool isLeaf);
		~TrieNode();

		static bool TryConvertToMapIndex(char character, int& mapIndexOutput); // return success and index

		static bool IsValidChar(char alphabet);

		bool IsLeaf();
		void ChangeToLeaf();
		void ChangeToNoLeaf();

		bool HasChild();
		int GetChildLength();

		char GetCharacter();
		void SetCharacter(char character);

		//failTo pointer
		TrieNode* GetFailToNode();
		void SetFailToNode(TrieNode* failToNode);

		string GetWord();

		int GetWordLength();

		TrieNode* GetParent();
		TrieChildNode* GetThisChild();

		TrieNode* GetChild(char character);
		TrieNode* GetChildByMapIndex(int mapIndex);

		TrieChildNode* GetFirstChildNode();
		TrieChildNode* GetLastChildNode();

		bool AddChild(char character); // return success
		bool AddChildAndGetMapIndex(char character, int& mapIndexOutput); // return success and child index

		bool DeleteChild(char character); // return success
		bool DeleteChild(TrieChildNode* child); // return success
	};
}
