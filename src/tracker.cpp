#include <vector>
#include <cstdlib>
#include "tracker.h"

using namespace std;

typedef list<Blob>::iterator lBi;
typedef list<Blob>::const_iterator lBci;

void Tracker::receive(list<Blob> in)
{
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
			moIt->update(*bestMatch);
			bestMatch->associated = true;
		} else {
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
