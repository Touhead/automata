Projet Automata FSM - Version 1.0 - Version Qt 5.3.2 - 23/01/2015

GENERAL USAGE NOTES
--------------------

- This program do not support Linux and Mac OS, and could not support Windows OS older than Seven.

- The file format to open a FSM is:

	alphabet - each symbole separate by space(s)
	state(s) - each symbole separate by space(s)
	inital state(s) - each symbole separate by space(s)
	final state(s) - each symbole separate by space(s)
	[empty line]
	state transition table - each set of symbole is separated by space(s) and there is no empty line between the table line / several transitions for a state are separated by a comma.

	Example :

	a b c d 
	1 2 3 4
	1
	1

	1 2,3 - - -
	2 - 3 - -
	3 - - 4 -
	4 - 1,2 - 1

	For the state transition table, the '-' symbol represents the absence of transition.

FEATURES
--------------------

Open: Opening a file.
Save: Saving the FSM in a file.
Reading: Testing if the word is recognized by the FSM.
Standardize: Creating the equivalent FSM with a unique initial state and any transition to this initial state. (Reload the orginal FSM)
Is a DFA ?: Displaying if the FSM is a DFA or not (Reload the original FSM).
DFA: Creating the equivalent DFA.
Completion: Creating the equivalent FSM with a transition for each symbol in all state. (Must be a DFA)
Minimize: Creating the equivalent FSM with the fewer state. (Must be completed)
	