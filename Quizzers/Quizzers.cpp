// Quizzers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <random>
#include <string>
#include <random>
#include <ctime>

//DO NOT DELETE
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS


using namespace sf;
using namespace std;

//create Text & Font
Text text;
Font font;
Event event;

//Program states
bool mainMenu = true;
bool highScore = false;
bool questionDisplay = false;
bool resultsDisplay = false;

class Question {
public:
	bool askQuestion(RenderWindow& window);
	friend istream& operator >> (istream& is, Question& ques);

private:
	string question_text;
	string answer_1;
	string answer_2;
	string answer_3;
	string answer_4;
	char correct_answer;
};

void drawMainMenu(RenderWindow& window, vector<Question>& questions, vector<string>& results);
void drawText(String string, int size, float xPos, float yPos, RenderWindow& window);
void InititializeQuizGame(ifstream data, RenderWindow& window, vector<Question>& questions, vector<string>& results);
void loadQuestions(istream& is, vector<Question>& questions);
void Shuffle(vector<Question>& questions);
void drawQuestion(vector<Question>& questions, RenderWindow& window, vector<string>& results);
void drawResults(int total, RenderWindow& window, vector<Question>& questions, vector<string>& results);
void saveResult(ofstream out, RenderWindow& window, int score);
void readBoard(ifstream& is, RenderWindow& window, vector<Question>& questions, vector<string>& results);
void drawBoard(RenderWindow& window, vector<Question>& questions, vector<string>& results);

int main()
{
	//create Window
	RenderWindow window(VideoMode(500, 400), "Quizzers");
	vector<Question> questions;
	vector<string> results;

	//Loading Font
	font.loadFromFile("VertigoFLF.ttf");
	text.setFont(font);

	drawMainMenu(window, questions, results);

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
		}
	}
}

void drawText(String string, int size, float xPos, float yPos, RenderWindow& window) {
	text.setString(string);
	text.setCharacterSize(size);
	text.setPosition(xPos, yPos);
	text.setFillColor(Color::White);
	window.draw(text);
}

void drawMainMenu(RenderWindow& window, vector<Question>& questions, vector<string>& results) {
	mainMenu = true;

	while (mainMenu) {
		//Clearing the screen and printing the main menu
		window.clear();
		drawText("Quiz Time!", 70, 180, 40, window);
		drawText("Please select an option below;", 35, 130, 120, window);
		drawText("1. Play", 35, 140, 160, window);
		drawText("2. Scoreboard", 35, 143, 200, window);
		drawText("3. Exit", 35, 143, 240, window);
		window.display();

		// get keyboard input for user decision
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Num1) {
					InititializeQuizGame(ifstream("questions.txt"), window, questions, results); //Load questions from .txt file
				}
				if (event.key.code == Keyboard::Num2) {
					readBoard(ifstream("results.txt"), window, questions, results);
				}
				if (event.key.code == Keyboard::Num3) {
					window.close();
				}
			}
		}
	}
}

void InititializeQuizGame(ifstream data, RenderWindow& window, vector<Question>& questions, vector<string>& results)
{
	mainMenu = false;

		if (data.is_open())
		{
			loadQuestions(data, questions);
			Shuffle(questions);
			drawQuestion(questions, window, results);
		}
		else
		{
			drawText("Error loading questions!", 70, 180, 280, window);
		}
}

istream& operator >> (istream& is, Question& ques)
{
	string line;
	//while there is a line for the stream to read it will continue
	while (getline(is, line))
	{
		//if the line is empty, skip it
		if (line.size() == 0) 
			continue;
		break;
	}
	ques.question_text = line;
	getline(is, ques.answer_1);
	getline(is, ques.answer_2);
	getline(is, ques.answer_3);
	getline(is, ques.answer_4);
	is >> ques.correct_answer;
	return is;
}

void loadQuestions(istream& is, vector<Question>& questions)
{
	Question q;
	while (is >> q)
		questions.push_back(q);
}

void Shuffle(vector<Question>& questions) //Shuffle the questions. 
{
	random_device rd;
	mt19937 randomGenerator(rd());
	shuffle(questions.begin(), questions.end(), randomGenerator);
}

void drawQuestion(vector<Question>& questions, RenderWindow& window, vector<string>& results)
{
	int total = 0;

	for (int i = 0; i < 10; ++i)
	{
		//setting state
		questionDisplay = true;

		//While questionDisplay state is true, loops throught the vector asking questions
		while (questionDisplay) {
			if (questions[i].askQuestion(window)) {
				total += 1;
			}
		}
		if (i < 10) {
			questionDisplay = false;
			resultsDisplay = true;
		}
	}
	//call results screen
	drawResults(total, window, questions, results);
}

bool Question::askQuestion(RenderWindow& window) //Ask the question. 
{
	window.clear();

	//Printing question details and potential answers to the screen
	drawText(question_text, 40, 50, 40, window);

	drawText("A. ", 35, 50, 100, window);
	drawText(answer_1, 35, 70, 100, window);
	drawText("B. ", 35, 50, 140, window);
	drawText(answer_2, 35, 70, 140, window);
	drawText("C. ", 35, 50, 180, window);
	drawText(answer_3, 35, 70, 180, window);
	drawText("D. ", 35, 50, 220, window);
	drawText(answer_4, 35, 70, 220, window);
	window.display();

	while (window.pollEvent(event)) {
		if (event.type == Event::Closed) {
			window.close();
		}
		//Checking for KeyPresses 
		//Comparing them against the stored correct answer
		//Changing the game state
		//And returning whether or not the answer was correct
		if (event.type == Event::KeyPressed) {
			if (event.key.code == Keyboard::A) {
				if (correct_answer == 'A') {
					questionDisplay = false;
					//drawText("Correct!", 35, 70, 260, window);
					cout << "You guessed correctly\n";
					return true;
				}
				questionDisplay = false;
				cout << "You guessed incorrectly\n";
				return false;
			}
			else if (event.key.code == Keyboard::B) {
				if (correct_answer == 'B') {
					questionDisplay = false;
					cout << "You guessed correctly\n";
					return true;
				}
				questionDisplay = false;
				cout << "You guessed incorrectly\n";
				return false;
			}
			else if (event.key.code == Keyboard::C) {
				if (correct_answer == 'C') {
					questionDisplay = false;
					cout << "You guessed correctly\n";
					return true;
				}
				questionDisplay = false;
				cout << "You guessed incorrectly\n";
				return false;
			}
			else if (event.key.code == Keyboard::D) {
				if (correct_answer == 'D') {
					questionDisplay = false;
					cout << "You guessed correctly\n";
					return true;
				}
				cout << "You guessed incorrectly\n";
				questionDisplay = false;
				return false;
			}
		}
	}
}

void drawResults(int total, RenderWindow& window, vector<Question>& questions, vector<string>& results) {

	//Converting score to string for printing
	string score = to_string(total);

	saveResult(ofstream("results.txt", ios::app), window, total);

	//While in the results state they will stay on screen
	while (resultsDisplay) {
		window.clear();
		drawText("Quiz Complete!", 40, 50, 40, window);
		drawText("You guessed ", 35, 50, 100, window);
		drawText(score, 35, 150, 100, window);
		drawText(" questions correctly.", 35, 160, 100, window);
		drawText("1. Play Again", 35, 50, 150, window);
		drawText("2. Scoreboard", 35, 53, 190, window);
		drawText("3. Exit", 35, 53, 230, window);

		window.display();

		// get keyboard input for user decision
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Num1) {
					Shuffle(questions);
					window.clear();
					drawQuestion(questions, window, results);
				}
				if (event.key.code == Keyboard::Num2) {
					readBoard(ifstream("results.txt"), window, questions, results);
				}
				if (event.key.code == Keyboard::Num3) {
					window.close();
				}
			}
		}
	}
}

void saveResult(ofstream out, RenderWindow& window, int score) {
	
	//current date/time based on current system
	time_t now = time(0);

	//converted to string form
	char* dt = ctime(&now);
	
	//if file is open print score , - , date & time
	if (out.is_open())
	{
		out << score << " - " << dt;
	}
	else
	{
		drawText("Error loading questions!", 35, 180, 280, window);
	}
	out.close();
}

void readBoard(ifstream& data, RenderWindow& window, vector<Question>& questions, vector<string>& results) {
	string line;
	//Pushing the contents of the text file onto a vector
	while (getline(data, line)) {
		results.push_back(line);
	}
	//sorting the vector and then calling the draw function
	sort(results.begin(), results.end());
	drawBoard(window, questions, results);
}

void drawBoard(RenderWindow& window, vector<Question>& questions, vector<string>& results) {
	//changing game states
	highScore = true;
	mainMenu = false;
	
	while (highScore) {
		//Clearing the screen and printing the main menu
		window.clear();
		drawText("Highscores", 70, 180, 40, window);
		drawText("1. ", 35, 120, 120, window);
		drawText(results[0], 35, 150, 120, window);
		drawText("2. ", 35, 120, 160, window);
		drawText(results[1], 35, 150, 160, window);
		drawText("3. ", 35, 120, 200, window);
		drawText(results[2], 35, 150, 200, window);
		drawText("4. ", 35, 120, 240, window);
		drawText(results[3], 35, 150, 240, window);
		drawText("5. ", 35, 121, 280, window);
		drawText(results[4], 35, 150, 280, window);
		drawText("Press 1 to return to Main Menu", 35, 130, 320, window);
		window.display();

		//Listening for events to close window or return to main menu
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Num1) {
					mainMenu = true;
					drawMainMenu(window, questions, results);
				}
			}
		}
	}
}