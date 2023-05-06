#include "ACAlgorithm.h"

using namespace ACAlgorithm;

ACStringSearching::ACStringSearching() {
}

ACStringSearching::~ACStringSearching() {
}

bool ACStringSearching::IsValidChar(const char character) {
	return TrieNode::IsValidChar(character);
}

void ACStringSearching::BuildACTrie() {
	acTrie.BuildACTrie();
}

void ACStringSearching::ClearACTrie() {
	acTrie.Clear();
}

int ACStringSearching::MatchText(const string text, KeywordLocationCollection& outputLocations) {
	return acTrie.MatchText(text, outputLocations);
}

void ACStringSearching::AddKeyword(const string keyword) {
	acTrie.AddWord(keyword);
}

void ACStringSearching::RemoveKeyword(const string keyword) {
	acTrie.RemoveWord(keyword);
}
