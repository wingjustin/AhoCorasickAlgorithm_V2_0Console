#pragma once

using namespace Tree;

namespace ACAlgorithm {
	class ACStringSearching {
	private:
		Trie acTrie;
	public:
		ACStringSearching();
		~ACStringSearching();

		static bool IsValidChar(char character);

		void BuildACTrie();
		void ClearACTrie();

		int MatchText(string text, KeywordLocationCollection& outputLocations);
		void AddKeyword(string keyword);
		void RemoveKeyword(string keyword);
	};
}
