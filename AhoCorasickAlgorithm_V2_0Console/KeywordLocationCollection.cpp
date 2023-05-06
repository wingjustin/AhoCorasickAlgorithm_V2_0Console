#include "KeywordLocationCollection.h"

using namespace ACAlgorithm;

KeywordLocationCollection::KeywordLocationCollection() {
	length = 0;
	firstLocation = NULL;
	lastLocation = NULL;
}

KeywordLocationCollection::~KeywordLocationCollection() {
	KeywordLocation* current = firstLocation;
	if (current) {
		KeywordLocation* temp = current;
		do {
			temp = current;
			current = temp->next;
			temp->prev = NULL;
			temp->next = NULL;
			delete temp;
		} while (current);
		temp = NULL;
	}
	firstLocation = NULL;
	lastLocation = NULL;
}

bool KeywordLocationCollection::IsEmpty() {
	return length == 0;
}

int KeywordLocationCollection::GetLength() {
	return length;
}

KeywordLocation* KeywordLocationCollection::GetFirstLocation() {
	return firstLocation;
}

KeywordLocation* KeywordLocationCollection::GetLastLocation() {
	return lastLocation;
}

KeywordLocation* KeywordLocationCollection::InsertLocation(KeywordLocation* const newLocation) {
	if (lastLocation) {
		lastLocation->next = newLocation;
		newLocation->prev = lastLocation;
		lastLocation = newLocation;
		lastLocation->next = NULL;
		length++;

		return lastLocation;
	}
	else {
		lastLocation = firstLocation = newLocation;
		firstLocation->prev = NULL;
		lastLocation->next = NULL;
		length++;

		return lastLocation;
	}
}
