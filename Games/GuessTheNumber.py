# A simple "Guess the Number" game

from random import *

def ready():
    while True:
        readyInput = input()
        if (readyInput == 'N'):
            print("\nNo? That's okay... didn't want to play with you anyways!")
            return False
        elif (readyInput == 'Y'):
            print("\nOkay! Let's get started then!")
            return True
        else:
            print("\nTell me using either a \"Y\" or an \"N\" please!")

# Start game
print("\nWanna play the \"Guess the Number\" game? (Y/N)")
playGame = ready()
while playGame:
    print("Hmm... I'm thinking of a number from 0 to 100")
    target = randint(0, 100)
    guessCount = 0

    print("What could it be?")
    while True:
        guess = input()
        guessCount += 1

        try:
            guess = int(guess)
            if (guess < 0 or guess > 100):
                # else check if in range
                print("\nThat's not a number from 0 to 100... try again please!")
            elif (guess == target):
                print("\nDING DING DING! That was my number!")
                break
            elif (guess > target):
                print("\nA little lower, but guess again!")
            elif (guess < target):
                print("\nMaybe a bit higher, try again!")
        except ValueError:
            print("\nThat's not even a number! Please try again!")

    if (guessCount > 1):
        print("Only took you " + str(guessCount) + " tries...")
    else:
        print("Wow, first try too! Are you a psychic or something?")
    
    print("\nWanna play again? (Y/N)")
    playGame = ready()
