#include <vector>
#include <cstdlib>
#include "tracker.h"

using namespace std;

void Tracker::receive(list<Blob> in)
{
	expiredBlobs.clear();
	for (lBi moIt = blobs.begin(); moIt != blobs.end(); ++moIt) {
		double minDif = thres;
		Blob* bestMatch = NULL;
		for (lBi inIt = in.begin(); inIt != in.end(); ++inIt) {
			if (inIt->associated)
				continue;

			double dif = moIt->dif(*inIt);
			if (dif < minDif) {
				minDif = dif;
				bestMatch = &*inIt;
			}
			// For each input blob
		}

		if (bestMatch) {
			moIt->associate(*bestMatch);
			bestMatch->associated = true;
		} else {
			expiredBlobs.push_back(*moIt);
			blobs.erase(moIt++);
		}
		// For each module blob
	}

	for (lBci inIt = in.cbegin(); inIt != in.cend(); ++inIt) {
		if (!inIt->associated) {
			blobs.push_back(*inIt);
		}
	}
}
