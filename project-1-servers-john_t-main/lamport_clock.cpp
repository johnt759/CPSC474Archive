#include<iostream>
#include<iomanip>
#include<fstream> // Needed to store results in a text file
#include<string>
#include<Windows.h> // Needed for sleep function
using namespace std;

// For n processors, it can be at most 5.
// For m events, there can be a maximum of 25 events per processor.
// Be sure to verify these input values for Calculate and Verify.
void Calculate(int n, int m)
{
	if ((n < 1 || n > 5) || (m < 1 || m > 25))
	{
		cout << "ERROR. Invalid number of processors and events.\n";
		cout << "Number of processors must be at least 1 and at most 5,\n";
		cout << "and there can be a maximum of 25 events for each processor.\n";
		return;
	}

	// For Calculate, calculate the LC-value for each of all events
	// and return the output as positive integer values.

	// Declare two arrays: an int array that contains the values as event results
	// and a string array that stores the result.
	string LCEvent;
	string** inputEvents = new string* [n];
	for (int x = 0; x < n; x++)
	{
		inputEvents[x] = new string[m];
	}

	int** outputResults = new int * [n];
	for (int y = 0; y < n; y++)
	{
		outputResults[y] = new int[m];
	}

	// Initialize inputEvents to NULL.
	for (int x = 0; x < n; x++)
	{
		for (int y = 0; y < m; y++)
		{
			inputEvents[x][y] = "NULL";
		}
	}

	// Ask the user to enter the events for each processor.
	// Then store each input event into the array.
	// If user enters NULL, then move on to the next processor.
	cout << "Enter the events for each processor.\n";
	cout << "Enter NULL to skip to the next processor.\n";
	for (int x = 0; x < n; x++)
	{
		cout << "Processor " << x << ":\n";
		for (int y = 0; y < m; y++)
		{
			cout << "Event " << y+1 << ": ";
			cin >> LCEvent;
			if (LCEvent == "NULL")
			{
				break;
			}
			inputEvents[x][y] = LCEvent;
		}
	}

	// Initialize outputResults to zero.
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			outputResults[i][j] = 0;
		}
	}

	// Print out the contents of the input array.
	for (int i = 0; i < n; i++)
	{
		cout << "Processor " << i << ": ";
		for (int j = 0; j < m; j++)
		{
			cout << setw(5) << inputEvents[i][j] << " ";
		}
		cout << endl;
	}

	cout << "Now calculating the matrix...\n";

	Sleep(1000);

	// Declare an array that stores the values for each send event found.
	int sendList[10];

	int numReceive = 0;

	for (int x = 0; x < 10; x++)
	{
		sendList[x] = 0;
	}

	// Count how many receive events are there in the events array.
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (inputEvents[i][j][0] == 'r')
			{
				numReceive++;
			}
		}
	}

	for (int r = 0; r < numReceive; r++)
	{
		for (int x = 0; x < n; x++)
		{
			// Set the clock value to 1 for each new processor.
			int clockValue = 1;
			for (int y = 0; y < m; y++)
			{
				// Obtain the first character for each event.
				char letter = inputEvents[x][y][0];
				int sendValue = 0; // This is for storing the value of each send event.

				// If the first character is send...
				if (letter == 's')
				{
					//...store the value of send event in the array.
					sendValue = inputEvents[x][y][1] - '0';
					sendList[sendValue] = clockValue;
					outputResults[x][y] = clockValue;
					clockValue++;
				}

				// Otherwise, if it is receive...
				else if (letter == 'r')
				{
					//...attempt to retrieve the value for the corresponding send event.
					sendValue = inputEvents[x][y][1] - '0';
					if (sendList[sendValue] == 0)
					{
						// There is no corresponding send event found, so move on to
						// the next processor.
						break;
					}
					else
					{
						// Retrieve the corresponding send event and assign the value.
						// It is done by taking the maximum between the value for send
						// event and value for the preceding event plus 1.
						if (outputResults[x][y-1] > sendList[sendValue])
						{
							clockValue = outputResults[x][y - 1] + 1;
						}
						else
						{
							clockValue = sendList[sendValue] + 1;
						}
						outputResults[x][y] = clockValue;
						clockValue++;
					}
				}

				// If there is null, then proceed to the next processor.
				else if (letter == 'N')
				{
					break;
				}
				// If an internal event is found (all letters except s and r),
				// then simply assign the appropriate LC value.
				else
				{
					outputResults[x][y] = clockValue;
					clockValue++;
				}
			}
		}
	}
	// Display the output matrix.
	cout << "Calculation complete. Here's the output matrix:\n";
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cout << setw(5) << outputResults[i][j] << " ";
		}
		cout << endl;
	}

	Sleep(1000);

	// Now save the results of Calculate() in a separate text file.
	string outputName;
	cout << "Enter the file name for the results array: ";
	cin >> outputName;

	ofstream resultsFile;
	resultsFile.open(outputName);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			resultsFile << setw(5) << outputResults[i][j] << " ";
		}
		resultsFile << endl;
	}
	resultsFile.close();

	cout << "Output written into file.\n";
	Sleep(1000);

	// Afterwards, clean up the dynamic arrays.
	for (int i = 0; i < n; i++)
	{
		delete[] inputEvents[i];
		delete[] outputResults[i];
	}
	delete[] inputEvents;
	delete[] outputResults;
	return;
}

void Verify(int n, int m)
{
	if ((n < 1 || n > 5) || (m < 1 || m > 25))
	{
		cout << "ERROR. Invalid number of processors and events.\n";
		cout << "Number of processors must be at least 1 and at most 5,\n";
		cout << "and there can be a maximum of 25 events for each processor.\n";
		return;
	}

	// For Verify, either return the output as a matrix of events if the
	// the input events are in a correct sequence or print error message if not.

	// Declare two arrays: an int array that contains the values as event results
	// and a string array that stores the result.
	int LCValue;
	int** inputValues = new int* [n];
	for (int x = 0; x < n; x++)
	{
		inputValues[x] = new int[m];
	}

	string** outputResults = new string * [n];
	for (int y = 0; y < n; y++)
	{
		outputResults[y] = new string[m];
	}

	// Initialize inputValues to zero.
	for (int x = 0; x < n; x++)
	{
		for (int y = 0; y < m; y++)
		{
			inputValues[x][y] = 0;
		}
	}

	// Ask the user to enter the values for each processor.
	// Then store each input value into the array.
	// If user enters zero, then move on to the next processor.
	cout << "Enter the values for each processor.\n";
	cout << "Enter zero to skip to the next processor.\n";
	for (int x = 0; x < n; x++)
	{
		cout << "Processor " << x << ":\n";
		for (int y = 0; y < m; y++)
		{
			cout << "Value " << y+1 << ": ";
			cin >> LCValue;
			if (LCValue == 0)
			{
				break;
			}
			inputValues[x][y] = LCValue;
		}
	}

	// Initialize outputResults to null.
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			outputResults[i][j] = "NULL";
		}
	}

	// Print out the contents of the input array.
	for (int i = 0; i < n; i++)
	{
		cout << "Processor " << i << ": ";
		for (int j = 0; j < m; j++)
		{
			cout << setw(5) << inputValues[i][j] << " ";
		}
		cout << endl;
	}

	// Keep a boolean variable that checks if the matrix is valid.
	bool isValidMatrix = true;
	
	cout << "Now verifying the matrix...\n";

	Sleep(1000);

	// Keep track of all values for the receive events found.
	// (A receive event exists if there is a gap between the current
	// and the preceding value, so for instance, (2-1)=1 means it 
	// is either a send or an internal event, but (8-2)=6 means it
	// is a receive event since the difference is larger than 1.)
	// If there are multiple values that are the same, then choose
	// either one to be the send event and have the other be the
	// internal event.

	// Value for send event is value of receive event minus 1.
	int sendList[10];

	for (int x = 0; x < 10; x++)
	{
		sendList[x] = 0;
	}

	for (int i = 0; i < n; i++)
	{
		if (isValidMatrix == false)
		{
			break;
		}
		for (int j = 0; j < m; j++)
		{
			// This is to check if this is the first event in each processor.
			if (j == 0)
			{
				// If the value of LC is 1, then it's an internal event, so mark that
				// as internal. If the value is larger than 1, then it may be a receive event,
				// so mark as receive event, subtract 1 from the value and store it
				// in the array of send event values.
				if (inputValues[i][j] == 1)
				{
					outputResults[i][j] = "internal";
				}
				else if (inputValues[i][j] > 1)
				{
					outputResults[i][j] = "r";
					outputResults[i][j] += to_string(j + 1);
					sendList[j] = inputValues[i][j] - 1;
				}
				else if (inputValues[i][j] < 0)
				{
					// The value cannot be negative, so abort the algorithm if is found.
					cout << "ERROR. Not a valid matrix.\n";
					cout << "All values in the matrix must be positive integers.\n";
					return;
				}
			}
			else if (inputValues[i][j] == 0)
			{
				// A zero means a null event exists, so leave it at is
				// and move on to the next processor.
				break;
			}
			else if (inputValues[i][j] < 0)
			{
				// The value cannot be negative, so abort the algorithm if is found.
				cout << "ERROR. Not a valid matrix.\n";
				cout << "All values in the matrix must be positive integers.\n";
				return;
			}
			else
			{
				// If the difference between the current value and preceding value
				// is one, mark as internal event. If the difference is greater
				// than one, mark as receive event, subtract 1 from the input value,
				// and then store the value in the array of send event values.
				if (inputValues[i][j] - inputValues[i][j - 1] == 1)
				{
					outputResults[i][j] = "internal";
				}
				else if (inputValues[i][j] - inputValues[i][j-1] > 1)
				{
					outputResults[i][j] = "r";
					outputResults[i][j] += to_string(j + 1);
					sendList[j] = inputValues[i][j] - 1;
				}
				else if (inputValues[i][j] - inputValues[i][j-1] < 0)
				{
					// The difference between current value and preceding value cannot
					// be negative. In other words, all values for each processor
					// must be in increasing order.
					// Break out of loop and abort Verify if that happens.
					isValidMatrix = false;
					break;
				}
			}
		}
	}

	if (isValidMatrix == false)
	{
		cout << "ERROR. Not a valid matrix.\n";
		cout << "All values in each processor must be in increasing order.\n";

		return;
	}

	// For the send event, obtain all the values stored in the array, search through
	// the input matrix and try to find a corresponding receive event value for the send
	// event. If a value is found, then map the corresponding send event to it.
	// If no such value can be found, then return an error when appropriate.
	
	// This will be the total number of send events for the matrix.
	int numSendEvents = 0;

	// This variable checks to see if a send event is mapped to the corresponding
	// receive event.
	// In case there are more than one value of the same number, then consider
	// selecting one value to be the send event and the other to be the internal event.
	// Be sure to set boolean to true in order to avoid double counting the same value.
	bool isMapped[10];

	for (int x = 0; x < 10; x++)
	{
		if (sendList[x] > 0)
		{
			numSendEvents++;
		}
	}

	for (int x = 0; x < 10; x++)
	{
		isMapped[x] = false;
	}

	for (int x = 0; x < numSendEvents; x++)
	{
		cout << "Value for send event " << x+1 << ": " << sendList[x] << endl;
	}

	// Each time there is a match for the value of send event and the value for
	// the input matrix, increment numMatch by 1 and set isMapped to true.
	for (int s = 0; s < numSendEvents; s++)
	{
		for (int x = 0; x < n; x++)
		{
			for (int y = 0; y < m; y++)
			{
				if (sendList[s] == inputValues[x][y] && isMapped[s] != true)
				{
					outputResults[x][y] = "s";
					outputResults[x][y] += to_string(s + 1);
					isMapped[s] = true;
				}
			}
		}
	}

	// This condition checks if all send events are mapped correctly to the corresponding
	// receive events. If at least one send event is missing, then set boolean to false.
	for (int i = 0; i < numSendEvents; i++)
	{
		if (isMapped[i] != true)
		{
			isValidMatrix = false;
			break;
		}
	}

	Sleep(1000);

	if (isValidMatrix == false)
	{
		cout << "ERROR. Not a valid matrix.\n";
		cout << "There must be a corresponding send event for each receive event.\n";
	}
	else
	{
		// Finalizing the Verify output.
		// Convert all internal events into alphabetic characters.
		char internalCharacter = 'a';
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				if (outputResults[i][j] == "internal")
				{
					outputResults[i][j] = internalCharacter;
					internalCharacter++;
				}
			}
		}

		// Display the output matrix.
		cout << "Verification complete. Here's the output matrix:\n";
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				cout << setw(5) << outputResults[i][j] << " ";
			}
			cout << endl;
		}
		// Now save the results of Verify() in a separate text file.
		string outputName;
		cout << "Enter the file name for the results array: ";
		cin >> outputName;

		ofstream resultsFile;
		resultsFile.open(outputName);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				resultsFile << setw(5) << outputResults[i][j] << " ";
			}
			resultsFile << endl;
		}
		resultsFile.close();

		cout << "Output written into file.\n";
		Sleep(1000);
	}

	// Afterwards, clean up the dynamic arrays.
	for (int i = 0; i < n; i++)
	{
		delete[] inputValues[i];
		delete[] outputResults[i];
	}
	delete[] inputValues;
	delete[] outputResults;
	return;
}

int main()
{
	int choice;
	int numProcessors;
	int numEvents;

	cout << "Welcome!\n";

	// Keep running the program until the user wants to quit.
	while (true)
	{
		cout << "Please choose the following options:\n";
		cout << "1: Calculate\n";
		cout << "2: Verify\n";
		cout << "3: Quit\n";
		cout << endl;
		cin >> choice;

		if (choice == 1)
		{
			cout << "Enter the number of processors (max 5 processors): ";
			cin >> numProcessors;
			cout << "Enter the number of events (max 25 events): ";
			cin >> numEvents;
			// Call the Calculate function.
			Calculate(numProcessors, numEvents);
			cout << endl;
		}
		else if (choice == 2)
		{
			cout << "Enter the number of processors (max 5 processors): ";
			cin >> numProcessors;
			cout << "Enter the number of events (max 25 events): ";
			cin >> numEvents;
			// Call the Verify function.
			Verify(numProcessors, numEvents);
			cout << endl;
		}
		else if (choice == 3)
		{
			// Exit the program.
			cout << endl;
			break;
		}
		else
		{
			cout << "Invalid choice. Please re-enter.\n";
			cout << endl;
		}
	}

	cout << "Goodbye!\n";
	return 0;
}