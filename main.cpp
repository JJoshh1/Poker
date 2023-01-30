#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <iostream>
#include <set>
#include <limits>
#include <cctype>

using namespace std;

// class Space{ 
//   private:
  
  
//   public:
//   bool look_for_nl(std::istream& is) {
//       for (char = d is.get(); is; d = is.get()){
//         if (d == '\n') return d;
//         if(!isspace(d)){
//           is.putback(d);
//         return ' ';
//         }
//       }
//     return '\n';
//     };
// };
int seedIncrement = 0;
enum winningHands {
NONE, PAIR, TWOPAIR, THREE, STRAIGHT, FLUSH, HOUSE, FOUR, STRFLUSH
};

class Card {
  private:
    int value;
    string face;
    string suit;

  public:
    /** 
    * constructor for card which assigns v to value and takes s as an index of the array
    * suits to assign to suit, along with setting face to either the value as a string or one
    * of the letters in the royalty array if the value exceeds 10
    */
    Card(int v ,int s) {
      string suits[4] = {"♡", "♤" , "♧", "♢"};
      string royalty[5] = {"T", "J", "Q", "K", "A"};
      this->value = v;
      this->suit = suits[s];
      if (v > 9) {
        this->face = royalty[v-10];
      } else this->face = to_string(v);
    }
  
    int getValue() { // gets card value
      return this->value;
    }
    string getFace() { // gets card face
      return this->face;
    }
    string getSuit() { // gets card suit
      return this->suit;
    }
    void printCard() {
      cout << "╭───╮" << endl;
      cout << "│ "<<this->suit << " │" << endl;
      cout << "│ " <<this->face << " │" << endl;
      cout << "╰───╯" << endl;
    }
 };

class Player {
private:
string name;
vector <Card> hand;
vector<int> score;
int bet;
int balance;
bool active = true;


public:
Player(string name){
  this->name = name;
  this->balance = 100;
}

void resetHand(Card card1, Card card2) {
  this->hand = {};
  this->hand.push_back(card1);
  this->hand.push_back(card2);
  this->score = {NONE, 2};
  this->bet = 0;
  this->active = true;
}

string getName() {
  return this->name;
}

int getBalance() {
  return this->balance;
}

int getBet() {
  return this->bet;
}

void setBet(int bet) {
  this->bet = bet;
}

vector <Card> getHand() {
  return this->hand;
}
void printHand() {
  this->hand[0].printCard();
  this->hand[1].printCard();
}
void fold() {
  this->active = false;
}
bool getActive() {
  return this->active;
}

void loseMoney(int money) {
  this->balance -= money;
}

void getMoney(int money) {
  this->balance += money;
}
vector <int> getScore() {
  return this->score;
}

void updateScore(int handValue, int rank) {
  if (handValue >= this->score[0]) {
    this->score[0] = handValue;
    if (rank >= this->score[1]) this->score[1] = rank;
  }
}

void checkHand(vector<Card> hand) {
  vector<Card> sortedHand = {};

  int handSize = hand.size();
  for (int n = 0; n < handSize; n++) {
    int maxValue = hand[0].getValue();
    int maxIndex = 0;
    for(int indx = 0; indx < hand.size(); indx++) {
      if (hand[indx].getValue() > maxValue) {
        maxValue = hand[indx].getValue();
        maxIndex = indx;
      }
    }
    sortedHand.push_back(hand[maxIndex]);
    auto handPointer = hand.begin();
    advance(handPointer, maxIndex);
    hand.erase(handPointer);
  }
  vector<int> values = {};
  vector<string> suits = {};
  for (int indx = 0; indx < handSize; indx++) {
    values.push_back(sortedHand[indx].getValue());
    suits.push_back(sortedHand[indx].getSuit());
  }
  
  bool hasThree = false;
  vector<int> groupValues = {};
  int sameSuitCount = 1;
  string lastSuit = suits[0];
  int consecutive = 1;
  int lastVal = *values.begin();
  int straightStart = 0;

  this->updateScore(NONE, values[0]);
  
  for (int indx = 0; indx < values.size(); indx++) {
    if (count(values.begin(), values.end(), values[indx]) >= 2) {
      this->updateScore(PAIR, values[indx]);
      if (find(groupValues.begin(), groupValues.end(), values[indx]) == groupValues.end()) {
        if (groupValues.size() > 0 && values[indx] != groupValues[groupValues.size()-1]) {
          groupValues.push_back(values[indx]);
        } else if (groupValues.size() == 0) groupValues.push_back(values[indx]);
      } 
      if (count(values.begin(), values.end(), values[indx]) >= 3) {
        hasThree = true;
        this->updateScore(THREE, values[indx]);
        if (count(values.begin(), values.end(), values[indx]) >= 4) {
          this->updateScore(FOUR, values[indx]);
        }
      }
    }
    if (groupValues.size() >= 2) {
      this->updateScore(TWOPAIR, *groupValues.begin());
      if (hasThree) this->updateScore(HOUSE, *groupValues.begin());
    }
    if (suits[indx] == lastSuit) {
      sameSuitCount ++;
    } else if (suits[indx] != lastSuit && values[indx] != lastVal) {
      sameSuitCount = 1;
    } 
    if (values[indx] == lastVal - 1) {
      consecutive++;
    } else if (values[indx] == lastVal){
    } else {
      consecutive = 1;
      straightStart = indx;
    }
    if (consecutive >= 5) {
      this->updateScore(STRAIGHT, values[straightStart]);
      if (sameSuitCount >=5) {
        this->updateScore(STRFLUSH, values[straightStart]);
      }
    }
  }
} 
};

class Deck{
private:
stack<Card> deck;
vector<Card> cards;

public:
Deck(){
  // string curr_suit;
  // string curr_card;
  for(int i = 0; i < 4 ; i++ ){
    // curr_suit = this->suits[i];
    for (int j = 2; j < 15; j++){
      this->cards.push_back(Card(j, i));
    }
  }
  srand(time(NULL) + seedIncrement);
  random_shuffle(this->cards.begin(), this->cards.end());
  seedIncrement++;
  for (int i = 0; i<52; i++) {
    this->deck.push(this->cards[i]);
  }
}
void printDeck(){
  for (int i = 0; i<2; i++){
    this->deck.top().printCard();
    cout << endl;
    this->deck.pop();
  }
}

Card getC() {
  return this->deck.top();
}

Card getCard() {
  Card card = this->deck.top();
  this->deck.pop();
  return card;
}
};

class Board {
private:
vector<Card> cards;
int curr_card = 0;
vector<string> suits = {"?","?","?","?","?"};
vector<string> value = {" "," "," "," "," "};

public:
void printBoard(){
  cout << "╔═════════════════════════════════════╗" << endl;
  cout << "║                                     ║" << endl;
  cout << "║  ╭───╮  ╭───╮  ╭───╮  ╭───╮  ╭───╮  ║" << endl;
  cout << "║  │ " << this->suits[0] << " │  │ " << this->suits[1] << " │  │ " << this->suits[2] << " │  │ " << this->suits[3] << " │  │ " << this->suits[4] << " │  ║" << endl;
  cout << "║  │ " << this->value[0] << " │  │ " << this->value[1] << " │  │ " << this->value[2] << " │  │ " << this->value[3] << " │  │ " << this->value[4] << " │  ║" << endl;
  cout << "║  ╰───╯  ╰───╯  ╰───╯  ╰───╯  ╰───╯  ║" << endl;
  cout << "║                                     ║" << endl;
  cout << "╚═════════════════════════════════════╝" << endl;
}

void getCard(Card card) {
  this->cards.push_back(card);
  auto it = this->suits.begin();
  advance(it, this->curr_card);
  this->suits.erase(it);
  this->suits.insert(it, card.getSuit());
  auto it2 = this->value.begin();
  advance(it2, this->curr_card);
  this->value.erase(it2);
  this->value.insert(it2, card.getFace());
  this->curr_card ++;
}

vector <Card> getCards() {
  return this->cards;
}
};


class Game {
private:
vector<Player> players;
Deck *deck = new Deck();
Board *b = new Board();
int pot = 0;
bool over = false;

public:
Game(vector<Player> players) {
  for (auto &p : players) {
    p.resetHand(this->deck->getCard(), this->deck->getCard());
    this->players.push_back(p);
  }
}

void printHands() {
  for (int i = 0; i<this->players.size(); i++) {
    this->players[i].printHand();
  }
}

// void printB() {
//   this->cards.push_back(this->deck->getC());
//   this->b->getCard(this->deck->getCard());
//   this->cards.push_back(this->deck->getC());
//   this->b->getCard(this->deck->getCard());
//   this->cards.push_back(this->deck->getC());
//   this->b->getCard(this->deck->getCard());
//   this->cards.push_back(this->deck->getC());
//   this->b->getCard(this->deck->getCard());
//   this->cards.push_back(this->deck->getC());
//   this->b->getCard(this->deck->getCard());
//   this->b->printBoard();
// }
void printB() {
  this->b->printBoard();
}

Board* getB() {
  return this->b;
}

Deck* getDeck() {
  return this->deck;
}

void checkAllHands() {
  for (auto &p : this->players) {
    vector <Card> combinedHand = this->b->getCards();
    for (auto &c : p.getHand()) {
      combinedHand.push_back(c);
    }
    p.checkHand(combinedHand);
  }
}

vector <Player> getPlayers() {
  return this->players;
}

void setPlayers(vector <Player> plrs) {
  this->players = {};
  for (auto &p : plrs) {
    this->players.push_back(p);
  }
}
// void nextTurn(bool first) {
//   this->
// }


void showHand(int pl_num) {
  cout << "\033[2J\033[0;0H";
  cout << this->players[pl_num].getName() << ", it is your turn. Your hand will be shown next, so be sure that none of your opponents can see it.\nInput anything to continue.";
        string anything;
        cin >> anything;
        cout << "\033[2J\033[0;0H";
        this->b->printBoard();
        for (auto &player : this->players) { // Display the board, all player balances, the pot, the string turnHistory and the hand of the current player
          cout << player.getName() << ": $" << player.getBalance() << "     ";
        }
        cout << endl;
        cout << "\t\tPot: $" << this->pot << endl;
        this->players[pl_num].printHand();
}

void betting() {
  vector<int> pl_amnt;
  bool increased = false;
  int amnt_raised = 0;
  string option;
  int raise = 0;
  int player_raised = 0;
  for (int i = 0; i < this->players.size(); i++) {
    if (this->players[i].getActive()) {
    showHand(i);
    if (increased == false) {
      cout << this->players[i].getName() << " would you like to raise(0) or check(1)?" << endl;
      cin >> option;
      if (option == "0") {
        while(true) {
        cout << "Enter the amount: " << endl;
        cin >> raise;
        if (raise > 0 && raise <= this->players[i].getBalance()) break;
        }
        amnt_raised += raise; 
        this->pot += raise;
        increased = true;
        this->players[i].loseMoney(raise);
        pl_amnt.push_back(raise);
        player_raised = i;
      }else pl_amnt.push_back(0);
    } else {
      cout << this->players[i].getName() << " would you like to raise(0) or call(1) or fold(2) to $" << amnt_raised << " ?" << endl;
      cin >> option;
      if (option == "0") {
        while(true) {
        cout << "Enter the amount: " << endl;
        cin >> raise;
        if (raise > 0 && raise <= this->players[i].getBalance()) break;
        }
        amnt_raised += raise; 
        this->pot += amnt_raised;
        this->players[i].loseMoney(amnt_raised);
        pl_amnt.push_back(amnt_raised);
        player_raised = i;
      }else if (option == "1") {
        this->pot += amnt_raised;
        this->players[i].loseMoney(amnt_raised);
        pl_amnt.push_back(amnt_raised);
      }else {
        this->players[i].fold();
        pl_amnt.push_back(0);
      }
    }
    cout << endl;
  }
  }
  cout << player_raised << endl;
  for (int i = 0; i<player_raised; i++) {
    if (this->players[i].getActive()) {
      showHand(i);
    cout << this->players[i].getName() << " would you like to call(0) or fold(1) to $" << amnt_raised-pl_amnt[i] << " ?" << endl;
    cin >> option;
    if (option == "0"){
      cout << amnt_raised-pl_amnt[i] << endl;
      this->pot += amnt_raised-pl_amnt[i];
      this->players[i].loseMoney(amnt_raised-pl_amnt[i]);
    }else {
      this->players[i].fold();
    }
    cout << endl;
    }
  }
  lastPlayer();
}
int getPot() {
  return this->pot;
}

void lastPlayer() {
  string anything;
  int act_num = 0;
  int ind = 0;
  for (int i = 0;i<this->players.size();i++) {
    if (this->players[i].getActive()) {
      act_num ++;
      ind = i;
      }
  }
  if (act_num == 1){
    this->players[ind].getMoney(this->pot);
    this->pot = 0;
    this->over = true;
    cout << this->players[ind].getName() << " Win!" <<endl;
    cout << "Enter anything.";
    cin >> anything;
  }
}
bool isOver() {
  return this->over;
}
void clearPot() {
  this->pot = 0;
}
void giveMoney(int ind) {
  this->players[ind].getMoney(this->pot);
  this->pot = 0;
}
};

string enumToString(int num) {
  switch (num) {
    case 0: 
      return "High Card";
    case 1: 
      return "One Pair";
    case 2: 
      return "Two Pairs";
    case 3: 
      return "Three of a Kind";
    case 4: 
      return "Straight";
    case 5: 
      return "Flush";
    case 6: 
      return "Full House";
    case 7: 
      return "Straight Flush";
    case 8: 
      return "Four of a Kind";
    
  }
}

int main() {
  
  srand(time(NULL) + seedIncrement);
  cout << "Welcome to Texas Holdem Poker!\n";
  cout << "\nIf you don't know the rules please click the link below:\n";
  cout << "https://bicyclecards.com/how-to-play/texas-holdem-poker\n";
  cout << "It is not an IP grabber :)\n\n";
  cout << "T stands for ten by the way\n\n";
  cout << "How many people are playing? (2-9)\n";
  int inp1;
  while (true) {
    cin >> inp1;
    if (inp1 >= 2 && inp1 <=9) {
      break;
    }
    cout << "How many people are playing? (2-9)\n";
  }
  vector <Player> players = {};
  for (int i = 1; i <= inp1; i++) {
    cout << "\033[2J\033[0;0H";
    cout << "Player " << i << ", what would you like to be called?\n";
    string inp2 = "";
    cin >> inp2;
    Player *plr = new Player(inp2);
    players.push_back(*plr);
  }
  cout << "Each player starts with 100 in their balance. Input anything to continue\n";
  string anything;
  cin >> anything;

  while (true) {
    Game *g = new Game(players);
    g->getB()->getCard(g->getDeck()->getCard());
    g->getB()->getCard(g->getDeck()->getCard());
    for (int i = 0; i <= 2; i++) { // This loop iterates 3 times for the 3 rounds of play
      g->getB()->getCard(g->getDeck()->getCard());
      g->checkAllHands();
      string turnHistory = "";
      // for (auto &currPlayer : g->getPlayers()) { // This loop iterates for each player in the game's players field, and the player's actions of their turn are made within. *The players field should be kept up to date such that only active players are in the vector on the start of the increment.
        
        g->betting();
        // add the player's action of this turn to the turnHistory string in sentence form
      //}
       if (g->isOver()) break; 
      }
      if (!g->isOver()) {
        int maxHand = 0;
        int maxRank = 2;
        int bestIndx = -1;
        for (int i = 0; i < g->getPlayers().size(); i++) {
          if (g->getPlayers()[i].getActive()) {
            if (g->getPlayers()[i].getScore()[0] > maxHand) {
              bestIndx = i;
              maxHand = g->getPlayers()[i].getScore()[0];
              maxRank = g->getPlayers()[i].getScore()[1]; 
            } else if (g->getPlayers()[i].getScore()[0] == maxHand) {
              if (g->getPlayers()[i].getScore()[1] > maxRank) {
              bestIndx = i;
              maxRank = g->getPlayers()[i].getScore()[1];
              } else if (g->getPlayers()[i].getScore()[1] == maxRank) {
                cout << "My money now. Try not to tie next time suckers!\n\n Input anything to continue." << endl;
                string anything;
                cin >> anything;
              }
            }
          }
        }
        if (bestIndx != -1) {
          cout << g->getPlayers()[bestIndx].getName() << " has won the pot with their " << enumToString(maxHand) <<" with the highest card value of " << maxRank << "!\n\nInput anything to continue.\n";
          string anything;
          cin >> anything;
          g->giveMoney(bestIndx);
        } else g->clearPot();
        
      }
      players = {};
      for (int i = 0; i < g->getPlayers().size(); i++) {
        players.push_back(g->getPlayers()[i]);
      }
      delete g;
    }
}
