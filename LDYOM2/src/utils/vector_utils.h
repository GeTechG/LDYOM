#pragma once
#include <vector>

namespace utils {

/**
 * Moves an element from one position to another within a vector.
 *
 * @tparam T The type of elements in the vector
 * @param vec The vector to modify
 * @param fromIndex The source index of the element to move
 * @param toIndex The destination index where the element should be placed
 * @return true if the move was successful, false otherwise
 */
template <typename T> bool moveInVector(std::vector<T>& vec, int fromIndex, int toIndex) {
	// Validate indices
	if (fromIndex < 0 || fromIndex >= static_cast<int>(vec.size()) || toIndex < 0 ||
	    toIndex >= static_cast<int>(vec.size()) + 1) {
		return false;
	}

	// No need to move if source and destination are the same
	if (fromIndex == toIndex) {
		return true;
	}

	// Store the element to move
	T movingElement = std::move(vec[fromIndex]);

	// Remove the element from its original position
	vec.erase(vec.begin() + fromIndex);

	// If toIndex was after fromIndex, it needs to be adjusted
	// because after erasing an element, indices after it shift down
	if (toIndex > fromIndex) {
		toIndex--;
	}

	// Insert the element at the new position
	if (toIndex == vec.size()) {
		vec.emplace_back(std::move(movingElement));
	} else {
		vec.insert(vec.begin() + toIndex, std::move(movingElement));
	}

	return true;
}

} // namespace utils