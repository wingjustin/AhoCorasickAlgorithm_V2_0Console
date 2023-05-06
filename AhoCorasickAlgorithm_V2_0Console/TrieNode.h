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

		//TrieNode* childMap[CHILDMAP_SIZE]; // A-Z = 0-25, a-z = 26-51

		TrieNode* failToNode; //failto pointer, for the scenario of fail to match, like KMP Algorithm

		//these 2 improvement must be built by BFS.
		//2 container, 1 map

		TrieChildNodeContainer transferContainer; // for building AC Trie

		TrieNode* transferMap[CHILDMAP_SIZE]; // A-Z = 0-25, a-z = 26-51
		//pay attention that this map not only map children, also map failto pointer's children and failto failto's children...
		// overlap char will not be changed
		//***example 1 : text = "shers", keywords = "she hers";
		// when finished matching the last character 'e' of word "she"
		//, jump to the third character 'r' of word "hers" though this transfer map if next char of the text is 'r'
		//***example 2 : text = "shersh", keywords = "herh rsk sh";
		// when finished matching the third character 'r' of word "herh"
		//, jump to the second character 's' of word "rsk" though 'r' transfer map at "herh" if next char of the text is 's'
		//, jump to the last character 'h' of word "sh" though 's' transfer map at "rsk" if next char of the text is 'h'
		//***example 3 : text = "shrk", keywords = "shrz hrm rk";
		// when finished matching the third character 'r' of word "shrz"
		//, jump to the last character 'k' of word "rk" and skip failto pointer which point to 'r' at "hrm" though 'r' transfer map at "shrz"

		TrieNode* suffixNode;
		//suffix pointer; change from failto pointer
		// during to the matching, skip the looping of checking of leaf failToNode and jump to other prefix which same with the suffix; If it is null, no need to check failto Node
		//***example 1 : text = "shrm", keywords = "shr hrk r"; suffixNode will skip second char 'r' at "hrk" and jump to "r"
		//***example 2 : text = "shrm", keywords = "shr hr r"; suffixNode will not skip second char 'r' at "hr", just like failTo pointer
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

		bool HasTransferNode();
		int GetTransferNodeLength();

		char GetCharacter();
		void SetCharacter(char character);

		//failTo pointer
		TrieNode* GetFailToNode();
		void SetFailToNode(TrieNode* failToNode);

		//suffix pointer
		TrieNode* GetSuffixNode();
		void SetSuffixNode(TrieNode* suffixNode);

		string GetWord();

		int GetWordLength();

		TrieNode* GetParent();
		TrieChildNode* GetThisChild();

		TrieNode* GetChild(char character);
		TrieNode* GetChildByMapIndex(int mapIndex);

		TrieChildNode* GetFirstChildNode();
		TrieChildNode* GetLastChildNode();

		TrieNode* GetTransferNode(char character);
		TrieNode* GetTransferNodeByMapIndex(int mapIndex);

		TrieChildNode* GetFirstTransferNode();
		TrieChildNode* GetLastTransferNode();

		bool AddChild(char character); // return success
		bool AddChildAndGetMapIndex(char character, int& mapIndexOutput); // return success and child index

		bool DeleteChild(char character); // return success
		bool DeleteChild(TrieChildNode* child); // return success

		bool AddTransferNode(TrieNode* transferNode); // return success
		bool AddTransferNodeAndGetMapIndex(TrieNode* transferNode, int& mapIndexOutput); // return success and child index

		bool DeleteTransferNode(char character); // return success
		bool DeleteTransferNode(TrieChildNode* transferNode); // return success
	};
}
