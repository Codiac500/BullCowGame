// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"


void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else 
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::ProcessGuess(FString Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(Guess + TEXT(" is correct! You've won!"));
        EndGame();
        return;
    }
    
    if (Guess != HiddenWord)
    {

        if (Guess.Len() != HiddenWord.Len()) //tell length if wrong length
        {
            PrintLine(TEXT("Remember, the word is %i letters long!"), HiddenWord.Len());
            return;
        }


        if (!IsIsogram(Guess)) //check if isogram
        {
            PrintLine(TEXT("Isograms have no repeating letters! \nTry again!"));
            return;
        }


        PrintLine(Guess + TEXT(" is not correct!")); //tell wrong

        --Lives; //update lives
        PrintLine(TEXT("You have %i Lives left!"), Lives);

        GetBullCows(Guess);
        //tell bull- cycle through string array, replace comparison with iterator. same place and letter vs just same letter but wrong place

        //tell cows

        
        if (Lives == 0) //endgame if lives are 0
        {
            EndGame();
            return;
        }
    }
}

bool UBullCowCartridge::IsIsogram(FString Word)
{
    const FString IsogramCheck = Word; 

    for (int i = 0; i < Word.Len(); i++)
    {
        for (int x = Word.Len() - 1; x > i; x--)
        {
            if (IsogramCheck[i] == Word[x])
            {
                return false;
            }
        }
    }

   return true;
}

void UBullCowCartridge::GetBullCows(const FString& Guess)
{
    int32 BullCount = 0;
    int32 CowCount = 0;

    //for every index guess is same as index hidden, bullcount ++, otherwise check if cow

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            BullCount++;
            continue;
        }

        for (int HiddenWordIndex = 0; HiddenWordIndex < HiddenWord.Len(); HiddenWordIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenWordIndex])
            {
                CowCount++;
                break;
            }
        }
    }

    PrintLine(TEXT("There are %i Bulls"), BullCount);
    PrintLine(TEXT("There are %i Cows"), CowCount);
    return;
}


void UBullCowCartridge::SetupGame()
{
    HiddenWord = Words[FMath::RandRange(0, Words.Num() - 1)];
    Lives = 10;
    bGameOver = false;

    //PrintLine(TEXT("The hidden word is %s"), *HiddenWord); //debug line

    PrintLine(TEXT("Welcome to BullCows!\nA Cattle Game of Isograms!"));
    PrintLine(TEXT("Press TAB to take control of the Terminal"));
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("Enter your Guess and press Enter..."), HiddenWord.Len());
}

void UBullCowCartridge::EndGame()
{
    PrintLine(TEXT("Press Enter to Play Again..."));
    bGameOver = true;
}