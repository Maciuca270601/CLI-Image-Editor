Name: Maciuca Alexandru Petru
Group: 314CA
Course: PC
Title: CLI Image Editor

## About the code

    I read instructions until entering the string of characters: "EXIT".

    If the command is not valid, the program will display: "Invalid command".

    If LOAD is given to a file, I make sure that it exists, and if it does
    I will complete the 2 structures defined by me: image and matrix.
    Image will contain details such as type, width, height or intensity, and
    matrix will contain the actual picture matrix.

    For "SELECT" I take care to receive the suitcase parameters and depending on them,
    decide if the selection is valid or not.

    For "ROTATE" I use 2 different functions explained in the comments, one for
    rotating a quadratic area and one for the cases where the resulting area in
    of the command: "SELECT ALL" is rectangular.

    For "CROP" I allocate a new matrix, complete it with the necessary data,
    free the old one and return to main the new one, process that is repeated as well
    for "GRAYSCALE" and "SEPIA".

    For "SAVE" I decide if a save is needed in ascii or binary mode,
    that's why I have 2 similar functions: save_txt, save_bin.
