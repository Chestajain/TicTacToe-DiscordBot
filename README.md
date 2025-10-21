# Tic Tac Toe Discord Bot

A simple Discord bot that allows users to play Tic Tac Toe against each other using slash commands and interactive buttons. Built with C++ and the [D++ (DPP)](https://dpp.dev/) library.

## Features

* Challenge another user to a game using `/tictactoe`.
* Accept or decline challenges via buttons.
* Play the game using an interactive 3x3 button grid.
* Displays whose turn it is and the game outcome (win/draw).
* Prevents users from playing against themselves or starting multiple games in the same channel.

## Prerequisites

* A C++ compiler supporting C++17 or later (e.g., GCC, Clang/LLVM). Your VSCode settings indicate you are using `clang++` on macOS.
* The [D++ library](https://dpp.dev/build.html) installed.
* CMake (Recommended for building D++ and this project).
* Git (for cloning if obtained from GitHub).
* A Discord Bot Token obtained from the [Discord Developer Portal](https://discord.com/developers/applications).

## Setup & Running Locally

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/Chestajain/TicTacToe-DiscordBot.git](https://github.com/Chestajain/TicTacToe-DiscordBot.git)
    cd TicTacToe-DiscordBot/TicTacToe
    ```
    *(Note: Assuming your code lives inside the `TicTacToe` subfolder within the repo)*

2.  **Install D++:** Follow the official [D++ build instructions](https://dpp.dev/build.html) for your operating system (macOS). This typically involves using CMake. Ensure the library is installed in a location your compiler can find.

3.  **Compile the Bot:** Navigate to the `TicTacToe` directory (if you aren't already there). You'll need to link against the D++ library. A basic compilation command might look like this (you may need to adjust paths/flags based on your D++ installation location):
    ```bash
    # Make sure you are in the TicTacToe directory containing main.cpp, etc.
    clang++ main.cpp TicTacToe.cpp -std=c++17 -ldpp -o TicTacToeBot
    ```
    *(Alternatively, if you create a `CMakeLists.txt` for this project, you would typically use `cmake . && make`)*

4.  **Set Environment Variable:** Before running, you **must** provide your bot token as an environment variable. Replace `YOUR_NEW_BOT_TOKEN_HERE` with the actual **new** token you generated after resetting it:
    ```bash
    export BOT_TOKEN="BOT_TOKEN"
    ```
    *(This variable needs to be set in the terminal session where you run the bot)*

5.  **Run the Bot:**
    ```bash
    ./TicTacToeBot
    ```

## Usage

1.  **Challenge a User:** In a Discord channel where the bot is present, type `/tictactoe` and select the user you want to play against from the `opponent` option.
2.  **Accept/Decline:** The challenged user will see buttons to "Accept" or "Decline" the game. Only the challenged user can accept. Either player can decline.
3.  **Play:** Once accepted, a 3x3 grid of buttons appears. Click an empty button (buttons show `\u200b` - a zero-width space - when empty) on your turn to make a move. The bot will update the board and indicate whose turn it is next by mentioning them.
4.  **Game End:** The bot announces the winner by mentioning them or declares a draw. The buttons become disabled. A new game can then be started in the channel.

## Invite the Bot

You can invite this bot to your Discord server using the following link (replace with the actual invite URL you generated in the Discord Developer Portal):

[Invite Link](https://discord.com/oauth2/authorize?client_id=1427666307515551866&permissions=68608&integration_type=0&scope=bot+applications.commands)

## License

No license provided.