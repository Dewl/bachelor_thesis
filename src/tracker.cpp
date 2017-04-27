#include "tracker.h"
#include <cstdlib>

using namespace std;

Tracker::Tracker(int _thres)
{
	this.thres = _thes;
}

void Tracker::receive(const list<Blob> blobs)
{
	int size = this.blobList.size();
	int inputSize = blobs.size();
	int minDist[size];
	Blob* match[size];

	for (int i = 0; i < size; ++i) {
		minDist[i] = this.thres;
		match[i] = NULL;
	}

	list<Blob>::iterator inputIt = blobs.begin();

	while (inputIt != blobs.end()) {
		list<Blob>::iterator it = this.blobList.begin();
		int index = 0;
	
		Blob &curInputBlob = *inputIt;

		while (it != this.blobList.end()) {
			Blob &curBlob = *it;
			int distance = 
			++it;
			++index;
			// Iterate module's blob list
		}	
		++inputIt;
		// Iterate input's blob list
	}
}
