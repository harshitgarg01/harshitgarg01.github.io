#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <Windows.h>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <atomic>
#include "MethodParameters.h"
#include "Method1.h"
#include "Method2.h"
#include "Method3.h"
#include "Method4.h"
#include "Method5.h"
#include "Method6.h"
#include "TimeStorage.h"

using namespace std;

vector <string> identifierStorage; //global variable to store input
vector <chrono::time_point<struct std::chrono::steady_clock, class std::chrono::duration<__int64, struct std::ratio<1, 1000000000> > >> identifierEntryTime; // time at which identifier is inputed
vector<TimeStorage> metrics1; //used to store the necessary metrics stored in csv  where metric1 stores for method1 and so on
vector<TimeStorage> metrics2;
vector<TimeStorage> metrics3;
vector<TimeStorage> metrics4;
vector<TimeStorage> metrics5;
vector<TimeStorage> metrics6;
atomic <bool> inputEnd; //global variable to indicate end of input intake
atomic <int> totalInputTaken;// global variable to indicate total inputs taken
atomic <int> inputBegin;//global variable to indicate beginning of identifier input
atomic <int> identifierCompletedCount;
vector <int> degradedTime;
vector <int> degradedMethodNumber;
int* testingSeed = new int[1800]; //only used to provide same seeds for comparison among models not needs to be part of the actual program. Also holds for testingSeed.
int degradedMaximum;//maximum degraded time provided in DegradedMode.txt
mutex controlOutput; // to synchronize output call
mutex controlMethod1; // to synch the calling of methods and metrics.
mutex controlMethod2;
mutex controlMethod3;
mutex controlMethod4;
mutex controlMethod5;
mutex controlMethod6;
mutex controlIdentifierStorage;
mutex controlIdentifierEntryTime;
mutex controlThreadClosure;
mutex controlAvailableThread;


template <typename T>
void retryLogicImplementation(MethodParameters* instance, int numberInputCompleted, int index, T worker)
{

	int flag = 0, retryCount = 0, expoCount = 0;
	long long int delayTime, delayTimeCutOff;
	chrono::duration<double> timeTillNow;
	bool result = false;
	controlIdentifierStorage.lock();
	string identifier = identifierStorage[numberInputCompleted];
	controlIdentifierStorage.unlock();
	while (result != true)
	{

		if (flag != 0)
		{
			delayTime = (rand() % 10); 
			delayTime += pow(2, expoCount) * 10; //exponential backoff
			controlIdentifierEntryTime.lock();
			timeTillNow = chrono::high_resolution_clock::now() - identifierEntryTime[numberInputCompleted];
			controlIdentifierEntryTime.unlock();
			delayTimeCutOff = ((long long int)(timeTillNow.count() * 1000000)) % (degradedMaximum * 1000000);
			if (delayTimeCutOff + delayTime > degradedMaximum * 1000000.0) //to prevent exponential delaytime from getting too high
			{
				delayTime = degradedMaximum * 1000000.0 - delayTimeCutOff;
				expoCount = -1;
			}
			this_thread::sleep_for(chrono::microseconds(delayTime));
			expoCount++;
		}
		while (instance->getRetryLimit(index) == 2)
		{
			continue;
		}
		flag++;
		retryCount++;
		if (index == 0)
		{
			controlMethod1.lock();
			metrics1[numberInputCompleted].attemptsStartingTime.push_back(chrono::high_resolution_clock::now());
			result = worker.modeGuesser(identifier, instance);
			metrics1[numberInputCompleted].attemptsEndingTime.push_back(chrono::high_resolution_clock::now());
			controlMethod1.unlock();
			if (result == true)
			{
				controlMethod1.lock();
				metrics1[numberInputCompleted].identifierCompletionTime = chrono::high_resolution_clock::now();
				metrics1[numberInputCompleted].attempts = retryCount;
				controlMethod1.unlock();
			}
		}
		else if (index == 1)
		{
			controlMethod2.lock();
			metrics2[numberInputCompleted].attemptsStartingTime.push_back(chrono::high_resolution_clock::now());
			result = worker.modeGuesser(identifier, instance);
			metrics2[numberInputCompleted].attemptsEndingTime.push_back(chrono::high_resolution_clock::now());
			controlMethod2.unlock();
			if (result == true)
			{
				controlMethod2.lock();
				metrics2[numberInputCompleted].identifierCompletionTime = chrono::high_resolution_clock::now();
				metrics2[numberInputCompleted].attempts = retryCount;
				controlMethod2.unlock();
			}
		}
		else if (index == 2)
		{
			controlMethod3.lock();
			metrics3[numberInputCompleted].attemptsStartingTime.push_back(chrono::high_resolution_clock::now());
			result = worker.modeGuesser(identifier, instance);
			metrics3[numberInputCompleted].attemptsEndingTime.push_back(chrono::high_resolution_clock::now());
			controlMethod3.unlock();
			if (result == true)
			{
				controlMethod3.lock();
				metrics3[numberInputCompleted].identifierCompletionTime = chrono::high_resolution_clock::now();
				metrics3[numberInputCompleted].attempts = retryCount;
				controlMethod3.unlock();
			}
		}
		else if (index == 3)
		{
			controlMethod4.lock();
			metrics4[numberInputCompleted].attemptsStartingTime.push_back(chrono::high_resolution_clock::now());
			result = worker.modeGuesser(identifier, instance);
			metrics4[numberInputCompleted].attemptsEndingTime.push_back(chrono::high_resolution_clock::now());
			controlMethod4.unlock();
			if (result == true)
			{
				controlMethod4.lock();
				metrics4[numberInputCompleted].identifierCompletionTime = chrono::high_resolution_clock::now();
				metrics4[numberInputCompleted].attempts = retryCount;
				controlMethod4.unlock();
			}
		}
		else if (index == 4)
		{
			controlMethod5.lock();
			metrics5[numberInputCompleted].attemptsStartingTime.push_back(chrono::high_resolution_clock::now());
			result = worker.modeGuesser(identifier, instance);
			metrics5[numberInputCompleted].attemptsEndingTime.push_back(chrono::high_resolution_clock::now());
			controlMethod5.unlock();
			if (result == true)
			{
				controlMethod5.lock();
				metrics5[numberInputCompleted].identifierCompletionTime = chrono::high_resolution_clock::now();
				metrics5[numberInputCompleted].attempts = retryCount;
				controlMethod5.unlock();
			}
		}
		else
		{
			controlMethod6.lock();
			metrics6[numberInputCompleted].attemptsStartingTime.push_back(chrono::high_resolution_clock::now());
			result = worker.modeGuesser(identifier, instance);
			metrics6[numberInputCompleted].attemptsEndingTime.push_back(chrono::high_resolution_clock::now());
			controlMethod6.unlock();
			if (result == true)
			{
				controlMethod6.lock();
				metrics6[numberInputCompleted].identifierCompletionTime = chrono::high_resolution_clock::now();
				metrics6[numberInputCompleted].attempts = retryCount;
				controlMethod6.unlock();
			}
		}
	}
	controlOutput.lock();
	identifierCompletedCount++;
	cout << "Did the program gave the result: " << result << " -> " << identifierCompletedCount << endl; //whether the program was able to sufficiently apply retry logic within 11 retries 
	cout << "Amount of retries required for \"" << identifier << "\" on the Method" << index + 1 << " are " << retryCount << endl;
	controlOutput.unlock();
	return;
}


void allMethodExecutor(MethodParameters* instance, int numberInputCompleted, int* availableThread, int* threadClosure, int threadNumber)
{
	Method1 worker1;
	Method2 worker2;
	Method3 worker3;
	Method4 worker4;
	Method5 worker5;
	Method6 worker6;
	retryLogicImplementation<Method1>(instance, numberInputCompleted, 0, worker1); // this is where we have the dependecy of the program and we can interchange methods according to the need of application
	retryLogicImplementation<Method2>(instance, numberInputCompleted, 1, worker2);
	retryLogicImplementation<Method3>(instance, numberInputCompleted, 2, worker3);
	retryLogicImplementation<Method4>(instance, numberInputCompleted, 3, worker4);
	retryLogicImplementation<Method5>(instance, numberInputCompleted, 4, worker5);
	retryLogicImplementation<Method6>(instance, numberInputCompleted, 5, worker6);
	controlThreadClosure.lock();
	threadClosure[threadNumber] = 1;
	controlThreadClosure.unlock();
	controlAvailableThread.lock();
	availableThread[threadNumber] = 1;
	controlAvailableThread.unlock();
	return;
}

void randomSeedSetUp(MethodParameters* instance, int numberInputCompleted, int* availableThread, int* threadClosure, int threadNumber)
{
	/*
	FILETIME fileTime; //This is used to provide unique seed to thread for randomness
	GetSystemTimeAsFileTime(&fileTime);
	ULARGE_INTEGER theTime;
	theTime.LowPart = fileTime.dwLowDateTime;
	theTime.HighPart = fileTime.dwHighDateTime;
	__int64 fileTime64Bit = theTime.QuadPart;
	srand((unsigned)fileTime64Bit); //used to get different seeds for each method for randomness
	//cout << (unsigned)fileTime64Bit << endl; //hence needs to be different for all methods
	allMethodExecutor(instance,numberInputCompleted, availableThread, threadClosure, threadNumber);
	return;
	*/
	srand(testingSeed[numberInputCompleted]); // this is only to get same seeds for all model for comparison
	allMethodExecutor(instance, numberInputCompleted, availableThread, threadClosure, threadNumber);
	return;
}

void setDegradationTime(MethodParameters* instance, int degradedPeriod, int index)
{
	while (inputBegin != 1)
	{
		continue;
	}
	instance->setMode(index, 1);
	int delay = (rand() * (index + 1)) % 1800;
	cout << index << "->" << delay << endl;
	this_thread::sleep_for(chrono::seconds(delay));
	instance->setMode(index, 2);
	this_thread::sleep_for(chrono::seconds(degradedPeriod));
	instance->setMode(index, 1);
	return;
}


void retryLimitingHandler(MethodParameters* instance, int index)
{
	while (instance->getCurrentState(index) == 0)
	{
		this_thread::sleep_for(chrono::seconds(1));
		instance->setRetryLimit(index, 0);
	}
	return;
}

void identifierDistributor()
{
	int threadNumber = 0, numberInputCompleted = 0, loopItem, breakLoop = 0, threadSize;
	int* threadClosure = new int[10];
	int* availableThread = new int[10];
	MethodParameters* instance = new MethodParameters;
	thread retryLimitingThread[6];
	for (loopItem = 0; loopItem < 6; loopItem++)
	{
		retryLimitingThread[loopItem] = thread(retryLimitingHandler, instance, loopItem);
	}
	thread arrayOfThreads[10];
	for (loopItem = 0; loopItem < 10; loopItem++)
	{
		threadClosure[loopItem] = 0;
		availableThread[loopItem] = 1;
	}
	srand(time(0));
	threadSize = degradedTime.size();
	thread degradedModeSetup[6];
	for (loopItem = 0; loopItem < threadSize; loopItem++)
	{
		degradedModeSetup[loopItem] = thread(setDegradationTime, instance, degradedTime[loopItem], degradedMethodNumber[loopItem]);
	}
	while (inputBegin != 1)
	{
		this_thread::sleep_for(chrono::seconds(1));
	}
	while (!inputEnd || !(totalInputTaken == numberInputCompleted))
	{
		controlIdentifierStorage.lock();
		if (numberInputCompleted + 1 <= (int)identifierStorage.size())
		{
			controlIdentifierStorage.unlock();
			controlAvailableThread.lock();
			availableThread[threadNumber] = 0;
			controlAvailableThread.unlock();
			arrayOfThreads[threadNumber] = thread(randomSeedSetUp, instance, numberInputCompleted, availableThread, threadClosure, threadNumber);
			numberInputCompleted++;
			while (true)
			{
				breakLoop = 0;
				for (loopItem = 0; loopItem < 10; loopItem++)
				{
					controlAvailableThread.lock();
					if (availableThread[loopItem] == 1)
					{
						controlAvailableThread.unlock();
						threadNumber = loopItem;
						breakLoop = 1;
						break;
					}
					controlAvailableThread.unlock();
				}
				if (breakLoop == 1)
				{
					break;
				}
			}
			controlThreadClosure.lock();
			if (threadClosure[threadNumber] != 0)
			{
				arrayOfThreads[threadNumber].join();
				threadClosure[threadNumber] = 0;
				controlThreadClosure.unlock();
			}
			else
			{
				controlThreadClosure.unlock();
			}
		}
		else
		{
			while (numberInputCompleted + 1 > (int)identifierStorage.size())
			{
				controlIdentifierStorage.unlock();
				if (inputEnd)
				{
					controlIdentifierStorage.lock();
					break;
				}
				this_thread::sleep_for(chrono::milliseconds(100));
				controlIdentifierStorage.lock();
			}
			controlIdentifierStorage.unlock();
		}
	}
	for (loopItem = 0; loopItem < 10; loopItem++)
	{
		controlAvailableThread.lock();
		controlThreadClosure.lock();
		if (threadClosure[loopItem] != 0 || availableThread[loopItem] == 0)
		{
			controlThreadClosure.unlock();
			controlAvailableThread.unlock();
			arrayOfThreads[loopItem].join();
		}
		else
		{
			controlThreadClosure.unlock();
			controlAvailableThread.unlock();
		}
	}
	for (loopItem = 0; loopItem < threadSize; loopItem++)
	{
		degradedModeSetup[loopItem].join();
	}
	for (loopItem = 0; loopItem < 6; loopItem++)
	{
		instance->changeState(loopItem);
		retryLimitingThread[loopItem].join();
	}
	return;
}

int stringToInt(string degradedSec, int size)
{
	int num = 0, loopItem;
	for (loopItem = 0; loopItem < size; loopItem++)
	{
		num = num * 10 + (degradedSec[loopItem] - '0');
	}
	return num;
}

int main()
{
	int countInput = 0, flag = 0, loopItem, innerLoop1, innerLoop2, degradedInt;
	string inputFile, degradedSec, identifier;
	char methodNumber;
	vector <string> identifierCollector;
	inputBegin = 0;
	totalInputTaken = 0;
	identifierCompletedCount = 0;
	inputEnd = false;
	MethodParameters* instance = new MethodParameters();
	ifstream fileInstance;
	fileInstance.open("DegradedMode.txt");
	if (fileInstance.is_open())
	{
		while (getline(fileInstance, inputFile))
		{
			flag = 0;
			degradedSec = "";
			for (loopItem = 0; loopItem < inputFile.length(); loopItem++)
			{
				if (flag == 2 && (inputFile[loopItem] > 57 || inputFile[loopItem] < 48))
				{
					break;
				}
				else if (flag == 2)
				{
					degradedSec = degradedSec + inputFile[loopItem];
				}
				else if (flag == 1 && inputFile[loopItem] < 58 && inputFile[loopItem] >47)
				{
					flag = 2;
					degradedSec = degradedSec + inputFile[loopItem];
				}
				else if (flag == 0 && inputFile[loopItem] < 58 && inputFile[loopItem] >47)
				{
					flag = 1;
					methodNumber = inputFile[loopItem];
				}
				else
				{
					continue;
				}
			}
			degradedInt = stringToInt(degradedSec, degradedSec.length());
			degradedMaximum = max(degradedMaximum, degradedInt);
			cout << degradedSec << " -> " << methodNumber << endl;
			degradedTime.push_back(degradedInt);
			degradedMethodNumber.push_back(methodNumber - '1');
		}
		fileInstance.close();
	}
	else
	{

		cout << "Unable to open the degradation setup file";
	}
	srand(time(0));
	fileInstance.open("InputFile.txt");
	getline(fileInstance, inputFile);
	for (loopItem = 0; loopItem < inputFile.length(); loopItem++)
	{
		identifier = "";
		while (inputFile[loopItem] != ' ' && loopItem != inputFile.length())
		{
			identifier = identifier + inputFile[loopItem];
			loopItem++;
		}
		identifierCollector.push_back(identifier);
		cout << identifier << " ";
	}
	for (loopItem = 0; loopItem < 1800; loopItem++)
	{
		testingSeed[loopItem] = loopItem;
	}
	cout << endl;
	thread identifierDistributorImplementation(identifierDistributor);
	flag = 0;
	while (countInput != identifierCollector.size())
	{
		controlMethod1.lock();
		metrics1.push_back(*new TimeStorage);
		controlMethod1.unlock();
		controlMethod2.lock();
		metrics2.push_back(*new TimeStorage);
		controlMethod2.unlock();
		controlMethod3.lock();
		metrics3.push_back(*new TimeStorage);
		controlMethod3.unlock();
		controlMethod4.lock();
		metrics4.push_back(*new TimeStorage);
		controlMethod4.unlock();
		controlMethod5.lock();
		metrics5.push_back(*new TimeStorage);
		controlMethod5.unlock();
		controlMethod6.lock();
		metrics6.push_back(*new TimeStorage);
		controlMethod6.unlock();
		controlIdentifierEntryTime.lock();
		identifierEntryTime.push_back(chrono::high_resolution_clock::now());
		controlIdentifierEntryTime.unlock();
		controlIdentifierStorage.lock();
		identifierStorage.push_back(identifierCollector[countInput]);
		controlIdentifierStorage.unlock();
		countInput++;
		if (flag == 0)
		{
			inputBegin = 1;
			flag = 1;
		}
		this_thread::sleep_for(chrono::microseconds(989850)); // to get a unique identifier input after every one second
	}
	totalInputTaken = countInput;
	inputEnd = true;
	identifierDistributorImplementation.join();
	auto endTime = chrono::high_resolution_clock::now();
	chrono::duration<double> finalDuration = endTime - identifierEntryTime[0];
	cout << "Total time taken by the application is " << finalDuration.count() << " seconds" << endl;
	cout << identifierCompletedCount << endl;
	ofstream outputCSV;
	outputCSV.open("retryStats.csv");
	outputCSV << "identifier,retries,identifier time,retrytime" << endl;
	for (innerLoop1 = 0; innerLoop1 < totalInputTaken; innerLoop1++)
	{
		chrono::duration<double> timeDiff = metrics1[innerLoop1].identifierCompletionTime - identifierEntryTime[innerLoop1];
		outputCSV << identifierStorage[innerLoop1] << "," << metrics1[innerLoop1].attempts << "," << timeDiff.count() << ",";
		int vectorSize = metrics1[innerLoop1].attemptsStartingTime.size();
		for (innerLoop2 = 0; innerLoop2 < vectorSize; innerLoop2++)
		{
			timeDiff = metrics1[innerLoop1].attemptsStartingTime[innerLoop2] - identifierEntryTime[innerLoop1];
			outputCSV << timeDiff.count() << ",";
			timeDiff = metrics1[innerLoop1].attemptsEndingTime[innerLoop2] - metrics1[innerLoop1].attemptsStartingTime[innerLoop2];
			outputCSV << timeDiff.count() << ",";
		}
		outputCSV << endl;
	}
	outputCSV << endl;
	outputCSV << "identifier,retries,identifier time,retrytime" << endl;
	for (innerLoop1 = 0; innerLoop1 < totalInputTaken; innerLoop1++)
	{
		chrono::duration<double> timeDiff = metrics2[innerLoop1].identifierCompletionTime - identifierEntryTime[innerLoop1];
		outputCSV << identifierStorage[innerLoop1] << "," << metrics2[innerLoop1].attempts << "," << timeDiff.count() << ",";
		int vectorSize = metrics2[innerLoop1].attemptsStartingTime.size();
		for (innerLoop2 = 0; innerLoop2 < vectorSize; innerLoop2++)
		{
			timeDiff = metrics2[innerLoop1].attemptsStartingTime[innerLoop2] - identifierEntryTime[innerLoop1];
			outputCSV << timeDiff.count() << ",";
			timeDiff = metrics2[innerLoop1].attemptsEndingTime[innerLoop2] - metrics2[innerLoop1].attemptsStartingTime[innerLoop2];
			outputCSV << timeDiff.count() << ",";
		}
		outputCSV << endl;
	}
	outputCSV << endl;
	outputCSV << "identifier,retries,identifier time,retrytime" << endl;
	for (innerLoop1 = 0; innerLoop1 < totalInputTaken; innerLoop1++)
	{
		chrono::duration<double> timeDiff = metrics3[innerLoop1].identifierCompletionTime - identifierEntryTime[innerLoop1];
		outputCSV << identifierStorage[innerLoop1] << "," << metrics3[innerLoop1].attempts << "," << timeDiff.count() << ",";
		int vectorSize = metrics3[innerLoop1].attemptsStartingTime.size();
		for (innerLoop2 = 0; innerLoop2 < vectorSize; innerLoop2++)
		{
			timeDiff = metrics3[innerLoop1].attemptsStartingTime[innerLoop2] - identifierEntryTime[innerLoop1];
			outputCSV << timeDiff.count() << ",";
			timeDiff = metrics3[innerLoop1].attemptsEndingTime[innerLoop2] - metrics3[innerLoop1].attemptsStartingTime[innerLoop2];
			outputCSV << timeDiff.count() << ",";
		}
		outputCSV << endl;
	}
	outputCSV << endl;
	outputCSV << "identifier,retries,identifier time,retrytime" << endl;
	for (innerLoop1 = 0; innerLoop1 < totalInputTaken; innerLoop1++)
	{
		chrono::duration<double> timeDiff = metrics4[innerLoop1].identifierCompletionTime - identifierEntryTime[innerLoop1];
		outputCSV << identifierStorage[innerLoop1] << "," << metrics4[innerLoop1].attempts << "," << timeDiff.count() << ",";
		int vectorSize = metrics4[innerLoop1].attemptsStartingTime.size();
		for (innerLoop2 = 0; innerLoop2 < vectorSize; innerLoop2++)
		{
			timeDiff = metrics4[innerLoop1].attemptsStartingTime[innerLoop2] - identifierEntryTime[innerLoop1];
			outputCSV << timeDiff.count() << ",";
			timeDiff = metrics4[innerLoop1].attemptsEndingTime[innerLoop2] - metrics4[innerLoop1].attemptsStartingTime[innerLoop2];
			outputCSV << timeDiff.count() << ",";
		}
		outputCSV << endl;
	}
	outputCSV << endl;
	outputCSV << "identifier,retries,identifier time,retrytime" << endl;
	for (innerLoop1 = 0; innerLoop1 < totalInputTaken; innerLoop1++)
	{
		chrono::duration<double> timeDiff = metrics5[innerLoop1].identifierCompletionTime - identifierEntryTime[innerLoop1];
		outputCSV << identifierStorage[innerLoop1] << "," << metrics5[innerLoop1].attempts << "," << timeDiff.count() << ",";
		int vectorSize = metrics5[innerLoop1].attemptsStartingTime.size();
		for (innerLoop2 = 0; innerLoop2 < vectorSize; innerLoop2++)
		{
			timeDiff = metrics5[innerLoop1].attemptsStartingTime[innerLoop2] - identifierEntryTime[innerLoop1];
			outputCSV << timeDiff.count() << ",";
			timeDiff = metrics5[innerLoop1].attemptsEndingTime[innerLoop2] - metrics5[innerLoop1].attemptsStartingTime[innerLoop2];
			outputCSV << timeDiff.count() << ",";
		}
		outputCSV << endl;
	}
	outputCSV << endl;
	outputCSV << "identifier,retries,identifier time,retrytime" << endl;
	for (innerLoop1 = 0; innerLoop1 < totalInputTaken; innerLoop1++)
	{
		chrono::duration<double> timeDiff = metrics6[innerLoop1].identifierCompletionTime - identifierEntryTime[innerLoop1];
		outputCSV << identifierStorage[innerLoop1] << "," << metrics6[innerLoop1].attempts << "," << timeDiff.count() << ",";
		int vectorSize = metrics6[innerLoop1].attemptsStartingTime.size();
		for (innerLoop2 = 0; innerLoop2 < vectorSize; innerLoop2++)
		{
			timeDiff = metrics6[innerLoop1].attemptsStartingTime[innerLoop2] - identifierEntryTime[innerLoop1];
			outputCSV << timeDiff.count() << ",";
			timeDiff = metrics6[innerLoop1].attemptsEndingTime[innerLoop2] - metrics6[innerLoop1].attemptsStartingTime[innerLoop2];
			outputCSV << timeDiff.count() << ",";
		}
		outputCSV << endl;
	}
	return 0;
}
