#include <dpp/dpp.h>
#include <map>
#include "TicTacToe.h"
#include <cstdlib>

const std::string BOT_TOKEN = "BOT_TOKEN";

struct PendingChallenge {
    dpp::snowflake challenger_id;
    dpp::snowflake opponent_id;
};

struct GameSession {
    TicTacToeGame game;
    dpp::snowflake playerX_id;
    dpp::snowflake playerO_id;
};

std::map<dpp::snowflake, PendingChallenge> pending_challenges;
std::map<dpp::snowflake, GameSession> active_games;


// This function builds the status message, pinging the relevant player.
std::string build_status_message(const GameSession& session) {
    if (session.game.isGameOver()) {
        char winner = session.game.getWinner();
        if (winner == 'X') {
            return dpp::user::get_mention(session.playerX_id) + " wins!";
        } else if (winner == 'O') {
            return dpp::user::get_mention(session.playerO_id) + " wins!";
        } else {
            return "It's a draw!";
        }
    } else {
        char current_player_char = session.game.getCurrentPlayer();
        dpp::snowflake current_player_id = (current_player_char == 'X') ? session.playerX_id : session.playerO_id;
        return dpp::user::get_mention(current_player_id) + "'s turn.";
    }
}

// This function builds the 3x3 grid of interactive Discord buttons based on the game state.
std::vector<dpp::component> build_board_components(const GameSession& session) {
    std::vector<dpp::component> action_rows;
    dpp::component current_row;

    const auto& board = session.game.getBoardState();
    bool game_is_over = session.game.isGameOver();

    for (int i = 0; i < 9; ++i) {
        if (i % 3 == 0 && i > 0) {
            action_rows.push_back(current_row);
            current_row = dpp::component();
        }

        dpp::component button;
        button.set_type(dpp::cot_button);
        button.set_id("tictactoe_move_" + std::to_string(i));

        if (board[i] == 'X') {
            button.set_label("X").set_style(dpp::cos_primary);
        } else if (board[i] == 'O') {
            button.set_label("O").set_style(dpp::cos_success);
        } else {
            button.set_label("\u200b").set_style(dpp::cos_secondary); // Zero-width space
        }
        
        if (board[i] != ' ' || game_is_over) {
            button.set_disabled(true);
        }
        
        current_row.add_component(button);
    }
    action_rows.push_back(current_row);
    return action_rows;
}


int main() {
    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);
    bot.on_log(dpp::utility::cout_logger());

    bot.on_button_click([&bot](const dpp::button_click_t& event) {
        std::string custom_id = event.custom_id;
        dpp::snowflake channel_id = event.command.channel_id;

        if (custom_id == "ttt_accept") {
            auto challenge_iter = pending_challenges.find(channel_id);
            if (challenge_iter == pending_challenges.end()) {
                event.reply(dpp::message("This challenge has expired or was cancelled.").set_flags(dpp::m_ephemeral));
                return;
            }

            PendingChallenge& challenge = challenge_iter->second;
            if (event.command.usr.id != challenge.opponent_id) {
                event.reply(dpp::message("Only the challenged player can accept this invitation.").set_flags(dpp::m_ephemeral));
                return;
            }

            GameSession new_session;
            new_session.playerX_id = challenge.challenger_id;
            new_session.playerO_id = challenge.opponent_id;
            active_games[channel_id] = new_session;
            pending_challenges.erase(challenge_iter);

            dpp::message updated_message(channel_id, build_status_message(active_games[channel_id]));
            updated_message.set_allowed_mentions(true, false, false, false);
            for (const auto& row : build_board_components(active_games[channel_id])) {
                updated_message.add_component(row);
            }
            event.reply(dpp::ir_update_message, updated_message);

        } else if (custom_id == "ttt_decline") {
            auto challenge_iter = pending_challenges.find(channel_id);
            if (challenge_iter == pending_challenges.end()) {
                event.reply(dpp::message("This challenge has already been handled.").set_flags(dpp::m_ephemeral));
                return;
            }

            PendingChallenge& challenge = challenge_iter->second;
            if (event.command.usr.id != challenge.opponent_id && event.command.usr.id != challenge.challenger_id) {
                event.reply(dpp::message("You cannot interact with this challenge.").set_flags(dpp::m_ephemeral));
                return;
            }
            
            dpp::message updated_message(channel_id, "The challenge was declined.");
            dpp::component action_row;
            action_row.add_component(dpp::component().set_label("Accept").set_style(dpp::cos_success).set_id("ttt_accept").set_disabled(true));
            action_row.add_component(dpp::component().set_label("Decline").set_style(dpp::cos_danger).set_id("ttt_decline").set_disabled(true));
            updated_message.add_component(action_row);
            event.reply(dpp::ir_update_message, updated_message);
            pending_challenges.erase(challenge_iter);

        } else if (custom_id.rfind("tictactoe_move_", 0) == 0) {
            auto game_iter = active_games.find(channel_id);
            if (game_iter == active_games.end()) {
                event.reply(dpp::message("This game has already ended.").set_flags(dpp::m_ephemeral));
                return;
            }

            GameSession& session = game_iter->second;
            char current_player_char = session.game.getCurrentPlayer();
            dpp::snowflake current_player_id = (current_player_char == 'X') ? session.playerX_id : session.playerO_id;

            if (event.command.usr.id != current_player_id) {
                event.reply(dpp::message("It's not your turn!").set_flags(dpp::m_ephemeral));
                return;
            }
            
            int position = std::stoi(event.custom_id.substr(15));
            
            if (session.game.makeMove(position)) {
                dpp::message updated_message(channel_id, build_status_message(session));
                updated_message.set_allowed_mentions(true, false, false, false);
                for (const auto& row : build_board_components(session)) {
                    updated_message.add_component(row);
                }
                event.reply(dpp::ir_update_message, updated_message);
                if (session.game.isGameOver()) {
                    active_games.erase(game_iter);
                }
            } else {
                event.reply(dpp::message("That's not a valid move!").set_flags(dpp::m_ephemeral));
            }
        }
    });

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "tictactoe") {
            dpp::snowflake channel_id = event.command.channel_id;
            dpp::snowflake challenger_id = event.command.usr.id;
            dpp::snowflake opponent_id = std::get<dpp::snowflake>(event.get_parameter("opponent"));
            
            if (challenger_id == opponent_id) {
                event.reply("You cannot play a game against yourself!");
                return;
            }
            if (active_games.count(channel_id) || pending_challenges.count(channel_id)) {
                event.reply("A game or challenge is already active in this channel! Please wait for it to finish.");
                return;
            }

            PendingChallenge challenge = {challenger_id, opponent_id};
            pending_challenges[channel_id] = challenge;

            dpp::message msg(channel_id, dpp::user::get_mention(opponent_id) + ", " + dpp::user::get_mention(challenger_id) + " has challenged you to a game of Tic-Tac-Toe!");
            dpp::component action_row;
            action_row.add_component(dpp::component().set_label("Accept").set_style(dpp::cos_success).set_id("ttt_accept"));
            action_row.add_component(dpp::component().set_label("Decline").set_style(dpp::cos_danger).set_id("ttt_decline"));
            msg.add_component(action_row);
            msg.set_allowed_mentions(true, false, false, false);
            event.reply(msg);
        }
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            dpp::slashcommand tictactoe_cmd("tictactoe", "Start a Tic-Tac-Toe game with another user.", bot.me.id);
            tictactoe_cmd.add_option(
                dpp::command_option(dpp::co_user, "opponent", "The user you want to play against.", true)
            );
            bot.global_command_create(tictactoe_cmd);
        }
    });

    bot.start(dpp::st_wait);
    return 0;
}

