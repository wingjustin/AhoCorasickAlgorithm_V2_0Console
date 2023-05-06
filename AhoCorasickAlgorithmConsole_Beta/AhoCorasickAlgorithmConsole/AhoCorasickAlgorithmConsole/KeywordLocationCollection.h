#pragma once

#include <string>

using namespace std;

namespace ACAlgorithm {
	struct KeywordLocation {
		string keyword;
		int startPosition = INT_MIN;
		int endPosition = INT_MIN;
		int length = 0;

		KeywordLocation* prev = NULL;
		KeywordLocation* next = NULL;
	};
	class KeywordLocationCollection {
		int length;
		KeywordLocation* firstLocation;
		KeywordLocation* lastLocation;
	public:
		KeywordLocationCollection();
		~KeywordLocationCollection();

		bool IsEmpty();
		int GetLength();

		KeywordLocation* GetFirstLocation();
		KeywordLocation* GetLastLocation();

		KeywordLocation* InsertLocation(KeywordLocation* newLocation);
	};
}
