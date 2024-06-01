#include <iostream>
#include <list>
#include "Pong.h"

void startingField(PlayField& field) {
	for (int i = 0; i < field.rows; i++) {
		for (int j = 0; j < field.cols; j++) {
			std::cout << field.location[i][j];
		}
		std::cout << '\n';
	}

}

template<typename T>
int findInList(std::list<T> &targetList, T target) {
	int index = -1;
	for (auto el : targetList) {
		index++;
		if (el == target) {
			return index;
		}
	}
	return index;
}

void PlayTwo(PlayField& field) {
	srand(time(0));
	
	setCursorAtPosition(0, 0);

	startingField(field);
	bool gameIsOver = false;

	if (rand() % 2 < 1) {
		field.velocity.x = 1;
	}
	else {
		field.velocity.x = -1;
	}

	std::list<char> pressedKeys;
	while (true) {
		if (field.MoveBall()) {
			gameIsOver = true;
			break;
		}

		// if you remove these comments in the if you will move only when you click for the first time
		// basically you won't be able to hold the button to move
		if ((GetKeyState('W') & 0x8000)/* && findInList(pressedKeys, 'W') == -1*/) {
			if (field.barrierOneStart > 1) {
				setCursorAtPosition(0, (field.barrierOneStart + field.barrierSize - 1));
				std::cout << ' ';
				field.barrierOneStart--;
				setCursorAtPosition(0, field.barrierOneStart);
				std::cout << '|';
				pressedKeys.push_back('W');
			}
		}
		else if ((GetKeyState('W') & 0x8000) == 0) {
			int ind = findInList(pressedKeys, 'W');
			if (ind != -1) {
				pressedKeys.remove('W');
			}
		}

		if ((GetKeyState('S') & 0x8000)/* && findInList(pressedKeys, 'S') == -1*/) {
			if (field.barrierOneStart + field.barrierSize < field.rows - 1) {
				setCursorAtPosition(0, (field.barrierOneStart));
				std::cout << ' ';
				field.barrierOneStart++;
				setCursorAtPosition(0, field.barrierOneStart + field.barrierSize - 1);
				std::cout << '|';
				pressedKeys.push_back('S');
			}
		}
		else if ((GetKeyState('S') & 0x8000) == 0) {
			int ind = findInList(pressedKeys, 'S');
			if (ind != -1) {
				pressedKeys.remove('S');
			}
		}

		if ((GetKeyState(VK_UP) & 0x8000)/* && findInList(pressedKeys, 'u') == -1*/) {
			if (field.barrierTwoStart > 1) {
				setCursorAtPosition(field.cols - 1, (field.barrierTwoStart + field.barrierSize - 1));
				std::cout << ' ';
				field.barrierTwoStart--;
				setCursorAtPosition(field.cols - 1, field.barrierTwoStart);
				std::cout << '|';
				pressedKeys.push_back('u');
			}
		}
		else if ((GetKeyState(VK_UP) & 0x8000) == 0) {
			int ind = findInList(pressedKeys, 'u');
			if (ind != -1) {
				pressedKeys.remove('u');
			}
		}

		if ((GetKeyState(VK_DOWN) & 0x8000)/* && findInList(pressedKeys, 'd') == -1*/) {
			if (field.barrierTwoStart + field.barrierSize < field.rows - 1) {
				setCursorAtPosition(field.cols - 1, (field.barrierTwoStart));
				std::cout << ' ';
				field.barrierTwoStart++;
				setCursorAtPosition(field.cols - 1, field.barrierTwoStart + field.barrierSize - 1);
				std::cout << '|';
				pressedKeys.push_back('d');
			}
		}
		else if ((GetKeyState(VK_DOWN) & 0x8000) == 0) {
			int ind = findInList(pressedKeys, 'd');
			if (ind != -1) {
				pressedKeys.remove('d');
			}
		}

		Sleep(20);
	}

	// this is here to give you some time to react to the game ending so you don't accidentally change the code or something after the program terminates
	Sleep(2000);
}