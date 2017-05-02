#include <vector>
#include <cstdlib>
#include "tracker.h"

using namespace std;

typedef list<Blob>::iterator lBi;
typedef list<Blob>::const_iterator lBci;

void Tracker::receive(const list<Blob> inputBlobs)
{
	vector<double> minDist(blobs.size());
	vector<const Blob*> matchedBlob(blobs.size());
	list<Blob> unassociated;

	for (unsigned int i = 0; i < blobs.size(); ++i) {
		minDist[i] = thres;
		matchedBlob[i] = NULL;
	}

	cout << "debug:tracker:input_size:" << inputBlobs.size() << endl;
	cout << "debug:tracker:module_size:" << blobs.size() << endl;

	for (lBci inIt = inputBlobs.begin(); inIt != inputBlobs.end(); ++inIt) {
		int index = 0;
		bool associated = false;
		for (lBci it = blobs.begin(); it != blobs.end(); ++it) {
			double dist = it->distance(*inIt);
			if (dist < minDist[index]) {
				minDist[index] = dist;
				matchedBlob[index] = &*inIt;
				associated = true;
			}
			// For each blob in module
		}

		if (!associated) {
			cout << "debug:tracker:unassociated:" << index << endl;
			unassociated.push_back(*inIt);
		}
		++index;
		// For each blob in input
	}

	unsigned int index = 0;
	for (lBi it = blobs.begin(); it != blobs.end(); ++it) {
		if (index >= blobs.size()) {
			break;
		}

		if (!matchedBlob[index]) {
			blobs.erase(it++);
		} else {
			Blob* cur = &*it;
			cur->update(*matchedBlob[index]);
			++it;
		}

		index += 1;
	}

	for (lBi it = unassociated.begin(); it != unassociated.end(); ++it) {
		blobs.push_back(*it);
	}
}
