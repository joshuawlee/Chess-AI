# Chess Game - C++ with SFML 3.0.2

A fully functional chess game with Player vs Player and Player vs AI modes, built with C++17 and SFML 3.0.2.

---

## 🎮 Features

- ✅ **Player vs Player (PVP)** - Play locally with a friend
- ✅ **Player vs AI (PVAI)** - Challenge the computer
- ✅ **Full Chess Rules** - Legal moves, castling, en passant, promotion
- ✅ **Check & Checkmate Detection**
- ✅ **Stalemate Detection**
- ✅ **AI with Minimax Algorithm** - Alpha-beta pruning (depth 3) for optimal performance
- ✅ **Graphical Interface** - Clean, intuitive board display
- ✅ **Mouse Controls** - Click-based piece selection and movement

---

## 📋 Requirements

### System Requirements
- **Operating System**: macOS, Linux, or Windows
- **C++ Compiler**: C++17 compatible (GCC, Clang, MSVC)
- **CMake**: Version 3.16 or higher
- **SFML**: Version 3.0.2

### Installing SFML 3.0.2

#### macOS (using Homebrew)
```bash
brew install sfml
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install libsfml-dev
```

#### Windows
Download SFML 3.0.2 from [SFML Downloads](https://www.sfml-dev.org/download.php)

---

## 📦 Installation & Setup

### 1. Clone the Repository

```bash
git clone https://github.com/joshuawlee/Chess-AI.git
cd Chess-AI
```

### 2. Set Up Assets Folder

The project requires a font file and 12 chess piece images.

#### Download Font File
```bash
mkdir -p assets
curl -L -o assets/font.ttf "https://github.com/google/fonts/raw/main/ofl/roboto/static/Roboto-Regular.ttf"
```

#### Chess Piece Images
You need 12 PNG files (100x100px recommended):

**White Pieces:**
- `white_pawn.png`
- `white_knight.png`
- `white_bishop.png`
- `white_rook.png`
- `white_queen.png`
- `white_king.png`

**Black Pieces:**
- `black_pawn.png`
- `black_knight.png`
- `black_bishop.png`
- `black_rook.png`
- `black_queen.png`
- `black_king.png`

**Where to Find Chess Pieces:**
- [OpenGameArt.org](https://opengameart.org) - Search "chess pieces"
- [Wikimedia Commons](https://commons.wikimedia.org/wiki/Category:SVG_chess_pieces)
- Create your own using image editing software

### 3. Project Structure

Your final folder structure should look like this:

```
Chess-AI/
├── CMakeLists.txt
├── README.md
├── assets/
│   ├── font.ttf
│   ├── white_pawn.png
│   ├── white_knight.png
│   ├── white_bishop.png
│   ├── white_rook.png
│   ├── white_queen.png
│   ├── white_king.png
│   ├── black_pawn.png
│   ├── black_knight.png
│   ├── black_bishop.png
│   ├── black_rook.png
│   ├── black_queen.png
│   └── black_king.png
└── src/
    ├── main.cpp
    ├── Piece.h
    ├── Move.h
    ├── Board.h
    ├── Board.cpp
    ├── AI.h
    ├── AI.cpp
    ├── Renderer.h
    ├── Renderer.cpp
    ├── Game.h
    └── Game.cpp
```

---

## 🔨 Building the Project

### Step-by-Step Build Instructions

```bash
# Navigate to project directory
cd Chess-AI

# Remove old build files (if rebuilding)
rm -rf build

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Compile the project
make

# Run the game
./ChessGame
```

### Quick Build Script

Create a file `build.sh` in your project root:

```bash
#!/bin/bash
echo "🔨 Building Chess Game..."
rm -rf build
mkdir build
cd build
cmake ..
make
echo "✅ Build complete!"
echo "🎮 Run with: cd build && ./ChessGame"
```

Make it executable and run:
```bash
chmod +x build.sh
./build.sh
```

---

## 🎮 How to Play

### Starting the Game

1. **Launch the game:**
   ```bash
   cd build
   ./ChessGame
   ```

2. **Main Menu:**
   - Click **"Player vs Player"** for local 2-player mode
   - Click **"Player vs AI"** to play against the computer

---

## 🕹️ Game Controls

### Mouse Controls

| Action | Control |
|--------|---------|
| **Select Piece** | Click on a piece of your color |
| **Move Piece** | Click on the piece, then click destination square |
| **Deselect Piece** | Click on an invalid square or opponent's piece |
| **Return to Menu** | Click "Return to Menu" button (from game over screen) |

### Gameplay

#### Player vs Player (PVP)
- **White moves first** (bottom of the board)
- Players alternate turns
- Click a piece to select it (highlighted in yellow)
- Click a valid square to move
- Game ends on checkmate or stalemate

#### Player vs AI (PVAI)
- **You play as White** (bottom of the board)
- **AI plays as Black** (top of the board)
- AI uses minimax algorithm with alpha-beta pruning
- AI difficulty: Depth 3 (approximately 1 second per move)
- Make your move, then AI responds automatically

---

## ♟️ Chess Rules Implemented

### Standard Moves
- **Pawn**: Forward one square, two squares from starting position, diagonal captures
- **Knight**: L-shaped moves (2+1 squares)
- **Bishop**: Diagonal movement
- **Rook**: Horizontal and vertical movement
- **Queen**: Combination of bishop and rook movement
- **King**: One square in any direction

### Special Moves
- ✅ **Castling** (kingside and queenside)
- ✅ **En Passant** capture
- ✅ **Pawn Promotion** (automatically promotes to Queen)

### Game End Conditions
- ✅ **Checkmate** - King is in check with no legal moves
- ✅ **Stalemate** - No legal moves available but king not in check
- ✅ **Check Detection** - Prevents illegal moves that leave king in check

---

## 🐛 Troubleshooting

### "Failed to load font" Error
**Problem:** Font file not found

**Solutions:**
1. Make sure `assets/font.ttf` exists in your project root
2. Download font:
   ```bash
   curl -L -o assets/font.ttf "https://github.com/google/fonts/raw/main/ofl/roboto/static/Roboto-Regular.ttf"
   ```
3. The code includes fallback to macOS system fonts if assets/font.ttf is not found

### "Failed to load texture" Error
**Problem:** Chess piece images not found

**Solution:**
- Ensure all 12 PNG files are in `assets/` folder
- Check file names match exactly (case-sensitive):
  - `white_pawn.png` not `White_Pawn.png`
  - `black_knight.png` not `black_night.png`

### Build Errors
**Problem:** CMake can't find SFML

**Solution:**
```bash
# macOS
brew install sfml

# Linux
sudo apt-get install libsfml-dev

# Verify installation
pkg-config --modversion sfml-all
```

### Game Window Doesn't Appear
**Problem:** Window opens and closes immediately

**Solution:**
- Check console for error messages
- Verify all assets loaded correctly
- Run from `build/` directory: `./ChessGame`
- Open an issue on [GitHub](https://github.com/joshuawlee/Chess-AI/issues) if problems persist

---

## ⚙️ Configuration

### Adjusting AI Difficulty

Edit `src/Game.cpp`, find the `handleAITurn()` function:

```cpp
void Game::handleAITurn() {
    Move aiMove = ai.getBestMove(board, 3);  // Change 3 to adjust depth
    // ...
}
```

**Depth Levels:**
- `depth = 1` - Very Easy (instant moves)
- `depth = 2` - Easy (fast moves)
- `depth = 3` - Medium (default, ~1 second)
- `depth = 4` - Hard (several seconds)
- `depth = 5+` - Very Hard (very slow)

### Changing Board Colors

Edit `src/Renderer.cpp`, in the `drawBoard()` function:

```cpp
if ((row + col) % 2 == 0) {
    tile.setFillColor(sf::Color(240, 217, 181)); // Light square
} else {
    tile.setFillColor(sf::Color(181, 136, 99));  // Dark square
}
```

### Changing Window Size

Edit `src/Game.cpp`:

```cpp
sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 800)), "Chess Game");
// Change both 800s to desired width/height (must be equal for square board)
```

Also update `TILE_SIZE` in `src/Renderer.h`:
```cpp
static const int TILE_SIZE = 100; // Change to: window_size / 8
```

---

## 📝 Code Structure

### Core Components

| File | Description |
|------|-------------|
| `main.cpp` | Entry point, initializes game |
| `Game.h/cpp` | Game loop, state management, input handling |
| `Board.h/cpp` | Chess logic engine, move generation, validation |
| `AI.h/cpp` | Minimax algorithm with alpha-beta pruning |
| `Renderer.h/cpp` | SFML graphics, drawing board and pieces |
| `Piece.h` | Piece types and colors |
| `Move.h` | Move structure with special flags |

### Architecture Flow

```
main.cpp
   └─→ Game::run()
        ├─→ handleInput() - Mouse clicks
        ├─→ update() - AI moves, game state
        └─→ render() - Draw everything
             └─→ Renderer
                  ├─→ drawBoard()
                  ├─→ drawPieces()
                  └─→ drawSelection()
```

---

## 🎯 Technical Implementation

### Move Generation Algorithm
1. Generate all pseudo-legal moves for current player
2. For each move:
   - Apply move to board
   - Check if own king is in check
   - If safe, add to legal moves list
   - Undo move
3. Return list of legal moves

### AI Algorithm (Minimax with Alpha-Beta Pruning)
- **Evaluation Function**: Material-based scoring (Pawn=100, Knight=300, Bishop=300, Rook=500, Queen=900, King=10000)
- **Search Depth**: 3 plies (half-moves)
- **Optimization**: Alpha-beta pruning reduces search space by ~50%
- **Time Complexity**: O(b^d) where b=branching factor, d=depth

---

## 💡 Strategy Tips

### Against AI
- **Opening**: Control the center (e4, d4)
- **Development**: Move knights and bishops early
- **King Safety**: Castle early (kingside usually safer)
- **Tactics**: Look for forks, pins, and skewers
- **Endgame**: Activate your king

### General Strategy
- Control the center squares
- Develop pieces before moving the same piece twice
- Protect your king
- Avoid moving pawns in front of castled king
- Think ahead before moving

---

## 🙏 Credits & License

**Developer:** Joshua Lee  
**SFML Library:** [www.sfml-dev.org](https://www.sfml-dev.org)  
**Chess Rules:** Standard FIDE rules  
**AI Algorithm:** Minimax with Alpha-Beta Pruning  
**License:** MIT License  

---

## 🎉 Enjoy the Game!

Have fun playing chess! Whether you're practicing against the AI or challenging a friend, may your games be full of brilliant tactics and strategic victories! ♟️👑

**Built with ❤️ using C++ and SFML**

### Gameplay

#### Player vs Player (PVP)
- **White moves first** (bottom of the board)
- Players alternate turns
- Click a piece to select it (highlighted in yellow)
- Click a valid square to move
- Game ends on checkmate or stalemate

#### Player vs AI (PVAI)
- **You play as White** (bottom of the board)
- **AI plays as Black** (top of the board)
- AI uses minimax algorithm with alpha-beta pruning
- AI difficulty: Depth 3 (adjustable in source code)
- Make your move, then AI responds automatically

---

## ♟️ Chess Rules Implemented

### Standard Moves
- **Pawn**: Forward one square, two squares from starting position, diagonal captures
- **Knight**: L-shaped moves (2+1 squares)
- **Bishop**: Diagonal movement
- **Rook**: Horizontal and vertical movement
- **Queen**: Combination of bishop and rook movement
- **King**: One square in any direction

### Special Moves
- ✅ **Castling** (kingside and queenside)
- ✅ **En Passant** capture
- ✅ **Pawn Promotion** (automatically promotes to Queen)

### Game End Conditions
- ✅ **Checkmate** - King is in check with no legal moves
- ✅ **Stalemate** - No legal moves available but king not in check
- ✅ **Check Detection** - Prevents illegal moves that leave king in check

---

## 🐛 Troubleshooting

### "Failed to load font" Error
**Problem:** Font file not found

**Solutions:**
1. Make sure `assets/font.ttf` exists
2. Download font:
   ```bash
   curl -L -o assets/font.ttf "https://github.com/google/fonts/raw/main/ofl/roboto/static/Roboto-Regular.ttf"
   ```
3. Or use system font (code already includes fallback to macOS system fonts)

### "Failed to load texture" Error
**Problem:** Chess piece images not found

**Solution:**
- Ensure all 12 PNG files are in `assets/` folder
- Check file names match exactly (case-sensitive):
  - `white_pawn.png` not `White_Pawn.png`
  - `black_knight.png` not `black_night.png`

### Build Errors
**Problem:** CMake can't find SFML

**Solution:**
```bash
# macOS
brew install sfml

# Linux
sudo apt-get install libsfml-dev

# Verify installation
pkg-config --modversion sfml-all
```

### Game Window Doesn't Appear
**Problem:** Window opens and closes immediately

**Solution:**
- Check console for error messages
- Verify all assets loaded correctly
- Run from `build/` directory: `./ChessGame`

---

## ⚙️ Configuration

### Adjusting AI Difficulty

Edit `src/Game.cpp`, find the `handleAITurn()` function:

```cpp
void Game::handleAITurn() {
    Move aiMove = ai.getBestMove(board, 3);  // Change 3 to adjust depth
    // ...
}
```

**Depth Levels:**
- `depth = 1` - Very Easy (instant moves)
- `depth = 2` - Easy (fast moves)
- `depth = 3` - Medium (default, ~1 second)
- `depth = 4` - Hard (several seconds)
- `depth = 5+` - Very Hard (very slow)

### Changing Board Colors

Edit `src/Renderer.cpp`, in the `drawBoard()` function:

```cpp
if ((row + col) % 2 == 0) {
    tile.setFillColor(sf::Color(240, 217, 181)); // Light square (change RGB)
} else {
    tile.setFillColor(sf::Color(181, 136, 99));  // Dark square (change RGB)
}
```

### Changing Window Size

Edit `src/Game.cpp`:

```cpp
sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 800)), "Chess Game");
// Change 800, 800 to desired width, height
```

Also update `TILE_SIZE` in `src/Renderer.h`:
```cpp
static const int TILE_SIZE = 100; // Change to: window_size / 8
```

---

## 📝 Code Structure

### Core Components

| File | Description |
|------|-------------|
| `main.cpp` | Entry point, initializes game |
| `Game.h/cpp` | Game loop, state management, input handling |
| `Board.h/cpp` | Chess logic, move generation, validation |
| `AI.h/cpp` | Minimax algorithm with alpha-beta pruning |
| `Renderer.h/cpp` | SFML graphics, drawing board and pieces |
| `Piece.h` | Piece types and colors |
| `Move.h` | Move structure with special flags |

### Architecture Flow

```
main.cpp
   └─→ Game::run()
        ├─→ handleInput() - Mouse clicks
        ├─→ update() - AI moves, game state
        └─→ render() - Draw everything
             └─→ Renderer
                  ├─→ drawBoard()
                  ├─→ drawPieces()
                  └─→ drawSelection()
```

---

## 🎯 Future Enhancements

Potential features to add:

- [ ] Move history display
- [ ] Undo/Redo moves
- [ ] Save/Load games
- [ ] Multiple AI difficulty levels (menu selection)
- [ ] Move animation
- [ ] Sound effects
- [ ] Online multiplayer
- [ ] Opening book for AI
- [ ] Time controls
- [ ] Move highlighting (show legal moves)

---

## 📄 License

This project is open source and available under the MIT License.

---

## 🙏 Credits

- **SFML Library**: [www.sfml-dev.org](https://www.sfml-dev.org)
- **Chess Rules**: Standard FIDE rules
- **AI Algorithm**: Minimax with Alpha-Beta Pruning

---

## 💡 Tips for Playing

### Against AI
- **Opening**: Control the center (e4, d4)
- **Development**: Move knights and bishops early
- **King Safety**: Castle early (kingside usually safer)
- **Tactics**: Look for forks, pins, and skewers
- **Endgame**: Activate your king

### General Strategy
- Control the center
- Develop pieces before moving same piece twice
- Protect your king
- Don't move pawns in front of castled king unnecessarily
- Think ahead before moving

---

## 📞 Support

If you encounter issues:

1. Check the Troubleshooting section above
2. Verify all assets are in place
3. Ensure SFML 3.0.2 is installed correctly
4. Check console output for error messages

---

## 🎉 Enjoy the Game!

Have fun playing chess! Whether you're practicing against the AI or challenging a friend, may your games be full of brilliant tactics and strategic victories! ♟️👑

---

**Built with ❤️ using C++ and SFML**